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

#ifndef _CAN_TX_QUEUE_H_
#define _CAN_TX_QUEUE_H_

#include "can_buffer.h"
#include "can_data.h"

typedef struct {
	bool	tx_req;
	CanDataType	data;
} CanTxQueueDataType;;

typedef struct {
	uint8	reqbufnum;
	CanTxQueueDataType	tx_data[CAN_TX_CHANNEL_BUFFER_NUM];
} CanTxQueueControlType;

typedef struct {
	struct {
		bool	intr_factor;	// 13 TXQIM
		bool	enable_intr;	// 12 TXQIE
		uint8	quelen;			// 11 ～ 8 TXQDC [3:0]
		bool	enable_queue; 	// 0 TXQE
	} config; // RSCANnTXQCCm
	struct {
		bool	intr_req;		// 2 TXQIF
		bool	is_full;		// 1 TXQFLL
		bool	is_empty;		// 0 TXQEMP
	} status; // RSCANnTXQSTSm
	CanTxQueueControlType	control;
} CanTxQueueType;

extern CanTxQueueType can_tx_queue[CAN_CHANNEL_NUM];

extern Std_ReturnType can_txque_peek_txdata(CanChannelIdType cid, CanBufferIdxType txbufno, CanDataType *txdata);
extern void can_txque_put_txdata(CanChannelIdType cid);
extern Std_ReturnType can_txque_get_txdata(CanChannelIdType cid, CanBufferIdxType txbufno, CanDataType *txdata);
extern Std_ReturnType can_txque_notify_txdone(CanChannelIdType cid, CanBufferIdxType txbufno);

#endif /* _CAN_TX_QUEUE_H_ */
