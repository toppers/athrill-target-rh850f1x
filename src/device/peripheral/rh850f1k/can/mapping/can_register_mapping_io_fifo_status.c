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

void can_register_reset_fifo_status(CanRegisterResetType type)
{
	if (type == CanRegisterResetType_MCU_RESET) {
		//nothing
	}
	else if (type == CanRegisterResetType_GLOBAL_RESET) {
		//nothing
	}
	else if (type == CanRegisterResetType_CHANNEL_RESET) {
		//nothing
	}

}

void can_register_mapping_io_RSCANnFESTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 txrxdata = 0x0;
	uint32 rxdata = 0x0;
	uint32 k = 0;
	uint32 x;
	CanChannelIdType cid;
	CanBufferIdxType bid;

	if (io_type == CanRegisterIo_Read) {
		for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
			for (bid = 0; bid < CAN_TXRX_CHANNEL_FIFO_NUM; bid++) {
				CAN_REG_UPDATE_REG_BIT(can_txrx_fifo_control.buf[cid][bid].status.txrx_empty, k, txrxdata);
				k++;
			}
		}
		for (x = 0; x < CAN_RX_FIFO_NUM; x++) {
			CAN_REG_UPDATE_REG_BIT(can_rx_fifo_control.fifo[x].status.rx_empty, x, rxdata);
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), ( (txrxdata << 8U) | rxdata ) );
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}
void can_register_mapping_io_RSCANnFFSTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 txrxdata = 0x0;
	uint32 rxdata = 0x0;
	uint32 k = 0;
	uint32 x;
	CanChannelIdType cid;
	CanBufferIdxType bid;

	if (io_type == CanRegisterIo_Read) {
		for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
			for (bid = 0; bid < CAN_TXRX_CHANNEL_FIFO_NUM; bid++) {
				CAN_REG_UPDATE_REG_BIT(can_txrx_fifo_control.buf[cid][bid].status.txrx_full, k, txrxdata);
				k++;
			}
		}
		for (x = 0; x < CAN_RX_FIFO_NUM; x++) {
			CAN_REG_UPDATE_REG_BIT(can_rx_fifo_control.fifo[x].status.rx_full, x, rxdata);
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), ( (txrxdata << 8U) | rxdata ) );
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}
void can_register_mapping_io_RSCANnFMSTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 txrxdata = 0x0;
	uint32 rxdata = 0x0;
	uint32 k = 0;
	uint32 x;
	CanChannelIdType cid;
	CanBufferIdxType bid;

	if (io_type == CanRegisterIo_Read) {
		for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
			for (bid = 0; bid < CAN_TXRX_CHANNEL_FIFO_NUM; bid++) {
				CAN_REG_UPDATE_REG_BIT(can_txrx_fifo_control.buf[cid][bid].status.txrx_lost, k, txrxdata);
				k++;
			}
		}
		for (x = 0; x < CAN_RX_FIFO_NUM; x++) {
			CAN_REG_UPDATE_REG_BIT(can_rx_fifo_control.fifo[x].status.rx_lost, x, rxdata);
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), ( (txrxdata << 8U) | rxdata ) );
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}
void can_register_mapping_io_RSCANnRFISTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 rxdata = 0x0;
	uint32 x;

	if (io_type == CanRegisterIo_Read) {
		for (x = 0; x < CAN_RX_FIFO_NUM; x++) {
			CAN_REG_UPDATE_REG_BIT(can_rx_fifo_control.fifo[x].status.rx_intr, x, rxdata);
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), ( rxdata ) );
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}

	return;
}
void can_register_mapping_io_RSCANnCFRISTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 txrxdata = 0x0;
	uint32 k = 0;
	CanChannelIdType cid;
	CanBufferIdxType bid;

	if (io_type == CanRegisterIo_Read) {
		for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
			for (bid = 0; bid < CAN_TXRX_CHANNEL_FIFO_NUM; bid++) {
				CAN_REG_UPDATE_REG_BIT(can_txrx_fifo_control.buf[cid][bid].status.rx_intr, k, txrxdata);
				k++;
			}
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), ( txrxdata ) );
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}
void can_register_mapping_io_RSCANnCFTISTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 txrxdata = 0x0;
	uint32 k = 0;
	CanChannelIdType cid;
	CanBufferIdxType bid;

	if (io_type == CanRegisterIo_Read) {
		for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
			for (bid = 0; bid < CAN_TXRX_CHANNEL_FIFO_NUM; bid++) {
				CAN_REG_UPDATE_REG_BIT(can_txrx_fifo_control.buf[cid][bid].status.tx_intr, k, txrxdata);
				k++;
			}
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), ( txrxdata ) );
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}
	return;
}
