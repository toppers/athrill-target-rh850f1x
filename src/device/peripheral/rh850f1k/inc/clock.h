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
#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "device.h"

/* MainOSC */
#define MOSCE	0xFFF81100
#define MOSCS	0xFFF81104

#define MOSCE_ISSTOP(data32)		(((data32) & 0x02) == 0x02)
#define MOSCE_ISSTART(data32)		(((data32) & 0x01) == 0x01)

#define MOSCS_SET_ACT(data32p)	\
do {	\
	*(data32p) |= (1U << 2);	\
} while (0)
#define MOSCS_CLR_ACT(data32p)	\
do {	\
	*(data32p) &= ~(1U << 2);	\
} while (0)


/* CKSC_PPLLCLKS */
#define CKSC_PPLLCLKS_CTL	0xFFF8A010
#define CKSC_PPLLCLKS_ACT	0xFFF8A018

/* CKSC_CPUCLKS */
#define CKSC_CPUCLKS_CTL	0xFFF8A000
#define CKSC_CPUCLKS_ACT	0xFFF8A008

/* CKSC_CPUCLKD */
#define CKSC_CPUCLKD_CTL	0xFFF8A100
#define CKSC_CPUCLKD_ACT	0xFFF8A108

/* CKSC_ATAUJS */
#define CKSC_ATAUJS_CTL		0xFFF82100
#define CKSC_ATAUJS_ACT		0xFFF82108

/* CKSC_ATAUJD */
#define CKSC_ATAUJD_CTL		0xFFF82200
#define CKSC_ATAUJD_ACT		0xFFF82208

/* CKSC_ILINS */
#define CKSC_ILINS_CTL		0xFFF8A400
#define CKSC_ILINS_ACT		0xFFF8A408

/* CKSC_ILIND */
#define CKSC_ILIND_CTL		0xFFF8A800
#define CKSC_ILIND_ACT		0xFFF8A808


#endif /* _CLOCK_H_ */
