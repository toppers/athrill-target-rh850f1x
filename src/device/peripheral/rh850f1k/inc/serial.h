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
#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "device.h"

/*
 *  RLIN3
 */
#define RLIN30_BASE 	0xffce2000
#define RLIN31_BASE 	0xffce2040
#define RLIN32_BASE 	0xffce2080
#define RLIN33_BASE 	0xffce20c0
#define RLIN34_BASE 	0xffce2100
#define RLIN35_BASE 	0xffce2140

#define RLIN3x_BASE        RLIN30_BASE

#define RLIN3xLWBR_B		0x00000001
#define RLIN3xLBRP01_H		0x00000002
#define RLIN3xLBRP0_B		0x00000002
#define RLIN3xLBRP1_B		0x00000003
#define RLIN3xLSTC_B		0x00000004
#define RLIN3xLMD_B			0x00000008
#define RLIN3xLBFC_B		0x00000009
#define RLIN3xLSC_B			0x0000000a
#define RLIN3xLWUP_B		0x0000000b
//#define RLIN3xLIE_B			0x0000000c
#define RLIN3xLEDE_B		0x0000000d
#define RLIN3xLCUC_B		0x0000000e
#define RLIN3xLTRC_B		0x00000010
#define RLIN3xLMST_B		0x00000011
//#define RLIN3xLST_B			0x00000012
#define RLIN3xLEST_B		0x00000013
#define RLIN3xLDFC_B		0x00000014
#define RLIN3xLIDB_B		0x00000015
#define RLIN3xLCBR_B		0x00000016
#define RLIN3xLUDB0_B		0x00000017
#define RLIN3xLDBR1_B		0x00000018
#define RLIN3xLDBR2_B		0x00000019
#define RLIN3xLDBR3_B		0x0000001a
#define RLIN3xLDBR4_B		0x0000001b
#define RLIN3xLDBR5_B		0x0000001c
#define RLIN3xLDBR6_B		0x0000001d
#define RLIN3xLDBR7_B		0x0000001e
#define RLIN3xLDBR8_B		0x0000001f
//#define RLIN3xLUOER_B		0x00000020
#define RLIN3xLUOR1_B		0x00000021
#define RLIN3xLUTDR_H		0x00000024
//#define RLIN3xLUTDRL_B		0x00000024
#define RLIN3xLUTDRH_B		0x00000025
#define RLIN3xLURDR_H		0x00000026
//#define RLIN3xLURDRL_B		0x00000026
#define RLIN3xLURDRH_B		0x00000027
#define RLIN3xLUWTDR_H		0x00000028
#define RLIN3xLUWTDRL_B		0x00000028
#define RLIN3xLUWTDRH_B		0x00000029


#define UDnCH0	0
#define UDnCH1	1
#define UDnCH2	2
#define UDnCH3	3
#define UDnCH4	4
#define UDnCH5	5

#define UDnChannelNum	4

#define UDxCH_OFFSET	0x40
#define RLIN_GET_CHANNEL(addr)		(((addr) - RLIN3x_BASE) / UDxCH_OFFSET)

/*
 * 出力
 * １バイト送信
 */
#define RLIN3xLUTDRL_B		0x00000024

#define RLIN3xLUTDRL(ch)	(RLIN3x_BASE + (UDxCH_OFFSET * (ch)) + RLIN3xLUTDRL_B)

/*
 * 送信許可
 * 0x10
 */
#define RLIN3xLST_B			0x00000012

#define RLIN3xLST(ch)		(RLIN3x_BASE + (UDxCH_OFFSET * (ch)) + RLIN3xLST_B)

#define RLIN3x_LST_URS_BIT		(5)
#define RLIN3x_LST_UTS_BIT		(4)

#define RLIN3x_LST_GET_URS(is_receiving)		(is_receiving)? (1) : (0)
#define RLIN3x_LST_GET_UTS(is_sending)			(is_sending)? (1) : (0)


/*
 * 動作許可
 * 0x03
 */
#define RLIN3xLUOER_B		0x00000020
#define RLIN3xLUOER(ch)		(RLIN3x_BASE + (UDxCH_OFFSET * (ch)) + RLIN3xLUOER_B)

#define RLIN3x_LUOER_ISSET_UTOE(data8)		 ( ((data8) & (1U << 0)) != 0 )
#define RLIN3x_LUOER_ISSET_UROE(data8)		 ( ((data8) & (1U << 1)) != 0 )


/*
 * オプション1制御
 */
#define RLIN3x_LUOR1_B		0x00000021
#define RLIN3x_LUOR1(ch)	(RLIN3x_BASE + (UDxCH_OFFSET * (ch)) + RLIN3x_LUOR1_B)

#define RLIN3x_LUOR1_ISSET_SEND_START_INTR(data8)		( ((data8) & (1U << 0)) == 0 )

/*
 * 入力
 * １バイト受信
 */
#define RLIN3xLURDRL_B		0x00000026
#define RLIN3xLURDRL(ch)	(RLIN3x_BASE + (UDxCH_OFFSET * (ch)) + RLIN3xLURDRL_B)

/*
 * 受信割り込み許可
 * 0x02
 */
#define RLIN3xLIE_B			0x0000000c
#define RLIN3xLIE(ch)		(RLIN3x_BASE + (UDxCH_OFFSET * (ch)) + RLIN3xLIE_B)


/*
 * UARTモードステータス管理
 */
#define RLIN3xLMST_B		0x00000011
#define RLIN3xLMST(ch)		(RLIN3x_BASE + (UDxCH_OFFSET * (ch)) + RLIN3xLMST_B)

#define RLIN3xLCUC_B		0x0000000e
#define RLIN3xLCUC(ch)		(RLIN3x_BASE + (UDxCH_OFFSET * (ch)) + RLIN3xLCUC_B)


#define RLIN_ISMODE_RESET(data8)	( (data8) == 0 )


/*
 *  INTNO
 */
#define RLIN30_TX_INTNO		UINT_C(34)
#define RLIN30_RX_INTNO		UINT_C(35)
#define RLIN30_ER_INTNO		UINT_C(36)
#define RLIN31_TX_INTNO		UINT_C(121)
#define RLIN31_RX_INTNO		UINT_C(122)
#define RLIN31_ER_INTNO		UINT_C(123)
#define RLIN32_TX_INTNO		UINT_C(165)
#define RLIN32_RX_INTNO		UINT_C(166)
#define RLIN32_ER_INTNO		UINT_C(167)
#define RLIN33_TX_INTNO		UINT_C(229)
#define RLIN33_RX_INTNO		UINT_C(230)
#define RLIN33_ER_INTNO		UINT_C(231)
#define RLIN34_TX_INTNO		UINT_C(225)
#define RLIN34_RX_INTNO		UINT_C(226)
#define RLIN34_ER_INTNO		UINT_C(227)
#define RLIN35_TX_INTNO		UINT_C(229)
#define RLIN35_RX_INTNO		UINT_C(230)
#define RLIN35_ER_INTNO		UINT_C(231)

static inline uint16 serial_rlin3x_get_rx_intno(uint8 ch)
{
	uint16 intno;
	switch (ch){
		case UDnCH0:
			intno = RLIN30_RX_INTNO;
			break;
		case UDnCH1:
			intno = RLIN31_RX_INTNO;
			break;
		case UDnCH2:
			intno = RLIN32_RX_INTNO;
			break;
		case UDnCH3:
			intno = RLIN33_RX_INTNO;
			break;
		case UDnCH4:
			intno = RLIN34_RX_INTNO;
			break;
		default:
			break;
	}
	return intno;
}

static inline uint16 serial_rlin3x_get_tx_intno(uint8 ch)
{
	uint16 intno;
	switch (ch){
		case UDnCH0:
			intno = RLIN30_TX_INTNO;
			break;
		case UDnCH1:
			intno = RLIN31_TX_INTNO;
			break;
		case UDnCH2:
			intno = RLIN32_TX_INTNO;
			break;
		case UDnCH3:
			intno = RLIN33_TX_INTNO;
			break;
		case UDnCH4:
			intno = RLIN34_TX_INTNO;
			break;
		default:
			break;
	}
	return intno;
}
#endif /* _SERIAL_H_ */
