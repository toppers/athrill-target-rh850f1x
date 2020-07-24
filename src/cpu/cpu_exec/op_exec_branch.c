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

bool op_exec_cond(TargetCoreType *cpu, uint16 cond)
{
	uint16 is_br = FALSE;
	uint16 flg_s = CPU_ISSET_S(&cpu->reg);
	uint16 flg_ov = CPU_ISSET_OV(&cpu->reg);
	uint16 flg_z = CPU_ISSET_Z(&cpu->reg);
	uint16 flg_cy = CPU_ISSET_CY(&cpu->reg);
	uint16 flg_sat = CPU_ISSET_SAT(&cpu->reg);

	switch (cond) {
	case 0b1110:	//BGE
		if ((flg_s ^ flg_ov) == FALSE) {
			is_br = TRUE;
		}
		break;
	case 0b1111:	//BGT
		if (((flg_s ^ flg_ov) | flg_z ) == FALSE) {
			is_br = TRUE;
		}
		break;
	case 0b0111:	//BLE
		if (((flg_s ^ flg_ov) | flg_z ) == TRUE) {
			is_br = TRUE;
		}
		break;
	case 0b0110:	//BLT
		if ((flg_s ^ flg_ov) == TRUE) {
			is_br = TRUE;
		}
		break;

	case 0b1011:	//BH
		if ((flg_cy | flg_z) == FALSE) {
			is_br = TRUE;
		}
		break;
	case 0b0001:	//BL
/*	case 0b0001: */	//BC
		if ((flg_cy) == TRUE) {
			is_br = TRUE;
		}
		break;
	case 0b0011:	//BNH
		if ((flg_cy | flg_z) == TRUE) {
			is_br = TRUE;
		}
		break;
	case 0b1001:	//BNL
/*	case 0b1001: */	//BNC
		if ((flg_cy) == FALSE) {
			is_br = TRUE;
		}
		break;

	case 0b0010:	//BE
/*	case 0b0010: */	//BZ
		if ((flg_z) == TRUE) {
			is_br = TRUE;
		}
		break;
	case 0b1010:	//BNZ
		if ((flg_z) == FALSE) {
			is_br = TRUE;
		}
		break;

	case 0b0100:	//BN
		if ((flg_s) == TRUE) {
			is_br = TRUE;
		}
		break;
	case 0b1000:	//BNV
		if ((flg_ov) == FALSE) {
			is_br = TRUE;
		}
		break;
	case 0b1100:	//BP
		if ((flg_s) == FALSE) {
			is_br = TRUE;
		}
		break;
	case 0b0101:	//BR
		is_br = TRUE;
		break;
	case 0b1101:	//BSA
		if ((flg_sat) == TRUE) {
			is_br = TRUE;
		}
		break;
	case 0b0000:	//BV
		if ((flg_ov) == TRUE) {
			is_br = TRUE;
		}
		break;
	default:
		break;
	}
	return is_br;
}

void op_exec_bcond(TargetCoreType *cpu, uint16 cond, sint32 disp, sint32 code_size)
{
	uint16 is_br = op_exec_cond(cpu, cond);
	if (is_br == TRUE) {
		sint32 pc = cpu->reg.pc;
		pc = pc + disp;
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: B cond(0x%x):0x%x\n", cpu->reg.pc, cond, pc));
		cpu->reg.pc = pc;
	}
	else {
		sint32 pc = cpu->reg.pc + code_size;
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: Bcond(0x%x):0x%x\n", cpu->reg.pc, cond, pc));
		cpu->reg.pc = pc;
	}
}
/*
 * Format1
 */
int op_exec_jmp(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type1.reg1;
	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: JMP r%d(0x%x)\n", cpu->reg.pc, reg1, cpu->reg.r[reg1]));
	cpu->reg.pc = cpu->reg.r[reg1];
	return 0;
}


/*
 * Format3
 */
int op_exec_bcond_3(TargetCoreType *cpu)
{
	uint16 cond = cpu->decoded_code->type3.cond;
	uint32 disp_u;
	sint32 disp;

	disp_u = cpu->decoded_code->type3.disp << 1;
	disp = op_sign_extend(8, disp_u);

	op_exec_bcond(cpu, cond, disp, 2);
	return 0;
}

/*
 * Format5
 */
int op_exec_jr(TargetCoreType *cpu)
{
	uint32 reg2 = cpu->decoded_code->type5.reg2;
	sint32 pc = (sint32)cpu->reg.pc;
	sint32 disp;

	if (reg2 > 0) {
		cpu->reg.r[reg2] = cpu->reg.pc + 4;
	}
	disp = op_sign_extend(21, cpu->decoded_code->type5.disp);
	pc += disp;

	if (reg2 == 0) {
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: JR disp22(%d):0x%x\n", cpu->reg.pc, disp, pc));
	}
	else {
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: JARL disp22(%d):0x%x r%u(0x%x)\n", cpu->reg.pc, disp, pc, reg2, cpu->reg.r[reg2]));
	}

	cpu->reg.pc = pc;
	return 0;
}


/*
 * Format6
 */
int op_exec_jmp_6(TargetCoreType *cpu)
{
	uint32 reg1 = cpu->decoded_code->type6.reg1;
	uint32 disp = cpu->decoded_code->type6.imm;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: JMP disp32(%u) r%d(0x%x)\n", cpu->reg.pc, disp, reg1, cpu->reg.r[reg1] + disp));
	cpu->reg.pc = cpu->reg.r[reg1] + disp;

	return 0;
}

int op_exec_jarl_6(TargetCoreType *cpu)
{
	sint32 reg1 = cpu->decoded_code->type6.reg1;
	uint32 disp = cpu->decoded_code->type6.imm;
	uint32 pc = (sint32)cpu->reg.pc;

	pc += disp;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: JARL disp32(%u) r%u(0x%x):0x%x r%u(0x%x)\n",
			cpu->reg.pc,
			disp,
			reg1, cpu->reg.r[reg1],
			pc,
			reg1, cpu->reg.pc + 6));

	cpu->reg.r[reg1] = cpu->reg.pc + 6;

	cpu->reg.pc = pc;
	return 0;
}

int op_exec_jr_6(TargetCoreType *cpu)
{
	uint32 disp = cpu->decoded_code->type6.imm;
	uint32 pc = (uint32)cpu->reg.pc;

	pc += disp;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: JR disp32(%u):0x%x\n",
			cpu->reg.pc,
			disp,
			pc));

	cpu->reg.pc = pc;
	return 0;
}

