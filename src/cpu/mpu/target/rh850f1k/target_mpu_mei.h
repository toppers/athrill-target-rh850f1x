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
#ifndef _TARGET_MPU_MEI_H_
#define _TARGET_MPU_MEI_H_

#include "std_types.h"
#include "cpu_dec/op_codeid.h"

typedef struct {
	uint8	reg;
	uint8	data_type;
	uint8	sign;
	uint8	rw;
	uint8	itype;
} TargetMpuExceptionErrorMemoryErrorInfoType;

#define TARGET_MPU_SYSREG_MEI_REG_OFFSET		16U
#define TARGET_MPU_SYSREG_MEI_DS_OFFSET			9U
#define TARGET_MPU_SYSREG_MEI_U_OFFSET			8U
#define TARGET_MPU_SYSREG_MEI_ITYPE_OFFSET		1U
#define TARGET_MPU_SYSREG_MEI_RW_OFFSET			0U

#define TARGET_MPU_MEI_REG_0					0U
#define TARGET_MPU_MEI_REG_1					1U
#define TARGET_MPU_MEI_REG_2					2U
#define TARGET_MPU_MEI_REG_3					3U
#define TARGET_MPU_MEI_REG_NONE					4U

#define TARGET_MPU_MEI_DATA_TYPE_BYTE			0U
#define TARGET_MPU_MEI_DATA_TYPE_HALF_WORD		1U
#define TARGET_MPU_MEI_DATA_TYPE_WORD			2U
#define TARGET_MPU_MEI_DATA_TYPE_DOUBLE_WORD	3U
#define TARGET_MPU_MEI_DATA_TYPE_NONE			4U

#define TARGET_MPU_MEI_SIGN_SIGNED			0U
#define TARGET_MPU_MEI_SIGN_UNSIGNED		1U
#define TARGET_MPU_MEI_SIGN_NONE			2U

#define TARGET_MPU_MEI_ITYPE_00000B			0x00
#define TARGET_MPU_MEI_ITYPE_00001B			0x01
#define TARGET_MPU_MEI_ITYPE_00010B			0x02
#define TARGET_MPU_MEI_ITYPE_00111B			0x07
#define TARGET_MPU_MEI_ITYPE_01000B			0x08
#define TARGET_MPU_MEI_ITYPE_01001B			0x09
#define TARGET_MPU_MEI_ITYPE_01100B			0x0C
#define TARGET_MPU_MEI_ITYPE_01101B			0x0D
#define TARGET_MPU_MEI_ITYPE_10000B			0x10
#define TARGET_MPU_MEI_ITYPE_10001B			0x11
#define TARGET_MPU_MEI_ITYPE_10010B			0x12
#define TARGET_MPU_MEI_ITYPE_NONE			0x1F

#define TARGET_MPU_MEI_RW_READ				0U
#define TARGET_MPU_MEI_RW_WRITE				1U
#define TARGET_MPU_MEI_RW_NONE				2U

extern const TargetMpuExceptionErrorMemoryErrorInfoType memory_error_info[OpCodeId_Num];


#endif /* _TARGET_MPU_MEI_H_ */
