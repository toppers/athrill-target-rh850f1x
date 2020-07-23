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
#ifndef _CAN_RULE_H_
#define _CAN_RULE_H_

#include "can_types.h"

typedef struct {
	struct {
		bool 	ide; 		// GAFLIDE
		bool 	rtr;		// GAFLRTR
		uint32	canid;		// GAFLID[28:0]
	} id;					// RSCANnGAFLIDj
	struct {
		bool	ide;		// GAFLIDEM
		bool	rtr;		// GAFLRTRM
		uint32	canid;		// GAFLIDM
	} mask;					// RSCANnGAFLMj
	struct {
		uint8	dlc;				// GAFLDLC[3:0]
		uint16	label;				// GAFLPTR[11:0]
		bool	enable_rxbuffer;	// GAFLRMV
		uint8	rxbuffer_number;	// GAFLRMDP[6:0]
	} filter_buffer;				// RSCANnGAFLP0j
	struct {
		/*
		 * k = 0 - (m * 3) + 2
		 */
		uint32	txrx_fifo;			// GAFLFDP[25:8]
		uint8	rx_fifo;			// GAFLFDP [7:0]
	} filter_fifo;					// RSCANnGAFLP1j
} CanRxRuleType;

#define CAN_TXRXFIFO_CHANNEL_ID(txrxfifo_id)	( (txrxfifo_id) / CAN_TXRX_CHANNEL_FIFO_NUM )
#define CAN_TXRXFIFO_BUFFER_ID(txrxfifo_id)		( (txrxfifo_id) % CAN_TXRX_CHANNEL_FIFO_NUM )
#define CAN_TXRXFIFO_BITMASK(cid, bid)			( 1U << ( (CAN_TXRX_CHANNEL_FIFO_NUM * (cid)) + (bid) ) )

typedef struct {
	CanRxRuleType	rule[CAN_RX_PAGE_RULE_NUM];
} CanRxPageType;

typedef struct {
	struct {
		bool	enable_write;	// AFLDAE
		uint8	page_number;	// AFLPN [4:0]
	} ctrl; // RSCANnGAFLECTR
	CanRxPageType	page[CAN_RX_RULE_TABLE_PAGE_NUM];
} CanRxRuleTableType;

#define CAN_RX_GET_PAGEID(rule_id)	( (rule_id) / CAN_RX_PAGE_RULE_NUM )
#define CAN_RX_GET_PAGEIDX(rule_id)	( (rule_id) % CAN_RX_PAGE_RULE_NUM )

typedef struct {
	uint32			channel_rx_rule_num[CAN_CHANNEL_NUM];
} CanRxRuleNumType;					// RSCANnGAFLCFG0, RSCANnGAFLCFG1

extern CanRxRuleNumType 	can_rx_number;
extern CanRxRuleTableType	can_rx_rule_table;

extern void can_rx_update_rule_table(void);

#endif /* _CAN_RULE_H_ */
