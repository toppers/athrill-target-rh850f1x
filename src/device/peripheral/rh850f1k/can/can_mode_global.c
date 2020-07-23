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

static void can_mode_global_event_clr_stop(CanModeGlobalType *gmode);
static void can_mode_global_event_set_stop(CanModeGlobalType *gmode);
static void can_mode_global_event_set_run(CanModeGlobalType *gmode);
static void can_mode_global_event_set_reset(CanModeGlobalType *gmode);
static void can_mode_global_event_set_test(CanModeGlobalType *gmode);

CanModeGlobalType can_mode_global;

static void can_mode_globa_reset(void)
{
	CanChannelIdType cid;

	can_register_reset_global(CanRegisterResetType_GLOBAL_RESET);
	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		can_register_reset_channel(CanRegisterResetType_GLOBAL_RESET, cid);
	}
	can_register_reset_fifo_status(CanRegisterResetType_GLOBAL_RESET);
	can_register_reset_rx_rule(CanRegisterResetType_GLOBAL_RESET);
	can_register_reset_rx_buffer(CanRegisterResetType_GLOBAL_RESET);
	can_register_reset_rx_fifo(CanRegisterResetType_GLOBAL_RESET);
	can_register_reset_tx_buffer(CanRegisterResetType_GLOBAL_RESET);
	can_register_reset_tx_buffer_status(CanRegisterResetType_GLOBAL_RESET);
	can_register_reset_txrx_fifo(CanRegisterResetType_GLOBAL_RESET);

}

void can_mode_global_event(CanModeGlobalType *gmode, CanModeGlobalEventType event)
{
	switch (event) {
	case CanModeGlobalEvent_CLR_STOP:
		can_mode_global_event_clr_stop(gmode);
		break;
	case CanModeGlobalEvent_SET_STOP:
		can_mode_global_event_set_stop(gmode);
		break;
	case CanModeGlobalEvent_SET_RUN:
		can_mode_global_event_set_run(gmode);
		break;
	case CanModeGlobalEvent_SET_RESET:
		can_mode_global_event_set_reset(gmode);
		break;
	case CanModeGlobalEvent_SET_TEST:
		can_mode_global_event_set_test(gmode);
		break;
	default:
		break;
	}
	return;
}
static void can_mode_global_event_clr_stop(CanModeGlobalType *gmode)
{
	switch (gmode->mode) {
	case GlobalMode_STOP:
		gmode->ctrl.stop = FALSE;
		can_mode_global_channel_event(gmode, CanModeGlobalEvent_CLR_STOP);
		can_mode_globa_reset();
		gmode->mode = GlobalMode_RESET;
		gmode->status.GSLPSTS = FALSE;
		gmode->status.GHLTSTS = FALSE;
		gmode->status.GRSTSTS = TRUE;
		break;
	default:
		break;
	}
	return;
}
static void can_mode_global_event_set_stop(CanModeGlobalType *gmode)
{
	switch (gmode->mode) {
	case GlobalMode_RESET:
		gmode->ctrl.stop = TRUE;
		/*
		 * 各 RSCAN1CmCTR レジスタの CSLPR ビットが “1”（チャネルストップモード）
		 * になります。その後、すべてのチャネルが強制的にチャネルストップモードへ遷移すると、
		 * グローバルストップモードになります。
		 */
		can_mode_global_channel_event(gmode, CanModeGlobalEvent_SET_STOP);
		gmode->mode = GlobalMode_STOP;
		gmode->status.GSLPSTS = TRUE;
		gmode->status.GHLTSTS = FALSE;
		gmode->status.GRSTSTS = TRUE;
		break;
	default:
		break;
	}
	return;
}
static void can_mode_global_event_set_run(CanModeGlobalType *gmode)
{
	switch (gmode->mode) {
	case GlobalMode_TEST:
	case GlobalMode_RESET:
		gmode->ctrl.select_mode = CAN_MODE_GLOBAL_MODE_RUN;
		can_mode_global_channel_event(gmode, CanModeGlobalEvent_SET_RUN);
		gmode->mode = GlobalMode_RUN;
		gmode->status.GSLPSTS = FALSE;
		gmode->status.GHLTSTS = FALSE;
		gmode->status.GRSTSTS = FALSE;
		break;
	default:
		break;
	}
	return;
}
static void can_mode_global_event_set_reset(CanModeGlobalType *gmode)
{
	switch (gmode->mode) {
	case GlobalMode_RUN:
	case GlobalMode_TEST:
		gmode->ctrl.select_mode = CAN_MODE_GLOBAL_MODE_RESET;
		/*
		 * RSCAN1GCTR レジスタの GMDC[1:0] ビットを “01B” に設定すると、各 RSCAN1CmCTR レ
		 * ジスタ（ m = 6）の CHMDC[1:0] ビットが “01B”（チャネルリセットモード）になります。す
		 * べてのチャネルが強制的にチャネルリセットモードへ遷移すると、グローバルリセットモー
		 * ドになります。すでにチャネルリセットモードまたはチャネルストップモードであるチャネ
		 * ルはモード遷移しません（ CHMDC[1:0] ビットがすでに “01B” に設定されているため）。
		 */
		can_mode_global_channel_event(gmode, CanModeGlobalEvent_SET_RESET);
		can_mode_globa_reset();
		gmode->mode = GlobalMode_RESET;
		gmode->status.GSLPSTS = FALSE;
		gmode->status.GHLTSTS = FALSE;
		gmode->status.GRSTSTS = TRUE;
		break;
	default:
		break;
	}
	return;
}
static void can_mode_global_event_set_test(CanModeGlobalType *gmode)
{
	switch (gmode->mode) {
	case GlobalMode_RESET:
	case GlobalMode_RUN:
		gmode->ctrl.select_mode = CAN_MODE_GLOBAL_MODE_WAIT;
		/*
		 * RSCAN1GCTR レジスタの GMDC[1:0] ビット を “10B” に設定すると、各 RSCAN1CmCTR レ
		 * ジスタの CHMDC[1:0] ビットが “10B”（チャネル待機モード）になります。すべてのチャネ
		 * ルが強制的にチャネル待機モードへ遷移すると、グローバルテストモードになります。チャ
		 * ネルストップモード、チャネルリセットモード、またはチャネル待機モードであるチャネル
		 * は、モード遷移しません。
		 */
		can_mode_global_channel_event(gmode, CanModeGlobalEvent_SET_TEST);
		gmode->mode = GlobalMode_TEST;
		gmode->status.GSLPSTS = FALSE;
		gmode->status.GHLTSTS = TRUE;
		gmode->status.GRSTSTS = FALSE;
		break;
	default:
		break;
	}
	return;
}
