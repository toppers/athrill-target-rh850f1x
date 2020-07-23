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
#include "cpu.h"
#include "bus.h"
#include "std_cpu_ops.h"
#include <stdio.h>
#include "cpu_common/cpu_ops.h"
#include "cpu_dec/op_parse.h"
#include "cpu_exec/op_exec.h"
#include "mpu_types.h"
#include <sys/time.h>
#include "std_device_ops.h"
#include "device.h"


#include "target_cpu.h"
#include "target_mpu_op.h"

CpuType virtual_cpu;

void cpu_init(void)
{
	CoreIdType i;
	for (i = 0; i < cpu_config_get_core_id_num(); i++) {
		virtual_cpu.cores[i].core.core_id = i;
		cpu_reset(i);
	}
	return;
}
bool cpu_has_permission(CoreIdType core_id, MpuAddressRegionEnumType region_type, CpuMemoryAccessType access_type, uint32 addr, uint32 size)
{
	TargetMpuAccessType mpu_access_type;

	switch (access_type) {
	case CpuMemoryAccess_READ:
		mpu_access_type = MPU_TARGET_ACCESS_READ;
		break;
	case CpuMemoryAccess_WRITE:
		mpu_access_type = MPU_TARGET_ACCESS_WRITE;
		break;
	case CpuMemoryAccess_EXEC:
		mpu_access_type = MPU_TARGET_ACCESS_EXEC;
		break;
	case CpuMemoryAccess_NONE:
	default:
		mpu_access_type = MPU_TARGET_ACCESS_NONE;
		break;
	}

	return target_mpu_has_permission(&(virtual_cpu.cores[core_id].core), mpu_access_type, addr, size);
}

bool cpu_illegal_access(CoreIdType core_id)
{
	if (virtual_cpu.cores[core_id].core.current_code_exception.is_code_exception == FALSE) {
		return FALSE;
	}

	switch (virtual_cpu.cores[core_id].core.current_code_exception.exception_type) {
	case ExceptionId_PIE:
		intc_raise_exception(core_id, ExceptionId_PIE);
		break;
	case ExceptionId_MDP:
		intc_raise_exception(core_id, ExceptionId_MDP);
		target_mpu_set_error(&(virtual_cpu.cores[core_id].core));
		break;
	case ExceptionId_MIP:
		intc_raise_exception(core_id, ExceptionId_MIP);
		target_mpu_set_error(&(virtual_cpu.cores[core_id].core));
		break;
	default:
		break;
	}

	virtual_cpu.cores[core_id].core.current_code_exception.is_code_exception = FALSE;
	virtual_cpu.cores[core_id].core.current_code_exception.exception_type = ExceptionId_Num;
	virtual_cpu.cores[core_id].core.mpu.excption_error_info.is_error_occurred = FALSE;

	return TRUE;
}

static void private_sys_init(void)
{
	(void)bus_put_data32(0, (uint32)PLLS,  0x00000004);
	return;
}

static void private_cpu_reset(TargetCoreType *cpu)
{
	uint32 *sysreg;
	cpu->reg.pc = 0x00;
	cpu->reg.r[0] = 0;

	cpu->reg.sys.current_grp = SYS_GRP_CPU;
	cpu->reg.sys.current_bnk = SYS_GRP_CPU_BNK_0;
	for (int regId = 0; regId < CPU_GREG_NUM; regId++) {
		sysreg = cpu_get_sysreg(&cpu->reg.sys, regId);
		*sysreg = 0;
	}
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW] = 0x20;

	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FPSR] = (1U << 17); // FS bit on

	sysreg = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_2, SYS_REG_HTCFG0);
	*sysreg = ( ((cpu->core_id + 1) << 16) | 0x8000 );

	private_sys_init();
	cpu->is_halt = FALSE;
	cpu->current_code_exception.is_code_exception = FALSE;
	cpu->current_code_exception.exception_type = ExceptionId_Num;
	return;
}

void cpu_reset(CoreIdType core_id)
{
	private_cpu_reset(&virtual_cpu.cores[core_id].core);
	target_mpu_init(&virtual_cpu.cores[core_id].core);
	virtual_cpu.cores[core_id].core.fatal_error = FALSE;
	virtual_cpu.cores[core_id].core.current_code_exception.exception_type = ExceptionId_Num;
	virtual_cpu.cores[core_id].core.current_code_exception.is_code_exception = FALSE;

	return;
}
bool cpu_is_halt(CoreIdType core_id)
{
	return virtual_cpu.cores[core_id].core.is_halt;
}
void cpu_set_current_core(CoreIdType core_id)
{
	virtual_cpu.current_core = &virtual_cpu.cores[core_id];
	return;
}

static Std_ReturnType cpu_supply_clock_not_cached(CoreIdType core_id, CachedOperationCodeType *cached_code, uint32 inx)
{
	int ret;
	Std_ReturnType err;
	static OpDecodedCodeType	decoded_code;
	OpDecodedCodeType		*p_decoded_code;
	OperationCodeType optype;
	bool permission;

	if (cached_code != NULL) {
		p_decoded_code = &cached_code->codes[inx].decoded_code;
		virtual_cpu.cores[core_id].core.decoded_code = &cached_code->codes[inx].decoded_code;
	}
	else {
		p_decoded_code = &decoded_code;
		virtual_cpu.cores[core_id].core.decoded_code = &decoded_code;
	}
	/*
	 * 命令取得する
	 */
	err = bus_get_pointer(core_id,
			virtual_cpu.cores[core_id].core.reg.pc,
				(uint8**)&(virtual_cpu.cores[core_id].core.current_code));
	if (err != STD_E_OK) {
		return err;
	}

	/*
	 * デコード
	 */
	ret = op_parse(virtual_cpu.cores[core_id].core.current_code,
			p_decoded_code, &optype);
	if (ret < 0) {
		printf("Decode Error\n");
		return STD_E_DECODE;
	}

	permission = cpu_has_permission(core_id,
			READONLY_MEMORY,
			CpuMemoryAccess_EXEC,
			virtual_cpu.cores[core_id].core.reg.pc,
			OpFormatSize[p_decoded_code->type_id]);
	if (permission == FALSE) {
		if (virtual_cpu.cores[core_id].core.mpu.excption_error_info.is_error_occurred) {
			virtual_cpu.cores[core_id].core.mpu.excption_error_info.op_codeid = optype.code_id;
			virtual_cpu.cores[core_id].core.current_code_exception.is_code_exception = TRUE;
			virtual_cpu.cores[core_id].core.current_code_exception.exception_type = ExceptionId_MIP;
			return -1;
		}
		return STD_E_SEGV;
	}

	if (op_exec_table[optype.code_id].exec == NULL) {
		printf("Not supported code(%d fmt=%d) Error code[0]=0x%x code[1]=0x%x type_id=0x%x\n",
				optype.code_id, optype.format_id,
				virtual_cpu.cores[core_id].core.current_code[0],
				virtual_cpu.cores[core_id].core.current_code[1],
				virtual_cpu.cores[core_id].core.decoded_code->type_id);
		return STD_E_EXEC;
	}

	/*
	 * 命令実行
	 */
	virtual_cpu.cores[core_id].core.real_elaps = (uint32)op_exec_table[optype.code_id].clocks;
	ret = op_exec_table[optype.code_id].exec(&virtual_cpu.cores[core_id].core);

	if (virtual_cpu.cores[core_id].core.mpu.excption_error_info.is_error_occurred) {
		virtual_cpu.cores[core_id].core.mpu.excption_error_info.op_codeid = optype.code_id;
		virtual_cpu.cores[core_id].core.current_code_exception.is_code_exception = TRUE;
		virtual_cpu.cores[core_id].core.current_code_exception.exception_type = ExceptionId_MDP;
		return -1;
	}

	if (ret < 0) {
		printf("Exec Error code[0]=0x%x code[1]=0x%x type_id=0x%x code_id=%u\n",
				virtual_cpu.cores[core_id].core.current_code[0],
				virtual_cpu.cores[core_id].core.current_code[1],
				virtual_cpu.cores[core_id].core.decoded_code->type_id,
				optype.code_id);
		return STD_E_EXEC;
	}
	virtual_cpu.cores[core_id].elaps += (uint64)virtual_cpu.cores[core_id].core.real_elaps;

	if (cached_code != NULL) {
		cached_code->codes[inx].code_id = optype.code_id;
		cached_code->codes[inx].op_exec = op_exec_table[optype.code_id].exec;
	}
	return STD_E_OK;
}

Std_ReturnType cpu_supply_clock(CoreIdType core_id)
{
	int ret;
	Std_ReturnType err;
	uint32 inx = 0U;
	CachedOperationCodeType *cached_code;

	if (virtual_cpu.cores[core_id].core.fatal_error) {
		return STD_E_INVALID;
	}

	if (virtual_cpu.cores[core_id].core.is_halt == TRUE) {
		return STD_E_OK;
	}
	virtual_cpu.cores[core_id].core.reg.r[0] = 0U;

	cached_code = virtual_cpu_get_cached_code(virtual_cpu.cores[core_id].core.reg.pc);
	if (cached_code != NULL) {
		inx = virtual_cpu.cores[core_id].core.reg.pc - cached_code->code_start_addr;
	}
	if ((cached_code == NULL) || (cached_code->codes[inx].op_exec == NULL)) {
		err = cpu_supply_clock_not_cached(core_id, cached_code, inx);
		if (err != STD_E_OK) {
			return err;
		}
		virtual_cpu.cores[core_id].core.reg.r[0] = 0U;
	}
	else {
		OpCodeId code_id = cached_code->codes[inx].code_id;
#ifdef CONFIG_STAT_PERF
		PROFSTAT_START(&op_exec_stat_table[code_id]);
#endif /* CONFIG_STAT_PERF */
		virtual_cpu.cores[core_id].core.decoded_code = &cached_code->codes[inx].decoded_code;
#ifndef DISABLE_MEMPROTECT
		bool permission;
		permission = cpu_has_permission(core_id,
				READONLY_MEMORY,
				CpuMemoryAccess_EXEC,
				virtual_cpu.cores[core_id].core.reg.pc,
				OpFormatSize[cached_code->codes[inx].decoded_code.type_id]);
		if (permission == FALSE) {
			if (virtual_cpu.cores[core_id].core.mpu.excption_error_info.is_error_occurred) {
				virtual_cpu.cores[core_id].core.mpu.excption_error_info.op_codeid = code_id;
				virtual_cpu.cores[core_id].core.current_code_exception.is_code_exception = TRUE;
				virtual_cpu.cores[core_id].core.current_code_exception.exception_type = ExceptionId_MIP;
				return -1;
			}
			return STD_E_SEGV;
		}
#endif /* DISABLE_MEMPROTECT */
		virtual_cpu.cores[core_id].core.real_elaps = (uint32)op_exec_table[code_id].clocks;
		ret = cached_code->codes[inx].op_exec(&virtual_cpu.cores[core_id].core);
#ifdef CONFIG_STAT_PERF
		PROFSTAT_END(&op_exec_stat_table[code_id]);
#endif /* CONFIG_STAT_PERF */

		if (virtual_cpu.cores[core_id].core.mpu.excption_error_info.is_error_occurred) {
			virtual_cpu.cores[core_id].core.mpu.excption_error_info.op_codeid = code_id;
			virtual_cpu.cores[core_id].core.current_code_exception.is_code_exception = TRUE;
			virtual_cpu.cores[core_id].core.current_code_exception.exception_type = ExceptionId_MDP;
			return -1;
		}

		if (ret < 0) {
			printf("Exec Error code[0]=0x%x code[1]=0x%x type_id=0x%x\n",
					virtual_cpu.cores[core_id].core.current_code[0],
					virtual_cpu.cores[core_id].core.current_code[1],
					virtual_cpu.cores[core_id].core.decoded_code->type_id);
			return STD_E_EXEC;
		}
		virtual_cpu.cores[core_id].elaps += (uint64)virtual_cpu.cores[core_id].core.real_elaps;
		virtual_cpu.cores[core_id].core.reg.r[0] = 0U;
	}
	return STD_E_OK;
}

void cpu_illegal_opcode_trap(CoreIdType core_id)
{
	uint32 eicc;
	uint32 ecr;

	eicc = 0x60;
	sys_get_cpu_base(&virtual_cpu.cores[core_id].core.reg)->r[SYS_REG_EIPC] = virtual_cpu.cores[core_id].core.reg.pc - 4;
	sys_get_cpu_base(&virtual_cpu.cores[core_id].core.reg)->r[SYS_REG_EIPSW] = sys_get_cpu_base(&virtual_cpu.cores[core_id].core.reg)->r[SYS_REG_PSW];

	ecr = sys_get_cpu_base(&virtual_cpu.cores[core_id].core.reg)->r[SYS_REG_ECR];
	ecr = ecr & 0x00FF;
	ecr |= (eicc << 16);
	sys_get_cpu_base(&virtual_cpu.cores[core_id].core.reg)->r[SYS_REG_ECR] = ecr;
	CPU_SET_NP(&virtual_cpu.cores[core_id].core.reg);
	CPU_SET_EP(&virtual_cpu.cores[core_id].core.reg);
	CPU_SET_ID(&virtual_cpu.cores[core_id].core.reg);
	virtual_cpu.cores[core_id].core.reg.pc = 0x60;

	return;
}


static Std_ReturnType cpu_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data);
static Std_ReturnType cpu_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data);

MpuAddressRegionOperationType cpu_register_operation = {
		.get_data8 = NULL,
		.get_data16 = NULL,
		.get_data32 = cpu_get_data32,
		.put_data8 = NULL,
		.put_data16 = NULL,
		.put_data32 = cpu_put_data32,
};
static uint32 *get_cpu_register_addr(MpuAddressRegionType *region, TargetCoreType *core, uint32 addr)
{
	uint32 inx = (addr - CPU_CONFIG_DEBUG_REGISTER_ADDR) / sizeof(uint32);

	//printf("get_cpu_register_addr:inx=%u\n", inx);
	if (inx >= 0 && inx <= 31) {
		return (uint32*)&core->reg.r[inx];
	}
	else if (addr == CPU_CONFIG_ADDR_PEID) {
		inx = (addr - CPU_CONFIG_DEBUG_REGISTER_ADDR) * core->core_id;
		return (uint32*)&region->data[inx];
	}
	else if ((addr >= CPU_CONFIG_ADDR_MEV_0) && (addr <= CPU_CONFIG_ADDR_MEV_7)) {
		inx = (addr - CPU_CONFIG_DEBUG_REGISTER_ADDR);
		return (uint32*)&region->data[inx];
	}
	else if ((addr >= CPU_CONFIG_ADDR_MIR_0) && (addr <= CPU_CONFIG_ADDR_MIR_1)) {
		inx = (addr - CPU_CONFIG_DEBUG_REGISTER_ADDR);
		return (uint32*)&region->data[inx];
	}
	return NULL;
}
static Std_ReturnType cpu_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data)
{
	uint32 *registerp = get_cpu_register_addr(region, &virtual_cpu.current_core->core, addr);
	if (registerp == NULL) {
		return STD_E_SEGV;
	}
	else if (addr == CPU_CONFIG_ADDR_PEID) {
		*registerp = (core_id + 1);
	}
	*data = *registerp;
	return STD_E_OK;
}

static Std_ReturnType cpu_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data)
{
	uint32 *registerp = get_cpu_register_addr(region, &virtual_cpu.current_core->core, addr);
	if (registerp == NULL) {
		return STD_E_SEGV;
	}
	else if (addr == CPU_CONFIG_ADDR_PEID) {
		return STD_E_SEGV;
	}
	else if ((addr == CPU_CONFIG_ADDR_MIR_0)) {
		intc_cpu_trigger_interrupt(core_id, CPU_CONFIG_ADDR_MIR_0_INTNO);
		return STD_E_OK;
	}
	else if ((addr == CPU_CONFIG_ADDR_MIR_1)) {
		intc_cpu_trigger_interrupt(core_id, CPU_CONFIG_ADDR_MIR_1_INTNO);
		return STD_E_OK;
	}
	*registerp = data;
	return STD_E_OK;
}


uint32 cpu_get_pc(const TargetCoreType *core)
{
	return core->reg.pc;
}
uint32 cpu_get_ep(const TargetCoreType *core)
{
	return core->reg.r[30];
}
uint32 cpu_get_current_core_id(void)
{
	return ((const TargetCoreType *)virtual_cpu.current_core)->core_id;
}
uint32 cpu_get_current_core_pc(void)
{
	return cpu_get_pc((const TargetCoreType *)virtual_cpu.current_core);
}
void cpu_set_core_pc(CoreIdType core_id, uint32 pc)
{
	virtual_cpu.cores[core_id].core.reg.pc = pc;
	return;
}
uint32 cpu_get_current_core_register(uint32 inx)
{
	return ((TargetCoreType *)virtual_cpu.current_core)->reg.r[inx];
}

uint32 cpu_get_sp(const TargetCoreType *core)
{
	return core->reg.r[3];
}
uint32 cpu_get_current_core_sp(void)
{
	return cpu_get_sp((const TargetCoreType *)virtual_cpu.current_core);
}
uint32 cpu_get_current_core_ep(void)
{
	return cpu_get_ep((const TargetCoreType *)virtual_cpu.current_core);
}


uint32 cpu_get_return_addr(const TargetCoreType *core)
{
	return core->reg.r[31];
}
CoreIdType cpu_get_core_id(const TargetCoreType *core)
{
	return core->core_id;
}

