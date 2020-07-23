#include "concrete_executor/target/dbg_target_serial.h"
#include "tcp/tcp_server.h"
#include "tcp/tcp_connection.h"
#include "comm_buffer.h"
#include "athrill_mpthread.h"
#include "assert.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	MpthrIdType			id;
	MpthrOperationType	op;
} DbgSerialTcpThreadType;

typedef struct {
	DbgSerialTcpThreadType	thread;
	CommFifoBufferType		fifo;
	CommBufferType			rcvbuffer;
	TcpServerType			server;
	TcpConnectionType		connection;
} DbgSerialTcpType;

#define DBG_SERIAL_TCP_BUFFER_SIZE	1024
static uint8 dbg_serial_channel_num = 0;
static DbgSerialTcpType *dbg_serial_tcp;
static Std_ReturnType dbg_serail_get_channel(MpthrIdType id, uint8 *channel)
{
	uint32 i;
	for (i = 0; i < dbg_serial_channel_num; i++) {
		if (dbg_serial_tcp[i].thread.id == id) {
			*channel = i;
			return STD_E_OK;
		}
	}
	return STD_E_NOENT;
}

static Std_ReturnType dbg_serial_op_do_init(MpthrIdType id)
{
	return STD_E_OK;
}

static void dbg_serial_do_transfer(uint8 channel, uint32 size)
{
	uint32 off = 0;
	uint32 res;
	Std_ReturnType err;

	do {
		mpthread_lock(dbg_serial_tcp[channel].thread.id);
		err = comm_fifo_buffer_add(&dbg_serial_tcp[channel].fifo, &dbg_serial_tcp[channel].rcvbuffer.data[off], size, &res);
		mpthread_unlock(dbg_serial_tcp[channel].thread.id);
		if (err != STD_E_OK) {
			usleep(500 * 1000); /* 500msec */
		}
		off += res;
	} while (off < size);
	return;
}

static Std_ReturnType dbg_serial_do_recv(uint8 channel)
{
	Std_ReturnType err;
	uint32 res;

	while (TRUE) {
		err = tcp_connection_receive(&dbg_serial_tcp[channel].connection, dbg_serial_tcp[channel].rcvbuffer.data, dbg_serial_tcp[channel].rcvbuffer.max_size, &res);
		if (err != STD_E_OK) {
			printf("SERIAL TCP CONNECTION INFO(channel=%u): receive error\n", channel);
			return err;
		}
		dbg_serial_do_transfer(channel, res);
	}
}

static Std_ReturnType dbg_serial_op_do_proc(MpthrIdType id)
{
	uint8 channel;
	Std_ReturnType err;

	err = dbg_serail_get_channel(id, &channel);
	ASSERT(err == STD_E_OK);

	while (TRUE) {
		err = tcp_server_accept((const TcpServerType *)&dbg_serial_tcp[channel].server, &dbg_serial_tcp[channel].connection);
		if (err != STD_E_OK) {
			continue;
		}
		printf("SERIAL TCP SERVER INFO(channel=%u:server-port=%u): connected\n", channel, dbg_serial_tcp[channel].server.config.server_port);

		(void)dbg_serial_do_recv(channel);
		tcp_connection_close(&dbg_serial_tcp[channel].connection);
	}
	return STD_E_OK;
}

Std_ReturnType dbg_serial_init_tcp(uint8 channel_num)
{
	uint32 i;

	dbg_serial_tcp = malloc(sizeof(DbgSerialTcpType) * channel_num);
	if (dbg_serial_tcp == NULL) {
		return STD_E_INVALID;
	}
	memset(dbg_serial_tcp, 0, sizeof(DbgSerialTcpType) * channel_num);
	dbg_serial_channel_num = channel_num;
	for (i = 0; i < channel_num; i++) {
		dbg_serial_tcp[i].thread.id = -1;
	}
	return STD_E_OK;
}

Std_ReturnType dbg_serial_open_tcp(uint8 channel, uint32 server_portno)
{
	Std_ReturnType err;
	TcpServerConfigType config;

	//buffer
	err = comm_fifo_buffer_create(DBG_SERIAL_TCP_BUFFER_SIZE, &dbg_serial_tcp[channel].fifo);
	ASSERT(err == STD_E_OK);
	err = comm_buffer_create(DBG_SERIAL_TCP_BUFFER_SIZE, &dbg_serial_tcp[channel].rcvbuffer);
	ASSERT(err == STD_E_OK);

	//server
	config.server_port = server_portno;
	err = tcp_server_create((const TcpServerConfigType *)&config, &dbg_serial_tcp[channel].server);
	ASSERT(err == STD_E_OK);

	//thread
	dbg_serial_tcp[channel].thread.op.do_init = dbg_serial_op_do_init;
	dbg_serial_tcp[channel].thread.op.do_proc = dbg_serial_op_do_proc;
	err = mpthread_register(&dbg_serial_tcp[channel].thread.id, &dbg_serial_tcp[channel].thread.op);
	ASSERT(err == STD_E_OK);

	err = mpthread_start_proc(dbg_serial_tcp[channel].thread.id);
	ASSERT(err == STD_E_OK);

	return STD_E_OK;
}

bool dbg_serial_getchar_tcp(uint8 channel, uint8 *data)
{
	Std_ReturnType ret;
	uint32 res;

	mpthread_lock(dbg_serial_tcp[channel].thread.id);
	while (TRUE) {
		ret = comm_fifo_buffer_get(&dbg_serial_tcp[channel].fifo, (char*)data, 1U, &res);
		if (ret != STD_E_OK) {
			mpthread_unlock(dbg_serial_tcp[channel].thread.id);
			return FALSE;
		}
		if ((*data) == '\r') {
			continue;
		}
		else {
			break;
		}
	}
	mpthread_unlock(dbg_serial_tcp[channel].thread.id);

	return TRUE;
}

bool dbg_serial_putchar_tcp(uint8 channel, uint8 data)
{
	Std_ReturnType ret;
	uint32 res;
	if (dbg_serial_tcp[channel].connection.connected == FALSE) {
		return FALSE;
	}
	ret = tcp_connection_send(&dbg_serial_tcp[channel].connection, (char*)&data, 1U, &res);
	if (ret != STD_E_OK) {
		printf("SERIAL TCP CONNECTION INFO(channel=%u): send error\n", channel);
		return FALSE;
	}
	return TRUE;
}
