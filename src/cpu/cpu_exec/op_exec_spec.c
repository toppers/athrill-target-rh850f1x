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

int op_exec_halt(TargetCoreType *cpu)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: HALT:0x%x\n", cpu->reg.pc, cpu->reg.pc + 4));
	//printf("0x%x: HALT:0x%x\n", cpu->reg.pc, cpu->reg.pc + 4);
	//fflush(stdout);
	cpu->is_halt = TRUE;
	cpu->reg.pc += 4;
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
	op_chk_and_set_overflow(&cpu->reg, reg2_data, -((sint64)token));
	op_chk_and_set_zero(&cpu->reg, result);
	op_chk_and_set_sign(&cpu->reg, result);


	cpu->reg.pc += 4;

	return 0;
}

