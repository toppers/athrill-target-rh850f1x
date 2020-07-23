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
#ifndef _CAN_MODE_GLOBAL_H_
#define _CAN_MODE_GLOBAL_H_

#include "can_types.h"
#include "can_mode_channel.h"

typedef enum {
	GlobalMode_STOP = 0,
	GlobalMode_RESET,
	GlobalMode_RUN,
	GlobalMode_TEST,
} GlobalModeType;

#define CAN_MODE_GLOBAL_MODE_RUN		0x0
#define CAN_MODE_GLOBAL_MODE_RESET		0x1
#define CAN_MODE_GLOBAL_MODE_WAIT		0x2
typedef struct {
	GlobalModeType	mode;
	struct {
		/* not supported ITRCP [15:0] */
		/* not supported TSBTCS[2:0] */
		/* not supported TSSS */
		/* not supported TSP[3:0] */
		/* not supported TMTSCE */
		/* not supported EEFE */
		/* not supported DCS */
		/* not supported MME */
		/* not supported DRE */
		bool enable_dlc_check;		// DCE
		bool enable_tx_bufno_first; // TPRI
	} config; // RSCANnGCFG
	struct {
		/* not supported TSRST */
		bool	enable_tx_history;		// THLEIE
		bool	enable_dlc_error;		// DEIE
		bool	enable_fifo_msglost;	// MEIE
		bool	stop;					// GSLPR
		uint8	select_mode; 			// GMDC[1:0]
	} ctrl; // RSCANnGCTR
	struct {
		// not supported GRAMINIT
		bool	GSLPSTS;	// GSLPSTS
		bool	GHLTSTS;	// GHLTSTS
		bool	GRSTSTS;	// GRSTSTS
		bool	dlc_error;				// RSCANnGERFL.DEF
	} status; // RSCANnGSTS

	CanModeChannelType channel[CAN_CHANNEL_NUM];
} CanModeGlobalType;



typedef enum {
	CanModeGlobalEvent_CLR_STOP = 0,
	CanModeGlobalEvent_SET_STOP,
	CanModeGlobalEvent_SET_RUN,
	CanModeGlobalEvent_SET_RESET,
	CanModeGlobalEvent_SET_TEST,
} CanModeGlobalEventType;
extern void can_mode_global_event(CanModeGlobalType *gmode, CanModeGlobalEventType event);
extern void can_mode_global_channel_event(CanModeGlobalType *gmode, CanModeGlobalEventType event);

extern CanModeGlobalType can_mode_global;
#include "can_fifo.h"
static inline bool can_mode_global_is_msglost(void)
{
	CanChannelIdType cid;
	uint32 bid;
	uint32 x;

	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		for (bid = 0; bid < CAN_TXRX_CHANNEL_FIFO_NUM; bid++) {
			if (can_txrx_fifo_control.buf[cid][bid].status.txrx_lost == TRUE) {
				return TRUE;
			}
		}
	}
	for (x = 0; x < CAN_RX_FIFO_NUM; x++) {
		if (can_rx_fifo_control.fifo[x].status.rx_lost == TRUE) {
			return TRUE;
		}
	}
	return FALSE;
}

#endif /* _CAN_MODE_GLOBAL_H_ */
