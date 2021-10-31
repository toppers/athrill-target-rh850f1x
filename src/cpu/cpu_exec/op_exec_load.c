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
#include <stdlib.h>

#include "cpu_exec/op_exec_ops.h"
#include "cpu.h"
#include "bus.h"

#include "device.h"
#include "target_mpu_op.h"

/*
 * Format4
 */
int op_exec_sldb(TargetCoreType *cpu)
{
	uint32 addr;
	sint32 ret;
	uint32 disp;
	uint32 reg1 = CPU_REG_EP;
	uint32 reg2 = cpu->decoded_code->type4_1.reg2;
	sint8 data8;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = cpu->decoded_code->type4_1.disp;
	disp = (disp << 1) | cpu->decoded_code->type4_1.gen;
	disp = op_zero_extend(7, disp);
	addr = cpu->reg.r[reg1] + disp;

	err = bus_get_data8(cpu->core_id, addr, (uint8*)&data8);
	if (err != STD_E_OK) {
		return -1;
	}
	ret = data8;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SLD.B disp7(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], ret));

	cpu->reg.r[reg2] = ret;

	cpu->reg.pc += 2;
	return 0;
}
int op_exec_sldbu(TargetCoreType *cpu)
{
	uint32 addr;
	uint32 ret;
	uint32 disp;
	uint32 reg1 = CPU_REG_EP;
	uint32 reg2 = cpu->decoded_code->type4_2.reg2;
	uint8 data8;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = cpu->decoded_code->type4_2.disp;
	disp = op_zero_extend(4, disp);
	addr = cpu->reg.r[reg1] + disp;

	err = bus_get_data8(cpu->core_id, addr, &data8);
	if (err != STD_E_OK) {
		return -1;
	}
	ret = data8;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SLD.BU disp4(%u),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], ret));

	cpu->reg.r[reg2] = ret;

	cpu->reg.pc += 2;
	return 0;
}

int op_exec_sldhu(TargetCoreType *cpu)
{
	uint32 addr;
	uint32 ret;
	uint32 disp;
	uint32 reg1 = CPU_REG_EP;
	uint32 reg2 = cpu->decoded_code->type4_2.reg2;
	uint16 data16;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = ( (cpu->decoded_code->type4_2.disp) << 1U );
	disp = op_zero_extend(5, disp);
	addr = cpu->reg.r[reg1] + disp;

	err = bus_get_data16(cpu->core_id, addr, &data16);
	if (err != STD_E_OK) {
		return -1;
	}
	ret = data16;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SLD.HU disp4(%u),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], ret));

	cpu->reg.r[reg2] = ret;

	cpu->reg.pc += 2;
	return 0;
}

int op_exec_sldh(TargetCoreType *cpu)
{
	uint32 addr;
	sint32 ret;
	uint32 disp;
	uint32 reg1 = CPU_REG_EP;
	uint32 reg2 = cpu->decoded_code->type4_1.reg2;
	sint16 data16;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = cpu->decoded_code->type4_1.disp;
	disp = (disp << 1) | cpu->decoded_code->type4_1.gen;
	disp = op_zero_extend(7, disp);
	disp = disp << 1;


	addr = cpu->reg.r[reg1] + disp;

	err = bus_get_data16(cpu->core_id, addr, (uint16*)&data16);
	if (err != STD_E_OK) {
		return -1;
	}
	ret = data16;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SLD.H disp8(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], ret));

	cpu->reg.r[reg2] = ret;

	cpu->reg.pc += 2;
	return 0;
}

int op_exec_sldw(TargetCoreType *cpu)
{
	uint32 addr;
	uint32 ret;
	uint32 disp;
	uint32 reg1 = CPU_REG_EP;
	uint32 reg2 = cpu->decoded_code->type4_1.reg2;
	uint32 data32;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = cpu->decoded_code->type4_1.disp;
	disp = op_zero_extend(6, disp);
	disp = disp << 2;
	addr = cpu->reg.r[reg1] + disp;

	//TODO:comm_hook_load_reg32(cpu, addr);

	err = bus_get_data32(cpu->core_id, addr, &data32);
	if (err != STD_E_OK) {
		return -1;
	}
	ret = data32;

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: SLD.W disp8(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], ret));

	cpu->reg.r[reg2] = ret;

	cpu->reg.pc += 2;
	return 0;
}
/*
 * Format7
 */

int op_exec_ldb(TargetCoreType *cpu)
{
	uint32 addr;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code->type7.reg1;
	uint32 reg2 = cpu->decoded_code->type7.reg2;
	sint8 data8;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = op_sign_extend(15, (cpu->decoded_code->type7.disp << 1) | cpu->decoded_code->type7.gen);

	addr = cpu->reg.r[reg1] + disp;

	err = bus_get_data8(cpu->core_id, addr, (uint8*)&data8);
	if (err != STD_E_OK) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.B disp16(%d),r%d(0x%x), r%d(0x%x) addr=0x%x:0x%x\n", cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], addr, data8));

	cpu->reg.r[reg2] = data8;

	cpu->reg.pc += 4;
	return 0;
}
int op_exec_ld_b_14(TargetCoreType *cpu)
{
	uint32 addr;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code->type14.reg1;
	uint32 reg3 = cpu->decoded_code->type14.reg3;
	sint8 data8;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = op_sign_extend(22, (cpu->decoded_code->type14.disp_high << 7U) | cpu->decoded_code->type14.disp_low);

	addr = cpu->reg.r[reg1] + disp;

	err = bus_get_data8(cpu->core_id, addr, (uint8*)&data8);
	if (err != STD_E_OK) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.B disp23(%d),r%d(0x%x), r%d(0x%x) addr=0x%x:0x%x\n",
			cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg3, cpu->reg.r[reg3], addr, data8));

	cpu->reg.r[reg3] = data8;

	cpu->reg.pc += 6;
	return 0;
}

int op_exec_ldbu(TargetCoreType *cpu)
{
	uint32 addr;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code->type7.reg1;
	uint32 reg2 = cpu->decoded_code->type7.reg2;
	sint32 disp_bit;
	uint8 data8;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp_bit = (cpu->decoded_code->type7.opcode & 0x0001);
	disp = op_sign_extend(15, (cpu->decoded_code->type7.disp << 1) | disp_bit);

	addr = cpu->reg.r[reg1] + disp;

	err = bus_get_data8(cpu->core_id, addr, &data8);
	if (err != STD_E_OK) {
		return -1;
	}

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.BU disp16(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], data8));

	cpu->reg.r[reg2] = (sint32)((uint32)data8);

	cpu->reg.pc += 4;
	return 0;
}

int op_exec_ld_bu_14(TargetCoreType *cpu)
{
	uint32 addr;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code->type14.reg1;
	uint32 reg3 = cpu->decoded_code->type14.reg3;
	uint8 data8;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = op_sign_extend(22, (cpu->decoded_code->type14.disp_high << 7U) | cpu->decoded_code->type14.disp_low);

	addr = cpu->reg.r[reg1] + disp;

	err = bus_get_data8(cpu->core_id, addr, &data8);
	if (err != STD_E_OK) {
		return -1;
	}

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.BU disp23(%d),r%d(0x%x), r%d(0x%x):0x%x\n",
			cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg3, cpu->reg.r[reg3], data8));

	cpu->reg.r[reg3] = (sint32)((uint32)data8);

	cpu->reg.pc += 6;
	return 0;
}

int op_exec_ldhw(TargetCoreType *cpu)
{
	uint32 addr;
	sint32 ret;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code->type7.reg1;
	uint32 reg2 = cpu->decoded_code->type7.reg2;
	sint16 data16;
	sint32 data32;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}


	if (cpu->decoded_code->type7.gen == 0x00) {
		//LD.H
		disp = op_sign_extend(15, (cpu->decoded_code->type7.disp << 1) );
		addr = cpu->reg.r[reg1] + disp;

		err = bus_get_data16(cpu->core_id, addr, (uint16*)&data16);
		if (err != STD_E_OK) {
			return -1;
		}
		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.H disp16(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], data16));
		ret = data16;
	}
	else {
		//LD.W
		disp = op_sign_extend(15, (cpu->decoded_code->type7.disp << 1) );
		addr = cpu->reg.r[reg1] + disp;

		//TODO: comm_hook_load_reg32(cpu, addr);

		err = bus_get_data32(cpu->core_id, addr, (uint32*)&data32);
		if (err != STD_E_OK) {
			return -1;
		}

		DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.W disp16(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], data32));
		ret = data32;

	}
	cpu->reg.r[reg2] = ret;

	cpu->reg.pc += 4;
	return 0;
}

int op_exec_ld_h_14(TargetCoreType *cpu)
{
	uint32 addr;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code->type14.reg1;
	uint32 reg3 = cpu->decoded_code->type14.reg3;
	sint16 data16;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = op_sign_extend(22, (cpu->decoded_code->type14.disp_high << 7U) | cpu->decoded_code->type14.disp_low);

	addr = cpu->reg.r[reg1] + disp;

	err = bus_get_data16(cpu->core_id, addr, (uint16*)&data16);
	if (err != STD_E_OK) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.H disp23(%d),r%d(0x%x), r%d(0x%x):0x%x\n",
			cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg3, cpu->reg.r[reg3], data16));

	cpu->reg.r[reg3] = data16;

	cpu->reg.pc += 6;
	return 0;
}

int op_exec_ld_w_14(TargetCoreType *cpu)
{
	uint32 addr;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code->type14.reg1;
	uint32 reg3 = cpu->decoded_code->type14.reg3;
	sint32 data32;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = op_sign_extend(22, (cpu->decoded_code->type14.disp_high << 7U) | cpu->decoded_code->type14.disp_low);

	addr = cpu->reg.r[reg1] + disp;

	err = bus_get_data32(cpu->core_id, addr, (uint32*)&data32);
	if (err != STD_E_OK) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.W disp23(%d),r%d(0x%x), r%d(0x%x):0x%x\n",
			cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg3, cpu->reg.r[reg3], data32));

	cpu->reg.r[reg3] = data32;

	cpu->reg.pc += 6;
	return 0;
}


int op_exec_ldhu(TargetCoreType *cpu)
{
	uint32 addr;
	uint32 ret;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code->type7.reg1;
	uint32 reg2 = cpu->decoded_code->type7.reg2;
	uint16 data16;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg2 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = op_sign_extend(15, (cpu->decoded_code->type7.disp << 1) );
	addr = cpu->reg.r[reg1] + disp;

	err = bus_get_data16(cpu->core_id, addr, &data16);
	if (err != STD_E_OK) {
		return -1;
	}

	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.HU disp16(%d),r%d(0x%x), r%d(0x%x):0x%x\n", cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg2, cpu->reg.r[reg2], data16));
	ret = data16;

	cpu->reg.r[reg2] = ret;

	cpu->reg.pc += 4;
	return 0;
}
int op_exec_ld_hu_14(TargetCoreType *cpu)
{
	uint32 addr;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code->type14.reg1;
	uint32 reg3 = cpu->decoded_code->type14.reg3;
	uint16 data16;
	Std_ReturnType err;

	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}

	disp = op_sign_extend(22, (cpu->decoded_code->type14.disp_high << 7U) | cpu->decoded_code->type14.disp_low);

	addr = cpu->reg.r[reg1] + disp;

	err = bus_get_data16(cpu->core_id, addr, (uint16*)&data16);
	if (err != STD_E_OK) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.HU disp23(%d),r%d(0x%x), r%d(0x%x):0x%x\n",
			cpu->reg.pc, disp, reg1, cpu->reg.r[reg1], reg3, cpu->reg.r[reg3], data16));

	cpu->reg.r[reg3] = (sint32)((uint32)data16);

	cpu->reg.pc += 6;
	return 0;
}
