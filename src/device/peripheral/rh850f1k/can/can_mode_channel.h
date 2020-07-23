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
#ifndef _CAN_MODE_CHANNEL_H_
#define _CAN_MODE_CHANNEL_H_

typedef enum {
	ChannelMode_STOP = 0,
	ChannelMode_RESET,
	ChannelMode_WAIT,
	ChannelMode_COMM,
} ChannelModeType;

typedef enum {
	ChannelModeComm_IDLE = 0,
	ChannelModeComm_RX,
	ChannelModeComm_TX,
} ChannelModeCommType;


#define CAN_MODE_CHANNEL_MODE_COMM		0x0
#define CAN_MODE_CHANNEL_MODE_RESET		0x1
#define CAN_MODE_CHANNEL_MODE_WAIT		0x2
typedef struct {
	CanChannelIdType	cid;
	ChannelModeType 	mode;
	ChannelModeCommType	comm_mode;
	struct {
		bool	stop;			// CSLPR
		uint8	select_mode;	// CHMDC[1:0]
	} ctrl;	// RSCANnCmCTR
	struct {
		bool 	can_comm;		// COMSTS
		bool 	receiving;		// RECSTS
		bool 	sending;		// TRMSTS
		bool	mode_stop;		// CSLPSTS
		bool	mode_wait;		// CHLTSTS
		bool	mode_reset;		// CRSTSTS
	} status; // RSCANnCmSTS
} CanModeChannelType;

#define CAN_MODE_CHANNEL_COMM_MODE_SET_IDLE(cmode)	\
do { 	\
	(cmode)->comm_mode = ChannelModeComm_IDLE;	\
	(cmode)->status.can_comm = TRUE;	\
	(cmode)->status.sending = FALSE;	\
	(cmode)->status.receiving = FALSE;	\
	(cmode)->status.mode_stop = FALSE;	\
	(cmode)->status.mode_wait = FALSE;	\
	(cmode)->status.mode_reset = FALSE;	\
} while (0)
#define CAN_MODE_CHANNEL_COMM_MODE_SET_RX(cmode)	\
do { 	\
	(cmode)->comm_mode = ChannelModeComm_RX;	\
	(cmode)->status.can_comm = TRUE;	\
	(cmode)->status.sending = FALSE;	\
	(cmode)->status.receiving = TRUE;	\
	(cmode)->status.mode_stop = FALSE;	\
	(cmode)->status.mode_wait = FALSE;	\
	(cmode)->status.mode_reset = FALSE;	\
} while (0)
#define CAN_MODE_CHANNEL_COMM_MODE_SET_TX(cmode)	\
do { 	\
	(cmode)->comm_mode = ChannelModeComm_TX;	\
	(cmode)->status.can_comm = TRUE;	\
	(cmode)->status.sending = TRUE;	\
	(cmode)->status.receiving = FALSE;	\
	(cmode)->status.mode_stop = FALSE;	\
	(cmode)->status.mode_wait = FALSE;	\
	(cmode)->status.mode_reset = FALSE;	\
} while (0)

typedef enum {
	CanModeChannelEvent_CLR_STOP = 0,
	CanModeChannelEvent_SET_STOP,
	CanModeChannelEvent_SET_COMM,
	CanModeChannelEvent_SET_RESET,
	CanModeChannelEvent_SET_WAIT,
	CanModeChannelEvent_COMM_TX_START,
	CanModeChannelEvent_COMM_TX_DONE,
	CanModeChannelEvent_COMM_RX_START,
	CanModeChannelEvent_COMM_RX_DONE,
} CanModeChannelEventType;
extern void can_mode_channel_event(CanModeChannelType *cmode, CanModeChannelEventType event);


#endif /* _CAN_MODE_CHANNEL_H_ */
