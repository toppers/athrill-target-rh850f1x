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
#include "can_tx.h"

typedef struct {
	CanModeGlobalType 			*can_mode_global;
	CanTxRxFifoControlType		*can_tx_fifo;
	CanTxBufferControlType		*can_tx_buffer;
	struct {
		CanBufferIdxType bufidx;
		CanDataType txdata;
		CanTxHistoryDataType	history;
		Std_ReturnType (*get_txdata) (CanChannelIdType cid, CanBufferIdxType txbufno, CanDataType *txdata);
		Std_ReturnType (*notify_txdone) (CanChannelIdType cid, CanBufferIdxType txbufno);
	} txreq[CAN_CHANNEL_NUM];
} CanTxControlType;

static CanTxControlType can_tx_control;

static Std_ReturnType can_tx_select_buffer_prio_bufno(CanChannelIdType cid);
static Std_ReturnType can_tx_select_buffer_prio_standard(CanChannelIdType cid);
static Std_ReturnType can_tx_select_buffer(CanChannelIdType cid);
static Std_ReturnType can_tx_send(CanChannelIdType cid);

static CanBusOperationType *canbus_tx_operation;
static CanModeGlobalType *can_tx_mode_global;
void can_tx_init(CanBusOperationType *canbus_op)
{
	can_tx_mode_global = &can_mode_global;
	canbus_tx_operation = canbus_op;
	can_tx_control.can_mode_global = &can_mode_global;
	can_tx_control.can_tx_fifo = &can_txrx_fifo_control;
	can_tx_control.can_tx_buffer = &can_tx_buffer_control;
	return;
}

void can_tx_proc(void)
{
	Std_ReturnType err;
	CanChannelIdType cid;

	if (can_tx_mode_global->mode != GlobalMode_RUN) {
		return;
	}

	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		if (can_tx_mode_global->channel[cid].mode != ChannelMode_COMM) {
			continue;
		}
		if (can_tx_mode_global->channel[cid].comm_mode != ChannelModeComm_IDLE) {
			continue;
		}
		err = can_tx_select_buffer(cid);
		if (err != STD_E_OK) {
			continue;
		}
		can_mode_channel_event(&can_tx_mode_global->channel[cid], CanModeChannelEvent_COMM_TX_START);
		if (can_tx_mode_global->channel[cid].comm_mode != ChannelModeComm_TX) {
			continue;
		}
		(void)can_tx_send(cid);
	}
	return;
}

void can_tx_done(void)
{
	Std_ReturnType err;
	CanChannelIdType cid;

	if (can_tx_mode_global->mode != GlobalMode_RUN) {
		return;
	}

	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		if (can_tx_mode_global->channel[cid].mode != ChannelMode_COMM) {
			continue;
		}
		if (can_tx_mode_global->channel[cid].comm_mode != ChannelModeComm_TX) {
			continue;
		}
		if (canbus_tx_operation->tx.is_done(cid) == FALSE) {
			continue;
		}
		err = can_tx_control.txreq[cid].notify_txdone(cid, can_tx_control.txreq[cid].bufidx);
		if (err == STD_E_OK) {
			can_tx_control.txreq[cid].get_txdata = NULL;
			can_tx_control.txreq[cid].notify_txdone = NULL;
			can_mode_channel_event(&can_tx_mode_global->channel[cid], CanModeChannelEvent_COMM_TX_DONE);
		}
	}
	return;
}


static Std_ReturnType can_tx_select_buffer(CanChannelIdType cid)
{
	can_tx_control.txreq[cid].get_txdata = NULL;
	can_tx_control.txreq[cid].notify_txdone = NULL;
	if (can_tx_control.can_mode_global->config.enable_tx_bufno_first == TRUE) {
		return can_tx_select_buffer_prio_bufno(cid);
	}
	else {
		return can_tx_select_buffer_prio_standard(cid);
	}
}
/*
  * 送信要求があるバッファの中で、最も小さいバッファ番号
 * の送信バッファのメッセージが最初に送信されます。送受信 FIFO バッファが送信バッファ
 * にリンクしている場合は、リンク先の送信バッファ番号で判定されます。
 */
static Std_ReturnType can_tx_select_buffer_prio_bufno(CanChannelIdType cid)
{
	CanBufferIdType min_tx_bufno = CAN_TX_BUFFER_ID(CAN_CHANNEL_NUM, CAN_TX_CHANNEL_BUFFER_NUM);
	CanBufferIdxType bufno;
	bool is_tx_buffer = TRUE;

	/*
	 * 送信バッファ
	 */
	for (bufno = 0; bufno < CAN_TX_CHANNEL_BUFFER_NUM; bufno++) {
		if (can_tx_control.can_tx_buffer->buf[cid][bufno].ctrl.req_tx == TRUE) {
			min_tx_bufno = CAN_TX_BUFFER_ID(cid, bufno);
			break;
		}
	}
	/*
	 * 送受信FIFOバッファ
	 */
	for (bufno = 0; bufno < CAN_TXRX_CHANNEL_FIFO_NUM; bufno++) {
		if (can_tx_control.can_tx_fifo->buf[cid][bufno].config.mode != CAN_TXRX_FIFO_MODE_TX) {
			continue;
		}
		if (can_tx_control.can_tx_fifo->buf[cid][bufno].fifo_control.count <= 0) {
			continue;
		}
		if (CAN_TX_BUFFER_ID(cid, can_tx_control.can_tx_fifo->buf[cid][bufno].config.tx_bufno) < min_tx_bufno) {
			min_tx_bufno = can_tx_control.can_tx_fifo->buf[cid][bufno].config.tx_bufno;
			is_tx_buffer = FALSE;
		}
	}
	if (min_tx_bufno == CAN_TX_BUFFER_ID(CAN_CHANNEL_NUM, CAN_TX_CHANNEL_BUFFER_NUM)) {
		return STD_E_NOENT;
	}

	can_tx_control.txreq[cid].bufidx = CAN_TX_BUFFER_IDX(min_tx_bufno);
	if (is_tx_buffer == TRUE) {
		can_tx_control.txreq[cid].history.buftype = CAN_TX_HISTORY_BUFTYPE_BUFFER;
		can_tx_control.txreq[cid].history.bufno = min_tx_bufno;
		can_tx_control.txreq[cid].get_txdata = can_txbuffer_get_txdata;
		can_tx_control.txreq[cid].notify_txdone = can_txbuffer_notify_txdone;
	}
	else {
		can_tx_control.txreq[cid].history.buftype = CAN_TX_HISTORY_BUFTYPE_FIFO;
		can_tx_control.txreq[cid].history.bufno = ( (cid * CAN_TXRX_CHANNEL_FIFO_NUM) + bufno );
		can_tx_control.txreq[cid].get_txdata = can_txrxfifo_get_txdata;
		can_tx_control.txreq[cid].notify_txdone = can_txrxfifo_notify_txdone;
	}
	can_tx_control.txreq[cid].history.label = can_tx_control.txreq[cid].txdata.label;
	return STD_E_OK;
}
/*
 * アービトレーション仕様
 *
 * (1) IDの優先順位
 *     IDの値が小さい方が優先度が高い
 *
 * (2) data frame and remote frame 優先順位
 *     データフレームの方が優先度が高い
 *
 * (3) standard format と extended format の優先順位
 *     標準フォーマットの方が優先度が高い
 *
 */

static uint32 can_get_check_id(CanDataType *data)
{
	uint32 checkid;
	uint32 stdid = (data->id & 0x7FF);/* 0:10 */
	uint32 extid = (data->id & 0x1FFFF800); /* 13:30 */

	checkid = stdid;
	if (data->ide == TRUE) {
		checkid |= (1U << 11U); /* SRR */
		checkid |= (1U << 12U); /* IDE */
		checkid |= (extid << 13U); /* extid */
		if (data->rtr == TRUE) {
			checkid |= (1U << 31U); /* RTR */
		}
	}
	else {
		if (data->rtr == TRUE) {
			checkid |= (1U << 11U); /* RTR */
		}
	}
	return checkid;
}

static Std_ReturnType can_tx_select_buffer_prio_standard(CanChannelIdType cid)
{
	CanBufferIdxType candidate_bufno = -1;
	CanBufferIdxType bufno;
	uint32 min_id = -1;
	uint32 checkid;
	CanDataType data;
	uint8 buftype = -1;
	/*
	 * 送信バッファ
	 */
	for (bufno = 0; bufno < CAN_TX_CHANNEL_BUFFER_NUM; bufno++) {
		if (can_tx_control.can_tx_buffer->buf[cid][bufno].ctrl.req_tx == TRUE) {
			if (can_txbuffer_peek_txdata(cid, bufno, &data) != STD_E_OK) {
				continue;
			}
			checkid = can_get_check_id(&data);
			if (checkid < min_id) {
				min_id = checkid;
				candidate_bufno = bufno;
				buftype = CAN_TX_HISTORY_BUFTYPE_BUFFER;
			}
		}
	}
	/*
	 * 送受信FIFOバッファ
	 */
	for (bufno = 0; bufno < CAN_TXRX_CHANNEL_FIFO_NUM; bufno++) {
		if (can_txrxfifo_peek_txdata(cid, bufno, &data) != STD_E_OK) {
			continue;
		}
		checkid = can_get_check_id(&data);
		if (checkid < min_id) {
			min_id = checkid;
			candidate_bufno = bufno;
			buftype = CAN_TX_HISTORY_BUFTYPE_FIFO;
		}
	}
	if (can_tx_queue[cid].config.enable_queue == TRUE) {
		// 送信キュー
		for (bufno = 0; bufno < CAN_TX_CHANNEL_BUFFER_NUM; bufno++) {
			if (can_txque_peek_txdata(cid, bufno, &data) != STD_E_OK) {
				continue;
			}
			checkid = can_get_check_id(&data);
			if (checkid < min_id) {
				min_id = checkid;
				candidate_bufno = bufno;
				buftype = CAN_TX_HISTORY_BUFTYPE_QUEUE;
			}
		}
	}

	if (min_id == -1) {
		return STD_E_NOENT;
	}
	can_tx_control.txreq[cid].bufidx = candidate_bufno;
	if (buftype == CAN_TX_HISTORY_BUFTYPE_BUFFER) {
		can_tx_control.txreq[cid].history.buftype = CAN_TX_HISTORY_BUFTYPE_BUFFER;
		can_tx_control.txreq[cid].history.bufno = CAN_TX_BUFFER_ID(cid, candidate_bufno);
		can_tx_control.txreq[cid].get_txdata = can_txbuffer_get_txdata;
		can_tx_control.txreq[cid].notify_txdone = can_txbuffer_notify_txdone;
	}
	else if (buftype == CAN_TX_HISTORY_BUFTYPE_FIFO) {
		can_tx_control.txreq[cid].history.buftype = CAN_TX_HISTORY_BUFTYPE_FIFO;
		can_tx_control.txreq[cid].history.bufno = ( (cid * CAN_TXRX_CHANNEL_FIFO_NUM) + candidate_bufno );
		can_tx_control.txreq[cid].get_txdata = can_txrxfifo_get_txdata;
		can_tx_control.txreq[cid].notify_txdone = can_txrxfifo_notify_txdone;
	}
	else {
		can_tx_control.txreq[cid].history.buftype = CAN_TX_HISTORY_BUFTYPE_QUEUE;
		can_tx_control.txreq[cid].history.bufno = CAN_TX_BUFFER_ID(cid, candidate_bufno);
		can_tx_control.txreq[cid].get_txdata = can_txque_get_txdata;
		can_tx_control.txreq[cid].notify_txdone = can_txque_notify_txdone;
	}
	can_tx_control.txreq[cid].history.label = can_tx_control.txreq[cid].txdata.label;
	return STD_E_OK;
}

static Std_ReturnType can_tx_send(CanChannelIdType cid)
{
	Std_ReturnType err;

	err = can_tx_control.txreq[cid].get_txdata(cid, can_tx_control.txreq[cid].bufidx, &can_tx_control.txreq[cid].txdata);
	if (err != STD_E_OK) {
		printf("ERROR: can_tx_send()/get_txdata(): cid=%u bufidx=%u err=%d\n", cid, can_tx_control.txreq[cid].bufidx, err);
		return err;
	}
	err = canbus_tx_operation->tx.start_send(cid, &can_tx_control.txreq[cid].txdata);
	if (err != STD_E_OK) {
		printf("ERROR: can_tx_send()/start_send(): cid=%u bufidx=%u err=%d\n", cid, can_tx_control.txreq[cid].bufidx, err);
		return err;
	}
	can_txhl_put_thldata(cid, &can_tx_control.txreq[cid].history);
	return STD_E_OK;
}
