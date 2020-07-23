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
#include "can_mode_global.h"
#include "mapping/can_register_mapping_reset.h"

static void can_mode_global_channel_event_set_run(CanModeGlobalType *gmode);
static void can_mode_global_channel_event_set_test(CanModeGlobalType *gmode);
static void can_mode_global_channel_event_set_stop(CanModeGlobalType *gmode);
static void can_mode_global_channel_event_set_reset(CanModeGlobalType *gmode);

static void can_mode_channel_reset(CanChannelIdType cid)
{
	can_register_reset_channel(CanRegisterResetType_CHANNEL_RESET, cid);
	can_register_reset_tx_buffer_channel(cid);
	can_register_reset_txrx_fifo_channel(cid);
}


void can_mode_global_channel_event(CanModeGlobalType *gmode, CanModeGlobalEventType event)
{
	switch (event) {
	case CanModeGlobalEvent_CLR_STOP:
		break;
	case CanModeGlobalEvent_SET_STOP:
		can_mode_global_channel_event_set_stop(gmode);
		break;
	case CanModeGlobalEvent_SET_RUN:
		can_mode_global_channel_event_set_run(gmode);
		break;
	case CanModeGlobalEvent_SET_RESET:
		can_mode_global_channel_event_set_reset(gmode);
		break;
	case CanModeGlobalEvent_SET_TEST:
		can_mode_global_channel_event_set_test(gmode);
		break;
	default:
		break;
	}
	return;
}
static void can_mode_global_channel_event_set_run(CanModeGlobalType *gmode)
{
	//nothing to do
	return;
}

static void can_mode_global_channel_event_set_reset(CanModeGlobalType *gmode)
{
	CanChannelIdType cid;
	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		can_mode_channel_event(&gmode->channel[cid], CanModeChannelEvent_SET_RESET);
	}
	return;
}
static void can_mode_global_channel_event_set_stop(CanModeGlobalType *gmode)
{
	CanChannelIdType cid;
	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		can_mode_channel_event(&gmode->channel[cid], CanModeChannelEvent_SET_STOP);
	}
	return;
}
static void can_mode_global_channel_event_set_test(CanModeGlobalType *gmode)
{
	CanChannelIdType cid;
	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		can_mode_channel_event(&gmode->channel[cid], CanModeChannelEvent_SET_WAIT);
	}
	return;
}

static void can_mode_channel_event_clr_stop(CanModeChannelType *cmode);
static void can_mode_channel_event_set_stop(CanModeChannelType *cmode);
static void can_mode_channel_event_set_comm(CanModeChannelType *cmode);
static void can_mode_channel_event_set_wait(CanModeChannelType *cmode);
static void can_mode_channel_event_set_reset(CanModeChannelType *cmode);
static void can_mode_channel_event_comm_tx_start(CanModeChannelType *cmode);
static void can_mode_channel_event_comm_tx_done(CanModeChannelType *cmode);
static void can_mode_channel_event_comm_rx_start(CanModeChannelType *cmode);
static void can_mode_channel_event_comm_rx_done(CanModeChannelType *cmode);

void can_mode_channel_event(CanModeChannelType *cmode, CanModeChannelEventType event)
{
	switch (event) {
	case CanModeChannelEvent_CLR_STOP:
		can_mode_channel_event_clr_stop(cmode);
		break;
	case CanModeChannelEvent_SET_STOP:
		can_mode_channel_event_set_stop(cmode);
		break;
	case CanModeChannelEvent_SET_COMM:
		can_mode_channel_event_set_comm(cmode);
		break;
	case CanModeChannelEvent_SET_RESET:
		can_mode_channel_event_set_reset(cmode);
		break;
	case CanModeChannelEvent_SET_WAIT:
		can_mode_channel_event_set_wait(cmode);
		break;
	case CanModeChannelEvent_COMM_TX_START:
		can_mode_channel_event_comm_tx_start(cmode);
		break;
	case CanModeChannelEvent_COMM_TX_DONE:
		can_mode_channel_event_comm_tx_done(cmode);
		break;
	case CanModeChannelEvent_COMM_RX_START:
		can_mode_channel_event_comm_rx_start(cmode);
		break;
	case CanModeChannelEvent_COMM_RX_DONE:
		can_mode_channel_event_comm_rx_done(cmode);
		break;
	default:
		break;
	}
	return;
}
static void can_mode_channel_event_clr_stop(CanModeChannelType *cmode)
{
	switch (cmode->mode) {
	case ChannelMode_STOP:
		cmode->ctrl.stop = FALSE;
		can_mode_channel_reset(cmode->cid);
		cmode->status.can_comm = FALSE;
		cmode->status.receiving = FALSE;
		cmode->status.sending = FALSE;
		cmode->status.mode_stop = FALSE;
		cmode->status.mode_reset = TRUE;
		cmode->status.mode_wait = FALSE;;
		cmode->mode = ChannelMode_RESET;
		break;
	default:
		break;
	}
	return;
}
static void can_mode_channel_event_set_stop(CanModeChannelType *cmode)
{
	switch (cmode->mode) {
	case ChannelMode_RESET:
		cmode->ctrl.stop = TRUE;
		cmode->mode = ChannelMode_STOP;
		cmode->status.can_comm = FALSE;
		cmode->status.receiving = FALSE;
		cmode->status.sending = FALSE;
		cmode->status.mode_stop = TRUE;
		cmode->status.mode_reset = TRUE;
		cmode->status.mode_wait = FALSE;
		break;
	default:
		break;
	}
	return;
}
static void can_mode_channel_event_set_comm(CanModeChannelType *cmode)
{
	switch (cmode->mode) {
	case ChannelMode_RESET:
	case ChannelMode_WAIT:
		cmode->mode = ChannelMode_COMM;
		cmode->ctrl.select_mode = CAN_MODE_CHANNEL_MODE_COMM;
		/*
		 * 遷移後、 11 ビットの連続するレセシブを検出すると、 RSCAN1CmSTS レジス
		 * タ（ m = 6）の COMSTS フラグが “1”（通信可能な状態）になり、 CAN ネットワーク上でア
		 * クティブノードとして、送受信が許可されます。
		 */
		CAN_MODE_CHANNEL_COMM_MODE_SET_IDLE(cmode);
		break;
	default:
		break;
	}
	return;
}
static void can_mode_channel_event_set_wait(CanModeChannelType *cmode)
{
	switch (cmode->mode) {
	case ChannelMode_RESET:
	case ChannelMode_COMM:
		cmode->ctrl.select_mode = CAN_MODE_CHANNEL_MODE_WAIT;
		/*
		 * チャネル待機モードでチャネルのテスト関連レジスタの設定を行います。チャネル待機モー
		 * ドに遷移すると、チャネルの CAN 通信は停止します。
		 * 表 21.291 に CAN 通信中に CHMDC[1:0] ビットを “10B”（チャネル待機モード）に設定した
		 * ときの動作を示します。
		 */
		/* 表 21.291 :
		 * 受信の終了を待ってチャネル待機モードに遷移
		 * 送信の終了を待ってチャネル待機モードに遷移
		 */
		if (cmode->comm_mode == ChannelModeComm_IDLE) {
			cmode->mode = ChannelMode_WAIT;
			cmode->status.can_comm = FALSE;
			cmode->status.receiving = FALSE;
			cmode->status.sending = FALSE;
			cmode->status.mode_stop = FALSE;
			cmode->status.mode_reset = FALSE;
			cmode->status.mode_wait = TRUE;
		}
		break;
	default:
		break;
	}
	return;
}
static void can_mode_channel_event_set_reset(CanModeChannelType *cmode)
{
	switch (cmode->mode) {
	case ChannelMode_COMM:
	case ChannelMode_WAIT:
		cmode->ctrl.select_mode = CAN_MODE_CHANNEL_MODE_RESET;
		/*
		 * CAN 通信中に RSCAN1CmCTR レジスタの CHMDC[1:0] ビットを “01B”（チャネルリセット
		 * モード）にすると、送受信の完了を待たずに通信が中断され、チャネルリセットモードへ遷
		 * 移します。 表 21.291 に CAN 通信中に CHMDC[1:0] ビットを “01B”（チャネルリセットモー
		 * ド）に設定したときの動作を示します。
		 */
		/* 表 21.291 :
		 * 受信の終了を待たずにチャネルリセットモードに遷移
		 * 送信の終了を待たずにチャネルリセットモードに遷移
		 */
		can_mode_channel_reset(cmode->cid);
		cmode->mode = ChannelMode_RESET;
		CAN_MODE_CHANNEL_COMM_MODE_SET_IDLE(cmode);
		cmode->status.can_comm = FALSE;
		cmode->status.receiving = FALSE;
		cmode->status.sending = FALSE;
		cmode->status.mode_stop = FALSE;
		cmode->status.mode_reset = TRUE;
		cmode->status.mode_wait = FALSE;;
		break;
	default:
		break;
	}
	return;
}

static void can_mode_channel_event_comm_tx_start(CanModeChannelType *cmode)
{
	switch (cmode->mode) {
	case ChannelMode_COMM:
		if (cmode->comm_mode == ChannelModeComm_IDLE) {
			CAN_MODE_CHANNEL_COMM_MODE_SET_TX(cmode);
		}
		break;
	default:
		break;
	}
	return;
}
static void can_mode_channel_event_comm_tx_done(CanModeChannelType *cmode)
{
	switch (cmode->mode) {
	case ChannelMode_COMM:
		if (cmode->comm_mode == ChannelModeComm_TX) {
			CAN_MODE_CHANNEL_COMM_MODE_SET_IDLE(cmode);
			if (cmode->ctrl.select_mode == CAN_MODE_CHANNEL_MODE_WAIT) {
				cmode->mode = ChannelMode_WAIT;
				cmode->status.can_comm = FALSE;
				cmode->status.receiving = FALSE;
				cmode->status.sending = FALSE;
				cmode->status.mode_stop = FALSE;
				cmode->status.mode_reset = FALSE;
				cmode->status.mode_wait = TRUE;
			}
		}
		break;
	default:
		break;
	}
	return;
}
static void can_mode_channel_event_comm_rx_start(CanModeChannelType *cmode)
{
	switch (cmode->mode) {
	case ChannelMode_COMM:
		if (cmode->comm_mode == ChannelModeComm_IDLE) {
			CAN_MODE_CHANNEL_COMM_MODE_SET_RX(cmode);
		}
		break;
	default:
		break;
	}
	return;
}
static void can_mode_channel_event_comm_rx_done(CanModeChannelType *cmode)
{
	switch (cmode->mode) {
	case ChannelMode_COMM:
		if (cmode->comm_mode == ChannelModeComm_RX) {
			CAN_MODE_CHANNEL_COMM_MODE_SET_IDLE(cmode);
			if (cmode->ctrl.select_mode == CAN_MODE_CHANNEL_MODE_WAIT) {
				cmode->status.can_comm = FALSE;
				cmode->status.receiving = FALSE;
				cmode->status.sending = FALSE;
				cmode->status.mode_stop = FALSE;
				cmode->status.mode_reset = FALSE;
				cmode->status.mode_wait = TRUE;;
				cmode->mode = ChannelMode_WAIT;
			}
		}
		break;
	default:
		break;
	}
	return;
}
