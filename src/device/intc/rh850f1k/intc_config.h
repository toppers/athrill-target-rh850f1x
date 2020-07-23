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
#ifndef _INTC_CONFIG_H_
#define _INTC_CONFIG_H_

#include "std_types.h"
#include "mpu.h"

typedef enum {
	IntcPriorityLevel_0 = 0,
	IntcPriorityLevel_1,
	IntcPriorityLevel_2,
	IntcPriorityLevel_3,
	IntcPriorityLevel_4,
	IntcPriorityLevel_5,
	IntcPriorityLevel_6,
	IntcPriorityLevel_7,
	IntcPriorityLevel_8,
	IntcPriorityLevel_9,
	IntcPriorityLevel_10,
	IntcPriorityLevel_11,
	IntcPriorityLevel_12,
	IntcPriorityLevel_13,
	IntcPriorityLevel_14,
	IntcPriorityLevel_15,
	IntcPriorityLevel_Num,
} IntcPriorityLevelType;

typedef enum {
	IntcPriorityDegree_0 = 0,
	IntcPriorityDegree_1,
	IntcPriorityDegree_2,
	IntcPriorityDegree_3,
	IntcPriorityDegree_4,
	IntcPriorityDegree_5,
	IntcPriorityDegree_6,
	IntcPriorityDegree_7,
	IntcPriorityDegree_8,
	IntcPriorityDegree_9,
	IntcPriorityDegree_10,
	IntcPriorityDegree_11,
	IntcPriorityDegree_12,
	IntcPriorityDegree_13,
	IntcPriorityDegree_14,
	IntcPriorityDegree_15,
	IntcPriorityDegree_Num,
} IntcPriorityDegreeType;


/*
 *  INTC
 */
#define TMIN_INTNO	(0)
#define TMAX_INTNO	(357)
#define TNUM_INT	(358)

#define INTC1_BASE  	0xFFFEEA00
#define INTC2_BASE  	0xFFFFB000

#define INTC1_MIN_INTNO	 TMIN_INTNO
#define INTC1_MAX_INTNO	 (31)
#define INTC2_MIN_INTNO	 (32)
#define INTC2_MAX_INTNO	 TMAX_INTNO

#define INTC_ADDR_END	0xFFFEEE00

#define INTC1_IC_ADDR_START		0xFFFEEA00
#define INTC1_IC_ADDR_END		0xFFFEEA3F	/* 0xFFFEEA3EH + 0x1 */

#define INTC2_IC_ADDR_START		0xFFFFB040
#define INTC2_IC_ADDR_END		0xFFFFB2CB	/* 0xFFFFB2CA + 0x1 */
#define INTC2_INTNO_OFFSET		INTC2_MIN_INTNO

#define IC_REG_LEN_BYTE			0x02
#define IC_REG_LEN_BIT			(16)

#define IMR_REG_LEN_BYTE		0x04
#define IMR_REG_LEN_BIT			(32)

#define INTC1_IMR_ADDR_START			0xFFFEEAF0
#define INTC1_IMR_ADDR_END				0xFFFEEAF3	/* 0xFFFEEAF0H + 0x3 */
#define INTC1_IMR_OFFSET_BIT(regaddr)	((regaddr - INTC1_IMR_ADDR_START) * 0x8)

#define INTC2_IMR_ADDR_START					0xFFFFB400
#define INTC2_IMR_ADDR_END						0xFFFFB42F	/* FFFF B400H + (04H × 11) + 0x3 */
#define INTC2_IMR_ADDR(imr_no)					(INTC2_IMR_ADDR_START + ((imr_no) * IMR_REG_LEN_BYTE))
#define INTC2_IMR_OFFSET_BIT(regaddr, imr_no)	((regaddr - INTC2_IMR_ADDR(imr_no)) * 0x8)

#define INTC_IMR_NUM			(11)
#define INTC_ACCESS_REGLEN_8BIT		(8)
#define INTC_ACCESS_REGLEN_16BIT	(16)
#define INTC_ACCESS_REGLEN_32BIT	(32)


#define INTC_ACCESS_REGLEN_1BYTE	0x01
#define INTC_ACCESS_REGLEN_2BYTE	0x02
#define INTC_ACCESS_REGLEN_4BYTE	0x04

/* ICレジスタ8ビットアクセス */
/* アクセス位置検出 */
#define IS_ICADDR_UPPER(ic_addr)	(((ic_addr) % IC_REG_LEN_BYTE) == 1U)
/* 上位アドレスを下位アドレスに変換 */
#define IC_ADDR_UPPER2LOWER(ic_addr)	((ic_addr) - 0x1)

/* 割り込み番号の取得 */
/* ICレジスタアドレスから取得 */
#define INTC1_ICADDR2INTNO(ic_addr)	(((ic_addr) - INTC1_IC_ADDR_START) / IC_REG_LEN_BYTE)
#define INTC2_ICADDR2INTNO(ic_addr)	((((ic_addr) - (INTC2_IC_ADDR_START)) / IC_REG_LEN_BYTE) + INTC2_INTNO_OFFSET)


/* 割り込み番号からレジスタのアドレス取得 */
#define IC_ADDRESS(intno)	(intno <= INTC1_MAX_INTNO)? (INTC1_IC_ADDR_START + (intno * IC_REG_LEN_BYTE)) : (INTC2_IC_ADDR_START  + ((intno - INTC2_INTNO_OFFSET) * IC_REG_LEN_BYTE))
#define IMR_ADDRESS(intno)	(intno <= INTC1_MAX_INTNO)? (INTC1_IMR_ADDR_START) : (INTC2_IMR_ADDR_START + ((intno / IMR_REG_LEN_BIT) * IMR_REG_LEN_BYTE))



// 16bitレジスタ
//
// |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
// |-------|-------|-------|-------|
//  J     K         Z X     YYYYYYY
//
//    X(6):table-bit(0:none, 1:table)
//    Y(0-3):優先度(0-15)
//    Z(7):割込み要求禁止フラグ(0:許可，1:禁止)
//    K(12):割込み要求(0:なし，1:あり)
//    J(15):割り込み検出タイプビット

#define IC_GET_PRIODEGREE(data16)		 ((data16) & 0x000F)

#define IC_ISSET_TABLEREF(data16)		 ( ((data16) & (1U << 6)) != 0 )
#define IC_ISSET_INTMASK(data16)		 ( ((data16) & (1U << 7)) != 0 )
#define IC_ISSET_INTREQ(data16)		 ( ((data16) & (1U << 12)) != 0 )

#define IC_SET_PRIODEGREE(data16p, pri) \
do {	\
	*(data16p) &= ~(3U << 0);	\
	*(data16p) |= ((pri) & 0x000F);	\
} while (0)

#define IC_SET_TABLEREF(data16p) \
do {	\
	*(data16p) |= (1U << 6);	\
} while (0)
#define IC_CLR_INTTABLEREF(data16p) \
do {	\
	*(data16p) &= ~(1U << 6);	\
} while (0)
#define IC_SET_INTMASK(data16p) \
do {	\
	*(data16p) |= (1U << 7);	\
} while (0)
#define IC_CLR_INTMASK(data16p) \
do {	\
	*(data16p) &= ~(1U << 7);	\
} while (0)

#define IC_SET_INTREQ(data16p) \
do {	\
	*(data16p) |= (1U << 12);	\
} while (0)
#define IC_CLR_INTREQ(data16p) \
do {	\
	*(data16p) &= ~(1U << 12);	\
} while (0)

#endif /* _INTC_CONFIG_H_ */
