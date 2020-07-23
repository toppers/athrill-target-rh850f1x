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
#ifndef _MPU_CONFIG_H_
#define _MPU_CONFIG_H_

#include "std_types.h"
#include "loader/elf.h"

#define MPU_CONFIG_REGION_NUM		UINT_C(8)

#define MPU_ADDRESS_REGION_INX_INTC1	(0U)
#define MPU_ADDRESS_REGION_INX_INTC2	(1U)
#define MPU_ADDRESS_REGION_INX_TIMER	(2U)
#define MPU_ADDRESS_REGION_INX_SERIAL	(3U)
#define MPU_ADDRESS_REGION_INX_CAN		(4U)
#define MPU_ADDRESS_REGION_INX_CLOCK	(5U)
#define MPU_ADDRESS_REGION_INX_PH0		(6U)
#define MPU_ADDRESS_REGION_INX_PH1		(7U)

#define ELF_MACHINE_TYPE	EM_V800


#define MPU_ADDRESS_REGION_MASK_PH		0xFFFFFFFF
#define MPU_ADDRESS_REGION_MASK_CPU		0x06FFFFFF

#define MPU_ADDRESS_REGION_SIZE_INX_INTC1	(0xFFFEEC00 - 0xFFFEEA00)
#define MPU_ADDRESS_REGION_SIZE_INX_INTC2	(0xFFFFC000 - 0xFFFFB000)
#define MPU_ADDRESS_REGION_SIZE_INX_SERIAL	(0xFFCE2180 - 0xFFCE2000)
#define MPU_ADDRESS_REGION_SIZE_INX_CAN		(0xFFD08000 - 0xFFD00000)
#define MPU_ADDRESS_REGION_SIZE_INX_CLOCK	(0xFFF8A80C - 0xFFF81100)
#define MPU_ADDRESS_REGION_SIZE_INX_TIMER	(0xFFE51100 - 0xFFE50000)

#define MPU_ADDRESS_REGION_SADDR_SYS		(0xFFF82800)
#define MPU_ADDRESS_REGION_EADDR_SYS		(0xFFF8C000)
#define MPU_ADDRESS_REGION_SIZE_INX_SYS		(MPU_ADDRESS_REGION_EADDR_SYS - MPU_ADDRESS_REGION_SADDR_SYS)

#define MPU_ADDRESS_REGION_SIZE_INX_CPU		(1024U * 1024U)
#define MPU_ADDRESS_REGION_SIZE_INX_PH0		(0xFFF8E015 - 0xFFF80000)
#define MPU_ADDRESS_REGION_SIZE_INX_PH1		(0xFFC340A0 - 0xFFA00004)
#define MPU_ADDRESS_REGION_SIZE_INX_PH2		(1024U * 8U)

extern uint8 memory_data_CAN[MPU_ADDRESS_REGION_SIZE_INX_CAN];

#endif /* _MPU_CONFIG_H_ */
