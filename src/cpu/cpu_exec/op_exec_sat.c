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

/*
 * Format1
 */
int op_exec_satadd_1(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type1.reg1;
	uint32 reg2 = cpu->decoded_code->type1.reg2;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_satadd(&cpu->reg, cpu->reg.r[reg2], cpu->reg.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SATADD r%d(%d),r%d(%d):%d\n", cpu->reg.pc, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 2;
	return 0;
}
int op_exec_satsub_1(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type1.reg1;
	uint32 reg2 = cpu->decoded_code->type1.reg2;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_satadd(&cpu->reg, cpu->reg.r[reg2], -((sint32)cpu->reg.r[reg1]));
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SATSUB r%d(%d),r%d(%d):%d\n", cpu->reg.pc, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 2;
	return 0;
}
int op_exec_satsubr_1(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type1.reg1;
	uint32 reg2 = cpu->decoded_code->type1.reg2;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_satadd(&cpu->reg, cpu->reg.r[reg1], -((sint32)cpu->reg.r[reg2]));
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SATSUBR r%d(%d),r%d(%d):%d\n",
			cpu->reg.pc,
			reg1, cpu->reg.r[reg1],
			reg2, cpu->reg.r[reg2],
			result));

	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 2;
	return 0;
}

/*
 * Format2
 */
int op_exec_satadd_2(TargetCoreType *cpu)
{
	sint32 imm_data = OP_FORMAT2_IMM_SIGN_EXTEND(cpu->decoded_code->type2.imm);
	uint32 reg2 = cpu->decoded_code->type2.reg2;
	sint32 result;

	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_satadd(&cpu->reg, cpu->reg.r[reg2], imm_data);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SATADD imm5(%d),r%d(%d):%d\n", cpu->reg.pc, imm_data, reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 2;
	return 0;
}



/*
 * Format6
 */

int op_exec_satsubi(TargetCoreType *cpu)
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

	result = op_satadd(&cpu->reg, cpu->reg.r[reg1], -imm_data);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SATSUBI imm16(%d), r%d(%d), r%d(%d):%d\n", cpu->reg.pc, imm_data, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], result));

	cpu->reg.r[reg2] = result;

	cpu->reg.pc += 4;
	return 0;
}

/*
 * Format11
 */
int op_exec_satadd_11(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_satadd(&cpu->reg, cpu->reg.r[reg2], cpu->reg.r[reg1]);
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SATADD r%d(%d),r%d(%d), r%d(%d):%d\n",
			cpu->reg.pc,
			reg1, cpu->reg.r[reg1],
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3],
			result));

	cpu->reg.r[reg3] = result;

	cpu->reg.pc += 4;
	return 0;
}
int op_exec_satsub_11(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg2 = cpu->decoded_code->type11.reg2;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	sint32 result;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}
	result = op_satadd(&cpu->reg, cpu->reg.r[reg2], -((sint32)cpu->reg.r[reg1]));
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SATSUB r%d(%d),r%d(%d), r%d(%d):%d\n",
			cpu->reg.pc,
			reg1, cpu->reg.r[reg1],
			reg2, cpu->reg.r[reg2],
			reg3, cpu->reg.r[reg3],
			result));

	cpu->reg.r[reg3] = result;

	cpu->reg.pc += 4;
	return 0;
}
