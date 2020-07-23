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
#include <string.h>

void can_register_reset_rx_buffer(CanRegisterResetType type)
{
	uint32 i;

	if (type == CanRegisterResetType_MCU_RESET) {
		can_rx_buffer_control.rx_buffer_num = 0U;

		for (i = 0; i < CAN_RX_MSGIN_FLAG_NUM; i++) {
			can_rx_buffer_control.rx_msgin_flag[i] = 0U;
		}

		for (i = 0; i < CAN_RX_BUFFER_NUM; i++) {
			can_rx_buffer_control.buf[i].id.ide = 0U;
			can_rx_buffer_control.buf[i].id.rtr = 0U;
			can_rx_buffer_control.buf[i].id.id = 0U;
			can_rx_buffer_control.buf[i].header.dlc = 0U;
			can_rx_buffer_control.buf[i].header.label = 0U;
			memset(can_rx_buffer_control.buf[i].body.buffer, 0, CAN_RX_BUFFER_DATA_SIZE);
		}
	}
	else if (type == CanRegisterResetType_GLOBAL_RESET) {
		for (i = 0; i < CAN_RX_MSGIN_FLAG_NUM; i++) {
			can_rx_buffer_control.rx_msgin_flag[i] = 0U;
		}
	}
	else if (type == CanRegisterResetType_CHANNEL_RESET) {
		// nothing
	}

}

void can_register_mapping_io_RSCANnRMNB(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;

	if (io_type == CanRegisterIo_Read) {
		// nothing to do
		// モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
	}
	else { /* CanRegisterIo_Write */
		//モデルへの同期処理
		can_register_read_data32(CAN_REG_RSCANnRMNB, &data);

		//7 ～ 0 NRXMB [7:0]
		can_rx_buffer_control.rx_buffer_num = CAN_REG_GET_MASK_DATA(data, 0xFFU);
	}
	return;
}
void can_register_mapping_io_RSCANnRMNDy(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnRMNDy(0), 4U);

	if (io_type == CanRegisterIo_Read) {
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), can_rx_buffer_control.rx_msgin_flag[index]);
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		can_rx_buffer_control.rx_msgin_flag[index] = data;
	}
	return;
}

void can_register_mapping_io_RX_BUFFER(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 addr = CAN_REG_GET_ALIGNED_ADDR(arg->address);

	switch (addr & 0x0000000F) {
	case 0x0:
		can_register_mapping_io_RSCANnRMIDq(io_type, arg);
		break;
	case 0x04:
		can_register_mapping_io_RSCANnRMPTRq(io_type, arg);
		break;
	case 0x8:
		can_register_mapping_io_RSCANnRMDF0q(io_type, arg);
		break;
	case 0xC:
		can_register_mapping_io_RSCANnRMDF1q(io_type, arg);
		break;
	default:
		break;
	}
	return;
}

void can_register_mapping_io_RSCANnRMIDq(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnRMIDq(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		// 31 RMIDE
		CAN_REG_UPDATE_REG_BIT(can_rx_buffer_control.buf[index].id.ide, 31U, data);
		// 30 RMRTR
		CAN_REG_UPDATE_REG_BIT(can_rx_buffer_control.buf[index].id.rtr, 30U, data);
		// 28 ～ 0 RMID [28:0]
		data |= can_rx_buffer_control.buf[index].id.id;

		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}
void can_register_mapping_io_RSCANnRMPTRq(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnRMPTRq(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		// 31 ～ 28 RMDLC [3:0]
		data  |= (can_rx_buffer_control.buf[index].header.dlc << 28U);
		// 27 ～ 16 RMPTR [11:0]
		data  |= (can_rx_buffer_control.buf[index].header.label << 16U);
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}
void can_register_mapping_io_RSCANnRMDF0q(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint8 *array = (uint8*)&data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnRMDF0q(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		// 7 ～ 0 RMDB0 [7:0]
		array[0] = can_rx_buffer_control.buf[index].body.buffer[0];
		// 15 ～ 8 RMDB1 [7:0]
		array[1] = can_rx_buffer_control.buf[index].body.buffer[1];
		// 23 ～ 16 RMDB2 [7:0]
		array[2] = can_rx_buffer_control.buf[index].body.buffer[2];
		// 31 ～ 24 RMDB3 [7:0]
		array[3] = can_rx_buffer_control.buf[index].body.buffer[3];
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}
void can_register_mapping_io_RSCANnRMDF1q(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint8 *array = (uint8*)&data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnRMDF1q(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		// 7 ～ 0 RMDB4 [7:0]
		array[0] = can_rx_buffer_control.buf[index].body.buffer[4];
		// 15 ～ 8 RMDB5 [7:0]
		array[1] = can_rx_buffer_control.buf[index].body.buffer[5];
		// 23 ～ 16 RMDB6 [7:0]
		array[2] = can_rx_buffer_control.buf[index].body.buffer[6];
		// 31 ～ 24 RMDB7 [7:0]
		array[3] = can_rx_buffer_control.buf[index].body.buffer[7];
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}
