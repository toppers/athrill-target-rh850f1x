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
#include "can_fifo.h"
#include "can_register_mapping_reset.h"

void can_register_reset_rx_fifo(CanRegisterResetType type)
{
	uint32 i;

	if (type == CanRegisterResetType_MCU_RESET) {
		for (i = 0; i < CAN_RX_FIFO_NUM; i++) {
			can_rx_fifo_control.fifo[i].config.intr_timing_cnt = 0U;
			can_rx_fifo_control.fifo[i].config.enable_intr_everytime = FALSE;
			can_rx_fifo_control.fifo[i].config.quelen = 0U;
			can_rx_fifo_control.fifo[i].config.enable_intr = FALSE;
			can_rx_fifo_control.fifo[i].config.enable_fifo = FALSE;
			(void)can_rxfifo_clear(i);
			can_rx_fifo_control.fifo[i].status.rx_intr = FALSE;
			can_rx_fifo_control.fifo[i].status.rx_lost = FALSE;
			can_rx_fifo_control.fifo[i].status.rx_full = FALSE;
			can_rx_fifo_control.fifo[i].status.rx_empty = TRUE;
			memset(&can_rx_fifo_control.fifo[i].buffer[0], 0, sizeof(CanRxBufferType) * CAN_RX_FIFO_QUEUE_MAX);
		}

	}
	else if (type == CanRegisterResetType_GLOBAL_RESET) {
		for (i = 0; i < CAN_RX_FIFO_NUM; i++) {
			can_rx_fifo_control.fifo[i].config.enable_fifo = FALSE;
			(void)can_rxfifo_clear(i);
			can_rx_fifo_control.fifo[i].status.rx_intr = FALSE;
			can_rx_fifo_control.fifo[i].status.rx_lost = FALSE;
			can_rx_fifo_control.fifo[i].status.rx_full = FALSE;
			can_rx_fifo_control.fifo[i].status.rx_empty = TRUE;
		}
	}
	else if (type == CanRegisterResetType_CHANNEL_RESET) {
		// nothing
	}

}
void can_register_mapping_io_RSCANnRFCCx(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnRFCCx(0), 4U);

	if (io_type == CanRegisterIo_Read) {
		// 15 ～ 13 RFIGCV [2:0]: モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
		// 12 RFIM: モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
		// 10 ～ 8 RFDC [2:0]: モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
		// 1 RFIE: モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
		// 0 RFE モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		//モデルへの同期処理
		// 15 ～ 13 RFIGCV [2:0]
		can_rx_fifo_control.fifo[index].config.intr_timing_cnt = CAN_REG_GET_MASK_DATA(data >> 13U, 0x3);
		// 12 RFIM
		CAN_REG_UPDATE_MODEL_FLAG(data, 12U, can_rx_fifo_control.fifo[index].config.enable_intr_everytime);
		// 10 ～ 8 RFDC [2:0]
		can_rx_fifo_control.fifo[index].config.quelen = CAN_REG_GET_MASK_DATA(data >> 8U, 0x7);
		// 1 RFIE
		CAN_REG_UPDATE_MODEL_FLAG(data, 1U, can_rx_fifo_control.fifo[index].config.enable_intr);
		// 0 RFE
		CAN_REG_UPDATE_MODEL_FLAG(data, 0U, can_rx_fifo_control.fifo[index].config.enable_fifo);
		if (can_rx_fifo_control.fifo[index].config.enable_fifo == FALSE) {
			(void)can_rxfifo_clear(index);
			can_rx_fifo_control.fifo[index].status.rx_empty = TRUE;
			can_rx_fifo_control.fifo[index].status.rx_full = FALSE;
		}
	}
	return;
}
void can_register_mapping_io_RSCANnRFSTSx(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnRFSTSx(0), 4U);

	if (io_type == CanRegisterIo_Read) {
		//15 ～ 8 RFMC[7:0]
		data |= (can_rx_fifo_control.fifo[index].fifo_control.count << 8U);
		//3 RFIF
		CAN_REG_UPDATE_REG_BIT(can_rx_fifo_control.fifo[index].status.rx_intr, 3U, data);
		//2 RFMLT
		CAN_REG_UPDATE_REG_BIT(can_rx_fifo_control.fifo[index].status.rx_lost, 2U, data);
		//1 RFFLL
		CAN_REG_UPDATE_REG_BIT(can_rx_fifo_control.fifo[index].status.rx_full, 1U, data);
		//0 RFEMPL
		CAN_REG_UPDATE_REG_BIT(can_rx_fifo_control.fifo[index].status.rx_empty, 0U, data);
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		//モデルへの同期処理
		//3 RFIF
		if ((data & (1U << 3U)) == 0x00) {
			can_rx_fifo_control.fifo[index].status.rx_intr = FALSE;
		}
		//2 RFMLT
		if ((data & (1U << 2U)) == 0x00) {
			can_rx_fifo_control.fifo[index].status.rx_lost = FALSE;
		}
	}
	return;
}
void can_register_mapping_io_RSCANnRFPCTRx(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnRFPCTRx(0), 4U);

	if (io_type == CanRegisterIo_Read) {
		// write only
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		//モデルへの同期処理
		if (CAN_REG_GET_MASK_DATA(data, 0xFF) == 0xFF) {
			(void)can_rxfifo_next_rxdata(index);
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), 0x0);
	}
	return;
}
void can_register_mapping_io_RX_FIFO(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 addr = CAN_REG_GET_ALIGNED_ADDR(arg->address);

	switch (addr & 0x0000000F) {
	case 0x0:
		can_register_mapping_io_RSCANnRFIDx(io_type, arg);
		break;
	case 0x04:
		can_register_mapping_io_RSCANnRFPTRx(io_type, arg);
		break;
	case 0x8:
		can_register_mapping_io_RSCANnRFDF0x(io_type, arg);
		break;
	case 0xC:
		can_register_mapping_io_RSCANnRFDF1x(io_type, arg);
		break;
	default:
		break;
	}
	return;
}

void can_register_mapping_io_RSCANnRFIDx(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	Std_ReturnType err;
	CanDataType rxdata;
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnRFIDx(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		err = can_rxfifo_peek_rxdata(index, &rxdata);
		if (err == STD_E_OK) {
			// 31 RFIDE
			CAN_REG_UPDATE_REG_BIT(rxdata.ide, 31U, data);
			// 30 RFRTR
			CAN_REG_UPDATE_REG_BIT(rxdata.rtr, 30U, data);
			// 28 ～ 0 RFID [28:0]
			data |= rxdata.id;
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}

void can_register_mapping_io_RSCANnRFPTRx(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	Std_ReturnType err;
	CanDataType rxdata;
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnRFPTRx(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		err = can_rxfifo_peek_rxdata(index, &rxdata);
		if (err == STD_E_OK) {
			// 31 ～ 28 RFDLC [3:0]
			data  |= (rxdata.dlc << 28U);
			// 27 ～ 16 RFPTR [11:0]
			data  |= (rxdata.label << 16U);
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}
void can_register_mapping_io_RSCANnRFDF0x(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	Std_ReturnType err;
	uint32 data = 0x0;
	uint8 *array = (uint8*)&data;
	CanDataType rxdata;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnRFDF0x(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		err = can_rxfifo_peek_rxdata(index, &rxdata);
		if (err == STD_E_OK) {
			// 7 ～ 0 RFDB0 [7:0]
			array[0] = rxdata.data[0];
			// 15 ～ 8 RFDB1 [7:0]
			array[1] = rxdata.data[1];
			// 23 ～ 16 RFDB2 [7:0]
			array[2] = rxdata.data[2];
			// 31 ～ 24 RFDB3 [7:0]
			array[3] = rxdata.data[3];
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}
void can_register_mapping_io_RSCANnRFDF1x(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	Std_ReturnType err;
	uint32 data;
	uint8 *array = (uint8*)&data;
	CanDataType rxdata;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnRFDF1x(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		err = can_rxfifo_peek_rxdata(index, &rxdata);
		if (err == STD_E_OK) {
			// 7 ～ 0 RFDB4 [7:0]
			array[0] = rxdata.data[4];
			// 15 ～ 8 RFDB5 [7:0]
			array[1] = rxdata.data[5];
			// 23 ～ 16 RFDB6 [7:0]
			array[2] = rxdata.data[6];
			// 31 ～ 24 RFDB7 [7:0]
			array[3] = rxdata.data[7];
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}
