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
#ifndef _CPU_OPS_H_
#define _CPU_OPS_H_
#include "target_cpu.h"

#define CPU_PSW_ID		(1 << 5)
#define CPU_PSW_SAT		(1 << 4)
#define CPU_PSW_CY		(1 << 3)
#define CPU_PSW_OV		(1 << 2)
#define CPU_PSW_S		(1 << 1)
#define CPU_PSW_Z		(1 << 0)

#define CPU_PSW_EP		(1 << 6)
#define CPU_PSW_NP		(1 << 7)
#define CPU_PSW_EBV		(1 << 15)
#define CPU_PSW_UM		(1 << 30)

#define CPU_SET_EBV(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] |= CPU_PSW_EBV
#define CPU_CLR_EBV(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] &= ~CPU_PSW_EBV
#define CPU_ISSET_EBV(cpu)	( (sys_get_cpu_base(cpu)->r[SYS_REG_PSW] & CPU_PSW_EBV) == CPU_PSW_EBV )

#define CPU_SET_UM(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] |= CPU_PSW_UM
#define CPU_CLR_UM(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] &= ~CPU_PSW_UM
#define CPU_ISSET_UM(cpu)	( (sys_get_cpu_base(cpu)->r[SYS_REG_PSW] & CPU_PSW_UM) == CPU_PSW_UM )

#define CPU_SET_EP(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] |= CPU_PSW_EP
#define CPU_CLR_EP(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] &= ~CPU_PSW_EP
#define CPU_ISSET_EP(cpu)	( (sys_get_cpu_base(cpu)->r[SYS_REG_PSW] & CPU_PSW_EP) == CPU_PSW_EP )

#define CPU_SET_NP(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] |= CPU_PSW_NP
#define CPU_CLR_NP(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] &= ~CPU_PSW_NP
#define CPU_ISSET_NP(cpu)	( (sys_get_cpu_base(cpu)->r[SYS_REG_PSW] & CPU_PSW_NP) == CPU_PSW_NP )


#define CPU_SET_ID(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] |= CPU_PSW_ID
#define CPU_CLR_ID(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] &= ~CPU_PSW_ID
#define CPU_ISSET_ID(cpu)	( (sys_get_cpu_base(cpu)->r[SYS_REG_PSW] & CPU_PSW_ID) == CPU_PSW_ID )

#define CPU_SET_SAT(cpu)	sys_get_cpu_base(cpu)->r[SYS_REG_PSW] |= CPU_PSW_SAT
#define CPU_CLR_SAT(cpu)	sys_get_cpu_base(cpu)->r[SYS_REG_PSW] &= ~CPU_PSW_SAT
#define CPU_ISSET_SAT(cpu)	( (sys_get_cpu_base(cpu)->r[SYS_REG_PSW] & CPU_PSW_SAT) == CPU_PSW_SAT)

#define CPU_SET_CY(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] |= CPU_PSW_CY
#define CPU_CLR_CY(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] &= ~CPU_PSW_CY
#define CPU_ISSET_CY(cpu)	( (sys_get_cpu_base(cpu)->r[SYS_REG_PSW] & CPU_PSW_CY) == CPU_PSW_CY)

#define CPU_SET_OV(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] |= CPU_PSW_OV
#define CPU_CLR_OV(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] &= ~CPU_PSW_OV
#define CPU_ISSET_OV(cpu)	( (sys_get_cpu_base(cpu)->r[SYS_REG_PSW] & CPU_PSW_OV) == CPU_PSW_OV)

#define CPU_SET_S(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] |= CPU_PSW_S
#define CPU_CLR_S(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] &= ~CPU_PSW_S
#define CPU_ISSET_S(cpu)	( (sys_get_cpu_base(cpu)->r[SYS_REG_PSW] & CPU_PSW_S) == CPU_PSW_S)

#define CPU_SET_Z(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] |= CPU_PSW_Z
#define CPU_CLR_Z(cpu)		sys_get_cpu_base(cpu)->r[SYS_REG_PSW] &= ~CPU_PSW_Z
#define CPU_ISSET_Z(cpu)	( (sys_get_cpu_base(cpu)->r[SYS_REG_PSW] & CPU_PSW_Z) == CPU_PSW_Z)



#define CPU_ISSET_INTCFG_ISPC(cpu) ( ((*cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_2, SYS_REG_INTCFG)) & (1U << 0)) != 0 )

#define CPU_ISSET_RBASE_RINT(cpu) ( ((*cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_1, SYS_REG_RBASE)) & (1U << 0)) != 0 )
#define CPU_ISSET_EBASE_RINT(cpu) ( ((*cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_1, SYS_REG_EBASE)) & (1U << 0)) != 0 )
#define CPU_RBASE_ADDR(cpu)		( (*cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_1, SYS_REG_RBASE)) & ~(1U << 0) )
#define CPU_EBASE_ADDR(cpu)		( (*cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_1, SYS_REG_EBASE)) & ~(1U << 0) )
#define CPU_INTBP_ADDR(cpu)		( (*cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_1, SYS_REG_INTBP)) )

extern int op_chk_and_set_carry(CpuRegisterType *cpu, uint32 a, uint32 b);
extern int op_chk_and_set_borrow(CpuRegisterType *cpu, uint32 a, uint32 b);
extern int op_chk_and_set_overflow(CpuRegisterType *cpu, sint64 a, sint64 b);
extern int op_chk_and_set_zero(CpuRegisterType *cpu, sint32 result);
extern int op_chk_and_set_sign(CpuRegisterType *cpu, sint32 result);
extern bool op_exec_cond(TargetCoreType *cpu, uint16 cond);

extern int op_chk_and_set_carry3(CpuRegisterType *cpu, uint32 a, uint32 b, uint32 c);
extern int op_chk_and_set_borrow3(CpuRegisterType *cpu, uint32 a, uint32 b, uint32 c);
extern int op_chk_and_set_overflow3(CpuRegisterType *cpu, sint64 a, sint64 b, sint64 c);

static inline sint32 op_addi(
		CpuRegisterType *cpu,
		sint32 imm_data, sint32 reg1_data)
{
	sint32 result;

	result = reg1_data + imm_data;

	op_chk_and_set_carry(cpu, imm_data, reg1_data);
	op_chk_and_set_overflow(cpu, imm_data, reg1_data);
	op_chk_and_set_zero(cpu, result);
	op_chk_and_set_sign(cpu, result);
	return result;
}
static inline sint32 op_andi(
		CpuRegisterType *cpu,
		sint32 imm_data, sint32 reg1_data)
{
	sint32 result = reg1_data & imm_data;

	op_chk_and_set_zero(cpu, result);
	op_chk_and_set_sign(cpu, result);
	CPU_CLR_OV(cpu);
	return result;
}
static inline sint32 op_and(
		CpuRegisterType *cpu,
		sint32 reg2_data, sint32 reg1_data)
{
	sint32 result = reg1_data & reg2_data;

	op_chk_and_set_zero(cpu, result);
	op_chk_and_set_sign(cpu, result);
	CPU_CLR_OV(cpu);
	return result;
}
static inline sint32 op_or(
		CpuRegisterType *cpu,
		sint32 reg2_data, sint32 reg1_data)
{
	sint32 result = reg1_data | reg2_data;

	op_chk_and_set_zero(cpu, result);
	op_chk_and_set_sign(cpu, result);
	CPU_CLR_OV(cpu);
	return result;
}

static inline sint32 op_movea(
		CpuRegisterType *cpu,
		sint32 imm_data, sint32 reg1_data)
{
	sint32 ret = reg1_data + imm_data;
	return ret;
}
static inline sint32 op_movhi(
		CpuRegisterType *cpu,
		sint32 imm_data, sint32 reg1_data)
{
	sint32 ret = reg1_data + (imm_data << 16);
	return ret;
}
static inline sint32 op_mulhi(
		CpuRegisterType *cpu,
		sint32 imm_data, sint32 reg1_data)
{
	sint32 ret = reg1_data * (imm_data);
	return ret;
}
static inline sint32 op_ori(
		CpuRegisterType *cpu,
		uint16 imm_data, sint32 reg1_data)
{
	sint32 ret = ( reg1_data | ((uint32)(imm_data)) );
	if (ret == 0) {
		CPU_SET_Z(cpu);
	}
	else {
		CPU_CLR_Z(cpu);
	}
	if (ret & (0x80000000)) {
		CPU_SET_S(cpu);
	}
	else {
		CPU_CLR_S(cpu);
	}
	return ret;
}

static inline sint32 op_satadd(
		CpuRegisterType *cpu,
		sint32 data_l, sint32 data_r)
{
	sint32 ret;
	sint64 data_r64 = data_r;
	sint64 data_l64 = data_l;
	sint64 ret64;

	ret64 = data_l64 + data_r64;

	op_chk_and_set_carry(cpu, data_l, data_r);
	op_chk_and_set_overflow(cpu, data_l, data_r);

	if (ret64 > (sint64)CPU_REG_PLUS_MAX) {
		CPU_SET_SAT(cpu);
		ret = CPU_REG_PLUS_MAX;
	}
	else if (ret64 < CPU_REG_MINUS_MAX) {
		CPU_SET_SAT(cpu);
		ret = CPU_REG_MINUS_MAX;
	}
	else {
		ret = (sint32)ret64;
	}
	if (ret64 < 0) {
		CPU_SET_S(cpu);
	}
	else {
		CPU_CLR_S(cpu);
	}
	if (ret64 == 0) {
		CPU_SET_Z(cpu);
	}
	else {
		CPU_CLR_Z(cpu);
	}
	return ret;
}
static inline sint32 op_xori(
		CpuRegisterType *cpu,
		uint16 imm_data, sint32 reg1_data)
{
	sint32 ret = ( reg1_data ^ ((uint32)(imm_data)) );
	if (ret == 0) {
		CPU_SET_Z(cpu);
	}
	else {
		CPU_CLR_Z(cpu);
	}
	if (ret < 0) {
		CPU_SET_S(cpu);
	}
	else {
		CPU_CLR_S(cpu);
	}
	CPU_CLR_OV(cpu);
	return ret;
}

/*
 * 指定されたbitに符号ビットがある場合は31bitまで1埋めする(符号拡張)
 */
static inline sint32 op_sign_extend(uint32 bit, uint32 data)
{
	int i;
	if (data & (1 << bit)) {
		for (i = bit; i < 32; i++) {
			data = ( data | (1 << i) );//符号拡張する
		}
	}
	return data;
}
#define OP_FORMAT2_IMM_SIGN_EXTEND(data)	op_sign_extend(4, (data))

/*
 * 指定されたbitから31bitまで０埋めする
 */
static inline uint32 op_zero_extend(uint32 bit, uint32 data)
{
	int i;
	for (i = bit; i < 32; i++) {
		data = ( data & ~(1 << i) );
	}
	return data;
}
#define OP_FORMAT2_IMM_ZERO_EXTEND(data)	op_zero_extend(5, (data))

static inline char *addr2devregname(uint32 addr)
{
	switch (addr) {
	case 0xFFFFF1FE:
		return "PSC";
	case 0xFFFFF308:
		return "SELCNT0";
	case 0xFFFFF30A:
		return "SELCNT1";
	case 0xFFFFF30C:
		return "SELCNT2";
	case 0xFFFFF30E:
		return "SELCNT3";
	case 0xFFFFF3F8:
		return "SELCNT4";
	case 0xFFFFF3FA:
		return "SELCNT5";
	case 0xFFFFF3F0:
		return "SSCGCTL";
	case 0xFFFFF3F1:
		return "SFC0";
	case 0xFFFFF3F2:
		return "SFC1";
	case 0xFFFFF6C0:
		return "OSTS";
	case 0xFFFFF6C1:
		return "PLLS";
	case 0xFFFFF6C2:
		return "OSTC";
	case 0xFFFFF80C:
		return "RCM";
	case 0xFFFFF820:
		return "PSMR";
	case 0xFFFFF824:
		return "LOCKR";
	case 0xFFFFF828:
		return "PCC";
	case 0xFFFFF82C:
		return "PLLCTL";
	case 0xFFFFF82E:
		return "CCLS";
	case 0xFFFFF82F:
		return "PCLM";
	case 0xFFFFF860:
		return "MCM";
	case 0xFFFFF870:
		return "CLM";
	case 0xFFFFF8B0:
		return "PRSM0";
	case 0xFFFFF8B1:
		return "PRSCM0";
	default:
		break;
	}
	return "NULL";
}

#endif /* _CPU_OPS_H_ */
