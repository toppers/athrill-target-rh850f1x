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
#include "can_register_mapping_io.h"
#include "can_register.h"
#include "can_buffer.h"
#include "can_register_mapping_reset.h"

void can_register_reset_tx_buffer(CanRegisterResetType type)
{
	CanChannelIdType cid;
	uint32 bid;
	uint32 i;

	if (type == CanRegisterResetType_MCU_RESET) {
		for (cid = 0U; cid < CAN_CHANNEL_NUM; cid++) {
			for (bid = 0U; bid < CAN_TX_CHANNEL_BUFFER_NUM; bid++) {
				can_tx_buffer_control.buf[cid][bid].ctrl.req_tx = FALSE;
				can_tx_buffer_control.buf[cid][bid].status.result = 0U;
				can_tx_buffer_control.buf[cid][bid].status.sending = FALSE;
				can_tx_buffer_control.buf[cid][bid].id.ide = FALSE;
				can_tx_buffer_control.buf[cid][bid].id.rtr = FALSE;
				can_tx_buffer_control.buf[cid][bid].id.id = 0U;
				can_tx_buffer_control.buf[cid][bid].header.dlc = 0U;
				memset(&can_tx_buffer_control.buf[cid][bid].body.buffer, 0, CAN_TX_BUFFER_DATA_SIZE);
			}
			for (i = 0; i < CAN_TX_BUFFER_FLAG_NUM; i++) {
				can_tx_buffer_control.intr_enable[i] = 0U;
			}
		}
	}
	else if (type == CanRegisterResetType_GLOBAL_RESET) {
		// nothing
	}
	else if (type == CanRegisterResetType_CHANNEL_RESET) {
		for (cid = 0U; cid < CAN_CHANNEL_NUM; cid++) {
			for (bid = 0U; bid < CAN_TX_CHANNEL_BUFFER_NUM; bid++) {
				can_tx_buffer_control.buf[cid][bid].ctrl.req_tx = FALSE;
				can_tx_buffer_control.buf[cid][bid].status.result = 0U;
				can_tx_buffer_control.buf[cid][bid].status.sending = FALSE;
			}
		}
	}

}
void can_register_reset_tx_buffer_channel(CanChannelIdType cid)
{
	uint32 bid;

	for (bid = 0U; bid < CAN_TX_CHANNEL_BUFFER_NUM; bid++) {
		can_tx_buffer_control.buf[cid][bid].ctrl.req_tx = FALSE;
		can_tx_buffer_control.buf[cid][bid].status.result = 0U;
		can_tx_buffer_control.buf[cid][bid].status.sending = FALSE;
	}
}
void can_register_mapping_io_RSCANnTMCp(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint8 data = 0x0;
	uint32 index = CAN_REG_INDEX(arg->address, CAN_REG_RSCANnTMCp(0), 1U);
	CanChannelIdType cid = index / CAN_TX_CHANNEL_BUFFER_NUM;
	CanBufferIdxType bid = index % CAN_TX_CHANNEL_BUFFER_NUM;

	if (io_type == CanRegisterIo_Read) {
		//0 TMTR
		CAN_REG_UPDATE_REG_BIT(can_tx_buffer_control.buf[cid][bid].ctrl.req_tx, 0U, data);
		can_register_write_data8(arg->address, data);
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data8(arg->address, &data);
		CAN_REG_UPDATE_MODEL_FLAG(data, 0U, can_tx_buffer_control.buf[cid][bid].ctrl.req_tx);
	}
	return;
}
void can_register_mapping_io_RSCANnTMSTSp(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint8 data = 0x0;
	uint32 index = CAN_REG_INDEX(arg->address, CAN_REG_RSCANnTMSTSp(0), 1U);
	CanChannelIdType cid = index / CAN_TX_CHANNEL_BUFFER_NUM;
	CanBufferIdxType bid = index % CAN_TX_CHANNEL_BUFFER_NUM;
	if (io_type == CanRegisterIo_Read) {
		//3 TMTRM
		CAN_REG_UPDATE_REG_BIT(can_tx_buffer_control.buf[cid][bid].ctrl.req_tx, 3U, data);
		//2、 1 TMTRF[1:0]
		data |= (can_tx_buffer_control.buf[cid][bid].status.result << 1U);
		//0 TMTSTS
		CAN_REG_UPDATE_REG_BIT(can_tx_buffer_control.buf[cid][bid].status.sending, 0U, data);

		can_register_write_data8(arg->address, data);
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data8(arg->address, &data);
		//2、 1 TMTRF[1:0]
		can_tx_buffer_control.buf[cid][bid].status.result = CAN_REG_GET_MASK_DATA(data >> 1U, 0x3);
	}
	return;
}

void can_register_mapping_io_TX_BUFFER(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 addr = CAN_REG_GET_ALIGNED_ADDR(arg->address);

	switch (addr & 0x0000000F) {
	case 0x0:
		can_register_mapping_io_RSCANnTMIDp(io_type, arg);
		break;
	case 0x04:
		can_register_mapping_io_RSCANnTMPTRp(io_type, arg);
		break;
	case 0x8:
		can_register_mapping_io_RSCANnTMDF0p(io_type, arg);
		break;
	case 0xC:
		can_register_mapping_io_RSCANnTMDF1p(io_type, arg);
		break;
	default:
		break;
	}
	return;
}

void can_register_mapping_io_RSCANnTMIDp(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnTMIDp(0), 16U);
	CanChannelIdType cid = index / CAN_TX_CHANNEL_BUFFER_NUM;
	CanBufferIdxType bid = index % CAN_TX_CHANNEL_BUFFER_NUM;

	if (io_type == CanRegisterIo_Read) {
		// 31 TMIDE
		CAN_REG_UPDATE_REG_BIT(can_tx_buffer_control.buf[cid][bid].id.ide, 31U, data);
		// 30 TMRTR
		CAN_REG_UPDATE_REG_BIT(can_tx_buffer_control.buf[cid][bid].id.rtr, 30U, data);
		// 28 ～ 0 TMID [28:0]
		data |= can_tx_buffer_control.buf[cid][bid].id.id;

		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		// 31 TMIDE
		CAN_REG_UPDATE_MODEL_FLAG(data, 31U, can_tx_buffer_control.buf[cid][bid].id.ide);
		// 30 TMRTR
		CAN_REG_UPDATE_MODEL_FLAG(data, 30U, can_tx_buffer_control.buf[cid][bid].id.rtr);
		// 28 ～ 0 TMID [28:0]
		can_tx_buffer_control.buf[cid][bid].id.id = CAN_REG_GET_MASK_DATA(data, 0x1FFFFFFF);
	}

	return;
}
void can_register_mapping_io_RSCANnTMPTRp(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnTMPTRp(0), 16U);
	CanChannelIdType cid = index / CAN_TX_CHANNEL_BUFFER_NUM;
	CanBufferIdxType bid = index % CAN_TX_CHANNEL_BUFFER_NUM;

	if (io_type == CanRegisterIo_Read) {
		// 31 ～ 28 TMDLC [3:0]
		data |= (can_tx_buffer_control.buf[cid][bid].header.dlc << 28U);
		// 23 ～ 16 TMPTR [7:0]
		data |= (can_tx_buffer_control.buf[cid][bid].header.label << 16U);

		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);

		can_tx_buffer_control.buf[cid][bid].header.dlc = CAN_REG_GET_MASK_DATA(data >> 28U, 0x0F);
		can_tx_buffer_control.buf[cid][bid].header.label = CAN_REG_GET_MASK_DATA(data >> 16U, 0xFF);
	}
	return;
}

void can_register_mapping_io_RSCANnTMDF0p(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint8 *array = (uint8*)&data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnTMDF0p(0), 16U);
	CanChannelIdType cid = index / CAN_TX_CHANNEL_BUFFER_NUM;
	CanBufferIdxType bid = index % CAN_TX_CHANNEL_BUFFER_NUM;

	if (io_type == CanRegisterIo_Read) {
		// 7 ～ 0 TMDB0 [7:0]
		array[0] = can_tx_buffer_control.buf[cid][bid].body.buffer[0];
		// 15 ～ 8 TMDB1 [7:0]
		array[1] = can_tx_buffer_control.buf[cid][bid].body.buffer[1];
		// 23 ～ 16 TMDB2 [7:0]
		array[2] = can_tx_buffer_control.buf[cid][bid].body.buffer[2];
		// 31 ～ 24 TMDB3 [7:0]
		array[3] = can_tx_buffer_control.buf[cid][bid].body.buffer[3];
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		// 7 ～ 0 TMDB0 [7:0]
		can_tx_buffer_control.buf[cid][bid].body.buffer[0] = array[0];
		// 15 ～ 8 TMDB1 [7:0]
		can_tx_buffer_control.buf[cid][bid].body.buffer[1] = array[1];
		// 23 ～ 16 TMDB2 [7:0]
		can_tx_buffer_control.buf[cid][bid].body.buffer[2] = array[2];
		// 31 ～ 24 TMDB3 [7:0]
		can_tx_buffer_control.buf[cid][bid].body.buffer[3] = array[3];
	}

	return;
}
void can_register_mapping_io_RSCANnTMDF1p(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint8 *array = (uint8*)&data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnTMDF1p(0), 16U);
	CanChannelIdType cid = index / CAN_TX_CHANNEL_BUFFER_NUM;
	CanBufferIdxType bid = index % CAN_TX_CHANNEL_BUFFER_NUM;

	if (io_type == CanRegisterIo_Read) {
		// 7 ～ 0 TMDB4 [7:0]
		array[0] = can_tx_buffer_control.buf[cid][bid].body.buffer[4];
		// 15 ～ 8 TMDB5 [7:0]
		array[1] = can_tx_buffer_control.buf[cid][bid].body.buffer[5];
		// 23 ～ 16 TMDB6 [7:0]
		array[2] = can_tx_buffer_control.buf[cid][bid].body.buffer[6];
		// 31 ～ 24 TMDB7 [7:0]
		array[3] = can_tx_buffer_control.buf[cid][bid].body.buffer[7];
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		// 7 ～ 0 TMDB4 [7:0]
		can_tx_buffer_control.buf[cid][bid].body.buffer[4] = array[0];
		// 15 ～ 8 TMDB5 [7:0]
		can_tx_buffer_control.buf[cid][bid].body.buffer[5] = array[1];
		// 23 ～ 16 TMDB6 [7:0]
		can_tx_buffer_control.buf[cid][bid].body.buffer[6] = array[2];
		// 31 ～ 24 TMDB7 [7:0]
		can_tx_buffer_control.buf[cid][bid].body.buffer[7] = array[3];
	}
	return;
}

void can_register_mapping_io_RSCANnTMIECy(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnTMIECy(0), 4U);

	if (io_type == CanRegisterIo_Read) {
		data = can_tx_buffer_control.intr_enable[index];
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		can_tx_buffer_control.intr_enable[index] = data;
	}
	return;
}
