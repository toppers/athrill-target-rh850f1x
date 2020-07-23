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
#ifndef _CAN_FIFO_H_
#define _CAN_FIFO_H_

#include "can_buffer.h"
#include "can_data.h"

typedef struct {
	uint32		count; 	// RSCANnRFSTSx.RFMC[7:0], RSCANnRFPCTRx.RFPC[7:0], RSCANnCFSTSk.CFMC [7:0]
	uint32		roff;	// RSCANnRFPCTRx.RFPC[7:0]
	uint32		woff;
} CanFifoControlType;
#define CAN_FIFO_CLEAR(fifo)	\
do { \
	(fifo)->count = 0U;	\
	(fifo)->roff = 0U;	\
	(fifo)->woff = 0U;	\
} while (0)

typedef struct {
	struct {
		uint8	intr_timing_cnt;		// RFIGCV [2:0]
		bool	enable_intr_everytime;	// RFIM
		uint8	quelen;					// RFDC [2:0]
		bool	enable_intr;			// RFIE
		bool	enable_fifo;			// RFE
	} config;							// RSCANnRFCCx
	struct {
		bool	rx_intr;				// RFIF
		bool	rx_lost;				// RFMLT
		bool	rx_full;				// RFFLL
		bool	rx_empty;				// RFEMP
	} status;							// RSCANnRFSTSx
	CanFifoControlType	fifo_control;
	CanRxBufferType	buffer[CAN_RX_FIFO_QUEUE_MAX];	// RSCANnRFIDx, RSCANnRFPTRx, RSCANnRFDF0x, RSCANnRFDF1x
} CanRxFifoBufferType;

typedef struct {
	CanRxFifoBufferType		fifo[CAN_RX_FIFO_NUM];
} CanRxFifoControlType;


#define CAN_TXRX_FIFO_MODE_RX			0x00
#define CAN_TXRX_FIFO_MODE_TX			0x01
#define CAN_TXRX_FIFO_MODE_GW			0x02

typedef struct {
	struct {
		// not supported CFITT[7:0]
		CanBufferIdxType	tx_bufno;				// CFTML[3:0]
		// not supported CFITR
		// not supported CFITSS
		uint8			mode;					// CFM[1:0]
		uint8			intr_timing_cnt;		// CFIGCV [2:0]
		bool			enable_intr_everytime;	// CFIM
		uint16			quelen;					// CFDC [2:0]
		bool			enable_tx_intr;			// CFTXIE
		bool			enable_rx_intr;			// CFRXIE
		bool			enable_fifo;			// CFE
	} config;							// RSCANnCFCCk
	struct {
		uint8	msg_num;				// CFMC [7:0]
		bool	tx_intr;				// CFTXIF
		bool	rx_intr;				// CFRXIF
		bool	txrx_lost;				// CFMLT
		bool	txrx_full;				// CFFLL
		bool	txrx_empty;				// CFEMP
	} status;							// RSCANnCFSTSk
	uint8 fifo_ptr;						// RSCANnCFPCTRk.CFPC [7:0]
	CanFifoControlType	fifo_control;
	union {
		CanRxBufferType	rx[CAN_RX_FIFO_QUEUE_MAX];
		CanTxBufferType	tx[CAN_TX_FIFO_QUEUE_MAX];
	} buffer; // RSCANnCFIDk, RSCANnCFIDk, RSCANnCFPTRk, RSCANnCFDF0k, RSCANnCFDF1k
} CanTxRxFifoBufferType;

typedef struct {
	CanTxRxFifoBufferType	buf[CAN_CHANNEL_NUM][CAN_TXRX_CHANNEL_FIFO_NUM];
} CanTxRxFifoControlType;


extern Std_ReturnType can_rxfifo_put_rxdata(CanChannelIdType cid, CanBufferIdType rxfifo_id, CanDataType *rxdata);
extern Std_ReturnType can_rxfifo_next_rxdata(CanBufferIdType rxfifo_id);
extern Std_ReturnType can_rxfifo_peek_rxdata(CanBufferIdType rxfifo_id, CanDataType *rxdata);
extern Std_ReturnType can_rxfifo_clear(CanBufferIdxType rxfifo_id);

extern Std_ReturnType can_txrxfifo_put_rxdata(CanChannelIdType cid, CanBufferIdxType txrxfifo_id, CanDataType *rxdata);
extern Std_ReturnType can_txrxfifo_next_rxdata(CanChannelIdType cid, CanBufferIdxType txrxfifo_id);
extern Std_ReturnType can_txrxfifo_peek_rxdata(CanChannelIdType cid, CanBufferIdxType txrxfifo_id, CanDataType *rxdata);

extern Std_ReturnType can_txrxfifo_put_txdata(CanChannelIdType cid, CanBufferIdxType txrxfifo_id, CanDataType *txdata);
extern Std_ReturnType can_txrxfifo_get_txdata(CanChannelIdType cid, CanBufferIdxType txrxfifo_id, CanDataType *txdata);
extern Std_ReturnType can_txrxfifo_peek_txdata(CanChannelIdType cid, CanBufferIdxType txrxfifo_id, CanDataType *txdata);
extern Std_ReturnType can_txrxfifo_clear(CanChannelIdType cid, CanBufferIdxType txrxfifo_id);
extern Std_ReturnType can_txrxfifo_notify_txdone(CanChannelIdType cid, CanBufferIdxType txrxfifo_id);

extern CanRxFifoControlType can_rx_fifo_control;
extern CanTxRxFifoControlType can_txrx_fifo_control;

#endif /* _CAN_FIFO_H_ */
