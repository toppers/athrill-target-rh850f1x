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
#include "intc_user.h"

#if 0
#define DBG_IPRINTF(intno, expect, arg) \
do { \
	if (intno == expect) {	\
		printf arg;	\
	}	\
} while (0)
#else
#define DBG_IPRINTF(intno, expect, arg)
#endif

static void common_raise_user_intr(uint32 intno, uint32 coreId);

void device_init_intc_UserIntr(CoreIdType coreId)
{
	List_Init(&intc_control[coreId].user_queue.pending, ExceptionRequestEntryType, 512U);
	return;
}

void intc_cpu_trigger_interrupt(CoreIdType coreId, int intno)
{
	uint32 triggered_coreId;

	triggered_coreId = (coreId == 0) ? 1 : 0;

	common_raise_user_intr(intno, triggered_coreId);
	return;
}

int intc_raise_intr(uint32 intno)
{
	uint32 coreId;

	for (coreId = 0; coreId < CPU_CONFIG_GET_CORE_ID_NUM(); coreId++) {
		common_raise_user_intr(intno, coreId);
	}
	return 0;
}


static ExceptionRequestEntryType *judge_mask_and_priority_UserIntr(CoreIdType coreId);
static void judge_cpu_mask_UserIntr(ExceptionRequestEntryType *candidate);

void judge_pending_UserIntr(CoreIdType coreId)
{
	ExceptionRequestEntryType *candidate;

	candidate = judge_mask_and_priority_UserIntr(coreId);
	if (candidate != NULL) {
		DBG_IPRINTF(candidate->data.intno, 35,
				("%s() candidate=0x%x\n", __FUNCTION__, candidate));
		judge_cpu_mask_UserIntr(candidate);
	}

	return;
}

void hook_UserIntr_Raised(const ExceptionRequestEntryType *exception)
{
	CoreIdType coreId = exception->data.coreId;
	ExceptionRequestEntryType *req;
	bool req_found = FALSE;
	ListEntry_Foreach(&intc_control[coreId].user_queue.pending, req) {
		if (req->data.intno == exception->data.intno) {
			ListEntry_RemoveEntry(&intc_control[coreId].user_queue.pending, req);
			ListEntry_Free(&intc_control[coreId].user_queue.pending, req);
			//DBG_PRINTF(("%s() pending user req removed candidate=0x%x\n", __FUNCTION__, req);
			DBG_IPRINTF(exception->data.intno, 35,
					("%s() pending user req removed candidate=0x%x\n", __FUNCTION__, req));
			req_found = TRUE;
			break;
		}
	}
	/* ispr update */
	intc_control_register_op.set_current_user_intr_priority_degree(coreId, exception->data.intno, TRUE);

	/* req cler */
	if (req_found == TRUE) {
		intc_control_register_op.set_user_intr_reqflag(req->data.coreId, req->data.intno, FALSE);
	}
	else {
		printf("ERROR:hook_UserIntr_Raised() not found req:intno = %d\n", exception->data.intno);
	}
	return;
}

static void common_raise_user_intr(uint32 intno, uint32 coreId)
{
	ExceptionRequestEntryType *req;

	if (intc_control_register_op.isset_user_intr_reqflag(coreId, intno)) {
		return;
	}

	ListEntry_Alloc(&intc_control[coreId].user_queue.pending, ExceptionRequestEntryType, &req);

	req->data.req = &exception_config[ExceptionId_UserIntr];
	req->data.status = ExceptionRequestStatus_PENDING;
	req->data.coreId = coreId;
	req->data.exno = ExceptionId_UserIntr;
	req->data.intno = intno;
	req->data.priority_degree = intc_control_register_op.get_exception_priority_degree(coreId, ExceptionId_UserIntr, intno);
	ListEntry_AddEntry(&intc_control[coreId].user_queue.pending, req);

	intc_control_register_op.set_user_intr_reqflag(req->data.coreId, req->data.intno, TRUE);
	return;
}

static ExceptionRequestEntryType *judge_mask_and_priority_UserIntr(CoreIdType coreId)
{
	ExceptionRequestEntryType *req;
	ExceptionRequestEntryType *candidate = NULL;
	uint32 tmp_degree;
	uint32 mindegree = 0xFFFFFFFF;

	DBG_PRINTF(("%s(): req_num=%u\n", __FUNCTION__,  intc_control[coreId].user_queue.pending.entry_num));
	ListEntry_Foreach(&intc_control[coreId].user_queue.pending, req) {
		//mask check
		if (intc_control_register_op.is_user_intr_masked(coreId, req->data.intno)) {
			continue;
		}
		DBG_IPRINTF(req->data.intno, 35,
				("req degree=%u \n", req->data.priority_degree));

		tmp_degree = req->data.priority_degree;
		if (tmp_degree < mindegree) {
			mindegree = tmp_degree;
			candidate = req;
		}
	}
	if ((candidate != NULL) && (candidate->data.status == ExceptionRequestStatus_RUNNABLE)) {
		/*
		 * すでに例外要求を上げているので，割り込み要求を上げる必要はない．
		 */
		DBG_PRINTF(("already raised candidate degree=%u default=%u\n", candidate->data.priority_degree, INTC_DEFAULT_PRIORITY(candidate->data.intno)));
		DBG_IPRINTF(candidate->data.intno, 35,
				("already raised candidate degree=%u \n", candidate->data.priority_degree));
		return NULL;
	}
	if (candidate != NULL) {
		/*
		 * マスクされている場合はHALT復帰しないので，ここで設定すべき
		 */
		intc_control[coreId].cpu->is_halt = FALSE;
		DBG_IPRINTF(candidate->data.intno, 35, ("raise user interrupt\n"));
	}
	return candidate;
}

std_bool is_cpu_mask_UserIntr(ExceptionRequestEntryType *candidate)
{
	/*
	 * current priority check
	 */
	IntcPriorityDegreeType intr_level = intc_control_register_op.get_user_intr_priority_degree(candidate->data.coreId, candidate->data.intno);

	/*
	 * mask priority check
	 */
	IntcPriorityDegreeType mask_level = intc_control_register_op.get_mask_priority_degree(candidate->data.coreId);
	if (mask_level <= intr_level) {
		intc_control_register_op.set_user_intr_state_priority_degree_masked(candidate->data.coreId, TRUE);
		return TRUE;
	}
	return FALSE;
}


static void judge_cpu_mask_UserIntr(ExceptionRequestEntryType *candidate)
{
	ASSERT(candidate->data.status == ExceptionRequestStatus_PENDING);
	/*
	 * current priority check
	 */
	IntcPriorityDegreeType curr_level = intc_control_register_op.get_current_user_intr_priority_degree(candidate->data.coreId);
	IntcPriorityDegreeType intr_level = intc_control_register_op.get_user_intr_priority_degree(candidate->data.coreId, candidate->data.intno);

	if (curr_level <= intr_level) {
		DBG_PRINTF(("%s() NONE: curr_level = %u intr_level = %u\n", __FUNCTION__, curr_level, intr_level));
		return;
	}

	/*
	 * mask priority check
	 */
	IntcPriorityDegreeType mask_level = intc_control_register_op.get_mask_priority_degree(candidate->data.coreId);
	if (mask_level <= intr_level) {
		intc_control_register_op.set_user_intr_state_priority_degree_masked(candidate->data.coreId, TRUE);
		DBG_PRINTF(("%s() NONE: mask_level = %u intr_level = %u\n", __FUNCTION__, mask_level, intr_level));
		return;
	}
	intc_control_register_op.set_user_intr_state_priority_degree_masked(candidate->data.coreId, FALSE);

	/*
	 * user queue
	 */
	IntcPriorityLevelType level = candidate->data.req->priority.level;
	candidate->data.status = ExceptionRequestStatus_RUNNABLE;

	/*
	 * system queue
	 */
	ExceptionRequestEntryType *exception;
	ListEntry_Alloc(&intc_control[candidate->data.coreId].exception_queue[level].pending, ExceptionRequestEntryType, &exception);
	exception->data = candidate->data;
	ListEntry_AddEntry(&intc_control[candidate->data.coreId].exception_queue[level].pending, exception);
	DBG_PRINTF(("%s() RAISED: level=%u mask_level = %u intr_level = %u req_num=%u\n", __FUNCTION__, level, mask_level, intr_level, intc_control[candidate->data.coreId].exception_queue[level].pending.entry_num));
	return;
}

