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
#include "concrete_executor/target/dbg_target_cpu.h"
#include "cpu.h"
#include "symbol_ops.h"

#include <stdio.h>
#include "intc.h"
#include "target_mpu_op.h"

static void print_register(const char* regname, uint32 addr, char* opt)
{
	 uint32 funcaddr;
	 int funcid;
	 uint32 gladdr;
	 int glid;

	 funcid = symbol_pc2funcid(addr, &funcaddr);
	 if (funcid >= 0) {
		printf("%s		0x%x %s(+0x%x)", regname, addr, symbol_funcid2funcname(funcid), addr - funcaddr);
	 }
	 else {
		glid = symbol_addr2glid(addr, &gladdr);
		if (glid >= 0) {
			printf("%s		0x%x %s(+0x%x)", regname, addr, symbol_glid2glname(glid), addr - gladdr);
		}
		else {
			printf("%s		0x%x", regname, addr);
		}
	 }
	 if (opt != NULL) {
		 printf(" %s\n", opt);
	 }
	 else {
		 printf("\n");
	 }
	return;
}

static const char* intr_status_str[ExceptionRequestStatus_Num] = {
		"PENDING",
		"RUNNABLE",
		"RUNNING",
		"WAITING",
};

static void show_intr_info(void)
{
	ExceptionRequestEntryType *req;
	ExceptionRequestEntryType *curr = intc_control[0].current_exception;

	printf("\n**** INTERRUPT CONTROLLER ****\n");
	if (curr != NULL) {
		printf("current exception = %s[%s] level=%u intno=%u degree=%x\n",
				curr->data.req->name,
				intr_status_str[curr->data.status],
				curr->data.req->level,
				curr->data.intno, curr->data.priority_degree);
	}
	else {
		printf("current exception = NONE\n");
	}

	ListEntry_Foreach(&intc_control[0].user_queue.pending, req) {
		printf(" *user intr(%s:%s) intno=%u degree=%x\n",
				req->data.req->name,
				intr_status_str[req->data.status],
				req->data.intno, req->data.priority_degree);
	}

	IntcPriorityLevelType level;
	for (level = IntcPriorityLevel_0; level < IntcPriorityLevel_Num; level++) {
		ListEntry_Foreach(&intc_control[0].exception_queue[level].pending, req) {
			printf(" *exception[%u] intr(%s:%s) intno=%u degree=%x\n",
					level,
					req->data.req->name,
					intr_status_str[req->data.status],
					req->data.intno, req->data.priority_degree);
		}
	}
}

void dbg_target_print_cpu(uint32 coreId)
{
	int i;
	uint32 pc;
	char buffer[128];

	printf("***CPU<%d>***\n", coreId);
	pc = virtual_cpu.cores[coreId].core.reg.pc;

	print_register("PC", pc, NULL);
	for (i = 0; i < 32; i++) {
		 char *opt = NULL;
		 sprintf(buffer, "R%d", i);
		 switch (i) {
		 case 3:
			 opt = "Stack Pointer";
			 break;
		 case 10:
			 opt = "Return Value";
			 break;
		 case 6:
			 opt = "Arg1";
			 break;
		 case 7:
			 opt = "Arg2";
			 break;
		 case 8:
			 opt = "Arg3";
			 break;
		 case 9:
			 opt = "Arg4";
			 break;
		 default:
			 break;
		 }
		 print_register(buffer, virtual_cpu.cores[coreId].core.reg.r[i], opt);
	 }
	 print_register("EIPC", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_EIPC], NULL);
	 printf("EIPSW		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_EIPSW]);
	 printf("ECR		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_ECR]);
	 printf("PSW		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_PSW]);
	 print_register("FEPC", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_FEPC], NULL);
	 printf("FEPSW 		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_FEPSW]);
	 printf("CTBP		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_CTBP]);

	 printf("FPSR		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_FPSR]);
	 printf("FPEPC		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_FPEPC]);
	 printf("FPST		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_FPST]);
	 printf("FPCC		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_FPCC]);
	 printf("FPCFG		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_FPCFG]);
	 printf("FPEC		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_FPEC]);

	 printf("RBASE		0x%x\n", *cpu_get_sysreg2(&virtual_cpu.cores[coreId].core.reg.sys, SYS_GRP_1, SYS_REG_RBASE));
	 printf("EBASE		0x%x\n", *cpu_get_sysreg2(&virtual_cpu.cores[coreId].core.reg.sys, SYS_GRP_1, SYS_REG_EBASE));
	 printf("INTBP		0x%x\n", *cpu_get_sysreg2(&virtual_cpu.cores[coreId].core.reg.sys, SYS_GRP_1, SYS_REG_INTBP));

	 printf("ISPR		0x%x\n", *cpu_get_sysreg2(&virtual_cpu.cores[coreId].core.reg.sys, SYS_GRP_2, SYS_REG_ISPR));
	 printf("PMR		0x%x\n", *cpu_get_sysreg2(&virtual_cpu.cores[coreId].core.reg.sys, SYS_GRP_2, SYS_REG_PMR));
	 printf("INTCFG		0x%x\n", *cpu_get_sysreg2(&virtual_cpu.cores[coreId].core.reg.sys, SYS_GRP_2, SYS_REG_INTCFG));

	 target_mpu_debug_print_mpu_status(&(virtual_cpu.cores[coreId].core));
	 show_intr_info();
	 return;
}
