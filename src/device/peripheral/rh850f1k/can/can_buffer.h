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
#ifndef _CAN_BUFFER_H_
#define _CAN_BUFFER_H_

#include "can_types.h"
#include "can_data.h"
#include <string.h>

/************************************************************
 * RX BUFFER
 ************************************************************/

typedef struct {
	struct {
		bool	ide;	// RMIDE, RFIDE, CFIDE
		bool	rtr;	// RMRTR, RFRTR, CFRTR
		uint32	id;		// RMID [28:0], RFID [28:0], CFID [28:0]
	} id;				// RSCANnRMIDq, RSCANnRFIDx, RSCANnCFIDk
	struct {
		uint8	dlc;	// RMDLC [3:0], RFDLC [3:0], CFDLC [3:0]
		uint16	label;	// RMPTR [11:0], RFPTR [11:0], CFPTR [11:0]
						/* not supported RMTS [15:0], RFTS [15:0], CFTS [15:0] */
	} header;			// RSCANnRMPTRq, RSCANnRFPTRx, RSCANnCFPTRk
	struct {
		uint8	buffer[CAN_RX_BUFFER_DATA_SIZE];
	} body;				// RSCANnRMDF0q, RSCANnRMDF1q, RSCANnRFDF0x, RSCANnRFDF1x, RSCANnCFDF0k, RSCANnCFDF1k
} CanRxBufferType;

typedef struct {
	uint8				rx_buffer_num;							// RSCANnRMNB.NRXMB[7:0]
	uint32				rx_msgin_flag[CAN_RX_MSGIN_FLAG_NUM];	// RSCANnRMNDy
	CanRxBufferType		buf[CAN_RX_BUFFER_NUM];
} CanRxBufferControlType;

#define CAN_RX_BUFFER_FLAG_ID(buffer_id)	((buffer_id) / CAN_RX_BUFFER_FLAG_BIT_NUM)
#define CAN_RX_BUFFER_FLAG_IDX(buffer_id)	((buffer_id) % CAN_RX_BUFFER_FLAG_BIT_NUM)


static inline void can_rxbuffer_set_data(CanRxBufferType *bufferp, CanDataType *rxdata)
{
	bufferp->id.id = rxdata->id;
	bufferp->id.ide = rxdata->ide;
	bufferp->id.rtr = rxdata->rtr;
	bufferp->header.label = rxdata->label;
	bufferp->header.dlc = rxdata->dlc;
	memcpy(&bufferp->body.buffer[0], &rxdata->data[0], CAN_RX_BUFFER_DATA_SIZE);
	return;
}
static inline void can_rxbuffer_ref_data(CanRxBufferType *bufferp, CanDataType *rxdata)
{
	rxdata->id = bufferp->id.id;
	rxdata->ide = bufferp->id.ide;
	rxdata->rtr = bufferp->id.rtr;
	rxdata->label = bufferp->header.label;
	rxdata->dlc = bufferp->header.dlc;
	memcpy(&rxdata->data[0], &bufferp->body.buffer[0], CAN_RX_BUFFER_DATA_SIZE);
	return;
}

/************************************************************
 * TX BUFFER
 ************************************************************/

#define CAN_TX_BUFFER_RESULT_NONE		0x00
#define CAN_TX_BUFFER_RESULT_SENDING	0x00
#define CAN_TX_BUFFER_RESULT_ABORT		0x01
#define CAN_TX_BUFFER_RESULT_OK			0x02

typedef struct {
	struct {
						/* not supported TMOM */
						/* not supported TMTAR */
		bool 	req_tx;	// TMTR
	} ctrl; 			// RSCANnTMCp
	struct {
						/* not supported TMTARM */
		bool 	req_tx;	// TMTRM
		uint8	result;	// TMTRF[1:0]
		bool	sending; // TMTSTS
	} status;			// RSCANnTMSTSp
	struct {
		bool	ide;	// TMIDE, CFIDE
		bool	rtr;	// TMRTR, CFRTR
						/* not supported THLEN */
		uint32	id;		// TMID [28:0], CFID [28:0]
	} id;				// RSCANnTMIDp, RSCANnCFIDk
	struct {
		uint8	dlc;	// TMDLC [3:0], CFDLC [3:0]
		uint16	label;	// TMPTR [7:0], CFPTR [11:0]
	} header;			// RSCANnTMPTRp, RSCANnCFPTRk
	struct {
		uint8	buffer[CAN_TX_BUFFER_DATA_SIZE];
	} body;				// RSCANnTMDF0p, RSCANnTMDF1p, RSCANnCFDF0k, RSCANnCFDF1k
} CanTxBufferType;

static inline void can_txbuffer_set_data(CanTxBufferType *bufferp, CanDataType *txdata)
{
	bufferp->id.id = txdata->id;
	bufferp->id.ide = txdata->ide;
	bufferp->id.rtr = txdata->rtr;
	bufferp->header.dlc = txdata->dlc;
	bufferp->header.label = txdata->label;
	memcpy(&bufferp->body.buffer[0], &txdata->data[0], CAN_TX_BUFFER_DATA_SIZE);
	return;
}
static inline void can_txbuffer_ref_data(CanTxBufferType *bufferp, CanDataType *txdata)
{
	txdata->id = bufferp->id.id;
	txdata->ide = bufferp->id.ide;
	txdata->rtr = bufferp->id.rtr;
	txdata->dlc = bufferp->header.dlc;
	txdata->label = bufferp->header.label;
	memcpy(&txdata->data[0], &bufferp->body.buffer[0], CAN_TX_BUFFER_DATA_SIZE);
	return;
}

typedef struct {
	uint32					intr_enable[CAN_TX_BUFFER_FLAG_NUM];		// RSCANnTMIECy
	CanTxBufferType			buf[CAN_CHANNEL_NUM][CAN_TX_CHANNEL_BUFFER_NUM];
} CanTxBufferControlType;
#define CAN_TX_BUFFER_ID(cid, bufno)		( ( (cid) * CAN_TX_CHANNEL_BUFFER_NUM ) + (bufno) )
#define CAN_TX_BUFFER_IDX(buffer_id)		( (buffer_id) % CAN_TX_CHANNEL_BUFFER_NUM)
#define CAN_TX_BUFFER_FLAG_ID(buffer_id)	((buffer_id) / CAN_TX_BUFFER_FLAG_BIT_NUM)
#define CAN_TX_BUFFER_FLAG_IDX(buffer_id)	((buffer_id) % CAN_TX_BUFFER_FLAG_BIT_NUM)

extern void can_rxbuffer_init(void);
extern Std_ReturnType can_rxbuffer_put_rxdata(CanChannelIdType cid, CanBufferIdType rxbuffer_id, CanDataType *rxdata);
extern Std_ReturnType can_rxbuffer_get_rxdata(CanChannelIdType cid, CanBufferIdType rxbuffer_id, CanDataType *rxdata);

extern Std_ReturnType can_txbuffer_put_txdata(CanChannelIdType cid, CanBufferIdxType bufno, CanDataType *txdata);
extern Std_ReturnType can_txbuffer_get_txdata(CanChannelIdType cid, CanBufferIdxType bufno, CanDataType *txdata);
extern Std_ReturnType can_txbuffer_peek_txdata(CanChannelIdType cid, CanBufferIdxType bufno, CanDataType *txdata);
extern Std_ReturnType can_txbuffer_notify_txdone(CanChannelIdType cid, CanBufferIdxType bufno);


extern CanRxBufferControlType can_rx_buffer_control;
extern CanTxBufferControlType can_tx_buffer_control;

#endif /* _CAN_BUFFER_H_ */
