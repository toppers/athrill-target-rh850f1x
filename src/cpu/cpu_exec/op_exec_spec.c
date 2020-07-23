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
#include "cpu_exec/op_exec_ops.h"
#include "cpu.h"
#include "bus.h"
#include "device.h"
#include <stdio.h> //TODO

#include "target_mpu_op.h"

#define CPU_PSW_UM_MASK		0x0000001F

static bool sys_reg_access_has_permission(TargetCoreType *cpu, uint32 regid, uint32 selid)
{

	/* PSW  */
	if (((selid == SYS_GRP_CPU) && (regid == SYS_REG_PSW))) {
		return TRUE;
	}
	else {
		/* SV Mode check */
		if (CPU_ISSET_UM(&(cpu->reg))) {
			return FALSE;
		}
		else {
			return TRUE;
		}
	}
}

static int get_sysreg2(CpuRegisterType *cpu, uint32 regid, uint32 selid, uint32 **regp)
{
	if (regid >= CPU_GREG_NUM) {
		return -1;
	}

	*regp = cpu_get_sysreg2(&cpu->sys, selid, regid);

	return 0;
}

/*
 * Format9
 */
int op_exec_ldsr(TargetCoreType *cpu)
{
	int ret;
	uint32 *sysreg;
	uint32 regid = cpu->decoded_code->type9.reg2;
	uint32 reg2 = cpu->decoded_code->type9.gen;
	uint32 selid = cpu->decoded_code->type9.rfu2;
	uint32 data;

	if (reg2 >= CPU_GREG_NUM) {
		printf("ERROR: ldsr reg=%d regID=%d\n", reg2, regid);
		return -1;
	}
	if (regid >= CPU_GREG_NUM) {
		printf("ERROR: ldsr reg=%d regID=%d\n", reg2, regid);
		return -1;
	}

	/* PIE check */
	if (!sys_reg_access_has_permission(cpu, regid, selid)) {
		cpu->current_code_exception.is_code_exception = TRUE;
		cpu->current_code_exception.exception_type = ExceptionId_PIE;
		return 0;
	}

	ret = get_sysreg2(&cpu->reg, regid, selid, &sysreg);
	if (ret < 0) {
		printf("ERROR: ldsr reg=%d regID=%d\n", reg2, regid);
		return -1;
	}

	if ((selid == SYS_GRP_2) && (regid == SYS_REG_ISPR)) {
		if (CPU_ISSET_INTCFG_ISPC(cpu)) {
			*sysreg = cpu->reg.r[reg2];
		}
	}
	else if ((selid == SYS_GRP_CPU) && (regid == SYS_REG_PSW)) {
		if (CPU_ISSET_UM(&(cpu->reg))){
			*sysreg |= (cpu->reg.r[reg2] & CPU_PSW_UM_MASK);
		}
		else {
			*sysreg = cpu->reg.r[reg2];
		}
	}
	else if (((selid == SYS_GRP_5) && (regid == SYS_REG_MCC))) {
		target_mpu_check(cpu);
	}
	else if (((selid == SYS_GRP_5) || (selid == SYS_GRP_6) || (selid == SYS_GRP_7))) {
		data = cpu->reg.r[reg2];
		target_mpu_hook_access(cpu, &data, MPU_TARGET_ACCESS_WRITE, selid, regid);
		*sysreg = data;
		target_mpu_set_config(cpu);
	}
	else {
		*sysreg = cpu->reg.r[reg2];
	}
	fpu_sync_sysreg(cpu, regid, selid);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LDSR r%d(0x%x) regID(%d) selID(%d):0x%x\n",
		cpu->reg.pc, reg2, cpu->reg.r[reg2], regid, selid, *sysreg));

	cpu->reg.pc += 4;

	return 0;
}

int op_exec_stsr(TargetCoreType *cpu)
{
	int ret;
	uint32 *sysreg;
	uint32 regid = cpu->decoded_code->type9.gen;
	uint32 reg2 = cpu->decoded_code->type9.reg2;
	uint32 selid = cpu->decoded_code->type9.rfu2;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (regid >= CPU_SYSREG_NUM) {
		return -1;
	}

	/* PIE check */
	if (!sys_reg_access_has_permission(cpu, regid, selid)) {
		cpu->current_code_exception.is_code_exception = TRUE;
		cpu->current_code_exception.exception_type = ExceptionId_PIE;
		return 0;
	}

	ret = get_sysreg2(&cpu->reg, regid, selid, &sysreg);
	if (ret < 0) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: STSR regID(%d) selID(%d) r%d(0x%x):0x%x\n",
		cpu->reg.pc, regid, selid, reg2, cpu->reg.r[reg2], *sysreg));

	cpu->reg.r[reg2] = *sysreg;

	cpu->reg.pc += 4;
	return 0;
}

/*
 * Format10
 */


int op_exec_diei(TargetCoreType *cpu)
{
	if (!(SYS_ISSET_BIT(cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_1, SYS_REG_MCTL), 0))) {
		if (CPU_ISSET_UM(&cpu->reg) == TRUE) {
			cpu->current_code_exception.is_code_exception = TRUE;
			cpu->current_code_exception.exception_type = ExceptionId_PIE;
			printf("ERROR: invalid ei/di access because of no supervisor mode or usermode MCTL.UIC is not set.\n");
			return -1;
		}
	}

	if (cpu->decoded_code->type10.gen1 == 0x04) {
		/* EI */
		CPU_CLR_ID(&cpu->reg);
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: EI\n", cpu->reg.pc));
	}
	else {
		/* DI */
		CPU_SET_ID(&cpu->reg);
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: DI\n", cpu->reg.pc));

	}
	cpu->reg.pc += 4;

	return 0;
}

int op_exec_nop(TargetCoreType *cpu)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: NOP\n", cpu->reg.pc));

	cpu->reg.pc += 2;

	return 0;
}
int op_exec_reti(TargetCoreType *cpu)
{
	if (CPU_ISSET_NP(&cpu->reg)) {
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: RETI:0x%x\n", cpu->reg.pc, sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPC]));
		cpu->reg.pc = SYS_REG_FEPC_GET();
		sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW] = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPSW];
		intc_clr_fe(cpu->core_id);
	}
	else {
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: RETI:0x%x\n", cpu->reg.pc, sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPC]));
		//printf("0x%x: RETI:0x%x\n", cpu->cpu.pc, cpu->cpu.eipc);
		//fflush(stdout);
		cpu->reg.pc = SYS_REG_EIPC_GET();
		sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW] = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPSW];
		intc_clr_ei(cpu->core_id);
	}
	return 0;
}

int op_exec_feret_10(TargetCoreType *cpu)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: FERET:0x%x\n", cpu->reg.pc, sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPC]));
	cpu->reg.pc = SYS_REG_FEPC_GET();
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW] = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPSW];
	intc_clr_fe(cpu->core_id);
	return 0;
}
int op_exec_eiret_10(TargetCoreType *cpu)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: EIRET:0x%x\n", cpu->reg.pc, sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPC]));
	cpu->reg.pc = SYS_REG_EIPC_GET();
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW] = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPSW];
	intc_clr_ei(cpu->core_id);
	return 0;
}

int op_exec_halt(TargetCoreType *cpu)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: HALT:0x%x\n", cpu->reg.pc, cpu->reg.pc + 4));
	//printf("0x%x: HALT:0x%x\n", cpu->reg.pc, cpu->reg.pc + 4);
	//fflush(stdout);
	cpu->is_halt = TRUE;
	cpu->reg.pc += 4;
	return 0;
}

int op_exec_trap(TargetCoreType *cpu)
{
	int ret = -1;
	uint32 pc;
	uint32 eicc;
	uint32 ecr;
	uint32 vector = cpu->decoded_code->type10.gen2;

	if (vector <= 0x0F) {
		ret = 0;
		pc = 0x40;
		eicc = 0x40 + vector;
	}
	else if (vector <= 0x1F) {
		ret = 0;
		pc = 0x50;
		eicc = 0x50 + (0x0F & vector);
	}

	if (ret == 0) {
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: TRAP vector(0x%x):0x%x\n", cpu->reg.pc, vector, pc));
		sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPC] = cpu->reg.pc + 4;
		sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPSW] = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW];

		ecr = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_ECR];
		ecr = ecr & 0x0000FFFF;
		ecr |= (eicc << 16);
		sys_get_cpu_base(&cpu->reg)->r[SYS_REG_ECR] = ecr;
		CPU_SET_EP(&cpu->reg);
		CPU_SET_ID(&cpu->reg);
		cpu->reg.pc = pc;
	}

	return 0;
}

int op_exec_fetrap_1(TargetCoreType *cpu)
{
	uint32 pc = 0x00000030;
	uint32 fepc;
	uint32 fecc;
	uint32 vector = cpu->decoded_code->type1.reg2;
	uint32 psw = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW];
	uint32 ecr = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_ECR];

	fepc = cpu->reg.pc + 2U;
	fecc = 0x30 + vector;

	ecr = ecr & 0x0000FFFF;
	ecr |= (fecc << 16);

	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPC] = fepc;
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEIC] = fecc;
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPSW] = psw;
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_ECR] = ecr;

	CPU_SET_EP(&cpu->reg);
	CPU_SET_ID(&cpu->reg);
	CPU_SET_NP(&cpu->reg);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: FETRAP vector(0x%x):0x%x\n", cpu->reg.pc, vector, pc));

	cpu->reg.pc = pc;
	return 0;
}

int op_exec_syscall_10(TargetCoreType *cpu)
{
	Std_ReturnType err;
	uint32 pc;
	uint32 eipc;
	uint32 eiic;
	uint32 ecr;
	uint32 addr;
	uint32 data;
	uint32 psw = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW];
	uint32 scbp = *cpu_get_sysreg2(&cpu->reg.sys,  SYS_GRP_1, SYS_REG_SCBP);
	uint32 sccfg_size = ((*cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_1, SYS_REG_SCCFG)) & 0x000000FF);
	uint32 vector8 = (cpu->decoded_code->type10.rfu3 << 5U)
		| (cpu->decoded_code->type10.gen2);

	eipc = cpu->reg.pc + 4U;
	eiic = 0x8000 + vector8;
	ecr = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_ECR];
	ecr = ecr & 0xFFFF0000;
	ecr |= eiic;

	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPC] = eipc;
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIIC] = eiic;
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPSW] = psw;
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_ECR] = ecr;

	CPU_CLR_UM(&cpu->reg);
	CPU_SET_EP(&cpu->reg);
	CPU_SET_ID(&cpu->reg);


	if (vector8 <= sccfg_size) {
		addr = scbp + (vector8 << 2U);
	}
	else {
		addr = scbp;
	}
	err = bus_get_data32(cpu->core_id, addr, (uint32*)&data);
	if (err != STD_E_OK) {
		printf("ERROR:SYSCALL pc=0x%x vector8=%u addr=0x%x\n",
			cpu->reg.pc, vector8, addr);
		return -1;
	}

	pc = scbp + data;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SYSCALL vector8=%u addr=0x%x:0x%x\n",
		cpu->reg.pc, vector8, addr, pc));

	cpu->reg.pc = pc;
	return 0;
}

int op_exec_switch(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type1.reg1;
	uint32 reg1_data;
	uint32 addr;
	sint32 tmp_pc;
	uint32 next_pc;
	sint16 data16;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}

	reg1_data = cpu->reg.r[reg1];

	addr = (cpu->reg.pc + 2U) + (reg1_data << 1U);
	/*
	 * Load-memory (adr, Half-word)
	 */
	err = bus_get_data16(cpu->core_id, addr, (uint16*)&data16);
	if (err != STD_E_OK) {
		printf("ERROR:SWITCH pc=0x%x reg1=%u(0x%x) addr=0x%x\n", cpu->reg.pc, reg1, reg1_data, addr);
		return -1;
	}
	/*
	 * (sign-extend (Load-memory (adr, Half-word) ))
	 */
	tmp_pc = (sint32)( data16 );
	/*
	 * (sign-extend (Load-memory (adr, Half-word) ) ) logically shift left by 1
	 */
	tmp_pc <<= 1U;

	/*
	 * (PC + 2) + (sign-extend (Load-memory (adr, Half-word) ) ) logically shift left by 1
	 */
	next_pc = (cpu->reg.pc + 2U) + ((uint32)tmp_pc);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SWITCH r%d(%d):%d\n", cpu->reg.pc, reg1, cpu->reg.r[reg1], next_pc));



	cpu->reg.pc = next_pc;
	return 0;
}

/*
 * Format13
 */
int op_exec_prepare(TargetCoreType *cpu)
{
	uint16 subop = cpu->decoded_code->type13.gen & 0x0007;
	uint16 ff = cpu->decoded_code->type13.gen >> 3U;
	uint16 start_reg = 20U;
	uint16 i;
	uint32 addr;
	uint32 *addrp;
	uint32 *sp = (uint32*)&(cpu->reg.r[3]);	//sp:r3
	uint32 imm = ( cpu->decoded_code->type13.imm << 2U );
	Std_ReturnType err;

	bool has_permission;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: PREPARE sp=0x%x ", cpu->reg.pc, *sp));
	for (i = start_reg; i < 32; i++) {
		if (cpu->decoded_code->type13.list[i] == 0) {
			continue;
		}
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "r%u(0x%x) ", i, cpu->reg.r[i]));

		addr = (*sp) - 4U;

		has_permission = target_mpu_has_permission(cpu, MPU_TARGET_ACCESS_WRITE, addr, 4U);
		if (!(has_permission)) {
			return -1;
		}

		err = bus_get_pointer(cpu->core_id, addr, (uint8**)&addrp);
		if (err != STD_E_OK) {
			printf("ERROR:PREPARE pc=0x%x sp=0x%x\n", cpu->reg.pc, *sp);
			return -1;
		}
		*addrp = cpu->reg.r[i];
		*sp = addr;
		cpu->real_elaps += 1U;
	}
	*sp = (*sp) - imm;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "imm5(%u) ", imm));

	if (subop == 1U) {
		cpu->reg.pc += 4;
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), ":sp=0x%x\n", *sp));
		return 0;
	}

	addr = cpu->reg.pc + 4U;

	err = bus_get_pointer(cpu->core_id, addr, (uint8**)&addrp);
	if (err != STD_E_OK) {
		printf("ERROR:PREPARE pc=0x%x sp=0x%x\n", cpu->reg.pc, *sp);
		return -1;
	}

	switch (ff) {
	case 0b00:
		cpu->reg.r[30] = *sp;
		cpu->reg.pc += 4;
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "ep=0x%x\n", cpu->reg.r[30]));
		break;
	case 0b01:
		cpu->reg.r[30] = (sint32)(*((sint16*)addrp));
		cpu->reg.pc += 6;
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "ep=0x%x\n", cpu->reg.r[30]));
		break;
	case 0b10:
		cpu->reg.r[30] = ((uint32)(*((uint16*)addrp))) << 16U;
		cpu->reg.pc += 6;
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "ep=0x%x\n", cpu->reg.r[30]));
		break;
	case 0b11:
		cpu->reg.r[30] = (*((uint32*)addrp));
		cpu->reg.pc += 8;
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "ep=0x%x\n", cpu->reg.r[30]));
		break;
	default:
		printf("ERROR:PREPARE pc=0x%x sp=0x%x\n", cpu->reg.pc, *sp);
		return -1;
	}
	return 0;
}

int op_exec_dispose(TargetCoreType *cpu)
{
	uint16 reg1 = cpu->decoded_code->type13.gen;
	uint16 start_reg = 20U;
	uint16 i;
	uint32 addr;
	uint32 *addrp;
	uint32 *sp = (uint32*)&(cpu->reg.r[3]);	//sp:r3
	uint32 imm = ( cpu->decoded_code->type13.imm << 2U );
	Std_ReturnType err;

	bool has_permission;

	cpu->real_elaps = 0U;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: DISPOSE imm=0x%x sp=0x%x ", cpu->reg.pc, imm, *sp));

	*sp = (*sp) + imm;
	for (i = 31; i >= start_reg; i--) {
		if (cpu->decoded_code->type13.list[i] == 0) {
			continue;
		}

		addr = (*sp);

		has_permission = target_mpu_has_permission(cpu, MPU_TARGET_ACCESS_READ, addr, 4U);
		if (!(has_permission)) {
			return -1;
		}

		err = bus_get_pointer(cpu->core_id, addr, (uint8**)&addrp);
		if (err != STD_E_OK) {
			printf("ERROR:DISPOSE pc=0x%x sp=0x%x\n", cpu->reg.pc, *sp);
			return -1;
		}
		cpu->reg.r[i] = *addrp;
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "r%u(0x%x) ", i, cpu->reg.r[i]));
		*sp = addr + 4;
		cpu->real_elaps += 1U;
	}

	if (reg1 != 0U) {
		cpu->real_elaps += 6U;
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), ":pc=r%u(0x%x) sp=0x%x\n", reg1, cpu->reg.r[reg1], cpu->reg.r[3]));
		cpu->reg.pc = cpu->reg.r[reg1];
	}
	else {
		cpu->real_elaps += 2U;
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), ":pc=r%u(0x%x) sp=0x%x\n", reg1, cpu->reg.pc, cpu->reg.r[3]));
		cpu->reg.pc += 4;
	}

	return 0;
}

int op_exec_pushsp_11(TargetCoreType *cpu)
{
	uint16 rh = cpu->decoded_code->type11.reg1;
	uint16 rt = cpu->decoded_code->type11.reg3;
	uint32 *sp = (uint32*)&(cpu->reg.r[3]);	//sp:r3
	uint16 cur;
	uint32 tmp_sp;
	Std_ReturnType err;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: PUSHSP rh=%u rt=%u sp=0x%x ",
		cpu->reg.pc, rh, rt, *sp));

	if (rh > rt) {
		cpu->reg.pc += 4;
		return 0;
	}
	/*
	* if rh ≦ rt
	* then cur ← rh
	* end ← rt
	* tmp ← sp
	* while (cur ≦ end) {
	* tmp ← tmp – 4
	* adr ← tmp 注 1, 注 2
	* Store-memory (adr, GR[cur], Word)
	* cur ← cur + 1
	* }
	* sp ← tmp
	*/
	cur = rh;
	tmp_sp = ((*sp) & 0xFFFFFFFC);
	while (cur <= rt) {
		tmp_sp = tmp_sp - 4;
		err = bus_put_data32(cpu->core_id, tmp_sp, cpu->reg.r[cur]);
		if (err != STD_E_OK) {
			printf("ERROR:PUSHSP pc=0x%x sp=0x%x\n", cpu->reg.pc, *sp);
			return -1;
		}
		cur++;
		cpu->real_elaps += 1U;
	}
	*sp = tmp_sp;
	cpu->reg.pc += 4;
	return 0;
}

int op_exec_popsp_11(TargetCoreType *cpu)
{
	uint16 rh = cpu->decoded_code->type11.reg1;
	uint16 rt = cpu->decoded_code->type11.reg3;
	uint32 *sp = (uint32*)&(cpu->reg.r[3]);	//sp:r3
	uint16 cur;
	uint32 tmp_sp;
	Std_ReturnType err;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: POPSP rh=%u rt=%u sp=0x%x ",
		cpu->reg.pc, rh, rt, *sp));

	if (rh > rt) {
		cpu->reg.pc += 4;
		return 0;
	}
	/*
	* if rh ≦ rt
	* then cur ← rt
	* end ← rh
	* tmp ← sp
	* while (cur ≧ end) {
	* adr ← tmp 注 1, 注 2
	* GR[cur] ← Load-memory (adr, Word)
	* cur ← cur – 1
	* tmp ← tmp + 4
	* }
	* sp ← tmp
	*/
	cur = rt;
	tmp_sp = ((*sp) & 0xFFFFFFFC);
	while (cur >= rh) {
		err = bus_get_data32(cpu->core_id, tmp_sp, (uint32*)&(cpu->reg.r[cur]));
		if (err != STD_E_OK) {
			printf("ERROR:POPSP pc=0x%x sp=0x%x\n", cpu->reg.pc, *sp);
			return -1;
		}
		cur--;
		tmp_sp = tmp_sp + 4;
		cpu->real_elaps += 1U;
	}
	*sp = tmp_sp;
	cpu->reg.pc += 4;
	return 0;
}


/*
 * ［命令形式］ CAXI [reg1], reg2, reg3
 *
 * ［オペレーション］ adr ← GR[reg1]注
 * token ← Load-memory(adr, Word)
 * result ← GR[reg2] – token
 * If result == 0
 * then Store-memory(adr, GR[reg3],Word)
 * GR[reg3] ← token
 * else Store-memory(adr, token,Word)
 * GR[reg3] ← token
 * 注 GR[reg1]の下位 2 ビットは， 0 にマスクしadr とします。
 */
int op_exec_caxi(TargetCoreType *cpu)
{
	Std_ReturnType err;
	uint16 reg1 = cpu->decoded_code->type11.reg1;;
	uint16 reg2 = cpu->decoded_code->type11.reg2;
	uint16 reg3 = cpu->decoded_code->type11.reg3;
	sint16 token;
	sint16 result;
	uint16 put_data;

	uint32 reg1_addr = (cpu->reg.r[reg1] & 0xFFFFFFFC);
	uint32 reg2_data = cpu->reg.r[reg2];
	uint32 reg3_data = cpu->reg.r[reg3];

	/*
	 * Load-memory (adr, Half-word)
	 */
	err = bus_get_data16(cpu->core_id, reg1_addr, (uint16*)&token);
	if (err != STD_E_OK) {
		printf("ERROR:CAXI pc=0x%x reg1=%u reg1_addr=%d\n", cpu->reg.pc, reg1, reg1_addr);
		return -1;
	}

	result = reg2_data - token;
	if (result == 0) {
		put_data = (uint16)reg3_data;
	}
	else {
		put_data = (uint16)token;
	}
	err = bus_put_data16(cpu->core_id, reg1_addr, put_data);
	if (err != STD_E_OK) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: CAXI r%d(%d),r%d(0x%x), r%d(0x%x):token=0x%x store=0x%x\n",
			cpu->reg.pc, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], reg3, cpu->reg.r[reg3], token, put_data));

	cpu->reg.r[reg3] = (sint32)((uint32)((uint16)token));

	op_chk_and_set_borrow(&cpu->reg, reg2_data, token);
	op_chk_and_set_overflow(&cpu->reg, reg2_data, -token);
	op_chk_and_set_zero(&cpu->reg, result);
	op_chk_and_set_sign(&cpu->reg, result);


	cpu->reg.pc += 4;

	return 0;
}

int op_exec_synce_1(TargetCoreType *cpu)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SYNCE\n", cpu->reg.pc));
	cpu->reg.pc += 2;
	return 0;
}
int op_exec_synci_1(TargetCoreType *cpu)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SYNCI\n", cpu->reg.pc));
	cpu->reg.pc += 2;
	return 0;
}
int op_exec_syncm_1(TargetCoreType *cpu)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SYNCM\n", cpu->reg.pc));
	cpu->reg.pc += 2;
	return 0;
}
int op_exec_syncp_1(TargetCoreType *cpu)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SYNCP\n", cpu->reg.pc));
	cpu->reg.pc += 2;
	return 0;
}
