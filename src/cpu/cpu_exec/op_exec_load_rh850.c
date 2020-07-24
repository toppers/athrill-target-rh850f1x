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

int op_exec_ld_dw_14(TargetCoreType *cpu)
{
	uint32 addr;
	sint32 disp;
	uint32 reg1 = cpu->decoded_code->type14.reg1;
	uint32 reg3 = cpu->decoded_code->type14.reg3;
	sint32 data32[2];
	Std_ReturnType err;

	bool has_permission;


	if (reg1 >= CPU_GREG_NUM) {
		return -1;
	}
	if (reg3 >= CPU_GREG_NUM) {
		return -1;
	}
	if ((reg3 % 2) != 0) {
		return -1;
	}

	disp = op_sign_extend(22, (cpu->decoded_code->type14.disp_high << 7U) | cpu->decoded_code->type14.disp_low);

	addr = cpu->reg.r[reg1] + disp;

	has_permission = target_mpu_has_permission(cpu, MPU_TARGET_ACCESS_READ, addr, 8U);
	if (!(has_permission)) {
		return -1;
	}

	err = bus_get_data32(cpu->core_id, addr, (uint32*)&data32[0]);
	if (err != STD_E_OK) {
		return -1;
	}
	err = bus_get_data32(cpu->core_id, addr, (uint32*)&data32[1]);
	if (err != STD_E_OK) {
		return -1;
	}
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(), "0x%x: LD.DW disp23(%d),r%d(0x%x), r%d(0x%x):0x%x 0x%x\n",
			cpu->reg.pc,
			disp, reg1, cpu->reg.r[reg1],
			reg3, cpu->reg.r[reg3],
			data32[0], data32[1]));

	cpu->reg.r[reg3 + 0] = data32[0];
	cpu->reg.r[reg3 + 1] = data32[1];

	cpu->reg.pc += 6;
	return 0;
}

