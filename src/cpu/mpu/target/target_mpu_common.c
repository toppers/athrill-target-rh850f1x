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
#include "target_cpu.h"
#include "target_mpu.h"
#include "cpu_common/cpu_ops.h"


void target_mpu_init(TargetCoreType *cpu)
{
	uint8 index;

	mpu_init(cpu);

	cpu->mpu.permission_region = object_container_create(sizeof(TargetMpuConfigType), TARGET_MPU_CONFIG_NUM);
	for (index = 0U; index < TARGET_MPU_CONFIG_NUM; index++) {
		(void)object_container_create_element(cpu->mpu.permission_region);
	}

	mpu_set_config(cpu);

	cpu->mpu.excption_error_info.intc_error_info.access_type = MPU_TARGET_ACCESS_NONE;
	cpu->mpu.excption_error_info.intc_error_info.exception_error_code = TARGET_MPU_EXCEPTION_ERROR_CODE_NONE;
	cpu->mpu.excption_error_info.intc_error_info.is_bl_error = FALSE;
	cpu->mpu.excption_error_info.intc_error_info.is_ms_error = FALSE;
	cpu->mpu.excption_error_info.intc_error_info.is_rmw_error = FALSE;
	cpu->mpu.excption_error_info.intc_error_info.is_sv_mode = FALSE;
	cpu->mpu.excption_error_info.error_addr = 0U;
	cpu->mpu.excption_error_info.op_codeid = OpCodeId_Num;
	cpu->mpu.excption_error_info.is_error_occurred = FALSE;

	return;
}

void target_mpu_set_config(TargetCoreType *cpu)
{
	mpu_set_config(cpu);
	return;
}

static void target_mpu_has_permission_set_error(TargetCoreType *cpu, TargetMpuAccessType access_type, uint32 addr)
{

	cpu->mpu.excption_error_info.error_addr = addr;
	cpu->mpu.excption_error_info.intc_error_info.access_type = access_type;

	if (access_type == MPU_TARGET_ACCESS_EXEC) {
		cpu->mpu.excption_error_info.intc_error_info.exception_error_code = TARGET_MPU_EXCEPTION_ERROR_CODE_MIP;
	}
	else {
		cpu->mpu.excption_error_info.intc_error_info.exception_error_code = TARGET_MPU_EXCEPTION_ERROR_CODE_MDP;
	}

	if ((CPU_ISSET_UM(&(cpu->reg)))) {
		cpu->mpu.excption_error_info.intc_error_info.is_sv_mode = FALSE;
	}
	else {
		cpu->mpu.excption_error_info.intc_error_info.is_sv_mode = TRUE;
	}

	return;
}

bool target_mpu_has_permission(TargetCoreType *cpu, TargetMpuAccessType access_type, uint32 addr, uint32 size)
{
	TargetMpuConfigType* enable_region_config[TARGET_MPU_CONFIG_NUM];
	uint8 enable_region_num = 0;
	uint8 region_index;
	TargetMpuConfigType *region_config;

	if (cpu->mpu.is_enable_mpu == FALSE) {
		return TRUE;
	}

	if (!(CPU_ISSET_UM(&(cpu->reg)))) {
		if (cpu->mpu.is_enable_sv == FALSE) {
			return TRUE;
		}
	}

	for (region_index = 0U; region_index < TARGET_MPU_CONFIG_NUM; region_index++) {
		region_config = (TargetMpuConfigType*)object_container_get_element(cpu->mpu.permission_region, region_index);
#if 0
		printf("enable:0x%x\n", region_config->is_enable);
		printf("apply:0x%x\n", region_config->is_enable_apply_config);
		printf("start:0x%x\n", region_config->addr_start);
		printf("end:0x%x\n", region_config->addr_end);
		printf("um_x:0x%x\n", region_config->is_enable_access_um.exec);
		printf("um_w:0x%x\n", region_config->is_enable_access_um.write);
		printf("um_r:0x%x\n", region_config->is_enable_access_um.read);
		printf("sv_x:0x%x\n", region_config->is_enable_access_sv.exec);
		printf("sv_w:0x%x\n", region_config->is_enable_access_sv.write);
		printf("sv_r:0x%x\n", region_config->is_enable_access_sv.read);
#endif
		if (!(mpu_is_enable_protection_region(cpu, region_config))) {
			continue;
		}

		if (!(mpu_is_apply_protection_region(cpu, region_config))) {
			continue;
		}

		if (mpu_has_permission_access_type(cpu, access_type, region_config)) {
			enable_region_config[enable_region_num] = region_config;
			enable_region_num++;
			continue;
		}
	}
#if 0
	printf("start=0x%x size=%d enable_region_num=%d\n", addr, size, enable_region_num);
	for (region_index = 0; region_index < enable_region_num; region_index++) {
		printf("start:0x%x\n", enable_region_config[region_index]->addr_start);
		printf("end:0x%x\n", enable_region_config[region_index]->addr_end);
		printf("um_x:0x%x\n", enable_region_config[region_index]->is_enable_access_um.exec);
		printf("um_w:0x%x\n", enable_region_config[region_index]->is_enable_access_um.write);
		printf("um_r:0x%x\n", enable_region_config[region_index]->is_enable_access_um.read);
	}
#endif
	if (mpu_has_permission_access_region(cpu, access_type, addr, size, &(enable_region_config[0]), enable_region_num)) {
		cpu->mpu.excption_error_info.is_error_occurred = FALSE;
		return TRUE;
	}

	cpu->mpu.excption_error_info.is_error_occurred = TRUE;
	//printf("is_error_occurred=TRUE:addr=0x%x size=%d type=%d\n", addr, size, access_type);
	/* error set */
	target_mpu_has_permission_set_error(cpu, access_type, addr);

	return FALSE;
}

void target_mpu_set_error(TargetCoreType *cpu)
{
	OpCodeId code_id = cpu->mpu.excption_error_info.op_codeid;

	cpu->mpu.excption_error_info.intc_error_info.is_bl_error = FALSE;
	cpu->mpu.excption_error_info.intc_error_info.is_rmw_error = FALSE;

	switch (code_id) {
	case OpCodeId_PREPARE_13:
	case OpCodeId_DISPOSE_13:
	case OpCodeId_PUSHSP_11:
	case OpCodeId_POPSP_11:
		/* BL */
		cpu->mpu.excption_error_info.intc_error_info.is_bl_error = TRUE;
		break;
	case OpCodeId_SET1_8:
	case OpCodeId_SET1_9:
	case OpCodeId_NOT1_8:
	case OpCodeId_NOT1_9:
	case OpCodeId_CLR1_8:
	case OpCodeId_CLR1_9:
		/* RMW */
		cpu->mpu.excption_error_info.intc_error_info.is_rmw_error = TRUE;
		break;
	default:
		break;

	}

	/* MS */
	cpu->mpu.excption_error_info.intc_error_info.is_ms_error = FALSE;

	mpu_set_error(cpu);

	return;
}


void target_mpu_check(TargetCoreType *cpu)
{
	mpu_check(cpu);
	return;
}

void target_mpu_hook_access(TargetCoreType *cpu, uint32 *data, TargetMpuAccessType access_type, uint8 regid, uint8 selid)
{
	mpu_hook_access(cpu, data, access_type, regid, selid);
	return;
}

void target_mpu_debug_print_mpu_status(TargetCoreType *cpu)
{
	mpu_debug_print_mpu_status(cpu);
	return;
}
