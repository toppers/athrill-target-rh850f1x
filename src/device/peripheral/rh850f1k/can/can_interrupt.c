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
#include "can_interrupt.h"
#include "device.h"
#include "can_config.h"
#include "can_mode_global.h"
#include "can_fifo.h"
#include "can_buffer.h"
#include "can_tx_queue.h"
#include "can_tx_history.h"

/*
 * Interrupt Number
 *
 * 	グローバル割り込み
 *		・ 受信FIFO
 * 		・グローバルエラー(DEF, MES)
 * 	チャネル割り込み
 * 		・CANm送信
 * 			・送信完了
 * 			・送受信FIFO送信完了
 * 		・CANm送受信FIFO受信完了
 */
//22 ICRCANGERR0 FFFE EA2CH INTRCANGERR0 CAN グローバルエラー割り込み RSCAN0 レベル 1016H ○ ○ ○ +058H
#define CAN_GLOBAL_ERROR_INTNO		22U
//23 ICRCANGRECC0 FFFE EA2EH INTRCANGRECC0 CAN 受信 FIFO 割り込み RSCAN0 レベル 1017H ○ ○ ○ +05CH
#define CAN_GLOBAL_RXFIFO_INTNO		23U


typedef struct {
	uint32	txrxfifo_rxdone_intno;
	uint32	txdone_intno;
} CanChannelIntnoType;

static const CanChannelIntnoType can_channel_intno[CAN_CHANNEL_NUM] = {
		//25 ICRCAN0REC FFFE EA32H INTRCAN0REC CAN0 送受信 FIFO 受信完了割り込み RSCAN0 レベル 1019H ○ ○ ○ +064H
		//26 ICRCAN0TRX FFFE EA34H INTRCAN0TRX CAN0 送信割り込み RSCAN0 レベル 101AH ○ ○ ○ +068H
		{ .txrxfifo_rxdone_intno = 25U, .txdone_intno = 26U },		/* CH0 */
		//114 ICRCAN1REC FFFF B0E4H INTRCAN1REC CAN1 送受信 FIFO 受信完了割り込み RSCAN0 レベル 1072H ○ ○ ○ +1C8H
		//115 ICRCAN1TRX FFFF B0E6H INTRCAN1TRX CAN1 送信割り込み RSCAN0 レベル 1073H ○ ○ ○ +1CCH
		{ .txrxfifo_rxdone_intno = 114U, .txdone_intno = 115U },		/* CH1 */
		//218 ICRCAN2REC FFFF B1B4H INTRCAN2REC CAN2 送受信 FIFO 受信完了割り込み RSCAN0 レベル 10DAH ○ ○ ○ +368H
		//219 ICRCAN2TRX FFFF B1B6H INTRCAN2TRX CAN2 送信割り込み RSCAN0 レベル 10DBH ○ ○ ○ +36CH
		{ .txrxfifo_rxdone_intno = 218U, .txdone_intno = 219U },		/* CH2 */
		//221 ICRCAN3REC FFFF B1BAH INTRCAN3REC CAN3 送受信 FIFO 受信完了割り込み RSCAN0 レベル 10DDH ○ ○ ○ +374H
		//222 ICRCAN3TRX FFFF B1BCH INTRCAN3TRX CAN3 送信割り込み RSCAN0 レベル 10DEH ○ ○ ○ +378H
		{ .txrxfifo_rxdone_intno = 221U, .txdone_intno = 222U },		/* CH3 */
		//273 ICRCAN4REC FFFF B222H INTRCAN4REC CAN4 送受信 FIFO 受信完了割り込み RSCAN0 レベル 1111H ○ ○ ○ +444H
		//274 ICRCAN4TRX FFFF B224H INTRCAN4TRX CAN4 送信割り込み RSCAN0 レベル 1112H ○ ○ ○ +448H
		{ .txrxfifo_rxdone_intno = 273U, .txdone_intno = 274U },		/* CH4 */
		//288 ICRCAN5REC FFFF B240H INTRCAN5REC CAN5 送受信 FIFO 受信完了割り込み RSCAN0 レベル 1120H ○ ○ ○ +480H
		//289 ICRCAN5TRX FFFF B242H INTRCAN5TRX CAN5 送信割り込み RSCAN0 レベル 1121H ○ ○ ○ +484H
		{ .txrxfifo_rxdone_intno = 288U, .txdone_intno = 289U },		/* CH5 */
};

static void can_intr_txrxfifo_run(CanChannelIdType cid);
static void can_intr_tx_run(CanChannelIdType cid);
static void can_intr_global_run(void);

void can_intr_run(void)
{
	CanChannelIdType cid;
	can_intr_global_run();

	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		can_intr_txrxfifo_run(cid);
		can_intr_tx_run(cid);
	}
	return;
}


static bool can_intr_rxfifo_check(void);
static bool can_intr_glerror_check(void);
static void can_intr_global_run(void)
{
	if (can_intr_glerror_check() == TRUE) {
		device_raise_int(CAN_GLOBAL_ERROR_INTNO);
	}
	if (can_intr_rxfifo_check() == TRUE) {
		device_raise_int(CAN_GLOBAL_RXFIFO_INTNO);
	}
	return;
}

static void can_intr_txrxfifo_run(CanChannelIdType cid)
{
	uint32 k;
	for (k = 0; k < CAN_TXRX_CHANNEL_FIFO_NUM; k++) {
		if (can_txrx_fifo_control.buf[cid][k].config.enable_rx_intr == FALSE) {
			continue;
		}
		if (can_txrx_fifo_control.buf[cid][k].status.rx_intr == FALSE) {
			continue;
		}
		device_raise_int(can_channel_intno[cid].txrxfifo_rxdone_intno);
		return;
	}
	return;
}

static bool can_intr_tx_buffer_check(CanChannelIdType cid)
{
	uint32 bufno;

	for (bufno = 0; bufno < CAN_TX_CHANNEL_BUFFER_NUM; bufno++) {
		uint32 flag_id = CAN_TX_BUFFER_FLAG_ID(CAN_TX_BUFFER_ID(cid, bufno));
		uint32 flag_idx = CAN_TX_BUFFER_FLAG_IDX(CAN_TX_BUFFER_ID(cid, bufno));
		if ((can_tx_buffer_control.intr_enable[flag_id] & (1U << flag_idx)) == 0U) {
			continue;
		}
		if ((can_tx_buffer_control.buf[cid][bufno].status.result & 0x2) == 0) {
			continue;
		}
		return TRUE;
	}
	if ((can_tx_queue[cid].config.enable_intr == TRUE) && (can_tx_queue[cid].status.intr_req == TRUE)) {
		return TRUE;
	}
	return FALSE;
}

static bool can_intr_tx_fifo_check(CanChannelIdType cid)
{
	uint32 k;
	for (k = 0; k < CAN_TXRX_CHANNEL_FIFO_NUM; k++) {
		if (can_txrx_fifo_control.buf[cid][k].config.enable_tx_intr == FALSE) {
			continue;
		}
		if (can_txrx_fifo_control.buf[cid][k].status.tx_intr == FALSE) {
			continue;
		}
		return TRUE;
	}
	return FALSE;
}

static void can_intr_tx_run(CanChannelIdType cid)
{
	if (can_intr_tx_buffer_check(cid) == TRUE) {
		device_raise_int(can_channel_intno[cid].txdone_intno);
		return;
	}
	if (can_intr_tx_fifo_check(cid) == TRUE) {
		device_raise_int(can_channel_intno[cid].txdone_intno);
		return;
	}
	//送信履歴バッファ
	if ((can_tx_history[cid].config.enable_intr == TRUE) && (can_tx_history[cid].status.thl_intr == TRUE)) {
		device_raise_int(can_channel_intno[cid].txdone_intno);
		return;
	}
	return;
}

static bool can_intr_rxfifo_check(void)
{
	uint32 x;
	for (x = 0; x < CAN_RX_FIFO_NUM; x++) {
		if (can_rx_fifo_control.fifo[x].config.enable_intr == FALSE) {
			continue;
		}
		if (can_rx_fifo_control.fifo[x].status.rx_intr == FALSE) {
			continue;
		}
		return TRUE;
	}
	return FALSE;
}


static bool can_intr_global_dlcerr_check(void);
static bool can_intr_global_msglost_check(void);
static bool can_intr_glerror_check(void)
{
	if (can_intr_global_dlcerr_check() == TRUE) {
		return TRUE;
	}
	else if (can_intr_global_msglost_check() == TRUE) {
		return TRUE;
	}
	return FALSE;
}

static bool can_intr_global_dlcerr_check(void)
{
	if (can_mode_global.ctrl.enable_dlc_error == FALSE) {
		return FALSE;
	}
	if (can_mode_global.status.dlc_error == FALSE) {
		return FALSE;
	}
	return TRUE;
}
static bool can_intr_global_msglost_check(void)
{
	if (can_mode_global.ctrl.enable_fifo_msglost == FALSE) {
		return FALSE;
	}
	if (can_mode_global_is_msglost() == FALSE) {
		return FALSE;
	}
	return TRUE;
}
