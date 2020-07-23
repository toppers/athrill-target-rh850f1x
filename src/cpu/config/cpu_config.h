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
#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_


/*
 * CPUコアの数
 */
#define CPU_CONFIG_CORE_NUM					(2)

#define CPU_CONFIG_CORE_ID_0				(0)
#define CPU_CONFIG_CORE_ID_1				(1)


#define CPU_CONFIG_DEBUG_REGISTER_ADDR		0x06FF0000
#define CPU_CONFIG_DEBUG_REGADDR_R0			0x06FF0600
#define CPU_CONFIG_DEBUG_REGADDR_R1			0x06FF0604
#define CPU_CONFIG_DEBUG_REGADDR_R2			0x06FF0608
#define CPU_CONFIG_DEBUG_REGADDR_R3			0x06FF060C
#define CPU_CONFIG_DEBUG_REGADDR_R4			0x06FF0610
#define CPU_CONFIG_DEBUG_REGADDR_R5			0x06FF0614
#define CPU_CONFIG_DEBUG_REGADDR_R6			0x06FF0618
#define CPU_CONFIG_DEBUG_REGADDR_R7			0x06FF061C
#define CPU_CONFIG_DEBUG_REGADDR_R8			0x06FF0620
#define CPU_CONFIG_DEBUG_REGADDR_R9			0x06FF0624
#define CPU_CONFIG_DEBUG_REGADDR_R10		0x06FF0628
#define CPU_CONFIG_DEBUG_REGADDR_R11		0x06FF062C
#define CPU_CONFIG_DEBUG_REGADDR_R12		0x06FF0630
#define CPU_CONFIG_DEBUG_REGADDR_R13		0x06FF0634
#define CPU_CONFIG_DEBUG_REGADDR_R14		0x06FF0638
#define CPU_CONFIG_DEBUG_REGADDR_R15		0x06FF063C
#define CPU_CONFIG_DEBUG_REGADDR_R16		0x06FF0640
#define CPU_CONFIG_DEBUG_REGADDR_R17		0x06FF0644
#define CPU_CONFIG_DEBUG_REGADDR_R18		0x06FF0648
#define CPU_CONFIG_DEBUG_REGADDR_R19		0x06FF064C
#define CPU_CONFIG_DEBUG_REGADDR_R20		0x06FF0650
#define CPU_CONFIG_DEBUG_REGADDR_R21		0x06FF0654
#define CPU_CONFIG_DEBUG_REGADDR_R22		0x06FF0658
#define CPU_CONFIG_DEBUG_REGADDR_R23		0x06FF065C
#define CPU_CONFIG_DEBUG_REGADDR_R24		0x06FF0660
#define CPU_CONFIG_DEBUG_REGADDR_R25		0x06FF0664
#define CPU_CONFIG_DEBUG_REGADDR_R26		0x06FF0668
#define CPU_CONFIG_DEBUG_REGADDR_R27		0x06FF066C
#define CPU_CONFIG_DEBUG_REGADDR_R28		0x06FF0670
#define CPU_CONFIG_DEBUG_REGADDR_R29		0x06FF0674
#define CPU_CONFIG_DEBUG_REGADDR_R30		0x06FF0678
#define CPU_CONFIG_DEBUG_REGADDR_R31		0x06FF067C

#define CPU_CONFIG_ADDR_PEID				0x06FF6490


#define CPU_CONFIG_ADDR_MIR_0				0x06FF6800
#define CPU_CONFIG_ADDR_MIR_1				0x06FF6804

#define CPU_CONFIG_ADDR_MEV_0				0x06FF6900
#define CPU_CONFIG_ADDR_MEV_1				0x06FF6904
#define CPU_CONFIG_ADDR_MEV_2				0x06FF6908
#define CPU_CONFIG_ADDR_MEV_3				0x06FF690C
#define CPU_CONFIG_ADDR_MEV_4				0x06FF6910
#define CPU_CONFIG_ADDR_MEV_5				0x06FF6914
#define CPU_CONFIG_ADDR_MEV_6				0x06FF6918
#define CPU_CONFIG_ADDR_MEV_7				0x06FF691C

/*
 * CPU間割り込み番号
 */
#define CPU_CONFIG_ADDR_MIR_0_INTNO			0
#define CPU_CONFIG_ADDR_MIR_1_INTNO			1

#define PLLS		0xfff89004


#endif /* _CPU_CONFIG_H_ */
