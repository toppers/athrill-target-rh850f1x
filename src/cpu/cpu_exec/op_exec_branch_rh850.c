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

extern void op_exec_bcond(TargetCoreType *cpu, uint16 cond, sint32 disp, sint32 code_size);

/*
 * Format7
 */
int op_exec_bcond_7(TargetCoreType *cpu)
{
	uint16 cond = (cpu->decoded_code->type7.reg1 & 0x0F);
	uint32 disp_u;
	uint32 disp_D;
	sint32 disp;
	disp_u = (cpu->decoded_code->type7.disp);
	disp_D = (cpu->decoded_code->type7.reg1 & 0x10) >> 4U;
	disp_u = ((disp_D << 15U) | (disp_u)) << 1U;

	disp = op_sign_extend(16, disp_u);

	op_exec_bcond(cpu, cond, disp, 4);
	return 0;
}
/*
 * Format11
 */
int op_exec_jarl_11(TargetCoreType *cpu)
{
	sint32 reg1 = cpu->decoded_code->type11.reg1;
	uint32 reg3 = cpu->decoded_code->type11.reg3;
	sint32 pc = (sint32)cpu->reg.pc;

	pc = cpu->reg.r[reg1];

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: JARL [r%u(0x%x)], r%u(0x%x):0x%x r%u(0x%x)\n",
			cpu->reg.pc,
			reg1, cpu->reg.r[reg1],
			reg3, cpu->reg.r[reg3],
			pc,
			reg3, cpu->reg.pc + 4));

	cpu->reg.r[reg3] = cpu->reg.pc + 4;

	cpu->reg.pc = pc;
	return 0;
}


/*
 * Format7
 */
int op_exec_loop_7(TargetCoreType *cpu)
{
	sint32 reg1 = cpu->decoded_code->type7.reg1;
	uint32 disp = 0;
	uint32 reg1_data = cpu->reg.r[reg1];
	uint32 result;
	uint32 add_data = 0xFFFFFFFF;
	sint32 pc;

	result = reg1_data + add_data;
	if (result == 0) {
		pc = cpu->reg.pc + 4;
		CPU_CLR_CY(&cpu->reg);
	}
	else {
		/*
		 * 15 ビット・イミーディエトを 1 ビット論理左シフトしてワード長までゼロ拡張した結果を現
		 * 在の PC から減算した値を PC に設定し，制御を移します。
		 */
		disp = op_zero_extend(16, (cpu->decoded_code->type7.disp << 1U));
		pc = cpu->reg.pc - ((sint32)disp);

		/*
		 * 汎用レジスタ reg1 の内容に対しては，−1（0xFFFFFFFF）を加算します。キャリー・フラグ
		 * の更新は SUB 命令ではなく，ADD 命令と同一の動作となります
		 */
		op_chk_and_set_carry(&cpu->reg, reg1_data, add_data);
	}
	op_chk_and_set_overflow(&cpu->reg, reg1_data, add_data);
	op_chk_and_set_zero(&cpu->reg, result);
	op_chk_and_set_sign(&cpu->reg, result);

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LOOP r%u(0x%x), disp=%u:0x%x\n",
			cpu->reg.pc,
			reg1, cpu->reg.r[reg1],
			disp,
			result));

	cpu->reg.r[reg1] = result;
	cpu->reg.pc = pc;

	return 0;
}
