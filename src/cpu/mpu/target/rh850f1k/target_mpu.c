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
#include "cpu_common/cpu_ops.h"

#include "target_mpu_mei.h"

void mpu_init(TargetCoreType *cpu)
{
	uint32* sysreg;

	/* MPM 0000 0000*/
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_5, SYS_REG_MPM);
	*sysreg = 0x00;

	/* MPRC 0000 0000*/
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_5, SYS_REG_MPRC);
	*sysreg = 0x00;

	/* MPBRGN 0000 0000*/
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_5, SYS_REG_MPBRGN);
	*sysreg = 0x00;

	/* MPTRGN 0001 0000*/
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_5, SYS_REG_MPTRGN);
	*sysreg = 0x10;

	/* MPATn(0~15) 0000 0000 */
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_6, SYS_REG_MPAT0);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_6, SYS_REG_MPAT1);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_6, SYS_REG_MPAT2);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_6, SYS_REG_MPAT3);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_6, SYS_REG_MPAT4);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_6, SYS_REG_MPAT5);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_6, SYS_REG_MPAT6);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_6, SYS_REG_MPAT7);
	*sysreg = 0x00;

	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_7, SYS_REG_MPAT8);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_7, SYS_REG_MPAT9);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_7, SYS_REG_MPAT10);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_7, SYS_REG_MPAT11);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_7, SYS_REG_MPAT12);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_7, SYS_REG_MPAT13);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_7, SYS_REG_MPAT14);
	*sysreg = 0x00;
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_7, SYS_REG_MPAT15);
	*sysreg = 0x00;

	/* MCC 0000 0000 */
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_5, SYS_REG_MCC);
	*sysreg = 0x00;

	return;
}

#define TARGET_MPU_SYSREG6_OFFSET_INDEX		0U
#define TARGET_MPU_SYSREG7_OFFSET_INDEX		8U
#define TARGET_MPU_SYSREG_PROTECTION_REGION_BASE(index, offset)		((index - offset) * 4U)
#define TARGET_MPU_SYSREG_MPLA(base)		(base)
#define TARGET_MPU_SYSREG_MPUA(base)		(base + 1U)
#define TARGET_MPU_SYSREG_MPAT(base)		(base + 2U)


#define TARGET_MPU_SYSREG_MPM_MPE		0x0001
#define TARGET_MPU_SYSREG_MPM_SVP		0x0002

#define TARGET_MPU_SYSREG_MPAT_E		0x0080
#define TARGET_MPU_SYSREG_MPAT_G		0x0040
#define TARGET_MPU_SYSREG_MPAT_SX		0x0020
#define TARGET_MPU_SYSREG_MPAT_SW		0x0010
#define TARGET_MPU_SYSREG_MPAT_SR		0x0008
#define TARGET_MPU_SYSREG_MPAT_UX		0x0004
#define TARGET_MPU_SYSREG_MPAT_UW		0x0002
#define TARGET_MPU_SYSREG_MPAT_UR		0x0001
#define TARGET_MPU_SYSREG_MPAT_ASID		0x03FF0000

#define TARGET_MPU_SYSREG_IS_BIT_SET(sysregp, bit)		((*sysregp & bit) == bit)
#define TARGET_MPU_SYSREG_MPAT_GET_ASID(sysregp)	((*sysregp & TARGET_MPU_SYSREG_MPAT_ASID) >> 16U)

#define TARGET_MPU_ADDR_START_MASK(addr)	((addr) & 0xFFFFFFFC)
#define TARGET_MPU_ADDR_END_MASK(addr)	((addr) | 0x3)

void mpu_set_config(TargetCoreType *cpu)
{
	TargetMpuConfigType* mpu_config_obj;
	uint32* sysregp;
	uint8 index;
	uint8 base;
	uint8 regid;
	uint8 selid;


	/* MPM */
	sysregp = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_5, SYS_REG_MPM);
	cpu->mpu.is_enable_mpu = TARGET_MPU_SYSREG_IS_BIT_SET(sysregp, TARGET_MPU_SYSREG_MPM_MPE);
	cpu->mpu.is_enable_sv = TARGET_MPU_SYSREG_IS_BIT_SET(sysregp, TARGET_MPU_SYSREG_MPM_SVP);
	cpu->mpu.is_enable_outside_protection_region.exec = FALSE;
	cpu->mpu.is_enable_outside_protection_region.read = FALSE;
	cpu->mpu.is_enable_outside_protection_region.write = FALSE;

	for(index = 0U; index < TARGET_MPU_CONFIG_NUM; index++){
		mpu_config_obj = (TargetMpuConfigType*)object_container_get_element(cpu->mpu.permission_region, index);
		if (index < TARGET_MPU_SYSREG7_OFFSET_INDEX) {
			selid = SYS_GRP_6;
			base = TARGET_MPU_SYSREG_PROTECTION_REGION_BASE(index, TARGET_MPU_SYSREG6_OFFSET_INDEX);
		}
		else {
			selid = SYS_GRP_7;
			base = TARGET_MPU_SYSREG_PROTECTION_REGION_BASE(index, TARGET_MPU_SYSREG7_OFFSET_INDEX);
		}

		/* MPLA */
		regid = TARGET_MPU_SYSREG_MPLA(base);
		sysregp = cpu_get_sysreg2(&cpu->reg.sys, selid, regid);
		mpu_config_obj->addr_start = TARGET_MPU_ADDR_START_MASK(*sysregp);

		/* MPUA */
		regid = TARGET_MPU_SYSREG_MPUA(base);
		sysregp = cpu_get_sysreg2(&cpu->reg.sys, selid, regid);
		mpu_config_obj->addr_end = TARGET_MPU_ADDR_END_MASK(*sysregp);

		/* MPAT */
		regid = TARGET_MPU_SYSREG_MPAT(base);
		sysregp = cpu_get_sysreg2(&cpu->reg.sys, selid, regid);
		mpu_config_obj->is_enable = TARGET_MPU_SYSREG_IS_BIT_SET(sysregp, TARGET_MPU_SYSREG_MPAT_E);
		mpu_config_obj->is_enable_apply_config = TARGET_MPU_SYSREG_IS_BIT_SET(sysregp, TARGET_MPU_SYSREG_MPAT_G);
		mpu_config_obj->is_enable_access_sv.exec = TARGET_MPU_SYSREG_IS_BIT_SET(sysregp, TARGET_MPU_SYSREG_MPAT_SX);
		mpu_config_obj->is_enable_access_sv.write = TARGET_MPU_SYSREG_IS_BIT_SET(sysregp, TARGET_MPU_SYSREG_MPAT_SW);
		mpu_config_obj->is_enable_access_sv.read = TARGET_MPU_SYSREG_IS_BIT_SET(sysregp, TARGET_MPU_SYSREG_MPAT_SR);
		mpu_config_obj->is_enable_access_um.exec = TARGET_MPU_SYSREG_IS_BIT_SET(sysregp, TARGET_MPU_SYSREG_MPAT_UX);
		mpu_config_obj->is_enable_access_um.write = TARGET_MPU_SYSREG_IS_BIT_SET(sysregp, TARGET_MPU_SYSREG_MPAT_UW);
		mpu_config_obj->is_enable_access_um.read = TARGET_MPU_SYSREG_IS_BIT_SET(sysregp, TARGET_MPU_SYSREG_MPAT_UR);
		mpu_config_obj->asid = TARGET_MPU_SYSREG_MPAT_GET_ASID(sysregp);

	}
	return;
}

bool mpu_is_enable_protection_region(TargetCoreType *cpu, TargetMpuConfigType* region_config)
{
	/* enable check */
	if (!(region_config->is_enable)) {
		return FALSE;
	}

	/* enable set region */
	if (region_config->addr_end < region_config->addr_start) {
		return FALSE;
	}

	return TRUE;
}

bool mpu_is_apply_protection_region(TargetCoreType *cpu, TargetMpuConfigType* region_config)
{
	uint32* sysregp;
	if (!region_config->is_enable_apply_config) {
		return FALSE;
	}

	/* ASID */
	sysregp = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_2, SYS_REG_ASID);
	if (region_config->asid == *sysregp) {
		return TRUE;
	}
	else{
		return FALSE;
	}

}

bool mpu_has_permission_access_type(TargetCoreType *cpu, TargetMpuAccessType access_type, TargetMpuConfigType* region_config)
{
	TargetMpuEnableAccessType *is_enable_access;

	if (CPU_ISSET_UM(&(cpu->reg))) {
		is_enable_access = &(region_config->is_enable_access_um);
	}
	else {
		is_enable_access = &(region_config->is_enable_access_sv);
	}

	switch (access_type) {
	case MPU_TARGET_ACCESS_READ:
		return is_enable_access->read;
	case MPU_TARGET_ACCESS_WRITE:
		return is_enable_access->write;
	case MPU_TARGET_ACCESS_EXEC:
		return is_enable_access->exec;
	default:
		break;
	}
	return FALSE;
}

static bool mpu_has_permission_access_region_mdp(uint32 addr, uint32 size, TargetMpuConfigType** region_config, uint8 enable_region_num)
{
	uint8 index;

	for (index = 0; index < enable_region_num; index++) {
		if ((region_config[index]->addr_start <= addr) && ((addr + size - 1) <= region_config[index]->addr_end)) {
			return TRUE;
		}
	}
	//printf("mpu_has_permission_access_region_mdp:start=0x%x size=%d\n", addr, size);
	return FALSE;
}

static bool mpu_has_permission_access_region_mip(uint32 addr, uint32 size, TargetMpuConfigType** region_config, uint8 enable_region_num)
{
	uint8 index;
	uint32 check_addr;

	/*
	 * Check every byte
	 */
	for (check_addr = addr; check_addr < addr + size; check_addr++) {
		for (index = 0U; index < enable_region_num; index++){
			if ((region_config[index]->addr_start <= check_addr) && (check_addr <= region_config[index]->addr_end)) {
				break;
			}
		}
		if (index == enable_region_num) {
			return FALSE;
		}
	}

	return TRUE;
}

bool mpu_has_permission_access_region(TargetCoreType *cpu, TargetMpuAccessType access_type, uint32 addr, uint32 size, TargetMpuConfigType** region_config, uint8 enable_region_num)
{
	bool has_permission;

	if (access_type == MPU_TARGET_ACCESS_EXEC) {
		has_permission = mpu_has_permission_access_region_mip(addr, size, region_config, enable_region_num);
	}
	else {
		has_permission = mpu_has_permission_access_region_mdp(addr, size, region_config, enable_region_num);
	}

	return has_permission;
}

void mpu_set_error(TargetCoreType *cpu)
{
	uint32* sysregp;
	uint32 data;
	OpCodeId code_id;

	/* MEA */
	sysregp = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_2, SYS_REG_MEA);
	*sysregp = cpu->mpu.excption_error_info.error_addr;

	if (cpu->mpu.excption_error_info.intc_error_info.access_type == MPU_TARGET_ACCESS_EXEC){
		return;
	}

	/* MEI */
	code_id = cpu->mpu.excption_error_info.op_codeid;

	data = 0U;
	/* reg */
	switch (memory_error_info[code_id].reg) {
	case TARGET_MPU_MEI_REG_0:
	case TARGET_MPU_MEI_REG_1:
	case TARGET_MPU_MEI_REG_2:
	case TARGET_MPU_MEI_REG_3:
		data = (memory_error_info[code_id].reg << TARGET_MPU_SYSREG_MEI_REG_OFFSET);
		break;
	case TARGET_MPU_MEI_REG_NONE:
		/* nop */
		break;
	default:
		break;
	}

	/* ds */
	data |= (memory_error_info[code_id].data_type << TARGET_MPU_SYSREG_MEI_DS_OFFSET);

	/* u */
	data |= (memory_error_info[code_id].sign << TARGET_MPU_SYSREG_MEI_U_OFFSET);

	/* itype */
	data |= (memory_error_info[code_id].itype << TARGET_MPU_SYSREG_MEI_ITYPE_OFFSET);

	/* rw */
	data |= (memory_error_info[code_id].rw << TARGET_MPU_SYSREG_MEI_RW_OFFSET);

	sysregp = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_2, SYS_REG_MEI);
	*sysregp = data;

	return;
}

#define TARGET_MPU_CHECK_ADDR_OFFSET			0x7FFFFFFF
#define TARGET_MPU_CHECK_SET(permission)		(permission) ? 1U : 0U

#define TARGET_MPU_SYSREG_MCR_OV_OFFSET			8U
#define TARGET_MPU_SYSREG_MCR_SXE_OFFSET		5U
#define TARGET_MPU_SYSREG_MCR_SWE_OFFSET		4U
#define TARGET_MPU_SYSREG_MCR_SRE_OFFSET		3U
#define TARGET_MPU_SYSREG_MCR_UXE_OFFSET		2U
#define TARGET_MPU_SYSREG_MCR_UWE_OFFSET		1U
#define TARGET_MPU_SYSREG_MCR_URE_OFFSET		0U

static void mpu_check_permission(TargetMpuEnableAccessType *permission, TargetMpuEnableAccessType *result)
{
	if (permission->exec == TRUE) {
		result->exec = TRUE;
	}
	if (permission->read == TRUE) {
		result->read = TRUE;
	}
	if (permission->write == TRUE) {
		result->write = TRUE;
	}
	return;
}

void mpu_check(TargetCoreType *cpu)
{
	bool ov_flag;
	uint32 *mcap;
	uint32 *mcsp;
	uint32 *mcrp;
	uint32 addr_start;
	uint32 addr_end;
	uint32 data;
	TargetMpuEnableAccessType sv;
	TargetMpuEnableAccessType um;

	TargetMpuConfigType* mpu_config_obj;
	uint8 index;

	mcap = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_2, SYS_REG_MCA);
	mcsp = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_2, SYS_REG_MCS);
	mcrp = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_2, SYS_REG_MCR);

	addr_start = *mcap;
	addr_end = *mcap + *mcsp;

	sv.exec = FALSE;
	sv.read = FALSE;
	sv.write = FALSE;
	um.exec = FALSE;
	um.read = FALSE;
	um.write = FALSE;

	data = 0U;

	/* ov */
	if (addr_start > addr_end) {
		ov_flag = TRUE;
	}
	else if ((addr_start <= TARGET_MPU_CHECK_ADDR_OFFSET) && (addr_end > TARGET_MPU_CHECK_ADDR_OFFSET)) {
		ov_flag = TRUE;
	}
	else {
		ov_flag = FALSE;
	}

	/* permission */
	for(index = 0U; index < TARGET_MPU_CONFIG_NUM; index++){
		mpu_config_obj = (TargetMpuConfigType*)object_container_get_element(cpu->mpu.permission_region, index);
		if (!(mpu_config_obj->addr_start <= addr_start) && (addr_end <= mpu_config_obj->addr_end)) {
			continue;
		}
		mpu_check_permission(&(mpu_config_obj->is_enable_access_sv), &sv);
		mpu_check_permission(&(mpu_config_obj->is_enable_access_um), &um);
	}


	/* MCR */
	data = (TARGET_MPU_CHECK_SET(ov_flag) << TARGET_MPU_SYSREG_MCR_OV_OFFSET)		|
			(TARGET_MPU_CHECK_SET(sv.exec) << TARGET_MPU_SYSREG_MCR_SXE_OFFSET)		|
			(TARGET_MPU_CHECK_SET(sv.read) << TARGET_MPU_SYSREG_MCR_SRE_OFFSET)		|
			(TARGET_MPU_CHECK_SET(sv.write) << TARGET_MPU_SYSREG_MCR_SWE_OFFSET)	|
			(TARGET_MPU_CHECK_SET(um.exec) << TARGET_MPU_SYSREG_MCR_UXE_OFFSET)		|
			(TARGET_MPU_CHECK_SET(um.read) << TARGET_MPU_SYSREG_MCR_URE_OFFSET)		|
			(TARGET_MPU_CHECK_SET(um.write) << TARGET_MPU_SYSREG_MCR_UWE_OFFSET);
	*mcrp = data;

	return;
}

#define TARGET_MPU_IS_MPLA(regid)	((regid % 4U) == 0U)
#define TARGET_MPU_IS_MPUA(regid)	((regid % 4U) == 1U)
#define TARGET_MPU_IS_MPTA(regid)	((regid % 4U) == 2U)

#define TARGET_MPU_SYSREG_MPLA_READ_MASK(data)		(data & 0xFFFFFFFC)
#define TARGET_MPU_SYSREG_MPUA_READ_MASK(data)		(data & 0xFFFFFFFC)

static void mpu_hook_access_read(TargetCoreType *cpu, uint32 *data, uint8 regid, uint8 selid)
{
	if ((selid == SYS_GRP_5) && (regid == SYS_REG_MPBRGN)) {
		*data = 0U;
		return;
	}
	if((selid == SYS_GRP_5) && (regid == SYS_REG_MPTRGN)) {
		*data = TARGET_MPU_CONFIG_NUM;
		return;
	}

	if ((selid == SYS_GRP_6) || (selid == SYS_GRP_7)) {
		if (TARGET_MPU_IS_MPLA(regid)) {
			*data = TARGET_MPU_SYSREG_MPLA_READ_MASK(*data);
			return;
		}
		if (TARGET_MPU_IS_MPUA(regid)) {
			*data = TARGET_MPU_SYSREG_MPUA_READ_MASK(*data);
			return;
		}
	}

	if ((selid == SYS_GRP_5) && (regid == SYS_REG_MCC)) {
		*data = 0U;
	}

	return;
}

#define TARGET_MPU_SYSREG_MPAT_E_OFFSET			7U
#define TARGET_MPU_SYSREG_MRC_En_IS_ENBALE(index)			((*data & (1U << index)) && (1U << index))


static void mpu_hook_access_write_mprc(TargetCoreType *cpu, uint32 *data)
{
	uint8 index;
	uint8 selid;
	uint8 regid;
	uint8 base;
	uint32* sysregp;

	for(index = 0U; index < TARGET_MPU_CONFIG_NUM; index++){
		if (index < TARGET_MPU_SYSREG7_OFFSET_INDEX) {
			selid = SYS_GRP_6;
			base = TARGET_MPU_SYSREG_PROTECTION_REGION_BASE(index, TARGET_MPU_SYSREG6_OFFSET_INDEX);
		}
		else {
			selid = SYS_GRP_7;
			base = TARGET_MPU_SYSREG_PROTECTION_REGION_BASE(index, TARGET_MPU_SYSREG7_OFFSET_INDEX);
		}

		regid = TARGET_MPU_SYSREG_MPAT(base);
		sysregp = cpu_get_sysreg2(&cpu->reg.sys, selid, regid);

		if (TARGET_MPU_SYSREG_MRC_En_IS_ENBALE(index)) {
			*sysregp |= (1U << TARGET_MPU_SYSREG_MPAT_E_OFFSET);
		}
		else {
			*sysregp &= ~(1U << TARGET_MPU_SYSREG_MPAT_E_OFFSET);
		}

	}

	return;
}

#define TARGET_MPU_GET_CONFIG_INDEX(base, regid)	(base + (regid / 4U))

static uint8 mpu_hook_access_write_mpta_get_config_index(uint8 regid, uint8 selid)
{
	uint8 base;

	if (selid == SYS_GRP_6) {
		base = TARGET_MPU_SYSREG6_OFFSET_INDEX;
	}
	else{
		base = TARGET_MPU_SYSREG7_OFFSET_INDEX;
	}

	return TARGET_MPU_GET_CONFIG_INDEX(base, regid);
}

static void mpu_hook_access_write_mpta(TargetCoreType *cpu, uint32 *data, uint8 regid, uint8 selid)
{

	uint32* sysregp;
	uint8 value;
	uint8 index;
	sysregp = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_5, SYS_REG_MPRC);

	value = ((*data & TARGET_MPU_SYSREG_MPAT_E) >> TARGET_MPU_SYSREG_MPAT_E_OFFSET);

	index = mpu_hook_access_write_mpta_get_config_index(regid, selid);

	if (value == 0U) {
		*sysregp &= ~(1U << index);
	}
	else {
		*sysregp |= (1U << index);
	}

	return;
}

static void mpu_hook_access_write(TargetCoreType *cpu, uint32 *data, uint8 regid, uint8 selid)
{

	if ((selid == SYS_GRP_5) && (regid == SYS_REG_MPRC)) {
		mpu_hook_access_write_mprc(cpu, data);
		return;
	}
	if ((selid == SYS_GRP_6) || (selid == SYS_GRP_7)) {
		if (TARGET_MPU_IS_MPTA(regid)) {
			mpu_hook_access_write_mpta(cpu, data, regid, selid);
			return;
		}
	}

	return;
}

void mpu_hook_access(TargetCoreType *cpu, uint32 *data, TargetMpuAccessType access_type, uint8 regid, uint8 selid)
{

	if (access_type == MPU_TARGET_ACCESS_READ) {
		mpu_hook_access_read(cpu, data, regid, selid);
	}
	else {
		mpu_hook_access_write(cpu, data, regid, selid);
	}

	return;
}

static void mpu_debug_print_mpu_status_config(TargetCoreType *cpu, uint8 base, uint8 selid, uint8 reg_num)
{
	uint8 regid;
	uint32* sysreg;

	for (regid = 0U; regid < reg_num; regid++) {
		if (TARGET_MPU_IS_MPLA(regid)){
			sysreg = cpu_get_sysreg2(&cpu->reg.sys, selid, regid);
			printf("MPLA%d		0x%x\n", TARGET_MPU_GET_CONFIG_INDEX(base, regid), *sysreg);
		}
		else if (TARGET_MPU_IS_MPUA(regid)) {
			sysreg = cpu_get_sysreg2(&cpu->reg.sys, selid, regid);
			printf("MPUA%d		0x%x\n", TARGET_MPU_GET_CONFIG_INDEX(base, regid), *sysreg);
		}
		else if (TARGET_MPU_IS_MPTA(regid)) {
			sysreg = cpu_get_sysreg2(&cpu->reg.sys, selid, regid);
			printf("MATA%d		0x%x\n", TARGET_MPU_GET_CONFIG_INDEX(base, regid), *sysreg);
		}
		else {
			/* nop */
		}

	}
	return;

}

void mpu_debug_print_mpu_status(TargetCoreType *cpu)
{
	uint32* sysreg;

	/* MPM 0000 0000*/
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_5, SYS_REG_MPM);
	printf("MPM		0x%x\n", *sysreg);

	/* MPRC 0000 0000*/
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_5, SYS_REG_MPRC);
	printf("MPRC		0x%x\n", *sysreg);

	/* MPBRGN 0000 0000*/
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_5, SYS_REG_MPBRGN);
	printf("MPBRGN		0x%x\n", *sysreg);

	/* MPTRGN 0001 0000*/
	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_5, SYS_REG_MPTRGN);
	printf("MPTRGN		0x%x\n", *sysreg);

	/* MPATn(0~15) 0000 0000 */
	mpu_debug_print_mpu_status_config(cpu, TARGET_MPU_SYSREG6_OFFSET_INDEX, SYS_GRP_6, SYS_SEL6_REG_NUM);
	mpu_debug_print_mpu_status_config(cpu, TARGET_MPU_SYSREG7_OFFSET_INDEX, SYS_GRP_7, SYS_SEL7_REG_NUM);

	return;
}
