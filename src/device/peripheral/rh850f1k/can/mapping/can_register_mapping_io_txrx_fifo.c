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

static 	CanDataType txrx_tx_fifodata;


void can_register_reset_txrx_fifo(CanRegisterResetType type)
{
	CanChannelIdType cid;
	uint32 fifo_id;
	memset(&txrx_tx_fifodata, 0, sizeof(CanDataType));
	if (type == CanRegisterResetType_MCU_RESET) {
		for (cid = 0U; cid < CAN_CHANNEL_NUM; cid++) {
			for (fifo_id = 0U; fifo_id < CAN_FIFO_FLAG_NUM; fifo_id++) {
				can_txrx_fifo_control.buf[cid][fifo_id].config.tx_bufno = 0U;
				can_txrx_fifo_control.buf[cid][fifo_id].config.mode = 0U;
				can_txrx_fifo_control.buf[cid][fifo_id].config.intr_timing_cnt = 0U;
				can_txrx_fifo_control.buf[cid][fifo_id].config.enable_intr_everytime = FALSE;
				can_txrx_fifo_control.buf[cid][fifo_id].config.quelen = 0U;
				can_txrx_fifo_control.buf[cid][fifo_id].config.enable_tx_intr = FALSE;
				can_txrx_fifo_control.buf[cid][fifo_id].config.enable_rx_intr = FALSE;
				can_txrx_fifo_control.buf[cid][fifo_id].config.enable_fifo = FALSE;
				can_txrxfifo_clear(cid, fifo_id);
				can_txrx_fifo_control.buf[cid][fifo_id].status.tx_intr = FALSE;
				can_txrx_fifo_control.buf[cid][fifo_id].status.rx_intr = FALSE;
				can_txrx_fifo_control.buf[cid][fifo_id].status.txrx_lost = FALSE;
				can_txrx_fifo_control.buf[cid][fifo_id].status.txrx_full = FALSE;
				can_txrx_fifo_control.buf[cid][fifo_id].status.txrx_empty = TRUE;
				memset(&can_txrx_fifo_control.buf[cid][fifo_id].buffer.tx[0], 0, sizeof(CanTxBufferType) * CAN_TX_FIFO_QUEUE_MAX);
			}
		}
	}
	else {
		for (cid = 0U; cid < CAN_CHANNEL_NUM; cid++) {
			for (fifo_id = 0U; fifo_id < CAN_FIFO_FLAG_NUM; fifo_id++) {
				can_txrx_fifo_control.buf[cid][fifo_id].config.enable_fifo = FALSE;
				can_txrxfifo_clear(cid, fifo_id);
				can_txrx_fifo_control.buf[cid][fifo_id].status.tx_intr = FALSE;
				can_txrx_fifo_control.buf[cid][fifo_id].status.rx_intr = FALSE;
				can_txrx_fifo_control.buf[cid][fifo_id].status.txrx_lost = FALSE;
				can_txrx_fifo_control.buf[cid][fifo_id].status.txrx_full = FALSE;
				can_txrx_fifo_control.buf[cid][fifo_id].status.txrx_empty = TRUE;
			}
		}
	}

}
void can_register_reset_txrx_fifo_channel(CanChannelIdType cid)
{
	uint32 fifo_id;
	for (fifo_id = 0U; fifo_id < CAN_FIFO_FLAG_NUM; fifo_id++) {
		can_txrx_fifo_control.buf[cid][fifo_id].config.enable_fifo = FALSE;
		can_txrxfifo_clear(cid, fifo_id);
		can_txrx_fifo_control.buf[cid][fifo_id].status.tx_intr = FALSE;
		can_txrx_fifo_control.buf[cid][fifo_id].status.rx_intr = FALSE;
		can_txrx_fifo_control.buf[cid][fifo_id].status.txrx_lost = FALSE;
		can_txrx_fifo_control.buf[cid][fifo_id].status.txrx_full = FALSE;
		can_txrx_fifo_control.buf[cid][fifo_id].status.txrx_empty = TRUE;
	}

}

void can_register_mapping_io_RSCANnCFCCk(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnCFCCk(0), 4U);
	CanChannelIdType cid = index / CAN_TXRX_CHANNEL_FIFO_NUM;
	CanBufferIdxType bid = index % CAN_TXRX_CHANNEL_FIFO_NUM;

	if (io_type == CanRegisterIo_Read) {
		// 31 ～ 24 CFITT[7:0]: 未サポート
		// 23 ～ 20 CFTML[3:0]:モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
		// 19 CFITR: 未サポート
		// 18 CFITSS: 未サポート
		// 17 ～ 16 CFM[1:0]: モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
		// 15 ～ 13 CFIGCV [2:0]: モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
		// 12 CFIM: モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
		// 10 ～ 8 CFDC [2:0]: モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
		// 2 CFTXIE: モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
		// 1 CFRXIE: モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
		// 0 CFE: モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		//モデルへの同期処理
		// 23 ～ 20 CFTML[3:0]
		can_txrx_fifo_control.buf[cid][bid].config.tx_bufno = CAN_REG_GET_MASK_DATA(data >> 20U, 0xF);
		{
			/*
			 * RSCANnTMCp レジスタが次の条件を満たす場合は、 “00H” にしてください。
			 *  RSCANnCFCCk レジスタの CFTML[3:0] ビットで選択した送信バッファ番号に対応する
			 *  RSCANnTMCp レジスタ（ p = m × 16 + CFTML[3:0] ビットの値）
			 */
			can_tx_buffer_control.buf[cid][can_txrx_fifo_control.buf[cid][bid].config.tx_bufno].ctrl.req_tx = FALSE;
		}
		// 17 ～ 16 CFM[1:0]
		can_txrx_fifo_control.buf[cid][bid].config.mode = CAN_REG_GET_MASK_DATA(data >> 16U, 0x3);
		// 15 ～ 13 CFIGCV [2:0]
		can_txrx_fifo_control.buf[cid][bid].config.intr_timing_cnt = CAN_REG_GET_MASK_DATA(data >> 13U, 0x7);
		// 12 CFIM
		CAN_REG_UPDATE_MODEL_FLAG(data, 12U, can_txrx_fifo_control.buf[cid][bid].config.enable_intr_everytime);
		// 10 ～ 8 CFDC [2:0]
		can_txrx_fifo_control.buf[cid][bid].config.quelen = CAN_REG_GET_MASK_DATA(data >> 8U, 0x7);
		// 2 CFTXIE
		CAN_REG_UPDATE_MODEL_FLAG(data, 2U, can_txrx_fifo_control.buf[cid][bid].config.enable_tx_intr);
		// 1 CFRXIE
		CAN_REG_UPDATE_MODEL_FLAG(data, 1U, can_txrx_fifo_control.buf[cid][bid].config.enable_rx_intr);
		// 0 CFE
		CAN_REG_UPDATE_MODEL_FLAG(data, 0U, can_txrx_fifo_control.buf[cid][bid].config.enable_fifo);
		if (can_txrx_fifo_control.buf[cid][bid].config.enable_fifo == FALSE) {
			(void)can_txrxfifo_clear(cid, bid);
			can_txrx_fifo_control.buf[cid][bid].status.txrx_empty = TRUE;
			can_txrx_fifo_control.buf[cid][bid].status.txrx_full = FALSE;
		}
	}
	return;
}
void can_register_mapping_io_RSCANnCFSTSk(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnCFSTSk(0), 4U);
	CanChannelIdType cid = index / CAN_TXRX_CHANNEL_FIFO_NUM;
	CanBufferIdxType bid = index % CAN_TXRX_CHANNEL_FIFO_NUM;

	if (io_type == CanRegisterIo_Read) {
		//15 ～ 8 CFMC [7:0
		data |= (can_txrx_fifo_control.buf[cid][bid].fifo_control.count << 8U);
		//4 CFTXIF
		CAN_REG_UPDATE_REG_BIT(can_txrx_fifo_control.buf[cid][bid].status.tx_intr, 4U, data);
		//3 CFRXIF
		CAN_REG_UPDATE_REG_BIT(can_txrx_fifo_control.buf[cid][bid].status.rx_intr, 3U, data);
		//2 CFMLT
		CAN_REG_UPDATE_REG_BIT(can_txrx_fifo_control.buf[cid][bid].status.txrx_lost, 2U, data);
		//1 CFFLL
		CAN_REG_UPDATE_REG_BIT(can_txrx_fifo_control.buf[cid][bid].status.txrx_full, 1U, data);
		//0 CFEMP
		CAN_REG_UPDATE_REG_BIT(can_txrx_fifo_control.buf[cid][bid].status.txrx_empty, 0U, data);
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else {
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		//モデルへの同期処理
		//4 CFTXIF
		CAN_REG_UPDATE_MODEL_FLAG(data, 4U, can_txrx_fifo_control.buf[cid][bid].status.tx_intr);
		//3 CFRXIF
		CAN_REG_UPDATE_MODEL_FLAG(data, 3U, can_txrx_fifo_control.buf[cid][bid].status.rx_intr);
		//2 CFMLT
		CAN_REG_UPDATE_MODEL_FLAG(data, 2U, can_txrx_fifo_control.buf[cid][bid].status.txrx_lost);
	}
	return;
}
void can_register_mapping_io_RSCANnCFPCTRk(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnCFPCTRk(0), 4U);
	CanChannelIdType cid = index / CAN_TXRX_CHANNEL_FIFO_NUM;
	CanBufferIdxType bid = index % CAN_TXRX_CHANNEL_FIFO_NUM;

	if (io_type == CanRegisterIo_Read) {
		// write only
	}
	else { /* CanRegisterIo_Write */
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		//モデルへの同期処理
		if (CAN_REG_GET_MASK_DATA(data, 0xFF) == 0xFF) {
			if (can_txrx_fifo_control.buf[cid][bid].config.mode == CAN_TXRX_FIFO_MODE_TX) {
				(void)can_txrxfifo_put_txdata(cid, bid, &txrx_tx_fifodata);
				memset(&txrx_tx_fifodata, 0, sizeof(CanDataType));
			}
			else {
				(void)can_txrxfifo_next_rxdata(cid, bid);
			}
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), 0x0);
	}
	return;
}

void can_register_mapping_io_TXRX_FIFO(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 addr = CAN_REG_GET_ALIGNED_ADDR(arg->address);

	switch (addr & 0x0000000F) {
	case 0x0:
		can_register_mapping_io_RSCANnCFIDk(io_type, arg);
		break;
	case 0x04:
		can_register_mapping_io_RSCANnCFPTRk(io_type, arg);
		break;
	case 0x8:
		can_register_mapping_io_RSCANnCFDF0k(io_type, arg);
		break;
	case 0xC:
		can_register_mapping_io_RSCANnCFDF1k(io_type, arg);
		break;
	default:
		break;
	}
	return;
}
void can_register_mapping_io_RSCANnCFIDk(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	Std_ReturnType err;
	CanDataType txrxdata;
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnCFIDk(0), 16U);
	CanChannelIdType cid = index / CAN_TXRX_CHANNEL_FIFO_NUM;
	CanBufferIdxType bid = index % CAN_TXRX_CHANNEL_FIFO_NUM;

	if (io_type == CanRegisterIo_Read) {
		if (can_txrx_fifo_control.buf[cid][bid].config.mode == CAN_TXRX_FIFO_MODE_RX) {
			err = can_txrxfifo_peek_rxdata(cid, bid, &txrxdata);
			if (err == STD_E_OK) {
				// 31 CFIDE
				CAN_REG_UPDATE_REG_BIT(txrxdata.ide, 31U, data);
				// 30 CFRTR
				CAN_REG_UPDATE_REG_BIT(txrxdata.rtr, 30U, data);
				// 28 ～ 0 CFID [28:0]
				data |= txrxdata.id;
			}
			can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
		}
	}
	else { /* CanRegisterIo_Write */
		if (can_txrx_fifo_control.buf[cid][bid].config.mode == CAN_TXRX_FIFO_MODE_TX) {
			can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
			// 31 CFIDE
			CAN_REG_UPDATE_MODEL_FLAG(data, 31U, txrxdata.ide);
			// 30 CFRTR
			CAN_REG_UPDATE_MODEL_FLAG(data, 30U, txrxdata.rtr);
			// 28 ～ 0 CFID [28:0]
			txrx_tx_fifodata.id = CAN_REG_GET_MASK_DATA(data, 0x1FFFFFFF);
		}
	}
	return;
}
void can_register_mapping_io_RSCANnCFPTRk(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	Std_ReturnType err;
	CanDataType txrxdata;
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnCFPTRk(0), 16U);
	CanChannelIdType cid = index / CAN_TXRX_CHANNEL_FIFO_NUM;
	CanBufferIdxType bid = index % CAN_TXRX_CHANNEL_FIFO_NUM;

	if (io_type == CanRegisterIo_Read) {
		if (can_txrx_fifo_control.buf[cid][bid].config.mode == CAN_TXRX_FIFO_MODE_RX) {
			err = can_txrxfifo_peek_rxdata(cid, bid, &txrxdata);
			if (err == STD_E_OK) {
				// 31 ～ 28 CFDLC [3:0]
				data  |= (txrxdata.dlc << 28U);
				// 27 ～ 16 CFPTR [11:0]
				data  |= (txrxdata.label << 16U);
			}
			can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
		}
	}
	else { /* CanRegisterIo_Write */
		if (can_txrx_fifo_control.buf[cid][bid].config.mode == CAN_TXRX_FIFO_MODE_TX) {
			can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
			// 31 ～ 28 CFDLC [3:0]
			txrx_tx_fifodata.dlc = CAN_REG_GET_MASK_DATA(data >> 28U, 0xF);
			// 27 ～ 16 CFPTR [11:0]
			txrx_tx_fifodata.label = CAN_REG_GET_MASK_DATA(data >> 16U, 0xFFF);
		}
	}
	return;
}
void can_register_mapping_io_RSCANnCFDF0k(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	Std_ReturnType err;
	uint32 data = 0x0;
	uint8 *array = (uint8*)&data;
	CanDataType txrxdata;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnCFDF0k(0), 16U);
	CanChannelIdType cid = index / CAN_TXRX_CHANNEL_FIFO_NUM;
	CanBufferIdxType bid = index % CAN_TXRX_CHANNEL_FIFO_NUM;

	if (io_type == CanRegisterIo_Read) {
		if (can_txrx_fifo_control.buf[cid][bid].config.mode == CAN_TXRX_FIFO_MODE_RX) {
			err = can_txrxfifo_peek_rxdata(cid, bid, &txrxdata);
			if (err == STD_E_OK) {
				// 7 ～ 0 CFDB0 [7:0]
				array[0] = txrxdata.data[0];
				// 15 ～ 8 CFDB1 [7:0]
				array[1] = txrxdata.data[1];
				// 23 ～ 16 CFDB2 [7:0]
				array[2] = txrxdata.data[2];
				// 31 ～ 24 CFDB3 [7:0]
				array[3] = txrxdata.data[3];
			}
			can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
		}
	}
	else { /* CanRegisterIo_Write */
		if (can_txrx_fifo_control.buf[cid][bid].config.mode == CAN_TXRX_FIFO_MODE_TX) {
			can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
			// 7 ～ 0 CFDB0 [7:0]
			txrx_tx_fifodata.data[0] = array[0];
			// 15 ～ 8 CFDB1 [7:0]
			txrx_tx_fifodata.data[1] = array[1];
			// 23 ～ 16 CFDB2 [7:0]
			txrx_tx_fifodata.data[2] = array[2];
			// 31 ～ 24 CFDB3 [7:0]
			txrx_tx_fifodata.data[3] = array[3];
		}
	}
	return;
}
void can_register_mapping_io_RSCANnCFDF1k(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	Std_ReturnType err;
	uint32 data = 0x0;
	uint8 *array = (uint8*)&data;
	CanDataType txrxdata;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnCFDF1k(0), 16U);
	CanChannelIdType cid = index / CAN_TXRX_CHANNEL_FIFO_NUM;
	CanBufferIdxType bid = index % CAN_TXRX_CHANNEL_FIFO_NUM;

	if (io_type == CanRegisterIo_Read) {
		if (can_txrx_fifo_control.buf[cid][bid].config.mode == CAN_TXRX_FIFO_MODE_RX) {
			err = can_txrxfifo_peek_rxdata(cid, bid, &txrxdata);
			if (err == STD_E_OK) {
				// 7 ～ 0 CFDB4 [7:0]
				array[0] = txrxdata.data[4];
				// 15 ～ 8 CFDB5 [7:0]
				array[1] = txrxdata.data[5];
				// 23 ～ 16 CFDB6 [7:0]
				array[2] = txrxdata.data[6];
				// 31 ～ 24 CFDB7 [7:0]
				array[3] = txrxdata.data[7];
			}
			can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
		}
	}
	else { /* CanRegisterIo_Write */
		if (can_txrx_fifo_control.buf[cid][bid].config.mode == CAN_TXRX_FIFO_MODE_TX) {
			can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
			// 7 ～ 0 CFDB4 [7:0]
			txrx_tx_fifodata.data[4] = array[0];
			// 15 ～ 8 CFDB5 [7:0]
			txrx_tx_fifodata.data[5] = array[1];
			// 23 ～ 16 CFDB6 [7:0]
			txrx_tx_fifodata.data[6] = array[2];
			// 31 ～ 24 CFDB7 [7:0]
			txrx_tx_fifodata.data[7] = array[3];
		}
	}
	return;
}
