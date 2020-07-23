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
#include "can_buffer.h"

CanRxBufferControlType can_rx_buffer_control;
CanTxBufferControlType can_tx_buffer_control;

void can_rxbuffer_init(void)
{
	return;
}

Std_ReturnType can_rxbuffer_put_rxdata(CanChannelIdType cid, CanBufferIdType rxbuffer_id, CanDataType *rxdata)
{
	uint32 flag_id = CAN_RX_BUFFER_FLAG_ID(rxbuffer_id);
	uint32 flag_idx = CAN_RX_BUFFER_FLAG_IDX(rxbuffer_id);
	can_rx_buffer_control.rx_msgin_flag[flag_id] |= (1U << flag_idx);

	can_rxbuffer_set_data(&can_rx_buffer_control.buf[rxbuffer_id], rxdata);

	return STD_E_OK;
}

Std_ReturnType can_rxbuffer_get_rxdata(CanChannelIdType cid, CanBufferIdType rxbuffer_id, CanDataType *rxdata)
{
	can_rxbuffer_ref_data(&can_rx_buffer_control.buf[rxbuffer_id], rxdata);
	return STD_E_OK;
}

Std_ReturnType can_txbuffer_put_txdata(CanChannelIdType cid, CanBufferIdxType bufno, CanDataType *txdata)
{
	can_txbuffer_set_data(&can_tx_buffer_control.buf[cid][bufno], txdata);
	return STD_E_OK;
}
Std_ReturnType can_txbuffer_get_txdata(CanChannelIdType cid, CanBufferIdxType bufno, CanDataType *txdata)
{

	can_txbuffer_ref_data(&can_tx_buffer_control.buf[cid][bufno], txdata);
	can_tx_buffer_control.buf[cid][bufno].status.sending = TRUE;
	return STD_E_OK;
}
Std_ReturnType can_txbuffer_peek_txdata(CanChannelIdType cid, CanBufferIdxType bufno, CanDataType *txdata)
{
	if (can_tx_buffer_control.buf[cid][bufno].ctrl.req_tx == TRUE) {
		can_txbuffer_ref_data(&can_tx_buffer_control.buf[cid][bufno], txdata);
		return STD_E_OK;
	}
	else {
		return STD_E_NOENT;
	}
}
Std_ReturnType can_txbuffer_notify_txdone(CanChannelIdType cid, CanBufferIdxType bufno)
{
	if (can_tx_buffer_control.buf[cid][bufno].ctrl.req_tx == TRUE) {
		can_tx_buffer_control.buf[cid][bufno].status.result = CAN_TX_BUFFER_RESULT_OK;
		can_tx_buffer_control.buf[cid][bufno].status.sending = FALSE;
		can_tx_buffer_control.buf[cid][bufno].ctrl.req_tx = FALSE;
	}

	return STD_E_OK;
}
