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
#include "can_fifo.h"


#define CAN_RXFIFO_QUEUE_ID_NUM		8U
static const uint32 can_fifo_quelen[CAN_RXFIFO_QUEUE_ID_NUM] = {
	0,
	4,
	8,
	16,
	32,
	48,
	64,
	CAN_RX_FIFO_QUEUE_MAX,
};

CanRxFifoControlType can_rx_fifo_control;
CanTxRxFifoControlType can_txrx_fifo_control;

#define INTR_TIMING_CNT_MAX		7U
typedef struct {
	bool enable_intr;
	bool enable_intr_everytime;
	uint32 intr_timing_cnt;
	uint32 quelen_max;
} CanFifoRxInterruptParamType;
static bool can_fifo_rx_interrupt_occured(CanFifoControlType *fifo, CanFifoRxInterruptParamType *param)
{
	uint32 intr_count;
	if (param->enable_intr == FALSE) {
		return FALSE;
	}
	if (param->enable_intr_everytime == TRUE) {
		return TRUE;
	}
	if (param->intr_timing_cnt == INTR_TIMING_CNT_MAX) {
		intr_count = param->quelen_max;
	}
	else {
		intr_count = ( param->quelen_max * (param->intr_timing_cnt + 1U) ) / 8U;
	}
	if (fifo->count >= intr_count) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

Std_ReturnType can_rxfifo_put_rxdata(CanChannelIdType cid, CanBufferIdType rxfifo_id, CanDataType *rxdata)
{
	CanRxFifoBufferType *fifo = &can_rx_fifo_control.fifo[rxfifo_id];
	CanFifoRxInterruptParamType param;

	if (fifo->config.enable_fifo == FALSE) {
		return STD_E_PERM;
	}

	if (fifo->fifo_control.count >= can_fifo_quelen[fifo->config.quelen]) {
		fifo->status.rx_lost = TRUE;
		return STD_E_LIMIT;
	}
	can_rxbuffer_set_data(&fifo->buffer[fifo->fifo_control.woff], rxdata);

	fifo->fifo_control.count++;
	fifo->fifo_control.woff++;
	if (fifo->fifo_control.woff >= can_fifo_quelen[fifo->config.quelen]) {
		fifo->fifo_control.woff = 0U;
	}

	/*
	 * status
	 */
	if (fifo->fifo_control.count >= can_fifo_quelen[fifo->config.quelen]) {
		fifo->status.rx_full = TRUE;
	}
	fifo->status.rx_empty = FALSE;

	param.enable_intr = fifo->config.enable_fifo;
	param.enable_intr_everytime = fifo->config.enable_intr_everytime;
	param.intr_timing_cnt = fifo->config.intr_timing_cnt;
	param.quelen_max = can_fifo_quelen[fifo->config.quelen];
	if (can_fifo_rx_interrupt_occured(&fifo->fifo_control, &param) == TRUE) {
		fifo->status.rx_intr = TRUE;
	}


	return STD_E_OK;
}
Std_ReturnType can_rxfifo_next_rxdata(CanBufferIdType rxfifo_id)
{
	CanRxFifoBufferType *fifo = &can_rx_fifo_control.fifo[rxfifo_id];

	if (fifo->config.enable_fifo == FALSE) {
		return STD_E_PERM;
	}
	if (fifo->fifo_control.count == 0) {
		return STD_E_NOENT;
	}

	fifo->fifo_control.count--;
	fifo->fifo_control.roff++;
	if (fifo->fifo_control.roff >= can_fifo_quelen[fifo->config.quelen]) {
		fifo->fifo_control.roff = 0U;
	}
	/*
	 * status
	 */
	if (fifo->fifo_control.count < can_fifo_quelen[fifo->config.quelen]) {
		fifo->status.rx_full = FALSE;
	}
	if (fifo->fifo_control.count <= 0) {
		fifo->status.rx_empty = TRUE;
	}

	return STD_E_OK;
}

Std_ReturnType can_rxfifo_peek_rxdata(CanBufferIdType rxfifo_id, CanDataType *rxdata)
{
	CanRxFifoBufferType *fifo = &can_rx_fifo_control.fifo[rxfifo_id];

	if (fifo->config.enable_fifo == FALSE) {
		return STD_E_PERM;
	}
	if (fifo->fifo_control.count == 0) {
		return STD_E_NOENT;
	}

	can_rxbuffer_ref_data(&fifo->buffer[fifo->fifo_control.roff], rxdata);
	return STD_E_OK;
}


Std_ReturnType can_rxfifo_clear(CanBufferIdxType rxfifo_id)
{
	CanRxFifoBufferType *fifo = &can_rx_fifo_control.fifo[rxfifo_id];
	CAN_FIFO_CLEAR(&fifo->fifo_control);
	return STD_E_OK;
}

Std_ReturnType can_txrxfifo_put_rxdata(CanChannelIdType cid, CanBufferIdxType txrxfifo_id, CanDataType *rxdata)
{
	CanTxRxFifoBufferType *fifo = &can_txrx_fifo_control.buf[cid][txrxfifo_id];
	CanFifoRxInterruptParamType param;

	if (fifo->config.mode == CAN_TXRX_FIFO_MODE_TX) {
		printf("ERROR: can_txrxfifo_put_rxdata():invalid fifo_mode TX\n");
		return STD_E_PERM;
	}
	if (fifo->config.enable_fifo == FALSE) {
		printf("ERROR: can_txrxfifo_put_rxdata():fifo is not enabled\n");
		return STD_E_PERM;
	}

	if (fifo->fifo_control.count >= can_fifo_quelen[fifo->config.quelen]) {
		fifo->status.txrx_lost = TRUE;
		return STD_E_LIMIT;
	}
	can_rxbuffer_set_data(&fifo->buffer.rx[fifo->fifo_control.woff], rxdata);

	fifo->fifo_control.count++;
	fifo->fifo_control.woff++;
	if (fifo->fifo_control.woff >= can_fifo_quelen[fifo->config.quelen]) {
		fifo->fifo_control.woff = 0U;
	}

	/*
	 * status
	 */
	if (fifo->fifo_control.count >= can_fifo_quelen[fifo->config.quelen]) {
		fifo->status.txrx_full = TRUE;
	}
	fifo->status.txrx_empty = FALSE;

	param.enable_intr = fifo->config.enable_fifo;
	param.enable_intr_everytime = fifo->config.enable_intr_everytime;
	param.intr_timing_cnt = fifo->config.intr_timing_cnt;
	param.quelen_max = can_fifo_quelen[fifo->config.quelen];
	if (can_fifo_rx_interrupt_occured(&fifo->fifo_control, &param) == TRUE) {
		fifo->status.rx_intr = TRUE;
	}
	return STD_E_OK;
}

Std_ReturnType can_txrxfifo_peek_rxdata(CanChannelIdType cid, CanBufferIdxType txrxfifo_id, CanDataType *rxdata)
{
	CanTxRxFifoBufferType *fifo = &can_txrx_fifo_control.buf[cid][txrxfifo_id];

	if (fifo->config.mode == CAN_TXRX_FIFO_MODE_TX) {
		return STD_E_PERM;
	}
	if (fifo->config.enable_fifo == FALSE) {
		return STD_E_PERM;
	}
	if (fifo->fifo_control.count == 0) {
		return STD_E_NOENT;
	}

	can_rxbuffer_ref_data(&fifo->buffer.rx[fifo->fifo_control.roff], rxdata);
	return STD_E_OK;
}
Std_ReturnType can_txrxfifo_next_rxdata(CanChannelIdType cid, CanBufferIdxType txrxfifo_id)
{
	CanTxRxFifoBufferType *fifo = &can_txrx_fifo_control.buf[cid][txrxfifo_id];

	if (fifo->config.mode == CAN_TXRX_FIFO_MODE_TX) {
		return STD_E_PERM;
	}
	if (fifo->config.enable_fifo == FALSE) {
		return STD_E_PERM;
	}
	if (fifo->fifo_control.count == 0) {
		return STD_E_NOENT;
	}

	fifo->fifo_control.count--;
	fifo->fifo_control.roff++;
	if (fifo->fifo_control.roff >= can_fifo_quelen[fifo->config.quelen]) {
		fifo->fifo_control.roff = 0U;
	}
	/*
	 * status
	 */
	if (fifo->fifo_control.count < can_fifo_quelen[fifo->config.quelen]) {
		fifo->status.txrx_full = FALSE;
	}
	if (fifo->fifo_control.count <= 0) {
		fifo->status.txrx_empty = TRUE;
	}
	return STD_E_OK;
}

Std_ReturnType can_txrxfifo_put_txdata(CanChannelIdType cid, CanBufferIdxType txrxfifo_id, CanDataType *txdata)
{
	CanTxRxFifoBufferType *fifo = &can_txrx_fifo_control.buf[cid][txrxfifo_id];

	if (fifo->config.mode != CAN_TXRX_FIFO_MODE_TX) {
		return STD_E_PERM;
	}
	if (fifo->config.enable_fifo == FALSE) {
		return STD_E_PERM;
	}

	if (fifo->fifo_control.count >= can_fifo_quelen[fifo->config.quelen]) {
		fifo->status.txrx_lost = TRUE;
		return STD_E_LIMIT;
	}
	can_txbuffer_set_data(&fifo->buffer.tx[fifo->fifo_control.woff], txdata);

	fifo->fifo_control.count++;
	fifo->fifo_control.woff++;
	if (fifo->fifo_control.woff >= can_fifo_quelen[fifo->config.quelen]) {
		fifo->fifo_control.woff = 0U;
	}

	/*
	 * status
	 */
	if (fifo->fifo_control.count >= can_fifo_quelen[fifo->config.quelen]) {
		fifo->status.txrx_full = TRUE;
	}
	fifo->status.txrx_empty = FALSE;

	return STD_E_OK;
}

Std_ReturnType can_txrxfifo_get_txdata(CanChannelIdType cid, CanBufferIdxType txrxfifo_id, CanDataType *txdata)
{
	CanTxRxFifoBufferType *fifo = &can_txrx_fifo_control.buf[cid][txrxfifo_id];

	if (fifo->config.mode != CAN_TXRX_FIFO_MODE_TX) {
		return STD_E_PERM;
	}
	if (fifo->config.enable_fifo == FALSE) {
		return STD_E_PERM;
	}
	if (fifo->fifo_control.count == 0) {
		return STD_E_NOENT;
	}

	can_txbuffer_ref_data(&fifo->buffer.tx[fifo->fifo_control.roff], txdata);
	fifo->fifo_control.count--;
	fifo->fifo_control.roff++;
	if (fifo->fifo_control.roff >= can_fifo_quelen[fifo->config.quelen]) {
		fifo->fifo_control.roff = 0U;
	}
	/*
	 * status
	 */
	if (fifo->fifo_control.count < can_fifo_quelen[fifo->config.quelen]) {
		fifo->status.txrx_full = FALSE;
	}
	return STD_E_OK;
}

Std_ReturnType can_txrxfifo_peek_txdata(CanChannelIdType cid, CanBufferIdxType txrxfifo_id, CanDataType *txdata)
{
	CanTxRxFifoBufferType *fifo = &can_txrx_fifo_control.buf[cid][txrxfifo_id];

	if (fifo->config.mode != CAN_TXRX_FIFO_MODE_TX) {
		return STD_E_PERM;
	}
	if (fifo->config.enable_fifo == FALSE) {
		return STD_E_PERM;
	}
	if (fifo->fifo_control.count == 0) {
		return STD_E_NOENT;
	}

	can_txbuffer_ref_data(&fifo->buffer.tx[fifo->fifo_control.roff], txdata);
	return STD_E_OK;
}
Std_ReturnType can_txrxfifo_notify_txdone(CanChannelIdType cid, CanBufferIdxType txrxfifo_id)
{
	CanTxRxFifoBufferType *fifo = &can_txrx_fifo_control.buf[cid][txrxfifo_id];

	//CFTXIF フラグ
	if (fifo->config.enable_intr_everytime == TRUE) {
		fifo->status.tx_intr = TRUE;
	}
	else if (fifo->fifo_control.count <= 0) {
		fifo->status.tx_intr = TRUE;
	}
	//CFEが0
	if (fifo->config.enable_fifo == FALSE) {
		CAN_FIFO_CLEAR(&fifo->fifo_control);
	}

	//CFEMP フラグ
	//すべてのメッセージを送信したとき
	if (fifo->fifo_control.count <= 0) {
		fifo->status.txrx_empty = TRUE;
	}

	return STD_E_OK;
}

Std_ReturnType can_txrxfifo_clear(CanChannelIdType cid, CanBufferIdxType txrxfifo_id)
{
	CanTxRxFifoBufferType *fifo = &can_txrx_fifo_control.buf[cid][txrxfifo_id];
	CAN_FIFO_CLEAR(&fifo->fifo_control);
	return STD_E_OK;
}
