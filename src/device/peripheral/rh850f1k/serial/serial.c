/*
 *  TOPPERS athrill
 *     Athrill is a CPU emulator. Athrill was developed to easily
 *     execute and debug embedded programs on virtual microcomputer.
 *
 *  Copyright (C) 2019 by Center for Embedded Computing Systems
 *                          Graduate School of Informatics, Nagoya Univ., JAPAN
 *  Copyright (C) 2019 by ESM, Inc.
 *
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  $Id$
 */
#include "inc/serial.h"
#include "device.h"
#include "std_errno.h"
#include "mpu_types.h"
#include "device_ex_serial_ops.h"
#include "cpuemu_ops.h"
#include <stdio.h>

typedef struct {
	uint32 					received_counter;
	uint16 					id;
	uint16 					rx_intno;
	uint16					tx_intno;
	uint32					send_counter;
	bool   					is_send_data;
	uint8 					send_data;
	DeviceExSerialOpType 	*ops;
	DeviceClockType 		*dev_clock;
	uint64					start_clock;
	bool					is_send_enable;
	bool					is_read_enable;
	bool					is_reset_mode;
	bool					is_send_start_intr;
	bool					is_sending;
	bool					is_receiving;
} SerialDeviceType;

static SerialDeviceType SerialDevice[UDnChannelNum];

static Std_ReturnType serial_get_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data);
static Std_ReturnType serial_get_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data);
static Std_ReturnType serial_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data);
static Std_ReturnType serial_put_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data);
static Std_ReturnType serial_put_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data);
static Std_ReturnType serial_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data);
static Std_ReturnType serial_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data);

MpuAddressRegionOperationType	serial_memory_operation = {
		.get_data8 		= 	serial_get_data8,
		.get_data16		=	serial_get_data16,
		.get_data32		=	serial_get_data32,

		.put_data8 		= 	serial_put_data8,
		.put_data16		=	serial_put_data16,
		.put_data32		=	serial_put_data32,

		.get_pointer	= serial_get_pointer,
};

#define SERIAL_SEND_TIME_DEFAULT		100U
#define SERIAL_RECEIVE_TIME_DEFAULT		1000U

static uint32 serial_send_time = SERIAL_SEND_TIME_DEFAULT;
static uint32 serial_receiv_time = SERIAL_SEND_TIME_DEFAULT;

static MpuAddressRegionType *serial_region;


void device_init_serial(MpuAddressRegionType *region)
{
	uint8 ch;

	(void)cpuemu_get_devcfg_value("DEVICE_CONFIG_SERIAL_SEND_TIME", &serial_send_time);
	(void)cpuemu_get_devcfg_value("DEVICE_CONFIG_SERIAL_RECIVE_TIME", &serial_receiv_time);

	for (ch = 0; ch < UDnChannelNum; ch++) {
		SerialDevice[ch].id = ch;
		SerialDevice[ch].rx_intno = serial_rlin3x_get_rx_intno(ch);
		SerialDevice[ch].tx_intno = serial_rlin3x_get_tx_intno(ch);
		SerialDevice[ch].is_send_data = FALSE;
		SerialDevice[ch].start_clock = 0U;
		SerialDevice[ch].ops = NULL;
		SerialDevice[ch].send_counter = 0U;
		SerialDevice[ch].received_counter = 0U;
		SerialDevice[ch].is_send_enable = FALSE;
		SerialDevice[ch].is_read_enable = FALSE;
		SerialDevice[ch].is_reset_mode = TRUE;
		SerialDevice[ch].is_send_start_intr = TRUE;
		SerialDevice[ch].is_sending = FALSE;
		SerialDevice[ch].is_receiving = FALSE;
	}

	serial_region = region;

	return;
}

static void serial_receive(SerialDeviceType *serial)
{
	uint8 data;
	bool ret;

	if (serial->ops == NULL) {
		return;
	}

	//受信の有効・無効確認
	if (!(serial->is_read_enable)) {
		return;
	}

	if (serial->is_receiving == FALSE) {
		/*
		 * ユーザがレディ状態
		 */
		ret = serial->ops->getchar(serial->id, &data);
		if (ret == TRUE) {
			serial->is_receiving = TRUE;
			//受信データをセットする．
			(void)serial_put_data8(serial_region, CPU_CONFIG_CORE_ID_0, (RLIN3xLURDRL(serial->id) & serial_region->mask), data);
			serial->received_counter = serial_receiv_time;
		}
	}
	else {
		if (serial->received_counter > 0U) {
			serial->received_counter--;
		}
		else {
			device_raise_int(serial->rx_intno);
			serial->is_receiving = FALSE;
		}
	}

	return;
}

static void serial_send(SerialDeviceType *serial)
{
	//送信の有効・無効確認
	if (!(serial->is_send_enable)) {
		return;
	}

	if (serial->is_send_data == TRUE) {
		serial->is_sending = TRUE;
	}

	//送信中処理
	if (serial->is_sending == TRUE) {
		if (serial->send_counter == 0U) {
			//送信開始
			//(void)serial->ops->putchar(serial->id, serial->send_data);
			if (serial->ops->flush != NULL) {
				serial->ops->flush(serial->id);
			}
			serial->is_send_data = FALSE;

			if (serial->is_send_start_intr) {
				device_raise_int(serial->tx_intno);
			}
			serial->send_counter++;
		}
		else if (serial->send_counter >= serial_send_time) {
			//送信完了
			if (!(serial->is_send_start_intr)) {
				device_raise_int(serial->tx_intno);
				serial->is_sending = FALSE;
			}
			else {
				if (!(serial->is_send_data)) {
					serial->is_sending = FALSE;
				}
			}
			serial->send_counter = 0U;
		}
		else {
			//送信中
			serial->send_counter++;
		}
	}

	return;
}

void device_do_serial(SerialDeviceType *serial)
{
	serial_receive(serial);
	serial_send(serial);
	return;
}

void device_supply_clock_serial(DeviceClockType *dev_clock)
{

	SerialDevice[UDnCH1].dev_clock = dev_clock;
	device_do_serial(&SerialDevice[UDnCH1]);

	return;
}

void device_ex_serial_register_ops(uint8 channel, DeviceExSerialOpType *ops)
{
	SerialDevice[channel].ops = ops;
	return;
}

static void serial_change_mode_reset(MpuAddressRegionType *region, uint8 ch)
{
	uint32 off;

	/* RLN3nLTRC (not supported) */
	/* RLN3nLEST (not supported) */
	/* RLN3nLST */
	off = ((RLIN3xLST(ch)) - region->start);
	*((uint8*)(&region->data[off])) = 0x00;
	SerialDevice[ch].is_sending = FALSE;
	SerialDevice[ch].is_receiving = FALSE;

	/* RLN3nLUOER */
	off = (RLIN3xLUOER(ch) - region->start);
	*((uint8*)(&region->data[off])) = 0x00;
	SerialDevice[ch].is_read_enable = FALSE;
	SerialDevice[ch].is_send_enable = FALSE;

	SerialDevice[ch].received_counter = 0U;
	SerialDevice[ch].send_counter = 0U;

	return;
}

static Std_ReturnType serial_get_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data)
{
	uint32 off = (addr - region->start);
	uint8 ch = RLIN_GET_CHANNEL(addr);

	if (addr == RLIN3xLST(ch)) {
		*data = ((RLIN3x_LST_GET_URS(SerialDevice[ch].is_receiving) << RLIN3x_LST_URS_BIT) |
				(RLIN3x_LST_GET_UTS(SerialDevice[ch].is_sending) << RLIN3x_LST_UTS_BIT));
	}
	else {
		*data = *((uint8*)(&region->data[off]));
	}

	return STD_E_OK;
}
static Std_ReturnType serial_get_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data)
{
	uint32 off = (addr - region->start);
	*data = *((uint16*)(&region->data[off]));
	return STD_E_OK;
}
static Std_ReturnType serial_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data)
{
	uint32 off = (addr - region->start);
	*data = *((uint32*)(&region->data[off]));
	return STD_E_OK;
}
static Std_ReturnType serial_put_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data)
{
	uint32 off = (addr - region->start);
	*((uint8*)(&region->data[off])) = data;
	uint8 ch = RLIN_GET_CHANNEL(addr);

	if (addr == RLIN3xLUTDRL(ch)) {
		SerialDevice[ch].send_data = data;;
		SerialDevice[ch].is_send_data = TRUE;
		(void)SerialDevice[ch].ops->putchar(SerialDevice[ch].id, SerialDevice[ch].send_data);
	}
	else if (addr == RLIN3xLCUC(ch)) {
		off = (RLIN3xLMST(ch) - region->start);
		*((uint8*)(&region->data[off])) = data;
		SerialDevice[ch].is_reset_mode = RLIN_ISMODE_RESET(data);
		if (SerialDevice[ch].is_reset_mode) {
			serial_change_mode_reset(region, ch);
		}
	}
	else if (addr == RLIN3xLUOER(ch)) {
		if (!(SerialDevice[ch].is_reset_mode)) {
			SerialDevice[ch].is_read_enable = RLIN3x_LUOER_ISSET_UROE(data);
			SerialDevice[ch].is_send_enable = RLIN3x_LUOER_ISSET_UTOE(data);
		}
	}
	else if (addr == (RLIN3x_LUOR1(ch))) {
		SerialDevice[ch].is_send_start_intr = RLIN3x_LUOR1_ISSET_SEND_START_INTR(data);
	}


	return STD_E_OK;
}
static Std_ReturnType serial_put_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data)
{
	uint32 off = (addr - region->start);
	*((uint16*)(&region->data[off])) = data;
	uint8 ch = RLIN_GET_CHANNEL(addr);

	if (addr == RLIN3xLUTDRL(ch)) {
		SerialDevice[ch].send_data = (uint8)data;
		SerialDevice[ch].is_send_data = TRUE;
	}

	return STD_E_OK;
}
static Std_ReturnType serial_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data)
{
	uint32 off = (addr - region->start);
	*((uint32*)(&region->data[off])) = data;
	return STD_E_OK;
}
static Std_ReturnType serial_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data)
{
	uint32 off = (addr - region->start);
	*data = &region->data[off];
	return STD_E_OK;
}

