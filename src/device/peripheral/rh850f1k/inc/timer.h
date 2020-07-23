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
#ifndef _TIMER_H_
#define _TIMER_H_

#include "device.h"

#define TAUJ_UNIT_NUM	2
#define TAUJ_CH_NUM		2

#define TAUFJ0I0_INTNO		(80)
#define TAUFJ0I1_INTNO		(81)
#define TAUFJ0I2_INTNO		(82)
#define TAUFJ0I3_INTNO		(83)
#define TAUFJ1I0_INTNO		(168)
#define TAUFJ1I1_INTNO		(169)
#define TAUFJ1I2_INTNO		(170)
#define TAUFJ1I3_INTNO		(171)

#define TAUJ_INTNO(n, ch)	(n == 0)? ((uint32) ((TAUFJ0I0_INTNO + ch))) : ((uint32) (TAUFJ1I0_INTNO + ch))

/*
 *  TAUJ関連レジスタ
 *
 *  Units：2
 */
#define TAUJ_BASE(n)	((uint32) (0xffe50000U + (n * 0x1000U)))

/*
 * プリスケーラ：未使用
 */
#define TAUJTPS(n)		(TAUJ_BASE(n) + 0x90U)

/*
 * タイマカウント周期設定
 * 32bitレジスタ
 */
#define TAUJCDR(n, ch)	(TAUJ_BASE(n) + (ch * 0x04U))

/*
 * 現在ティック値
 * 32bitレジスタ
 */
#define TAUJCNT(n, ch)	(TAUJ_BASE(n) + 0x10U + (ch * 0x04U))

/*
 * インターバルタイマ設定用：未使用
 */
#define TAUJCMOR(n, ch)	(TAUJ_BASE(n) + 0x80U + (ch * 0x04U))

/*
 * インターバルタイマ設定用：未使用
 */
#define TAUJCMUR(n, ch)	(TAUJ_BASE(n) + 0x20U + (ch * 0x04U))
/*
 *　 タイマ開始処理
 */
// 16bitレジスタ(ch)
//
// |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
// |-------|-------|-------|-------|
//
// 1:start
// 0:not start
#define TAUJTS(n)		(TAUJ_BASE(n) + 0x54U)
/*
 *　タイマ停止処理
 */
// 16bitレジスタ(ch)
//
// |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
// |-------|-------|-------|-------|
//
// 1:stop
// 0:not stop
#define TAUJTT(n)		(TAUJ_BASE(n) + 0x58U)

#define MCU_TAUJ_MASK_CK0				((uint16) 0xfff0)
#define MCU_TAUJ_CK0_0					((uint16) 0x0000)
#define MCU_TAUJ00_CMOR					((uint16) 0x0000)
#define MCU_TAUJ00_CMUR					((uint8) 0x01)



#endif /* _TIMER_H_ */
