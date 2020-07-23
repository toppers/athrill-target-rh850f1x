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
#ifndef _CAN_REGISTER_H_
#define _CAN_REGISTER_H_

#define RSCANn_base							0xFFD00000

#define CAN_REG_RSCANnCmCFG(index)			( (RSCANn_base + 0x0000) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnCmCTR(index)			( (RSCANn_base + 0x0004) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnCmSTS(index)			( (RSCANn_base + 0x0008) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnCmERFL(index)			( (RSCANn_base + 0x000C) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnGCFG					( (RSCANn_base + 0x0084) )
#define CAN_REG_RSCANnGCTR					( (RSCANn_base + 0x0088) )
#define CAN_REG_RSCANnGSTS					( (RSCANn_base + 0x008C) )
#define CAN_REG_RSCANnGERFL					( (RSCANn_base + 0x0090) )
#define CAN_REG_RSCANnGTSC					( (RSCANn_base + 0x0094) )
#define CAN_REG_RSCANnGTINTSTS0				( (RSCANn_base + 0x0460) )
#define CAN_REG_RSCANnGTINTSTS1				( (RSCANn_base + 0x0464) )
#define CAN_REG_RSCANnGFDCFG				( (RSCANn_base + 0x0474) )
#define CAN_REG_RSCANnGAFLECTR				( (RSCANn_base + 0x0098) )
#define CAN_REG_RSCANnGAFLCFG0				( (RSCANn_base + 0x009C) )
#define CAN_REG_RSCANnGAFLCFG1				( (RSCANn_base + 0x00A0) )
#define CAN_REG_RSCANnGAFLIDj(index)		( (RSCANn_base + 0x0500) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnGAFLMj(index)			( (RSCANn_base + 0x0504) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnGAFLP0j(index)		( (RSCANn_base + 0x0508) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnGAFLP1j(index)		( (RSCANn_base + 0x050C) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnRMNB					( (RSCANn_base + 0x00A4) )
#define CAN_REG_RSCANnRMNDy(index)			( (RSCANn_base + 0x00A8) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnRMIDq(index)			( (RSCANn_base + 0x0600) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnRMPTRq(index)			( (RSCANn_base + 0x0604) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnRMDF0q(index)			( (RSCANn_base + 0x0608) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnRMDF1q(index)			( (RSCANn_base + 0x060C) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnRFCCx(index)			( (RSCANn_base + 0x00B8) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnRFSTSx(index)			( (RSCANn_base + 0x00D8) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnRFPCTRx(index)		( (RSCANn_base + 0x00F8) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnRFIDx(index)			( (RSCANn_base + 0x0E00) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnRFPTRx(index)			( (RSCANn_base + 0x0E04) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnRFDF0x(index)			( (RSCANn_base + 0x0E08) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnRFDF1x(index)			( (RSCANn_base + 0x0E0C) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnCFCCk(index)			( (RSCANn_base + 0x0118) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnCFSTSk(index)			( (RSCANn_base + 0x0178) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnCFPCTRk(index)		( (RSCANn_base + 0x01D8) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnCFIDk(index)			( (RSCANn_base + 0x0E80) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnCFPTRk(index)			( (RSCANn_base + 0x0E84) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnCFDF0k(index)			( (RSCANn_base + 0x0E88) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnCFDF1k(index)			( (RSCANn_base + 0x0E8C) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnFESTS					( (RSCANn_base + 0x0238) )
#define CAN_REG_RSCANnFFSTS					( (RSCANn_base + 0x023C) )
#define CAN_REG_RSCANnFMSTS					( (RSCANn_base + 0x0240) )
#define CAN_REG_RSCANnRFISTS				( (RSCANn_base + 0x0244) )
#define CAN_REG_RSCANnCFRISTS				( (RSCANn_base + 0x0248) )
#define CAN_REG_RSCANnCFTISTS			    ( (RSCANn_base + 0x024C) )
#define CAN_REG_RSCANnTMCp(index)			( (RSCANn_base + 0x0250) +  (0x01 * (index) ) )
#define CAN_REG_RSCANnTMSTSp(index)			( (RSCANn_base + 0x02D0) +  (0x01 * (index) ) )
#define CAN_REG_RSCANnTMIDp(index)			( (RSCANn_base + 0x1000) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnTMPTRp(index)			( (RSCANn_base + 0x1004) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnTMDF0p(index)			( (RSCANn_base + 0x1008) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnTMDF1p(index)			( (RSCANn_base + 0x100C) +  (0x10 * (index) ) )
#define CAN_REG_RSCANnTMIECy(index)			( (RSCANn_base + 0x0390) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnTMTRSTSy(index)		( (RSCANn_base + 0x0350) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnTMTARSTSy(index)		( (RSCANn_base + 0x0360) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnTMTCSTSy(index)		( (RSCANn_base + 0x0370) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnTMTASTSy(index)		( (RSCANn_base + 0x0380) +  (0x04 * (index) ) )

#define CAN_REG_RSCANnTXQCCm(index)			( (RSCANn_base + 0x03A0) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnTXQSTSm(index)		( (RSCANn_base + 0x03C0) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnTXQPCTRm(index)		( (RSCANn_base + 0x03E0) +  (0x04 * (index) ) )


#define CAN_REG_RSCANnTHLCCm(index)			( (RSCANn_base + 0x0400) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnTHLSTSm(index)		( (RSCANn_base + 0x0420) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnTHLPCTRm(index)		( (RSCANn_base + 0x0440) +  (0x04 * (index) ) )
#define CAN_REG_RSCANnTHLACCm(index)		( (RSCANn_base + 0x1800) +  (0x04 * (index) ) )

#define CAN_REG_UPDATE_REG_BIT(model_flag, bits, reg_data)	\
do {	\
	if ((model_flag) == TRUE) {	\
		(reg_data)	|=	(1U << (bits));	\
	}	\
	else {	\
		(reg_data)	&=	~(1U << (bits));	\
	}	\
} while (0)
#define CAN_REG_GET_MASK_DATA(reg_data, mask)	((reg_data) & (mask))

#define CAN_REG_UPDATE_MODEL_FLAG(reg_data, bits, model_flag)	\
do {	\
	if ( ((reg_data) & (1U << (bits))) != 0U) {	\
		(model_flag) = TRUE;	\
	}	\
	else {	\
		(model_flag) = FALSE;	\
	}	\
} while (0)

#define CAN_DATA_IS_BITSET(data, bits)							( ((data) << (bits)) != 0U )

#define CAN_REG_GET_ALIGNED_ADDR(reg_data)						((reg_data) & 0xFFFFFFFCU)

#define CAN_REG_GROUP_INDEX(addr, start_addr, group_size)		( ( CAN_REG_GET_ALIGNED_ADDR(addr) - (start_addr) ) / ((group_size)) )

#define CAN_REG_INDEX(addr, start_addr, group_size)				( ( (addr) - (start_addr) ) / ((group_size)) )
#endif /* _CAN_REGISTER_H_  */