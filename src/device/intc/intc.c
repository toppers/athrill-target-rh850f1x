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
#include "intc.h"
#include "intc_user.h"
#include "std_device_ops.h"
#include "cpu_common/cpu_ops.h"

ExceptionControllerType intc_control[CPU_CONFIG_CORE_NUM];
static ExceptionRequestEntryType* judge_pending_Exception(CoreIdType coreId);
static void raise_Exception(ExceptionRequestEntryType *exception);


void device_init_intc(CpuType *cpu, MpuAddressRegionType *region)
{
	CoreIdType coreId = 0;
	CoreIdType coreId_num = CPU_CONFIG_GET_CORE_ID_NUM();

	intc_region = region;

	/*
	 * intc_controlの初期化
	 */
	for (coreId = 0; coreId < coreId_num; coreId++) {
		intc_control[coreId].cpu = &cpu->cores[coreId].core;
		intc_control_register_op.reset_registers(coreId);

		intc_control[coreId].current_exception = NULL;
		device_init_intc_UserIntr(coreId);
		IntcPriorityLevelType level;
		for (level = IntcPriorityLevel_0; level < IntcPriorityLevel_Num; level++) {
			List_Init(&intc_control[coreId].exception_queue[level].pending, ExceptionRequestEntryType, 512U);
		}
	}

	return;
}

void device_supply_clock_intc(DeviceClockType *dev_clock)
{
	CoreIdType coreId;
	CoreIdType coreId_num = CPU_CONFIG_GET_CORE_ID_NUM();
	ExceptionRequestEntryType *exception;

#ifndef CPUEMU_CLOCK_BUG_FIX
	dev_clock->clock++;
#else
#endif /* CPUEMU_CLOCK_BUG_FIX */

	for (coreId = 0; coreId < coreId_num; coreId++) {
		judge_pending_UserIntr(coreId);
		exception = judge_pending_Exception(coreId);
		if (exception != NULL) {
			//DBG_PRINTF(("%s() exp=0x%x\n", __FUNCTION__, exception);
			raise_Exception(exception);
		}
	}

	for (coreId = 0; coreId < coreId_num; coreId++) {
		if (intc_control[coreId].current_exception != NULL) {
			dev_clock->intclock++;
			break;
		}
	}

	return;
}


int intc_raise_exception(CoreIdType coreId, ExceptionIdType exno)
{
	ExceptionRequestEntryType *req;
	IntcPriorityLevelType level = intc_control_register_op.get_exception_priority_level(coreId, exno);

	ListEntry_Alloc(&intc_control[coreId].exception_queue[level].pending, ExceptionRequestEntryType, &req);

	req->data.req = &exception_config[exno];
	req->data.status = ExceptionRequestStatus_RUNNABLE;
	req->data.coreId = coreId;
	req->data.exno = exno;
	req->data.intno = 0;
	req->data.priority_degree = intc_control_register_op.get_exception_priority_degree(coreId, exno, 0);
	req->data.is_user = FALSE;
	ListEntry_AddEntry(&intc_control[coreId].exception_queue[level].pending, req);
	intc_control[coreId].cpu->is_halt = FALSE;

	return 0;
}
void intc_clr_fe(CoreIdType coreId)
{
	ExceptionRequestEntryType *exp = intc_control[coreId].current_exception;
	if (exp == NULL) {
		return;
	}
	IntcPriorityLevelType level = intc_control_register_op.get_exception_priority_level(coreId, exp->data.exno);
	ListEntry_RemoveEntry(&intc_control[coreId].exception_queue[level].pending, exp);
	intc_control[coreId].current_exception = NULL;
	return;
}

void intc_clr_ei(CoreIdType coreId)
{
	ExceptionRequestEntryType *exp = intc_control[coreId].current_exception;
	//DBG_PRINTF(("%s() : exp=0x%x\n", __FUNCTION__, exp);
	if (exp == NULL) {
		return;
	}
	//DBG_PRINTF(("%s() : exp=0x%x exno=%u intno=%u\n", __FUNCTION__, exp, exp->data.exno, exp->data.intno);
	if (exp->data.exno == ExceptionId_UserIntr) {
		intc_control_register_op.set_current_user_intr_priority_degree(coreId, exp->data.intno, FALSE);
	}
	IntcPriorityLevelType level = intc_control_register_op.get_exception_priority_level(coreId, exp->data.exno);
	ListEntry_RemoveEntry(&intc_control[coreId].exception_queue[level].pending, exp);
	intc_control[coreId].current_exception = NULL;
	return;
}

/* ----------------- static -------------------------------- */

static bool can_raise_Exception(ExceptionRequestEntryType *exception);

static bool can_raise_Exception(ExceptionRequestEntryType *exception)
{
	TargetCoreType *cpu = intc_control[exception->data.coreId].cpu;

	/*
	 * PSW.NP
	 */
	if ((exception->data.req->cond.NP == FALSE) && (CPU_ISSET_NP(&cpu->reg) == TRUE)) {
		return FALSE;
	}
	/*
	 * PSW.ID
	 */
	if ((exception->data.req->cond.ID == FALSE) && (CPU_ISSET_ID(&cpu->reg) == TRUE)) {
		if (cpu->is_halt == TRUE) {
			cpu->is_halt = FALSE;
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

static void raise_Exception(ExceptionRequestEntryType *exception)
{
	TargetCoreType *cpu = intc_control[exception->data.coreId].cpu;
	uint32 wdata32;

	ASSERT(exception == intc_control[exception->data.coreId].current_exception);
	ASSERT(exception->data.status == ExceptionRequestStatus_RUNNING);

	if (exception->data.exno == ExceptionId_UserIntr) {
		hook_UserIntr_Raised(exception);
	}

	if (exception->data.req->level == ExceptionLevel_FE) {
		sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPC] = cpu->reg.pc;
		sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPSW] = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW];
		wdata32 = intc_control_register_op.get_exception_code(exception->data.coreId, exception->data.exno, 0U);
		sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEIC] = wdata32;
	}
	else {
		sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPC] = cpu->reg.pc;
		sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPSW] = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW];
		wdata32 = (sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIIC] & 0xFFFF0000);
		wdata32 |= 	intc_control_register_op.get_exception_code(exception->data.coreId, exception->data.exno, exception->data.intno);
		sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIIC] = wdata32;
	}

	/*
	 * ID
	 */
	if (exception->data.req->info.ID == ExceptionUpdatePswOp_SET) {
		CPU_SET_ID(&cpu->reg);
	}
	else if (exception->data.req->info.ID == ExceptionUpdatePswOp_CLR) {
		CPU_CLR_ID(&cpu->reg);
	}
	/*
	 * NP
	 */
	if (exception->data.req->info.NP == ExceptionUpdatePswOp_SET) {
		CPU_SET_NP(&cpu->reg);
	}
	else if (exception->data.req->info.NP == ExceptionUpdatePswOp_CLR) {
		CPU_CLR_NP(&cpu->reg);
	}
	/*
	 * EP
	 */
	if (exception->data.req->info.EP == ExceptionUpdatePswOp_SET) {
		CPU_SET_EP(&cpu->reg);
	}
	else if (exception->data.req->info.EP == ExceptionUpdatePswOp_CLR) {
		CPU_CLR_EP(&cpu->reg);
	}
	/*
	 * UM
	 */
	if (exception->data.req->info.UM == ExceptionUpdatePswOp_SET) {
		CPU_SET_UM(&cpu->reg);
	}
	else if (exception->data.req->info.UM == ExceptionUpdatePswOp_CLR) {
		CPU_CLR_UM(&cpu->reg);
	}
	intc_control[exception->data.coreId].cpu->reg.pc = intc_control_register_op.get_exception_handler_addr(
			exception->data.coreId, exception->data.exno, exception->data.intno);
	intc_control[exception->data.coreId].cpu->is_halt = FALSE;
	return;
}


static ExceptionRequestEntryType* judge_pending_Exception(CoreIdType coreId)
{
	ExceptionRequestEntryType *req;
	ExceptionRequestEntryType *candidate = NULL;
	IntcPriorityLevelType level;
	IntcPriorityLevelType current_level = IntcPriorityLevel_Num - 1;
	uint32 tmpdgree;
	uint32 mindegree = 0xFFFFFFFF;

	candidate = intc_control[coreId].current_exception;
	if (candidate != NULL) {
		//ASSERT(candidate->data.status == ExceptionRequestStatus_RUNNING);
		DBG_PRINTF(("%s(): already raised exception:exno=%u intno=%u\n", __FUNCTION__, candidate->data.exno, candidate->data.intno));
		current_level = intc_control_register_op.get_exception_priority_level(coreId, candidate->data.exno);
		mindegree = candidate->data.priority_degree;
	}
	DBG_PRINTF(("%s() current_level=%u\n", __FUNCTION__, current_level));
	/*
	 * すでに実行中の例外が新規のものより高ければ検索を打ち切る．
	 * 同一優先レベルの場合は，優先度が高いものがキューに存在している可能性があるため打ち切らない．
	 */
	for (level = IntcPriorityLevel_0; level <= current_level; level++) {
		//DBG_PRINTF(("level=%u current_level = %u req_num=%u\n", level, current_level, intc_control[coreId].exception_queue[level].pending.entry_num));
		/*
		 * すでに実行中の例外よりも新規のものの方が高ければ，検索する．
		 */
		ListEntry_Foreach(&intc_control[coreId].exception_queue[level].pending, req) {
#ifdef USER_INTR_PMR_BUGFIX
			if (req->data.is_user == TRUE) {
				if (is_cpu_mask_UserIntr(req) == TRUE) {
					continue;
				}
			}
#endif /* USER_INTR_PMR_BUGFIX */
			tmpdgree = req->data.priority_degree;
			DBG_PRINTF(("tmpdgree=0x%x mindegree=0x%x\n", tmpdgree, tmpdgree));
			if (tmpdgree < mindegree) {
				mindegree = tmpdgree;
				candidate = req;
			}
		}
		if ((candidate == intc_control[coreId].current_exception)) {
			continue;
		}
		if (candidate != NULL) {
			break;
		}
	}

	if ((candidate == NULL) || (candidate == intc_control[coreId].current_exception)) {
		/*
		 * 候補がない，または，現在実行中のものが最高優先度の場合
		 */
		DBG_PRINTF(("%s(): can not raise exp=0x%x\n", __FUNCTION__, candidate));
		return NULL;
	}
	if (candidate->data.status == ExceptionRequestStatus_WAITING) {
		/*
		 * 多重割り込み復帰した場合は，新規割り込みではないため，割り込みを発生させない．
		 */
		candidate->data.status = ExceptionRequestStatus_RUNNING;
		intc_control[coreId].current_exception = candidate;
		DBG_PRINTF(("%s(): can not raise because waiting  exp=0x%x\n", __FUNCTION__, candidate));
		return NULL;
	}
	if (intc_control[coreId].current_exception != NULL) {
		/*
		 * 多重割り込みが発生したので，既存のものはWAITING状態にし，新規割り込みを発生させる
		 */
		intc_control[coreId].current_exception->data.status = ExceptionRequestStatus_WAITING;
		//intc_control[coreId].current_exception = NULL;
		DBG_PRINTF(("%s(): raise multi  exp=0x%x\n", __FUNCTION__, candidate));
	}

	if (can_raise_Exception(candidate)) {
		DBG_PRINTF(("%s(): raise exp=0x%x\n", __FUNCTION__, candidate));
		candidate->data.status = ExceptionRequestStatus_RUNNING;
		intc_control[coreId].current_exception = candidate;
#ifdef USER_INTR_PMR_BUGFIX
		if (candidate->data.is_user == TRUE) {
			intc_control_register_op.set_user_intr_state_priority_degree_masked(candidate->data.coreId, FALSE);
		}
#endif /* USER_INTR_PMR_BUGFIX */
		return candidate;
	}
	else {
		DBG_PRINTF(("%s(): can not raise exp=0x%x because of psw\n", __FUNCTION__, candidate));
		return NULL;
	}
}


