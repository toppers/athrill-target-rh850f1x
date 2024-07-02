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
#include "bus.h"
#include <stdio.h> //TODO

/*
 * [符号なし加減算]
 * 加算の場合 A+B=D
 *  ⇒D<AまたはD<Bのときキャリー発生
 * 減算の場合 A-B=D
 *  ⇒A<Bの場合ボロー発生
 *
 *  オーバーフローフラグ(OF) ： 同符号どうしの加算結果が異符合
 *  CF = ( A & B ) | ( A & CY) | ( B & CY )
 *　　where A=オペランド1のMSB, B=オペランド2のMSB, CY=MSBへの桁上がり
 *
 *　　オーバーフローフラグの意味は、元の演算数と被演算数及び演算結果を符号付き２進数と考
 *  えた時、結果がｎビットの符号付き２進数で表現できる範囲を超えているという意味である。
 *
 *  キャリーフラグ(CF) ： MSBからの桁上がり
 *  OF = ( A & B & (~C) ) | ( (~A) & (~B) & C )
 *  where A=オペランド1のMSB, B=オペランド2のMSB, C=加算結果のMSB
 *
 *  元の演算数と被演算数を符号無し２進数と考え、結果も符合無し２進数として捉える場合、もしキ
　*  ャリーフラグが１になっていたら、それは「桁溢れ」または「上の桁からの借り」が発生し、結果のｎビッ
　*  トだけで数学的な結果が表せていないことを意味する。
 */

/*
 *  元の演算数と被演算数を符号無し２進数と考え、結果も符合無し２進数として捉える場合、もしキ
　*  ャリーフラグが１になっていたら、それは「桁溢れ」または「上の桁からの借り」が発生し、結果のｎビッ
　*  トだけで数学的な結果が表せていないことを意味する。
 */
int op_chk_and_set_carry(CpuRegisterType *cpu, uint32 a32, uint32 b32)
{
	uint64 tmp1 = a32;
	uint64 tmp2 = b32;
	uint64 result = tmp1 + tmp2;

	if (result > CPU_REG_UINT_MAX) {
		//printf("SET_CY:a32=%llx b32=%llx result=%llx\n", tmp1, tmp2, result);
		CPU_SET_CY(cpu);
	}
	else {
		//printf("CLR_CY:a32=%llx b32=%llx result=%llx\n", tmp1, tmp2, result);
		CPU_CLR_CY(cpu);
	}
	return 0;
}
int op_chk_and_set_carry3(CpuRegisterType *cpu, uint32 a32, uint32 b32, uint32 c32)
{
	uint64 tmp1 = a32;
	uint64 tmp2 = b32;
	uint64 tmp3 = c32;
	uint64 result = tmp1 + tmp2 + tmp3;

	if (result > CPU_REG_UINT_MAX) {
		//printf("SET_CY:a32=%llx b32=%llx result=%llx\n", tmp1, tmp2, result);
		CPU_SET_CY(cpu);
	}
	else {
		//printf("CLR_CY:a32=%llx b32=%llx result=%llx\n", tmp1, tmp2, result);
		CPU_CLR_CY(cpu);
	}
	return 0;
}

int op_chk_and_set_borrow(CpuRegisterType *cpu, uint32 a32, uint32 b32)
{
	uint64 tmp1 = a32;
	uint64 tmp2 = b32;
	uint64 result = tmp1 - tmp2;

	if (result & 0x100000000ULL) {
		//printf("SET_CY:a32=%llx b32=%llx result=%llx\n", tmp1, tmp2, result);
		CPU_SET_CY(cpu);
	}
	else {
		//printf("CLR_CY:a32=%llx b32=%llx result=%llx\n", tmp1, tmp2, result);
		CPU_CLR_CY(cpu);
	}
	return 0;
}

int op_chk_and_set_borrow3(CpuRegisterType *cpu, uint32 a32, uint32 b32, uint32 c32)
{
	uint64 tmp1 = a32;
	uint64 tmp2 = b32;
	uint64 tmp3 = c32;
	uint64 result = tmp1 - tmp2 - tmp3;

	if (result & 0x100000000ULL) {
		//printf("SET_CY:a32=%llx b32=%llx result=%llx\n", tmp1, tmp2, result);
		CPU_SET_CY(cpu);
	}
	else {
		//printf("CLR_CY:a32=%llx b32=%llx result=%llx\n", tmp1, tmp2, result);
		CPU_CLR_CY(cpu);
	}
	return 0;
}

/*
 *　　オーバーフローフラグの意味は、元の演算数と被演算数及び演算結果を符号付き２進数と考
 *  えた時、結果がｎビットの符号付き２進数で表現できる範囲を超えているという意味である。
 */
int op_chk_and_set_overflow(CpuRegisterType *cpu, sint64 a64, sint64 b64)
{
	sint64 result = a64 + b64;

	if (result > (sint64)CPU_REG_PLUS_MAX) {
		CPU_SET_OV(cpu);
	}
	else if (result < (sint64)CPU_REG_MINUS_MAX) {
		CPU_SET_OV(cpu);
	}
	else {
		CPU_CLR_OV(cpu);
	}
	return 0;
}
int op_chk_and_set_overflow3(CpuRegisterType *cpu, sint64 a64, sint64 b64, sint64 c64)
{
	sint64 result = a64 + b64 + c64;

	if (result > (sint64)CPU_REG_PLUS_MAX) {
		CPU_SET_OV(cpu);
	}
	else if (result < (sint64)CPU_REG_MINUS_MAX) {
		CPU_SET_OV(cpu);
	}
	else {
		CPU_CLR_OV(cpu);
	}
	return 0;
}

int op_chk_and_set_zero(CpuRegisterType *cpu, sint32 result)
{
	if (result == 0) {
		CPU_SET_Z(cpu);
	}
	else {
		CPU_CLR_Z(cpu);
	}
	return 0;
}
int op_chk_and_set_sign(CpuRegisterType *cpu, sint32 result)
{
	if (result < 0) {
		CPU_SET_S(cpu);
	}
	else {
		CPU_CLR_S(cpu);
	}
	return 0;
}

/*
 * Format1
 */
int op_exec_sub(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type1.reg1;
	uint32 reg2 = cpu->decoded_code->type1.reg2;
	sint32 reg1_data;
	sint32 reg2_data;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->reg.r[reg2];
	reg1_data = cpu->reg.r[reg1];
	result = reg2_data - reg1_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SUB r%d(%d),r%d(%d):%d\n", cpu->reg.pc, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], result));
	cpu->reg.r[reg2] = result;


	op_chk_and_set_borrow(&cpu->reg, reg2_data, reg1_data);
	op_chk_and_set_overflow(&cpu->reg, reg2_data, -((sint64)reg1_data));
	op_chk_and_set_zero(&cpu->reg, result);
	op_chk_and_set_sign(&cpu->reg, result);

	cpu->reg.pc += 2;
	return 0;
}

int op_exec_subr(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type1.reg1;
	uint32 reg2 = cpu->decoded_code->type1.reg2;
	sint32 reg1_data;
	sint32 reg2_data;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->reg.r[reg2];
	reg1_data = cpu->reg.r[reg1];
	result = reg1_data - reg2_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SUBR r%d(%d),r%d(%d):%d\n", cpu->reg.pc, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], result));
	cpu->reg.r[reg2] = result;

	op_chk_and_set_borrow(&cpu->reg, reg1_data, reg2_data);
	op_chk_and_set_overflow(&cpu->reg, reg1_data, -((sint64)reg2_data));
	op_chk_and_set_zero(&cpu->reg, result);
	op_chk_and_set_sign(&cpu->reg, result);

	cpu->reg.pc += 2;
	return 0;
}
int op_exec_cmp_1(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type1.reg1;
	uint32 reg2 = cpu->decoded_code->type1.reg2;
	sint32 reg1_data;
	sint32 reg2_data;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->reg.r[reg2];
	reg1_data = cpu->reg.r[reg1];
	result = reg2_data - reg1_data;

	op_chk_and_set_borrow(&cpu->reg, reg2_data, reg1_data);
	op_chk_and_set_overflow(&cpu->reg, reg2_data, -((sint64)reg1_data));
	op_chk_and_set_zero(&cpu->reg, result);
	op_chk_and_set_sign(&cpu->reg, result);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: CMP r%d(%d),r%d(%d):%d:psw=0x%x\n", cpu->reg.pc, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], result, sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW]));

	cpu->reg.pc += 2;
	return 0;
}

int op_exec_add_1(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type1.reg1;
	uint32 reg2 = cpu->decoded_code->type1.reg2;
	sint32 reg1_data;
	sint32 reg2_data;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->reg.r[reg2];
	reg1_data = cpu->reg.r[reg1];
	result = reg2_data + reg1_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ADD r%d(%d),r%d(%d):%d\n", cpu->reg.pc, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;

	op_chk_and_set_carry(&cpu->reg, reg2_data, reg1_data);
	op_chk_and_set_overflow(&cpu->reg, reg2_data, reg1_data);
	op_chk_and_set_zero(&cpu->reg, result);
	op_chk_and_set_sign(&cpu->reg, result);

	cpu->reg.pc += 2;
	return 0;
}
int op_exec_mov_1(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type1.reg1;
	uint32 reg2 = cpu->decoded_code->type1.reg2;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MOV r%d(%d),r%d(%d)\n", cpu->reg.pc, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2]));

	cpu->reg.r[reg2] = cpu->reg.r[reg1];

	cpu->reg.pc += 2;
	return 0;
}
int op_exec_mulh_1(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type1.reg1;
	uint32 reg2 = cpu->decoded_code->type1.reg2;
	sint16 tmp1;
	sint16 tmp2;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	tmp1 = (sint16)cpu->reg.r[reg1];
	tmp2 = (sint16)cpu->reg.r[reg2];

	result = (sint32)tmp1 * (sint32)tmp2;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MULH r%d(%d),r%d(%d):%d\n", cpu->reg.pc, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 2;
	return 0;
}

int op_exec_divh_1(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type1.reg1;
	uint32 reg2 = cpu->decoded_code->type1.reg2;
	sint32 reg1_data = cpu->reg.r[reg1];
	sint32 reg2_data = cpu->reg.r[reg2];

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	/*
	 * GR [reg2] ← GR [reg2] ÷ GR [reg1]
	 */
	if (reg1_data == 0U) {
		CPU_SET_OV(&cpu->reg);
	}
	else {
		CPU_CLR_OV(&cpu->reg);
		cpu->reg.r[reg2] = ( (sint32)reg2_data / ((sint32)((sint16)reg1_data)) );
	}

	/*
	 * Z 演算結果が0のとき1，そうでないとき0
	 */
	op_chk_and_set_zero(&cpu->reg, (sint32)cpu->reg.r[reg2]);
	/*
	 * S 演算結果が負のとき1，そうでないとき0
	 */
	op_chk_and_set_sign(&cpu->reg, (sint32)cpu->reg.r[reg2]);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: DIVH r%d(%d) r%d(%d):r%d(0x%x))\n",
			cpu->reg.pc,
			reg1, reg1_data,
			reg2, reg2_data,
			reg2, cpu->reg.r[reg2]));

	cpu->reg.pc += 2;
	return 0;
}

/*
 * Format2
 */
int op_exec_cmp_2(TargetCoreType *cpu)
{
	uint32 reg2 = cpu->decoded_code->type2.reg2;
	sint32 imm_data;
	sint32 reg2_data;
	sint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->reg.r[reg2];
	imm_data = OP_FORMAT2_IMM_SIGN_EXTEND(cpu->decoded_code->type2.imm);
	result = reg2_data - imm_data;

	op_chk_and_set_borrow(&cpu->reg, reg2_data, imm_data);
	op_chk_and_set_overflow(&cpu->reg, reg2_data, -((sint64)imm_data));
	op_chk_and_set_zero(&cpu->reg, result);
	op_chk_and_set_sign(&cpu->reg, result);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: CMP imm5(%d),r%d(%d):%d:psw=0x%x\n", cpu->reg.pc, imm_data, reg2, cpu->reg.r[reg2], result, sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW]));

	cpu->reg.pc += 2;
	return 0;
}
int op_exec_add_2(TargetCoreType *cpu)
{
	uint32 reg2 = cpu->decoded_code->type2.reg2;
	sint32 imm_data;
	sint32 reg2_data;
	sint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	reg2_data = cpu->reg.r[reg2];
	imm_data = OP_FORMAT2_IMM_SIGN_EXTEND(cpu->decoded_code->type2.imm);
	result = reg2_data + imm_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ADD imm5(%d),r%d(%d):%d\n", cpu->reg.pc, imm_data, reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;
	op_chk_and_set_carry(&cpu->reg, reg2_data, imm_data);
	op_chk_and_set_overflow(&cpu->reg, reg2_data, imm_data);
	op_chk_and_set_zero(&cpu->reg, result);
	op_chk_and_set_sign(&cpu->reg, result);

	cpu->reg.pc += 2;

	return 0;
}
int op_exec_mov_2(TargetCoreType *cpu)
{
	uint32 reg2 = cpu->decoded_code->type2.reg2;
	sint32 imm_data;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	imm_data = OP_FORMAT2_IMM_SIGN_EXTEND(cpu->decoded_code->type2.imm);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MOV imm5(%d),r%d(%d)\n", cpu->reg.pc, imm_data, reg2, cpu->reg.r[reg2]));
	cpu->reg.r[reg2] = imm_data;

	cpu->reg.pc += 2;
	return 0;
}
int op_exec_mulh_2(TargetCoreType *cpu)
{
	uint32 reg2 = cpu->decoded_code->type2.reg2;
	sint32 imm_data;
	sint16 tmp2;
	sint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	imm_data = OP_FORMAT2_IMM_SIGN_EXTEND(cpu->decoded_code->type2.imm);
	tmp2 = (sint16)cpu->reg.r[reg2];
	result = imm_data * ((sint32)tmp2);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MULH imm5(%d),r%d(%d):%d\n", cpu->reg.pc, imm_data, reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 2;
	return 0;
}

/*
 * Format6
 */
int op_exec_addi(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type6.reg1;
	uint32 reg2 = cpu->decoded_code->type6.reg2;
	sint32 imm_data = op_sign_extend(15, cpu->decoded_code->type6.imm);
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_addi(&cpu->reg, imm_data, cpu->reg.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ADDI imm5(%d),r%d(%d) r%d(%d):%d\n", cpu->reg.pc, imm_data, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 4;
	return 0;
}
int op_exec_movea(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type6.reg1;
	uint32 reg2 = cpu->decoded_code->type6.reg2;
	sint32 imm_data = op_sign_extend(15, cpu->decoded_code->type6.imm);
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_movea(&cpu->reg, imm_data, cpu->reg.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MOVEA imm16(%d),r%d(%d) r%d(%d):%d\n", cpu->reg.pc, imm_data, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], result));
	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 4;
	return 0;
}
int op_exec_mov_6(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type6.reg1;
	uint32 imm_high_addr;
	uint16 imm_high16;
	uint32 imm_high;
	uint32 imm_data = cpu->decoded_code->type6.imm;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	imm_high_addr = cpu->reg.pc + 4U;

	err = bus_get_data16(cpu->core_id, imm_high_addr, &imm_high16);
	if (err != STD_E_OK) {
		printf("ERROR:MOV pc=0x%x reg1=%u(0x%x) addr=0x%x\n", cpu->reg.pc, reg1, cpu->reg.r[reg1], imm_high_addr);
		return -1;
	}

	imm_high = (uint32)(imm_high16);

	imm_data |= (imm_high << 16U);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MOV imm32(%d),r%d(%d):%d\n", cpu->reg.pc, imm_data, reg1, cpu->reg.r[reg1], imm_data));
	cpu->reg.r[reg1] = imm_data;

	cpu->reg.pc += 6;
	return 0;
}


int op_exec_movhi(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type6.reg1;
	uint32 reg2 = cpu->decoded_code->type6.reg2;
	sint32 imm_data = op_sign_extend(15, cpu->decoded_code->type6.imm);
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_movhi(&cpu->reg, imm_data, cpu->reg.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MOVHI imm16(%d),r%d(%d) r%d(%d):%d\n", cpu->reg.pc, imm_data, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 4;
	return 0;
}

int op_exec_mulhi(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type6.reg1;
	uint32 reg2 = cpu->decoded_code->type6.reg2;
	sint32 imm_data = op_sign_extend(15, cpu->decoded_code->type6.imm);
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_mulhi(&cpu->reg, imm_data, (sint32)((sint16)cpu->reg.r[reg1]));
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MULHI imm16(%d),r%d(%d) r%d(%d):%d\n", cpu->reg.pc, imm_data, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 4;
	return 0;
}

/*
 * Format9
 */
static int setf_chk_cond(TargetCoreType *cpu, uint16 cond)
{
	int ret = FALSE;
	uint16 flg_s = CPU_ISSET_S(&cpu->reg);
	uint16 flg_ov = CPU_ISSET_OV(&cpu->reg);
	uint16 flg_z = CPU_ISSET_Z(&cpu->reg);
	uint16 flg_cy = CPU_ISSET_CY(&cpu->reg);
	uint16 flg_sat = CPU_ISSET_SAT(&cpu->reg);

	switch (cond) {
	case 0b0000: //V
		if (flg_ov == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b0001: //C/L
		if (flg_cy == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b0010: //Z
		if (flg_z == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b0011: //NH
		if ((flg_cy | flg_z) == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b0100: //S/N
		if ((flg_s) == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b0101: //T
		ret = TRUE;
		break;
	case 0b0110: //LT
		if ((flg_s ^ flg_ov) == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b0111: //LE
		if (((flg_s ^ flg_ov) | flg_z) == TRUE) {
			ret = TRUE;
		}
		break;
	case 0b1000: //NV
		if (flg_ov == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1001: //NC/NL
		if (flg_cy == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1010: //NZ
		if (flg_z == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1011: //H
		if ((flg_cy | flg_z) == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1100: //NS/P
		if ((flg_s) == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1101: //SA
		if ((flg_sat) == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1110: //GE
		if ((flg_s ^ flg_ov) == FALSE) {
			ret = TRUE;
		}
		break;
	case 0b1111: //GT
		if (((flg_s ^ flg_ov) | flg_z) == FALSE) {
			ret = TRUE;
		}
		break;
	default:
		break;
	}

	return ret;
}
int op_exec_setf(TargetCoreType *cpu)
{
	uint32 reg2 = cpu->decoded_code->type9.reg2;
	uint16 cond = cpu->decoded_code->type9.gen;
	sint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	if (setf_chk_cond(cpu, cond) == TRUE) {
		result = 1;
	}
	else {
		result = 0;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SETF cond(0x%x),r%d(%d):%d\n", cpu->reg.pc, cond, reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 4;

	return 0;
}
int op_exec_sasf_9(TargetCoreType *cpu)
{
	uint32 reg2 = cpu->decoded_code->type9.reg2;
	uint16 cond = cpu->decoded_code->type9.gen;
	sint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	result = cpu->reg.r[reg2] << 1U;
	if (op_exec_cond(cpu, cond) == TRUE) {
		result |= 1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SASF cond(0x%x),r%d(%d):%d\n",
			cpu->reg.pc, cond, reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 4;

	return 0;
}
/*
 * Format 11
 */
int op_exec_divu(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	uint32 reg1_data = cpu->reg.r[reg1];
	uint32 reg2_data = cpu->reg.r[reg2];
	uint32 reg3_data = cpu->reg.r[reg3];

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	/*
	 * GR [reg2] ← GR [reg2] ÷ GR [reg1]
	 */
	if (reg1_data == 0U) {
		CPU_SET_OV(&cpu->reg);
	}
	else {
		CPU_CLR_OV(&cpu->reg);
		cpu->reg.r[reg2] = reg2_data / reg1_data;
		/*
		 * GR [reg3] ← GR [reg2] % GR [reg1]
		 */
		cpu->reg.r[reg3] = reg2_data % reg1_data;
	}

	/*
	 * Z 演算結果が0のとき1，そうでないとき0
	 */
	op_chk_and_set_zero(&cpu->reg, (sint32)cpu->reg.r[reg2]);
	/*
	 * S 演算結果が負のとき1，そうでないとき0
	 */
	op_chk_and_set_sign(&cpu->reg, (sint32)cpu->reg.r[reg2]);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: DIVU r%d(%d) r%d(%d) r%d(%d):r%d(0x%x), r%d(0x%x)\n",
			cpu->reg.pc,
			reg1, reg1_data,
			reg2, reg2_data,
			reg3, reg3_data,
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3]));

	cpu->reg.pc += 4;
	return 0;
}
int op_exec_divhu(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	uint32 reg1_data = cpu->reg.r[reg1];
	uint32 reg2_data = cpu->reg.r[reg2];
	uint32 reg3_data = cpu->reg.r[reg3];

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	/*
	 * GR [reg2] ← GR [reg2] ÷ GR [reg1]
	 */
	if (reg1_data == 0U) {
		CPU_SET_OV(&cpu->reg);
	}
	else {
		CPU_CLR_OV(&cpu->reg);
		cpu->reg.r[reg2] = ( (uint32)reg2_data / ((uint32)((uint16)reg1_data)) );
		/*
		 * GR [reg3] ← GR [reg2] % GR [reg1]
		 */
		cpu->reg.r[reg3] = ( (uint32)reg2_data % ((uint32)((uint16)reg1_data)) );
	}

	/*
	 * Z 演算結果が0のとき1，そうでないとき0
	 */
	op_chk_and_set_zero(&cpu->reg, (sint32)cpu->reg.r[reg2]);
	/*
	 * S 演算結果が負のとき1，そうでないとき0
	 */
	op_chk_and_set_sign(&cpu->reg, (sint32)cpu->reg.r[reg2]);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: DIVHU r%d(%d) r%d(%d) r%d(%d):r%d(0x%x), r%d(0x%x)\n",
			cpu->reg.pc,
			reg1, reg1_data,
			reg2, reg2_data,
			reg3, reg3_data,
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3]));

	cpu->reg.pc += 4;
	return 0;
}
int op_exec_divqu_11(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	uint32 reg1_data = cpu->reg.r[reg1];
	uint32 reg2_data = cpu->reg.r[reg2];
	uint32 reg3_data = cpu->reg.r[reg3];

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	/*
	 * GR [reg2] ← GR [reg2] ÷ GR [reg1]
	 */
	if (reg1_data == 0U) {
		CPU_SET_OV(&cpu->reg);
	}
	else {
		CPU_CLR_OV(&cpu->reg);
		cpu->reg.r[reg2] = ( (uint32)reg2_data / ((uint32)(reg1_data)) );
		/*
		 * GR [reg3] ← GR [reg2] % GR [reg1]
		 */
		cpu->reg.r[reg3] = ( (uint32)reg2_data % ((uint32)(reg1_data)) );
	}

	/*
	 * Z 演算結果が0のとき1，そうでないとき0
	 */
	op_chk_and_set_zero(&cpu->reg, (sint32)cpu->reg.r[reg2]);
	/*
	 * S 演算結果が負のとき1，そうでないとき0
	 */
	op_chk_and_set_sign(&cpu->reg, (sint32)cpu->reg.r[reg2]);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: DIVQU r%d(%d) r%d(%d) r%d(%d):r%d(0x%x), r%d(0x%x)\n",
			cpu->reg.pc,
			reg1, reg1_data,
			reg2, reg2_data,
			reg3, reg3_data,
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3]));

	cpu->reg.pc += 4;
	return 0;
}

int op_exec_divh_11(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	sint32 reg1_data = cpu->reg.r[reg1];
	sint32 reg2_data = cpu->reg.r[reg2];
	sint32 reg3_data = cpu->reg.r[reg3];

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	/*
	 * GR [reg2] ← GR [reg2] ÷ GR [reg1]
	 */
	if (reg1_data == 0U) {
		CPU_SET_OV(&cpu->reg);
	}
	else {
		CPU_CLR_OV(&cpu->reg);
		cpu->reg.r[reg2] = ( (sint32)reg2_data / ((sint32)((sint16)reg1_data)) );
		/*
		 * GR [reg3] ← GR [reg2] % GR [reg1]
		 */
		cpu->reg.r[reg3] = ( (sint32)reg2_data % ((sint32)((sint16)reg1_data)) );
	}

	/*
	 * Z 演算結果が0のとき1，そうでないとき0
	 */
	op_chk_and_set_zero(&cpu->reg, (sint32)cpu->reg.r[reg2]);
	/*
	 * S 演算結果が負のとき1，そうでないとき0
	 */
	op_chk_and_set_sign(&cpu->reg, (sint32)cpu->reg.r[reg2]);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: DIVH r%d(%d) r%d(%d) r%d(%d):r%d(0x%x), r%d(0x%x)\n",
			cpu->reg.pc,
			reg1, reg1_data,
			reg2, reg2_data,
			reg3, reg3_data,
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3]));

	cpu->reg.pc += 4;
	return 0;
}
static int op_exec_div_common(TargetCoreType *cpu, bool isq)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	sint32 reg1_data = cpu->reg.r[reg1];
	sint32 reg2_data = cpu->reg.r[reg2];
	sint32 reg3_data = cpu->reg.r[reg3];
	char *opname;

	if (isq == TRUE) {
		opname = "DIVQ";
	}
	else {
		opname = "DIV";
	}

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	/*
	 * GR [reg2] ← GR [reg2] ÷ GR [reg1]
	 */
	if (reg1_data == 0U) {
		CPU_SET_OV(&cpu->reg);
	}
	else {
		/*
		 * オーバフローは負の最大値（ 80000000H）を−1で割ったとき
		 */
		if ((reg1_data == -1) && (reg2_data == 0x80000000)) {
			CPU_SET_OV(&cpu->reg);
		}
		else {
			CPU_CLR_OV(&cpu->reg);
		}
		cpu->reg.r[reg2] = reg2_data / reg1_data;
		/*
		 * GR [reg3] ← GR [reg2] % GR [reg1]
		 */
		cpu->reg.r[reg3] = reg2_data % reg1_data;
	}

	/*
	 * Z 演算結果が0のとき1，そうでないとき0
	 */
	op_chk_and_set_zero(&cpu->reg, (sint32)cpu->reg.r[reg2]);
	/*
	 * S 演算結果が負のとき1，そうでないとき0
	 */
	op_chk_and_set_sign(&cpu->reg, (sint32)cpu->reg.r[reg2]);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: %s r%d(%d) r%d(%d) r%d(%d):r%d(0x%x), r%d(0x%x)\n",
			cpu->reg.pc,
			opname,
			reg1, reg1_data,
			reg2, reg2_data,
			reg3, reg3_data,
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3]));

	cpu->reg.pc += 4;
	return 0;
}
int op_exec_div(TargetCoreType *cpu)
{
	int ret;
	ret = op_exec_div_common(cpu, FALSE);
	return ret;
}
int op_exec_divq(TargetCoreType *cpu)
{
	int ret;
	ret = op_exec_div_common(cpu, TRUE);
	return ret;
}
int op_exec_mul(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	sint64 reg1_data = cpu->reg.r[reg1];
	sint64 reg2_data = cpu->reg.r[reg2];
	sint64 reg3_data = cpu->reg.r[reg3];
	sint64 result;
	uint64 result_u;

	//printf("0x%x:op_exec_divu:reg2_data(%u)=0x%x reg3_data(%u)=0x%x\n", cpu->cpu.pc, reg2, reg2_data, reg3, reg3_data);
	//fflush(stdout);

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}
	result = reg1_data * reg2_data;
	result_u = result;
	cpu->reg.r[reg2] = (uint32)result_u;
	cpu->reg.r[reg3] = (uint32)(result_u >> 32U);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(),
			"0x%x: MUL r%d(%d) r%d(%d) r%d(%d):r%d(0x%x), r%d(0x%x)\n",
			cpu->reg.pc,
			reg1, (uint32)reg1_data,
			reg2, (uint32)reg2_data,
			reg3, (uint32)reg3_data,
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3]));

	cpu->reg.pc += 4;
	return 0;
}

int op_exec_mulu(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	uint64 reg1_data = (uint32)cpu->reg.r[reg1];
	uint64 reg2_data = (uint32)cpu->reg.r[reg2];
	uint64 reg3_data = (uint32)cpu->reg.r[reg3];
	uint64 result;
	uint64 result_u;

	//printf("0x%x:op_exec_divu:reg2_data(%u)=0x%x reg3_data(%u)=0x%x\n", cpu->cpu.pc, reg2, reg2_data, reg3, reg3_data);
	//fflush(stdout);

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}
	result = reg1_data * reg2_data;
	result_u = result;
	cpu->reg.r[reg2] = (uint32)result_u;
	cpu->reg.r[reg3] = (uint32)(result_u >> 32U);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(),
			"0x%x: MULU r%d(%d) r%d(%d) r%d(%d):r%d(0x%x), r%d(0x%x)\n",
			cpu->reg.pc,
			reg1, (uint32)reg1_data,
			reg2, (uint32)reg2_data,
			reg3, (uint32)reg3_data,
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3]));

	cpu->reg.pc += 4;
	return 0;
}

int op_exec_cmov_11(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	uint32 reg1_data = cpu->reg.r[reg1];
	uint32 reg2_data = cpu->reg.r[reg2];
	uint32 reg3_data = cpu->reg.r[reg3];
	uint16 cond = ( (cpu->decoded_code->type11.rfu << 1U) | (cpu->decoded_code->type11.sub2) );


	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	/*
	 * if conditions are satisfied
	 * then GR [reg3] ← GR [reg1]
	 * else GR [reg3] ← GR [reg2]
	 */
	if (setf_chk_cond(cpu, cond) == TRUE) {
		cpu->reg.r[reg3] = cpu->reg.r[reg1];
	}
	else {
		cpu->reg.r[reg3] = cpu->reg.r[reg2];
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(),
			"0x%x: CMOV cond(0x%x) r%d(%d) r%d(%d) r%d(%d):r%d(0x%x)\n",
			cpu->reg.pc,
			cond,
			reg1, reg1_data,
			reg2, reg2_data,
			reg3, reg3_data,
			reg3, cpu->reg.r[reg3]));

	cpu->reg.pc += 4;
	return 0;
}

/*
 * Format 12
 */
int op_exec_mulu_12(TargetCoreType *cpu)
{
	uint64 imm9;
	uint32 imm9_low;
	uint32 imm9_high;
	uint32 reg2 = cpu->decoded_code->type12.reg2;
	uint32 reg3 = cpu->decoded_code->type12.reg3;
	uint64 reg2_data;
	uint64 result;
	uint32 result_low;
	uint32 result_high;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}
	imm9_low = cpu->decoded_code->type12.imml;
	imm9_high = (cpu->decoded_code->type12.immh & 0xF);
	imm9 = (imm9_high << 5U) | imm9_low;

	reg2_data = (uint32)cpu->reg.r[reg2];

	result = reg2_data * imm9;

	result_low = (uint32)result;
	result_high = (uint32)(result >> 32U);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MULU imm9(0x%x),r%d(%d) r%d(%d):0x%x, 0x%x\n", cpu->reg.pc, (uint32)imm9,
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3], result_high, result_low));

	cpu->reg.r[reg2] = result_low;
	cpu->reg.r[reg3] = result_high;

	cpu->reg.pc += 4;
	return 0;
}
int op_exec_mul_12(TargetCoreType *cpu)
{
	sint64 imm9;
	uint32 imm9_32bit;
	uint32 imm9_low;
	uint32 imm9_high;
	sint32 reg2 = cpu->decoded_code->type12.reg2;
	sint32 reg3 = cpu->decoded_code->type12.reg3;
	sint64 reg2_data;
	sint64 result;
	sint32 result_low;
	sint32 result_high;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}
	imm9_low = cpu->decoded_code->type12.imml;
	imm9_high = (cpu->decoded_code->type12.immh & 0xF);
	imm9_32bit = (imm9_high << 5U) | imm9_low;
	imm9 = (sint64)op_sign_extend(8, imm9_32bit);

	reg2_data = cpu->reg.r[reg2];

	result = reg2_data * imm9;

	result_low = (sint32)result;
	result_high = (sint32)(result >> 32U);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MUL imm9(0x%x),r%d(%d) r%d(%d):0x%x, 0x%x\n", cpu->reg.pc, (sint32)imm9,
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3], result_high, result_low));

	cpu->reg.r[reg2] = result_low;
	cpu->reg.r[reg3] = result_high;

	cpu->reg.pc += 4;
	return 0;
}

int op_exec_cmov_12(TargetCoreType *cpu)
{
	sint32 imm_data = op_sign_extend(4, cpu->decoded_code->type12.imml);
	uint32 reg2 = cpu->decoded_code->type12.reg2;
	uint32 reg3 = cpu->decoded_code->type12.reg3;
	uint32 reg2_data = cpu->reg.r[reg2];
	uint32 reg3_data = cpu->reg.r[reg3];
	uint16 cond = ( ((cpu->decoded_code->type12.immh &  0x07) << 1U) | (cpu->decoded_code->type12.sub2) );


	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	/*
	 * if conditions are satisfied
	 * then GR [reg3] ← sign-extended (imm5)
	 * else GR [reg3] ← GR [reg2]
	 */
	if (setf_chk_cond(cpu, cond) == TRUE) {
		cpu->reg.r[reg3] = imm_data;
	}
	else {
		cpu->reg.r[reg3] = cpu->reg.r[reg2];
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(),
			"0x%x: CMOV cond(0x%x) imm5(%d) r%d(%d) r%d(%d):r%d(0x%x)\n",
			cpu->reg.pc,
			cond,
			imm_data,
			reg2, reg2_data,
			reg3, reg3_data,
			reg3, cpu->reg.r[reg3]));

	cpu->reg.pc += 4;
	return 0;
}


/*
 * Format11
 */
int op_exec_adf_11(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	sint32 reg1_data;
	sint32 reg2_data;
	sint32 add_data = 0;
	sint32 result;
	uint16 cond =
			  ((cpu->decoded_code->type11.rfu) << 1U)
			| (cpu->decoded_code->type11.sub2);

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	reg1_data = cpu->reg.r[reg1];
	reg2_data = cpu->reg.r[reg2];
	if (op_exec_cond(cpu, cond) == TRUE) {
		add_data = 1;
	}
	else {
		add_data = 0;
	}
	result = reg2_data + reg1_data + add_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ADF cond=0x%x, r%d(%d),r%d(%d),r%d(%d):%d\n",
			cpu->reg.pc,
			cond,
			reg1, cpu->reg.r[reg1],
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3],
			result));

	cpu->reg.r[reg3] = result;

	op_chk_and_set_carry3(&cpu->reg, reg2_data, reg1_data, add_data);
	op_chk_and_set_overflow3(&cpu->reg, reg2_data, reg1_data, add_data);
	op_chk_and_set_zero(&cpu->reg, result);
	op_chk_and_set_sign(&cpu->reg, result);

	cpu->reg.pc += 4;
	return 0;
}
int op_exec_sbf_11(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	sint32 reg1_data;
	sint32 reg2_data;
	sint32 add_data = 0;
	sint32 result;
	uint16 cond =
			  ((cpu->decoded_code->type11.rfu) << 1U)
			| (cpu->decoded_code->type11.sub2);

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}
	if (op_exec_cond(cpu, cond) == TRUE) {
		add_data = 1;
	}
	else {
		add_data = 0;
	}

	reg2_data = cpu->reg.r[reg2];
	reg1_data = cpu->reg.r[reg1];
	result = reg2_data - reg1_data - add_data;
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SBF cond=0x%x, r%d(%d),r%d(%d),r%d(%d):%d\n",
			cpu->reg.pc,
			cond,
			reg1, cpu->reg.r[reg1],
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3],
			result));

	cpu->reg.r[reg3] = result;


	op_chk_and_set_borrow3(&cpu->reg, reg2_data, reg1_data, add_data);
	op_chk_and_set_overflow3(&cpu->reg, reg2_data, -((sint64)reg1_data), -((sint64)add_data));
	op_chk_and_set_zero(&cpu->reg, result);
	op_chk_and_set_sign(&cpu->reg, result);

	cpu->reg.pc += 4;
	return 0;
}

int op_exec_mac_11(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	uint32 reg4 = ((cpu->decoded_code->type11.rfu << 1U) | cpu->decoded_code->type11.sub2) << 1U;
	uint32 reg3plus1 = reg3 + 1;
	uint32 reg4plus1 = reg4 + 1;
	sint64 reg1_data = cpu->reg.r[reg1];
	sint64 reg2_data = cpu->reg.r[reg2];
	sint64 reg3plus1_data = cpu->reg.r[reg3plus1];
	sint64 reg3_data  = cpu->reg.r[reg3];
	sint64 result1;
	sint64 result2;
	sint64 result;
	sint32 *result_data;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg4 >= CPU_GREG_NUM) {
		return -1;
	}
	if ((reg3 % 2) != 0) {
		return -1;
	}
	if ((reg4 % 2) != 0) {
		return -1;
	}
	result1 = reg2_data * reg1_data;
	result2 = (reg3plus1_data << 32U) | reg3_data;

	result = result1 + result2;
	result_data = (sint32*)&result;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MAC r%d(%d),r%d(%d),r%d(%d),r%d(%d):%d, %d\n",
			cpu->reg.pc,
			reg1, cpu->reg.r[reg1],
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3],
			reg4, cpu->reg.r[reg4],
			result_data[0], result_data[1]));

	cpu->reg.r[reg4] = result_data[0];
	cpu->reg.r[reg4plus1] = result_data[1];

	cpu->reg.pc += 4;
	return 0;
}

int op_exec_macu_11(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	uint32 reg4 = ((cpu->decoded_code->type11.rfu << 1U) | cpu->decoded_code->type11.sub2) << 1U;
	uint32 reg3plus1 = reg3 + 1;
	uint32 reg4plus1 = reg4 + 1;
	uint64 reg1_data = (uint32)cpu->reg.r[reg1];
	uint64 reg2_data = (uint32)cpu->reg.r[reg2];
	uint64 reg3plus1_data = (uint32)cpu->reg.r[reg3plus1];
	uint64 reg3_data  = (uint32)cpu->reg.r[reg3];
	uint64 result1;
	uint64 result2;
	uint64 result;
	uint32 *result_data;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg4 >= CPU_GREG_NUM) {
		return -1;
	}
	if ((reg3 % 2) != 0) {
		return -1;
	}
	if ((reg4 % 2) != 0) {
		return -1;
	}
	result1 = reg2_data * reg1_data;
	result2 = (reg3plus1_data << 32U) | reg3_data;

	result = result1 + result2;
	result_data = (uint32*)&result;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: MACU r%d(%u),r%d(%u),r%d(%u),r%d(%u):%u, %u\n",
			cpu->reg.pc,
			reg1, cpu->reg.r[reg1],
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3],
			reg4, cpu->reg.r[reg4],
			result_data[0], result_data[1]));

	cpu->reg.r[reg4] = result_data[0];
	cpu->reg.r[reg4plus1] = result_data[1];

	cpu->reg.pc += 4;
	return 0;
}
