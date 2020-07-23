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
#include "can_buffer.h"
#include "can_tx_queue.h"
#include "can_tx_history.h"
#include "can_mode_global.h"
#include "can_register_mapping_reset.h"

void can_register_reset_global(CanRegisterResetType type)
{
	if (type == CanRegisterResetType_MCU_RESET) {
		can_mode_global.config.enable_dlc_check = FALSE;
		can_mode_global.config.enable_tx_bufno_first = FALSE;
		can_mode_global.ctrl.enable_fifo_msglost = FALSE;
		can_mode_global.ctrl.enable_dlc_error = FALSE;
		can_mode_global.ctrl.stop = TRUE;
		can_mode_global.ctrl.select_mode = CAN_MODE_GLOBAL_MODE_RESET;
		can_mode_global.mode = GlobalMode_STOP;
		can_mode_global.status.GSLPSTS = TRUE;
		can_mode_global.status.GHLTSTS = FALSE;
		can_mode_global.status.GRSTSTS = TRUE;
	}
	else if (type == CanRegisterResetType_GLOBAL_RESET) {
		can_mode_global.status.dlc_error = FALSE;
		can_mode_global.status.GHLTSTS = FALSE;
	}
	else if (type == CanRegisterResetType_CHANNEL_RESET) {
		/* nothing */
	}

	return;
}


void can_register_mapping_io_RSCANnGCFG(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;

	if (io_type == CanRegisterIo_Read) {
		// nothing to do
		// モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
	}
	else { /* CanRegisterIo_Write */
		//モデルへの同期処理
		can_register_read_data32(CAN_REG_RSCANnGCFG, &data);

		//1 DCE
		CAN_REG_UPDATE_MODEL_FLAG(data, 1U, can_mode_global.config.enable_dlc_check);
		//0 TPRI
		CAN_REG_UPDATE_MODEL_FLAG(data, 0U, can_mode_global.config.enable_tx_bufno_first);
	}
	return;
}
void can_register_mapping_io_RSCANnGCTR(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint32 current_gslpr;
	uint32 current_gmdc;
	static uint32 old_gslpr = 0x01; // reset value
	static uint32 old_gmdc = 0x01; // reset value

	can_register_read_data32(CAN_REG_RSCANnGCTR, &data);
	current_gslpr = CAN_REG_GET_MASK_DATA(data >> 2U, 0x1);
	current_gmdc = CAN_REG_GET_MASK_DATA(data, 0x3);
	if (io_type == CanRegisterIo_Write) {
		//10 THLEIE
		CAN_REG_UPDATE_MODEL_FLAG(data, 10U, can_mode_global.ctrl.enable_tx_history);
		//9 MEIE
		CAN_REG_UPDATE_MODEL_FLAG(data, 9U, can_mode_global.ctrl.enable_fifo_msglost);
		//8 DEIE
		CAN_REG_UPDATE_MODEL_FLAG(data, 8U, can_mode_global.ctrl.enable_dlc_error);

		// 2 GSLPR
		if (current_gslpr != old_gslpr) {
			if (current_gslpr != 0U) {
				can_mode_global_event(&can_mode_global, CanModeGlobalEvent_SET_STOP);
			}
			else {
				can_mode_global_event(&can_mode_global, CanModeGlobalEvent_CLR_STOP);
			}
		}
		// 1、 0 GMDC[1:0]
		if (current_gmdc != old_gmdc) {
			if (current_gmdc == CAN_MODE_GLOBAL_MODE_RUN) {
				can_mode_global_event(&can_mode_global, CanModeGlobalEvent_SET_RUN);
			}
			else if (current_gmdc == CAN_MODE_GLOBAL_MODE_RESET) {
				can_mode_global_event(&can_mode_global, CanModeGlobalEvent_SET_RESET);
			}
			else if (current_gmdc == CAN_MODE_GLOBAL_MODE_WAIT) {
				can_mode_global_event(&can_mode_global, CanModeGlobalEvent_SET_TEST);
			}
		}
		old_gmdc = current_gmdc;
		old_gslpr = current_gslpr;
	}
	// 2 GSLPR
	CAN_REG_UPDATE_REG_BIT(can_mode_global.ctrl.stop, 2U, data);
	// 1、 0 GMDC[1:0]
	data &= ~0x3;
	data |= can_mode_global.ctrl.select_mode;
	can_register_write_data32(CAN_REG_RSCANnGCTR, data);

	return;
}
void can_register_mapping_io_RSCANnGSTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	if (io_type == CanRegisterIo_Read) {
		// 0 GRSTSTS
		CAN_REG_UPDATE_REG_BIT(can_mode_global.status.GRSTSTS, 0U, data);
		// 1 GHLTSTS
		CAN_REG_UPDATE_REG_BIT(can_mode_global.status.GHLTSTS, 1U, data);
		// 2 GSLPSTS
		CAN_REG_UPDATE_REG_BIT(can_mode_global.status.GSLPSTS, 2U, data);
		// 3 GRAMINIT　固定値(0)とする
		can_register_write_data32(CAN_REG_RSCANnGSTS, data);
	}
	else {
		// nothing to do
	}
	return;
}
void can_register_mapping_io_RSCANnGERFL(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 i;
	uint32 data;

	can_register_read_data32(CAN_REG_RSCANnGERFL, &data);
	if (io_type == CanRegisterIo_Read) {
		//0 DEF
		CAN_REG_UPDATE_REG_BIT(can_mode_global.status.dlc_error, 0U, data);

		//1 MES
		CAN_REG_UPDATE_REG_BIT(can_mode_global_is_msglost(), 1U, data);

		for (i = 0; i < CAN_CHANNEL_NUM; i++) {
			if (can_tx_history[i].status.thl_ovr == TRUE) {
				//2 THLES
				data |= (1U << 2U);
				break;
			}
		}

		can_register_write_data32(CAN_REG_RSCANnGERFL, data);
	}
	else { /* CanRegisterIo_Write */
		//モデルへの同期処理

		//0 DEF
		if ((data & 0x1) == 0x0) {
			can_mode_global.status.dlc_error = FALSE;
		}
	}
	return;
}

bool can_register_mapping_io_RSCANnGTINTSTS_is_tx_intr(CanChannelIdType cid)
{
	CanBufferIdxType bid;

	for (bid = 0; bid < CAN_TX_CHANNEL_BUFFER_NUM; bid++) {
		uint32 flag_id = CAN_TX_BUFFER_FLAG_ID(CAN_TX_BUFFER_ID(cid, bid));
		uint32 flag_idx = CAN_TX_BUFFER_FLAG_IDX(CAN_TX_BUFFER_ID(cid, bid));
		if (CAN_DATA_IS_BITSET(can_tx_buffer_control.intr_enable[flag_id], flag_idx)) {
			if ((can_tx_buffer_control.buf[cid][bid].status.result & 0x10) != 0U) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

bool can_register_mappin_io_RSCANnGTINTSTS_is_CFTIFm(CanChannelIdType cid)
{
	CanBufferIdxType bid;

	for (bid = 0; bid < CAN_TXRX_CHANNEL_FIFO_NUM; bid++) {
		if (can_txrx_fifo_control.buf[cid][bid].config.enable_tx_intr == FALSE) {
			continue;
		}
		if (can_txrx_fifo_control.buf[cid][bid].status.tx_intr == TRUE) {
			return TRUE;
		}
	}
	return FALSE;
}
bool can_register_mappin_io_RSCANnGTINTSTS_is_TQIFm(CanChannelIdType cid)
{
	CanBufferIdxType bid;

	for (bid = 0; bid < CAN_TXRX_CHANNEL_FIFO_NUM; bid++) {
		if (can_tx_queue[cid].config.enable_intr == FALSE) {
			continue;
		}
		if (can_tx_queue[cid].status.intr_req == TRUE) {
			return TRUE;
		}
	}
	return FALSE;
}
bool can_register_mappin_io_RSCANnGTINTSTS_is_THIFm(CanChannelIdType cid)
{
	if (can_tx_history[cid].config.enable_intr == FALSE) {
		return FALSE;
	}
	if (can_tx_history[cid].status.thl_intr == TRUE) {
		return FALSE;
	}
	return FALSE;
}
void can_register_mapping_io_RSCANnGTINTSTS0(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;

	can_register_read_data32(CAN_REG_RSCANnGTINTSTS0, &data);
	if (io_type == CanRegisterIo_Read) {
		//0 TSIF0
		CAN_REG_UPDATE_REG_BIT(can_register_mapping_io_RSCANnGTINTSTS_is_tx_intr(0), 0U, data);
		//8 TSIF1
		CAN_REG_UPDATE_REG_BIT(can_register_mapping_io_RSCANnGTINTSTS_is_tx_intr(1), 8U, data);
		//16 TSIF2
		CAN_REG_UPDATE_REG_BIT(can_register_mapping_io_RSCANnGTINTSTS_is_tx_intr(2), 16U, data);
		//24 TSIF3
		CAN_REG_UPDATE_REG_BIT(can_register_mapping_io_RSCANnGTINTSTS_is_tx_intr(3), 24U, data);

		//3 CFTIF0
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_CFTIFm(0), 3U, data);
		//11 CFTIF1
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_CFTIFm(1), 11U, data);
		//19 CFTIF2
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_CFTIFm(2), 19U, data);
		//27 CFTIF3
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_CFTIFm(3), 27U, data);

		//2 TQIF0
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_TQIFm(0), 2U, data);
		//10 TQIF1
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_TQIFm(1), 10U, data);
		//18 TQIF2
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_TQIFm(2), 18U, data);
		//26 TQIF3
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_TQIFm(3), 26U, data);


		//4 THIF0
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_THIFm(0), 4U, data);
		//12 THIF1
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_THIFm(1), 12U, data);
		//20 THIF2
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_THIFm(2), 20U, data);
		//28 THIF3
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_THIFm(3), 28U, data);

		can_register_write_data32(CAN_REG_RSCANnGTINTSTS0, data);
	}
	else {
		//nothing to do
	}

	return;
}
void can_register_mapping_io_RSCANnGTINTSTS1(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;

	can_register_read_data32(CAN_REG_RSCANnGTINTSTS1, &data);
	if (io_type == CanRegisterIo_Read) {
		//0 TSIF4
		CAN_REG_UPDATE_REG_BIT(can_register_mapping_io_RSCANnGTINTSTS_is_tx_intr(4), 0U, data);
		//8 TSIF5
		CAN_REG_UPDATE_REG_BIT(can_register_mapping_io_RSCANnGTINTSTS_is_tx_intr(5), 8U, data);

		//3 CFTIF4
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_CFTIFm(4), 3U, data);
		//11 CFTIF5
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_CFTIFm(5), 11U, data);

		//2 TQIF4
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_TQIFm(4), 2U, data);
		//10 TQIF5
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_TQIFm(5), 10U, data);

		//4 THIF4
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_THIFm(4), 4U, data);
		//12 THIF5
		CAN_REG_UPDATE_REG_BIT(can_register_mappin_io_RSCANnGTINTSTS_is_THIFm(5), 12U, data);
		can_register_write_data32(CAN_REG_RSCANnGTINTSTS1, data);
	}
	else {
		//nothing to do
	}

	return;
}
