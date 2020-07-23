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
#include "can_tx_history.h"
#include "can_register_mapping_reset.h"

void can_register_reset_tx_history(CanRegisterResetType type)
{
	CanChannelIdType cid;

	if (type == CanRegisterResetType_MCU_RESET) {
		for (cid = 0U; cid < CAN_CHANNEL_NUM; cid++) {
			can_tx_history[cid].status.thl_empty = TRUE;
		}
	}
	else {
		// not supported
	}
	return;
}

void can_register_mapping_io_RSCANnTHLCCm(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnTHLCCm(0), 4U);

	if (io_type == CanRegisterIo_Read) {
		// none
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		CAN_REG_UPDATE_MODEL_FLAG(data, 10U, can_tx_history[index].config.select_tx_buffer);
		CAN_REG_UPDATE_MODEL_FLAG(data, 9U, can_tx_history[index].config.select_intr_factor);
		CAN_REG_UPDATE_MODEL_FLAG(data, 8U, can_tx_history[index].config.enable_intr);
		CAN_REG_UPDATE_MODEL_FLAG(data, 0U, can_tx_history[index].config.enable_history);
	}
	return;
}

void can_register_mapping_io_RSCANnTHLSTSm(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnTHLSTSm(0), 4U);

	if (io_type == CanRegisterIo_Read) {
		//12 ～ 8 THLMC [4:0]
		data |= (can_tx_history[index].fifo_control.count << 8U);
		CAN_REG_UPDATE_REG_BIT(can_tx_history[index].status.thl_intr, 3U, data);
		CAN_REG_UPDATE_REG_BIT(can_tx_history[index].status.thl_ovr, 2U, data);
		CAN_REG_UPDATE_REG_BIT(can_tx_history[index].status.thl_full, 1U, data);
		CAN_REG_UPDATE_REG_BIT(can_tx_history[index].status.thl_empty, 0U, data);
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		if ( (data & (1U << 3U)) == 0x0) {
			can_tx_history[index].status.thl_intr = FALSE;
		}
		if ( (data & (1U << 2U)) == 0x0) {
			can_tx_history[index].status.thl_ovr = FALSE;
		}
	}
	return;
}
void can_register_mapping_io_RSCANnTHLPCTRm(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnTHLPCTRm(0), 4U);

	if (io_type == CanRegisterIo_Read) {
		/* write only */
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		//モデルへの同期処理
		if (CAN_REG_GET_MASK_DATA(data, 0xFF) == 0xFF) {
			(void)can_txhl_next_thldata(index);
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), 0x0);
	}
	return;
}
void can_register_mapping_io_RSCANnTHLACCm(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnTHLACCm(0), 4U);
	CanTxHistoryDataType thldata;

	if (io_type == CanRegisterIo_Read) {
		can_txhl_peek_thldata(index, &thldata);
		//15 ～ 8 TID[7:0]
		data |= (thldata.label << 8U);
		//6 ～ 3 BN[3:0]
		data |= (thldata.bufno << 3U);
		//2 ～ 0 BT[2:0]
		data |= (thldata.buftype << 0U);
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		/* read only */
	}
	return;
}
