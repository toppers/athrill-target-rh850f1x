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
#ifndef _TARGET_MPU_COMMON_H_
#define _TARGET_MPU_COMMON_H_

#include "object_container.h"


#define TARGET_MPU_CONFIG_NUM		16U

typedef enum {
	TARGET_MPU_EXCEPTION_ERROR_CODE_NONE = 0,
	TARGET_MPU_EXCEPTION_ERROR_CODE_MIP,
	TARGET_MPU_EXCEPTION_ERROR_CODE_MDP,
} TargetMpuExceptionErrorCodeType;

typedef enum {
	MPU_TARGET_ACCESS_NONE = 0,
	MPU_TARGET_ACCESS_READ,
	MPU_TARGET_ACCESS_WRITE,
	MPU_TARGET_ACCESS_EXEC,
} TargetMpuAccessType;


/* mpu type */
/* common */
typedef struct {
	bool		read;
	bool		write;
	bool		exec;
} TargetMpuEnableAccessType;

/* error */

typedef struct {
	TargetMpuAccessType				access_type;
	TargetMpuExceptionErrorCodeType	exception_error_code;
	bool	is_sv_mode;
	bool	is_bl_error;
	bool	is_ms_error;
	bool	is_rmw_error;
} TargetMpuExceptionErrorintcInfoType;


typedef struct {
	TargetMpuExceptionErrorintcInfoType			intc_error_info;
	uint32										error_addr;
	OpCodeId									op_codeid;
	bool										is_error_occurred;
} TargetMpuExceptionErrorInfoType;


/* set config */

typedef struct {
	uint32		addr_start;
	uint32		addr_end;
	bool		is_enable;
	TargetMpuEnableAccessType	is_enable_access_sv;
	TargetMpuEnableAccessType	is_enable_access_um;
	bool	is_enable_apply_config;
	uint16	asid;
} TargetMpuConfigType;

typedef struct {
	TargetMpuExceptionErrorInfoType		excption_error_info;
	ObjectContainerType					*permission_region;
	bool								is_enable_mpu;
	bool								is_enable_sv;
	TargetMpuEnableAccessType			is_enable_outside_protection_region;
} TargetMpuType;


#endif /* _TARGET_MPU_COMMON_H_ */
