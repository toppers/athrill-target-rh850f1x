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
#include "can_tx_history.h"

CanTxHistoryType can_tx_history[CAN_CHANNEL_NUM];

Std_ReturnType can_txhl_put_thldata(CanChannelIdType cid, CanTxHistoryDataType *thldata)
{
	if (can_tx_history[cid].config.enable_history == FALSE) {
		return STD_E_INVALID;
	}
	if (can_tx_history[cid].fifo_control.count >= CAN_TX_HISTORY_BUFFER_NUM) {
		can_tx_history[cid].status.thl_ovr = TRUE;
		return STD_E_LIMIT;
	}
	can_tx_history[cid].buffer[can_tx_history[cid].fifo_control.woff] = *thldata;
	can_tx_history[cid].fifo_control.count++;
	can_tx_history[cid].fifo_control.woff++;
	if (can_tx_history[cid].fifo_control.woff >= CAN_TX_HISTORY_BUFFER_NUM) {
		can_tx_history[cid].fifo_control.woff = 0U;
	}

	if (can_tx_history[cid].config.select_intr_factor == TRUE) {
		can_tx_history[cid].status.thl_intr = TRUE;
	}
	else if (can_tx_history[cid].fifo_control.count == CAN_TX_HISTORY_INTR_BUFNUM) {
		can_tx_history[cid].status.thl_intr = TRUE;
	}

	if (can_tx_history[cid].fifo_control.count >= CAN_TX_HISTORY_BUFFER_NUM) {
		can_tx_history[cid].status.thl_full = TRUE;
	}
	can_tx_history[cid].status.thl_empty = FALSE;
	return STD_E_OK;
}

Std_ReturnType can_txhl_next_thldata(CanChannelIdType cid)
{
	if (can_tx_history[cid].config.enable_history == FALSE) {
		return STD_E_INVALID;
	}
	if (can_tx_history[cid].fifo_control.count <= 0) {
		return STD_E_NOENT;
	}
	can_tx_history[cid].fifo_control.count--;
	can_tx_history[cid].fifo_control.roff++;
	if (can_tx_history[cid].fifo_control.roff >= CAN_TX_HISTORY_BUFFER_NUM) {
		can_tx_history[cid].fifo_control.roff = 0U;
	}

	if (can_tx_history[cid].fifo_control.count <= 0) {
		can_tx_history[cid].status.thl_empty = TRUE;
	}
	can_tx_history[cid].status.thl_full = FALSE;
	return STD_E_OK;
}

Std_ReturnType can_txhl_peek_thldata(CanChannelIdType cid, CanTxHistoryDataType *thldata)
{
	if (can_tx_history[cid].config.enable_history == FALSE) {
		return STD_E_INVALID;
	}
	if (can_tx_history[cid].fifo_control.count <= 0) {
		return STD_E_NOENT;
	}
	*thldata = can_tx_history[cid].buffer[can_tx_history[cid].fifo_control.roff];
	return STD_E_OK;
}
