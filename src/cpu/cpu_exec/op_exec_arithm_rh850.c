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
 * Format9
 */
int op_exec_bins_9(TargetCoreType *cpu)
{
	uint32 chk = cpu->decoded_code->type9.sub;
	uint32 reg1 = cpu->decoded_code->type9.gen;
	uint32 reg2 = cpu->decoded_code->type9.reg2;
	uint32 msb = (cpu->decoded_code->type9.rfu2 >> 1U);
	uint32 lsb = ((cpu->decoded_code->type9.rfu2 & 0x01) << 3U) | (cpu->decoded_code->type9.rfu1 & 0x07);
	uint32 i;
	uint32 reg2_data = cpu->reg.r[reg2];


	switch (chk) {
	case 0b100:
		//(msb ≥ 16, lsb ≥ 16)
		msb += 16;
		lsb += 16;
		break;
	case 0b101:
		//(msb ≥ 16, lsb < 16)
		msb += 16;
		break;
	case 0b110:
		//(msb < 16, lsb < 16)
		break;
	default:
		break;
	}

	for (i = lsb; i <= msb; i++) {
		if (cpu->reg.r[reg1] & (1U << (i - lsb))) {
			reg2_data |= (1U << i);
		}
		else {
			reg2_data &= ~(1U << i);
		}
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: BINS r%d(0x%x), msb(%u), lsb(%u), r%d(0x%x):0x%x\n",
			cpu->reg.pc,
			reg1, cpu->reg.r[reg1],
			msb, lsb,
			reg2, cpu->reg.r[reg2],
			reg2_data));

	cpu->reg.r[reg2] = reg2_data;

	CPU_CLR_OV(&cpu->reg);
	op_chk_and_set_sign(&cpu->reg, cpu->reg.r[reg2]);
	op_chk_and_set_zero(&cpu->reg, cpu->reg.r[reg2]);

	cpu->reg.pc += 4;

	return 0;
}


/*
 * Format7
 */
int op_exec_rotl_7(TargetCoreType *cpu)
{
	uint32 reg2 = cpu->decoded_code->type7.reg2;
	uint32 reg3 = ( (cpu->decoded_code->type7.disp >> 10U) & 0x1F );
	uint32 chk = (cpu->decoded_code->type7.disp & 0x03FF);
	uint32 rotate_size = 0;
	uint32 reg2_data;
	uint32 i;

	reg2_data = cpu->reg.r[reg2];
	switch (chk) {
	case 0b0001100010:
		//1）ROTL imm5, reg2, reg3
		rotate_size = op_zero_extend(5, cpu->decoded_code->type7.reg1);
		for (i = 0; i < rotate_size; i++) {
			reg2_data <<= 1U;
			if (cpu->reg.r[reg2] & (1U << (31U - i))) {
				reg2_data |= 0x01;
			}
		}
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ROTL imm5(%u) r%d(0x%x), r%d(0x%x):0x%x\n",
				cpu->reg.pc,
				rotate_size, reg2, reg2_data, reg3, cpu->reg.r[reg3],
				reg2_data));
		break;
	case 0b0001100011:
		//2）ROTL reg1,reg2,reg3
		rotate_size = cpu->reg.r[cpu->decoded_code->type7.reg1];
		for (i = 0; i < rotate_size; i++) {
			reg2_data <<= 1U;
			if (cpu->reg.r[reg2] & (1U << (31U - i))) {
				reg2_data |= 0x01;
			}
		}
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: ROTL r%d(%u), r%d(0x%x), r%d(0x%x):0x%x\n",
				cpu->reg.pc,
				cpu->decoded_code->type7.reg1, rotate_size, reg2, reg2_data, reg3, cpu->reg.r[reg3],
				reg2_data));
		break;
	default:
		break;
	}

	cpu->reg.r[reg3] = reg2_data;

	if ((rotate_size > 0) && ((cpu->reg.r[reg3] & 0x01) == 0x01)) {
		CPU_SET_CY(&cpu->reg);
	}
	else {
		CPU_CLR_CY(&cpu->reg);
	}
	CPU_CLR_OV(&cpu->reg);
	op_chk_and_set_sign(&cpu->reg, cpu->reg.r[reg3]);
	op_chk_and_set_zero(&cpu->reg, cpu->reg.r[reg3]);

	cpu->reg.pc += 4;

	return 0;
}

