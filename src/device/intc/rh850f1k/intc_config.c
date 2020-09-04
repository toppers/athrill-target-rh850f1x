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
#include "cpu_common/cpu_ops.h"
#include "mpu_ops.h"
#include "device.h"

static IntcPriorityLevelType intc_get_exception_priority_level(CoreIdType coreId, uint32 exno);
static uint32 intc_get_exception_priority_degree(CoreIdType coreId, uint32 exno, uint32 intno);

static IntcPriorityDegreeType intc_get_current_user_intr_priority_degree(CoreIdType coreId);
static void intc_set_current_user_intr_priority_degree(CoreIdType coreId, uint32 intno, bool flag);
static IntcPriorityDegreeType intc_get_mask_priority_degree(CoreIdType coreId);
static void intc_set_mask_priority(CoreIdType coreId, IntcPriorityDegreeType degree);
static void intc_reset_registers(CoreIdType coreId);
static void intc_set_user_intr_reqflag(CoreIdType coreId, uint32 intno, bool flag);
static bool intc_isset_user_intr_reqflag(CoreIdType coreId, uint32 intno);

static  IntcPriorityDegreeType intc_get_user_intr_priority_degree(CoreIdType coreId, uint32 intno);
static  void intc_set_user_intr_priority_degree(CoreIdType coreId, uint32 intno, IntcPriorityDegreeType degree);

static  uint32 intc_get_exception_handler_addr(CoreIdType coreId, uint32 exno, uint32 intno);
static  uint32 intc_get_exception_code(CoreIdType coreId, uint32 exno, uint32 intno);

static bool intc_is_user_intr_masked(CoreIdType coreId, uint32 intno);
static void intc_set_user_intr_state_priority_degree_masked (CoreIdType coreId, bool flag);

IntcControlRegisterOperationType intc_control_register_op = {
		.get_exception_priority_level 				= intc_get_exception_priority_level,
		.get_exception_priority_degree 				= intc_get_exception_priority_degree,
		.get_current_user_intr_priority_degree		= intc_get_current_user_intr_priority_degree,
		.set_current_user_intr_priority_degree		= intc_set_current_user_intr_priority_degree,
		.get_mask_priority_degree					= intc_get_mask_priority_degree,
		.set_mask_priority_degree					= intc_set_mask_priority,
		.set_user_intr_reqflag						= intc_set_user_intr_reqflag,
		.isset_user_intr_reqflag					= intc_isset_user_intr_reqflag,
		.get_user_intr_priority_degree				= intc_get_user_intr_priority_degree,
		.set_user_intr_priority_degree				= intc_set_user_intr_priority_degree,
		.is_user_intr_masked						= intc_is_user_intr_masked,
		.set_user_intr_state_priority_degree_masked = intc_set_user_intr_state_priority_degree_masked,
		.get_exception_handler_addr					= intc_get_exception_handler_addr,
		.get_exception_code							= intc_get_exception_code,
		.reset_registers							= intc_reset_registers,
};

ExceptionType exception_config[ExceptionId_Num] = {
		{
			.name = "RESET",
			.priority = { IntcPriorityLevel_0, IntcPriorityDegree_0 },
			.handler = { .addr = 0x00, .code = 0x00, .is_rbase = FALSE },
			.level = ExceptionLevel_RESET,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_CLR, .EP = ExceptionUpdatePswOp_CLR, .EBV = ExceptionUpdatePswOp_CLR },
		},
		{
			.name = "FENMI",
			.priority = { IntcPriorityLevel_2, IntcPriorityDegree_0 },
			.handler = { .addr = 0xE0, .code = 0xE0, .is_rbase = TRUE },
			.level = ExceptionLevel_FE,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SET, .EP = ExceptionUpdatePswOp_CLR, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "SYSERR1",
			.priority = { IntcPriorityLevel_2, IntcPriorityDegree_1 },
			.handler = { .addr = 0x10, .code = 0x10, .is_rbase = TRUE },
			.level = ExceptionLevel_FE,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SET, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "FEINT",
			.priority = { IntcPriorityLevel_2, IntcPriorityDegree_2 },
			.handler = { .addr = 0xF0, .code = 0xF0, .is_rbase = TRUE },
			.level = ExceptionLevel_FE,
			.cond = { .ID = TRUE, .NP = FALSE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SET, .EP = ExceptionUpdatePswOp_CLR, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "FPI",
			.priority = { IntcPriorityLevel_3, IntcPriorityDegree_0 },
			.handler = { .addr = 0x70, .code = 0x72, .is_rbase = TRUE },
			.level = ExceptionLevel_EI,
			.cond = { .ID = FALSE, .NP = FALSE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SAVE, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "UserIntr",
			.priority = { IntcPriorityLevel_3, IntcPriorityDegree_0 },
			.handler = { .addr = 0x100, .code = 0x1000, .is_rbase = TRUE },
			.level = ExceptionLevel_EI,
			.cond = { .ID = FALSE, .NP = FALSE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SAVE, .EP = ExceptionUpdatePswOp_CLR, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "MIP",
			.priority = { IntcPriorityLevel_9, IntcPriorityDegree_0 },
			.handler = { .addr = 0x90, .code = 0x90, .is_rbase = TRUE },
			.level = ExceptionLevel_FE,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SET, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "SYSERR2",
			.priority = { IntcPriorityLevel_9, IntcPriorityDegree_2 },
			.handler = { .addr = 0x10, .code = 0x10, .is_rbase = TRUE },
			.level = ExceptionLevel_FE,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SET, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "RIE",
			.priority = { IntcPriorityLevel_9, IntcPriorityDegree_3 },
			.handler = { .addr = 0x60, .code = 0x60, .is_rbase = TRUE },
			.level = ExceptionLevel_FE,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SET, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "UCPOP",
			.priority = { IntcPriorityLevel_9, IntcPriorityDegree_4 },
			.handler = { .addr = 0x80, .code = 0x80, .is_rbase = TRUE },
			.level = ExceptionLevel_FE,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SET, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "PIE",
			.priority = { IntcPriorityLevel_9, IntcPriorityDegree_5 },
			.handler = { .addr = 0xA0, .code = 0xA0, .is_rbase = TRUE },
			.level = ExceptionLevel_FE,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SET, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "MAE",
			.priority = { IntcPriorityLevel_10, IntcPriorityDegree_0 },
			.handler = { .addr = 0xC0, .code = 0xC0, .is_rbase = TRUE },
			.level = ExceptionLevel_FE,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SET, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "MDP",
			.priority = { IntcPriorityLevel_10, IntcPriorityDegree_0 },
			.handler = { .addr = 0x90, .code = 0x91, .is_rbase = TRUE },
			.level = ExceptionLevel_FE,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SET, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "FPP",
			.priority = { IntcPriorityLevel_10, IntcPriorityDegree_0 },
			.handler = { .addr = 0x70, .code = 0x71, .is_rbase = TRUE },
			.level = ExceptionLevel_EI,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SAVE, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "SYSCALL",
			.priority = { IntcPriorityLevel_11, IntcPriorityDegree_0 },
			.handler = { .addr = 0x40, .code = 0x8000, .is_rbase = TRUE },
			.level = ExceptionLevel_EI,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SAVE, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "FETRAP",
			.priority = { IntcPriorityLevel_11, IntcPriorityDegree_0 },
			.handler = { .addr = 0x30, .code = 0x31, .is_rbase = TRUE },
			.level = ExceptionLevel_FE,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SET, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "TRAP0",
			.priority = { IntcPriorityLevel_11, IntcPriorityDegree_0 },
			.handler = { .addr = 0x40, .code = 0x40, .is_rbase = TRUE },
			.level = ExceptionLevel_EI,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SAVE, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
		{
			.name = "TRAP1",
			.priority = { IntcPriorityLevel_11, IntcPriorityDegree_0 },
			.handler = { .addr = 0x50, .code = 0x50, .is_rbase = TRUE },
			.level = ExceptionLevel_EI,
			.cond = { .ID = TRUE, .NP = TRUE },
			.info = { .UM = ExceptionUpdatePswOp_CLR, .ID = ExceptionUpdatePswOp_SET, .NP = ExceptionUpdatePswOp_SAVE, .EP = ExceptionUpdatePswOp_SET, .EBV = ExceptionUpdatePswOp_SAVE },
		},
};

typedef enum {
	HandlerAddressRefMethod_RBASE_RINT0 = 0,
	HandlerAddressRefMethod_RBASE_RINT1,
	HandlerAddressRefMethod_EBASE_RINT0,
	HandlerAddressRefMethod_EBASE_RINT1,
	HandlerAddressRefMethod_TABLE,
} HandlerAddressRefMethodType;

static bool intc_is_devsetting_tableref(CoreIdType coreId, uint32 exno, uint32 intno)
{

	Std_ReturnType err;
	uint32 base_addr = IC_ADDRESS(intno);
	uint16 data;

	err = device_get_data16(base_addr, &data);
	ASSERT(err == 0);
	return IC_ISSET_TABLEREF(data);
}

static IntcPriorityDegreeType intc_get_user_intr_priority_degree(CoreIdType coreId, uint32 intno)
{

	Std_ReturnType err;
	uint32 base_addr = IC_ADDRESS(intno);
	uint16 data;

	err = device_get_data16(base_addr, &data);
	ASSERT(err == 0);

	return (IntcPriorityDegreeType)IC_GET_PRIODEGREE(data);
}
static IntcPriorityDegreeType intc_get_user_intr_priority_degree_default(CoreIdType coreId, uint32 intno)
{
	return intno;
}
static bool intc_is_user_intr_masked(CoreIdType coreId, uint32 intno)
{
	Std_ReturnType err;
	uint32 base_addr = IC_ADDRESS(intno);
	uint16 data;

	err = device_get_data16(base_addr, &data);
	ASSERT(err == 0);

	//printf("%s():intno=%u base_addr=0x%x data=0x%x isset=%u\n", __FUNCTION__, intno, base_addr, data, IC_ISSET_INTMASK(data));
	return IC_ISSET_INTMASK(data);
}

static void intc_set_user_intr_reqflag(CoreIdType coreId, uint32 intno, bool flag)
{
	Std_ReturnType err;
	uint32 base_addr = IC_ADDRESS(intno);
	uint16 *datap;

	err = mpu_get_pointer(coreId, base_addr, (uint8**)&datap);
	ASSERT(err == 0);

	if (flag) {
		IC_SET_INTREQ(datap);
	}
	else {
		IC_CLR_INTREQ(datap);
	}

	return;
}
static bool intc_isset_user_intr_reqflag(CoreIdType coreId, uint32 intno)
{

	Std_ReturnType err;
	uint32 base_addr = IC_ADDRESS(intno);
	uint16 data;

	err = device_get_data16(base_addr, &data);
	ASSERT(err == 0);

	if (IC_ISSET_INTREQ(data)) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


static void intc_reset_registers(CoreIdType coreId)
{
	uint32 ino, imr_no;
	Std_ReturnType err;

	for (ino = 0; ino < TNUM_INT; ino++) {
		uint32 regaddr = IC_ADDRESS(ino);
		uint32 *pregaddr;
		err = mpu_get_pointer(coreId, regaddr, (uint8**)&pregaddr);
		ASSERT(err == 0);
		uint16* xxIcn = (uint16*)pregaddr;

		*xxIcn = 0x008F;
	}

	for (imr_no = 0; imr_no < INTC_IMR_NUM; imr_no++) {
		uint32 regaddr = IMR_ADDRESS(imr_no * INTC2_INTNO_OFFSET);
		uint32 *pregaddr;
		err = mpu_get_pointer(coreId, regaddr, (uint8**)&pregaddr);
		ASSERT(err == 0);
		uint32* imrp = (uint32*)pregaddr;

		*imrp = 0xFFFFFFFF;
	}
	return;
}

static uint8 intc2_conv_imraddr2imrno(uint32 imr_addr)
{
	return ((imr_addr -(INTC2_IMR_ADDR_START)) / IMR_REG_LEN_BYTE);
}

static Std_ReturnType intc_hook_update_imr(CoreIdType coreId, uint32 regaddr, uint8* data, uint8 len)
{
	uint8 imr_no;
	uint8 byte;
	uint8 bit;
	uint8 index;
	uint8* ic_datap;
	uint32 intno;
	Std_ReturnType err;

	TargetCoreType *cpu = intc_control[coreId].cpu;

	/* UM check */
	if (CPU_ISSET_UM(&(cpu->reg))) {
		cpu->current_code_exception.is_code_exception = TRUE;
		cpu->current_code_exception.exception_type = ExceptionId_PIE;
		return STD_E_PERM;
	}

	/* prepare */
	if ((INTC1_IMR_ADDR_START <= regaddr ) && (INTC1_IMR_ADDR_END >= regaddr)) {
		imr_no = 0U;
		index = INTC1_IMR_OFFSET_BIT(regaddr);
	}
	else {
		imr_no = intc2_conv_imraddr2imrno(regaddr);
		index = INTC2_IMR_OFFSET_BIT(regaddr, imr_no);
	}

	for (byte = 0U; byte < len; byte++) {
		for (bit = 0U; bit < INTC_ACCESS_REGLEN_8BIT; bit++) {
			intno = (imr_no * IMR_REG_LEN_BIT) + ((byte * INTC_ACCESS_REGLEN_8BIT) + bit) + index;
			err = mpu_get_pointer(coreId, (IC_ADDRESS(intno)), (uint8**)&ic_datap);
			if (err != STD_E_OK) {
				printf("%s():addr=0x%x\n", __FUNCTION__, regaddr);
				return err;
			}
			if ((data[byte] & (1U << bit)) != 0U) {
				IC_SET_INTMASK((uint16*)ic_datap);
			}
			else {
				IC_CLR_INTMASK((uint16*)ic_datap);
			}
		}
	}
	return STD_E_OK;
}

static Std_ReturnType intc_hook_update_ic_lowerbit(CoreIdType coreId, uint32 regaddr, uint16 data)
{
	uint32 intno;
	uint8* imr_datap;
	Std_ReturnType err;
	uint32 min = INTC1_IC_ADDR_START;
	uint32 max = IC_ADDRESS(INTC1_MAX_INTNO);

	/* IMR register update */
	/* get intno */
	if ((min <= regaddr) && (max >= regaddr)) {
		/* INTC_IC1 */
		intno = INTC1_ICADDR2INTNO(regaddr);
	}
	else {
		/* INTC_IC2 */
		intno = INTC2_ICADDR2INTNO(regaddr);
	}

	err = mpu_get_pointer(coreId, (IMR_ADDRESS(intno)), (uint8**)&imr_datap);
	if (err != STD_E_OK) {
		printf("%s():addr=0x%x intno=%u IMR_ADDR=0x%x\n", __FUNCTION__, regaddr, intno, IMR_ADDRESS(intno));
		return err;
	}
	if (IC_ISSET_INTMASK(data)) {
		*imr_datap |= (1U << (intno % IMR_REG_LEN_BIT));
	}
	else {
		*imr_datap &= ~(1U << (intno % IMR_REG_LEN_BIT));
	}

	return STD_E_OK;
}

static void intc_hook_update_ic_upperbit(CoreIdType coreId, uint32 regaddr, uint16 data)
{
	uint32 ino_regaddr;
	uint32 intno;
	ExceptionRequestEntryType *req;
	uint32 min = INTC1_IC_ADDR_START;
	uint32 max = IC_ADDRESS(INTC1_MAX_INTNO);

	/* req set */
	if (IC_ISSET_INTREQ(data)) {
		/* get intno */
		if ((regaddr >= min) && (regaddr <= max)) {
			/* INTC_IC1 */
			intno = INTC1_ICADDR2INTNO(regaddr);
		}
		else {
			/* INTC_IC2 */
			intno = INTC2_ICADDR2INTNO(regaddr);
		}
		intc_raise_intr(intno);
	}
	else {
		/* req clr */
		{
			ListEntry_Foreach(&intc_control[coreId].user_queue.pending, req) {
				ino_regaddr= IC_ADDRESS(req->data.intno);
				if ((ino_regaddr == regaddr)) {
					ListEntry_RemoveEntry(&intc_control[coreId].user_queue.pending, req);
					ListEntry_Free(&intc_control[coreId].user_queue.pending, req);
					return;
				}
			}
		}
		{
			ListEntry_Foreach(&intc_control[coreId].exception_queue[ExceptionId_UserIntr].pending, req) {
				ino_regaddr= IC_ADDRESS(req->data.intno);
				if ((ino_regaddr == regaddr) && req->data.status == ExceptionRequestStatus_RUNNABLE) {
					ListEntry_RemoveEntry(&intc_control[coreId].exception_queue[ExceptionId_UserIntr].pending, req);
					ListEntry_Free(&intc_control[coreId].exception_queue[ExceptionId_UserIntr].pending, req);
					return;
				}
			}
		}
	}
}

static Std_ReturnType intc_hook_update_ic_reg8(CoreIdType coreId, uint32 regaddr, uint8 data)
{
	uint16 tmp_data;
	uint32 tmp_regaddr;
	TargetCoreType *cpu = intc_control[coreId].cpu;

	/* UM check */
	if (CPU_ISSET_UM(&(cpu->reg))) {
		cpu->current_code_exception.is_code_exception = TRUE;
		cpu->current_code_exception.exception_type = ExceptionId_PIE;
		return STD_E_PERM;
	}

	if (IS_ICADDR_UPPER(regaddr)) {
		tmp_data = (((uint16)data) << INTC_ACCESS_REGLEN_8BIT);
		tmp_regaddr = IC_ADDR_UPPER2LOWER(regaddr);
		intc_hook_update_ic_upperbit(coreId, tmp_regaddr, tmp_data);
		return STD_E_OK;
	}
	else {
		return intc_hook_update_ic_lowerbit(coreId, regaddr, (uint16)data);
	}

}

static Std_ReturnType intc_hook_update_ic_reg16(CoreIdType coreId, uint32 regaddr, uint16* data)
{
	TargetCoreType *cpu = intc_control[coreId].cpu;

	/* UM check */
	if (CPU_ISSET_UM(&(cpu->reg))) {
		cpu->current_code_exception.is_code_exception = TRUE;
		cpu->current_code_exception.exception_type = ExceptionId_PIE;
		return STD_E_PERM;
	}

	intc_hook_update_ic_lowerbit(coreId, regaddr, *data);
	intc_hook_update_ic_upperbit(coreId, regaddr, *data);

	return STD_E_OK;
}

Std_ReturnType intc_hook_update_reg8(CoreIdType core_id, uint32 regaddr, uint8* data)
{
	Std_ReturnType err = STD_E_OK;

	/* hook update registers */
	if (((INTC1_IC_ADDR_START <= regaddr) && (INTC1_IC_ADDR_END >= regaddr)) ||
		((INTC2_IC_ADDR_START <= regaddr) && (INTC2_IC_ADDR_END >= regaddr))) {
		/* INTC_IC */
		err = intc_hook_update_ic_reg8(core_id, regaddr, *data);
	}
	else if (((INTC1_IMR_ADDR_START <= regaddr) && (INTC1_IMR_ADDR_END >= regaddr)) ||
			((INTC2_IMR_ADDR_START <= regaddr) && (INTC2_IMR_ADDR_END >= regaddr))) {
		/* INTC_IMR */
		err = intc_hook_update_imr(core_id, regaddr, data, INTC_ACCESS_REGLEN_1BYTE);
	}
	else {
		/* nop */
	}

	return err;
}

Std_ReturnType intc_hook_update_reg16(CoreIdType coreId, uint32 regaddr, uint16* data)
{
	Std_ReturnType err = STD_E_OK;

	/* hook update registers */
	if (((INTC1_IC_ADDR_START <= regaddr) && (INTC1_IC_ADDR_END >= regaddr)) ||
		((INTC2_IC_ADDR_START <= regaddr) && (INTC2_IC_ADDR_END >= regaddr))) {
		/* INTC_IC */
		if ((regaddr % INTC_ACCESS_REGLEN_2BYTE) != 0U) {
			return STD_E_INVALID;
		}
		err = intc_hook_update_ic_reg16(coreId, regaddr, data);
	}
	else if (((INTC1_IMR_ADDR_START <= regaddr) && (INTC1_IMR_ADDR_END >= regaddr)) ||
				((INTC2_IMR_ADDR_START <= regaddr) && (INTC2_IMR_ADDR_END >= regaddr))) {
		/* INTC_IMR */
		if ((regaddr % INTC_ACCESS_REGLEN_2BYTE) != 0U) {
			return STD_E_INVALID;
		}
		err = intc_hook_update_imr(coreId, regaddr, (uint8*)data, INTC_ACCESS_REGLEN_2BYTE);
	}
	else {
			/* nop */
	}

	return err;
}

Std_ReturnType intc_hook_update_reg32(CoreIdType coreId, uint32 regaddr, uint32* data)
{
	Std_ReturnType err = STD_E_OK;

	/* hook update registers */
	if (((INTC1_IMR_ADDR_START <= regaddr) && (INTC1_IMR_ADDR_END >= regaddr)) ||
			((INTC2_IMR_ADDR_START <= regaddr) && (INTC2_IMR_ADDR_END >= regaddr))) {
		/* INTC_IMR */
		if ((regaddr % INTC_ACCESS_REGLEN_4BYTE) != 0U) {
			return STD_E_INVALID;
		}
		err = intc_hook_update_imr(coreId, regaddr, (uint8*)data, INTC_ACCESS_REGLEN_4BYTE);
	}
	else {
		/* nop */
	}

	return err;
}

static HandlerAddressRefMethodType intc_get_addr_refmethod(CoreIdType coreId, uint32 exno, uint32 intno)
{
	TargetCoreType *cpu = intc_control[coreId].cpu;
	bool is_ebase = CPU_ISSET_EBV(&cpu->reg);
	bool is_rint;

	if (is_ebase) {
		is_rint = CPU_ISSET_EBASE_RINT(cpu);
	}
	else {
		is_rint = CPU_ISSET_RBASE_RINT(cpu);
	}

	if (is_rint) {
		if (is_ebase) {
			return HandlerAddressRefMethod_EBASE_RINT1;
		}
		else {
			return HandlerAddressRefMethod_RBASE_RINT1;
		}
	}

	if (!intc_is_devsetting_tableref(coreId, exno, intno)) {
		if (is_ebase) {
			return HandlerAddressRefMethod_EBASE_RINT0;
		}
		else {
			return HandlerAddressRefMethod_RBASE_RINT0;
		}
	}
	else {
		return HandlerAddressRefMethod_TABLE;
	}
}

static uint32 intc_get_handler_addr_rbase(CoreIdType coreId, uint32 exno, uint32 intno, bool is_rint)
{
	TargetCoreType *cpu = intc_control[coreId].cpu;
	uint32 base_addr = CPU_RBASE_ADDR(cpu)	+ exception_config[exno].handler.addr;
	//printf("%s():exno=%u intno=%u base_addr=0x%x\n", __FUNCTION__,  exno, intno, base_addr);
	if (exno == ExceptionId_UserIntr) {
		if (is_rint) {
			return base_addr;
		}
		else {
			return base_addr + (intc_get_user_intr_priority_degree(coreId, intno) * 16U);
		}
	}
	else {
		return base_addr;
	}
}
static uint32 intc_get_handler_addr_ebase(CoreIdType coreId, uint32 exno, uint32 intno, bool is_rint)
{
	TargetCoreType *cpu = intc_control[coreId].cpu;
	uint32 base_addr = CPU_EBASE_ADDR(cpu)	+ exception_config[exno].handler.addr;
	//printf("%s():exno=%u intno=%u base_addr=0x%x\n", __FUNCTION__,  exno, intno, base_addr);
	if (exno == ExceptionId_UserIntr) {
		if (is_rint) {
			return base_addr;
		}
		else {
			return base_addr + (intc_get_user_intr_priority_degree(coreId, intno) * 16U);
		}
	}
	else {
		if (exception_config[exno].handler.is_rbase == TRUE) {
			return CPU_RBASE_ADDR(cpu)	+ exception_config[exno].handler.addr;
		}
		else {
			return base_addr;
		}
	}
}
static uint32 intc_get_handler_addr_intbp(CoreIdType coreId, uint32 exno, uint32 intno)
{
	Std_ReturnType err;
	TargetCoreType *cpu = intc_control[coreId].cpu;
	uint32 base_addr = CPU_INTBP_ADDR(cpu) + (intno * 4U);
	uint32 data;
	//printf("%s():exno=%u intno=%u base_addr=0x%x\n", __FUNCTION__,  exno, intno, base_addr);

	ASSERT(exno == ExceptionId_UserIntr);

	err = mpu_get_data32(coreId, base_addr, &data);

	if (err != STD_E_OK) {
		cpu->fatal_error = TRUE;
		data = cpu->reg.pc;
		printf("Exec Error addr = 0x%x has not permission read handler table\n", base_addr);
	}

	return data;
}

static  uint32 intc_get_exception_handler_addr(CoreIdType coreId, uint32 exno, uint32 intno)
{
	uint32 addr = 0;
	HandlerAddressRefMethodType method = intc_get_addr_refmethod(coreId, exno, intno);

	//printf("%s():exno=%u intno=%u method=%d\n", __FUNCTION__, exno, intno, method);
	switch (method) {
	case HandlerAddressRefMethod_RBASE_RINT0:
		addr = intc_get_handler_addr_rbase(coreId, exno, intno, FALSE);
		break;
	case HandlerAddressRefMethod_RBASE_RINT1:
		addr = intc_get_handler_addr_rbase(coreId, exno, intno, TRUE);
		break;
	case HandlerAddressRefMethod_EBASE_RINT0:
		addr = intc_get_handler_addr_ebase(coreId, exno, intno, FALSE);
		break;
	case HandlerAddressRefMethod_EBASE_RINT1:
		addr = intc_get_handler_addr_ebase(coreId, exno, intno, TRUE);
		break;
	case HandlerAddressRefMethod_TABLE:
		addr = intc_get_handler_addr_intbp(coreId, exno, intno);
		break;
	default:
		ASSERT(0);
		break;
	}
	return addr;
}

#define INTC_GET_EXCEPTION_CODE_MPU_SET(result, offset)		(result) ? (1U << (offset)):(0U << (offset))
#define INTC_GET_EXCEPTION_CODE_MPU_FEIC_MS_OFF				24U
#define INTC_GET_EXCEPTION_CODE_MPU_FEIC_BL_OFF				23U
#define INTC_GET_EXCEPTION_CODE_MPU_FEIC_RMW_OFF			22U
#define INTC_GET_EXCEPTION_CODE_MPU_FEIC_SX_OFF				21U
#define INTC_GET_EXCEPTION_CODE_MPU_FEIC_SW_OFF				20U
#define INTC_GET_EXCEPTION_CODE_MPU_FEIC_SR_OFF				19U
#define INTC_GET_EXCEPTION_CODE_MPU_FEIC_UX_OFF				18U
#define INTC_GET_EXCEPTION_CODE_MPU_FEIC_UW_OFF				17U
#define INTC_GET_EXCEPTION_CODE_MPU_FEIC_UR_OFF				16U

#define INTC_GET_EXCEPTION_CODE_MPU_FEIC_MIP				0x90
#define INTC_GET_EXCEPTION_CODE_MPU_FEIC_MDP				0x91

static uint32 intc_get_exception_code_mpu(CoreIdType coreId)
{

	uint32 wdata32 = 0U;
	TargetCoreType *cpu = intc_control[coreId].cpu;

	wdata32 = INTC_GET_EXCEPTION_CODE_MPU_SET(cpu->mpu.excption_error_info.intc_error_info.is_ms_error , INTC_GET_EXCEPTION_CODE_MPU_FEIC_MS_OFF) |
			INTC_GET_EXCEPTION_CODE_MPU_SET(cpu->mpu.excption_error_info.intc_error_info.is_bl_error, INTC_GET_EXCEPTION_CODE_MPU_FEIC_BL_OFF) |
			INTC_GET_EXCEPTION_CODE_MPU_SET(cpu->mpu.excption_error_info.intc_error_info.is_rmw_error, INTC_GET_EXCEPTION_CODE_MPU_FEIC_RMW_OFF);

	if (cpu->mpu.excption_error_info.intc_error_info.is_sv_mode) {
		if (cpu->mpu.excption_error_info.intc_error_info.access_type == MPU_TARGET_ACCESS_READ) {
			wdata32 |= (1U << INTC_GET_EXCEPTION_CODE_MPU_FEIC_SR_OFF);
		}
		else {
			wdata32 |= (1U << INTC_GET_EXCEPTION_CODE_MPU_FEIC_SW_OFF);
		}
	}
	else{
		if (cpu->mpu.excption_error_info.intc_error_info.access_type == MPU_TARGET_ACCESS_READ) {
			wdata32 |= (1U << INTC_GET_EXCEPTION_CODE_MPU_FEIC_UR_OFF);
		}
		else {
			wdata32 |= (1U << INTC_GET_EXCEPTION_CODE_MPU_FEIC_UW_OFF);
		}
	}

	if (cpu->mpu.excption_error_info.intc_error_info.exception_error_code == TARGET_MPU_EXCEPTION_ERROR_CODE_MIP) {
		wdata32 |= INTC_GET_EXCEPTION_CODE_MPU_FEIC_MIP;
	}
	else {
		wdata32 |= INTC_GET_EXCEPTION_CODE_MPU_FEIC_MDP;
	}

	return wdata32;
}

static  uint32 intc_get_exception_code(CoreIdType coreId, uint32 exno, uint32 intno)
{
	if (exno == ExceptionId_UserIntr) {
		return exception_config[exno].handler.code + intno;
	}
	else if ((exno == ExceptionId_MIP) || (exno == ExceptionId_MDP)) {
		return intc_get_exception_code_mpu(coreId);
	}
	else {
		return exception_config[exno].handler.code;
	}
}

static IntcPriorityLevelType intc_get_exception_priority_level(CoreIdType coreId, uint32 exno)
{
	return exception_config[exno].priority.level;
}

static uint32 intc_get_exception_priority_degree(CoreIdType coreId, uint32 exno, uint32 intno)
{
	uint32 degree;
	if (exno == ExceptionId_UserIntr) {
		degree = ((uint32)intc_get_user_intr_priority_degree(coreId, intno)) << 16;
		degree |= ((intc_get_user_intr_priority_degree_default(coreId, intno) << 0));
	}
	else {
		degree = ((uint32)exception_config[exno].priority.degree_custom) << 16;
		degree |= (uint32)0xFFFF;
	}
	return degree;
}


static IntcPriorityDegreeType intc_get_current_user_intr_priority_degree(CoreIdType coreId)
{
	TargetCoreType *cpu = intc_control[coreId].cpu;
	uint32 *ispr = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_2, SYS_REG_ISPR);

	IntcPriorityDegreeType degree;
	for (degree = IntcPriorityDegree_0; degree < IntcPriorityDegree_Num; degree++) {
		if (((*ispr) & (1U << (uint32)degree)) != 0) {
			return degree;
		}
	}
	return IntcPriorityDegree_Num;
}

static void intc_set_current_user_intr_priority_degree(CoreIdType coreId, uint32 intno, bool flag)
{
	TargetCoreType *cpu = intc_control[coreId].cpu;
	if (CPU_ISSET_INTCFG_ISPC(cpu)) {
		return;
	}
	IntcPriorityDegreeType degree = intc_get_user_intr_priority_degree(coreId, intno);
	uint32 *ispr = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_2, SYS_REG_ISPR);
	if (flag == TRUE) {
		*ispr |= (1U << ((uint32)degree));
	}
	else if (!CPU_ISSET_EP(&cpu->reg)) {
		/*
		 *  PSW.EP = 0 の場合、 ISP15-0 でセット
		  * （ 1）されているビットのうち、最も高優先度（ 0 が高優先度側）の
		  * ビットがクリア（ 0）されます 注 2。
		 */
		IntcPriorityDegreeType degree;
		for (degree = IntcPriorityDegree_0; degree < IntcPriorityDegree_Num; degree++) {
			if (((*ispr) & (1U << (uint32)degree)) != 0) {
				*ispr &= ~(1U << ((uint32)degree));
				break;
			}
		}
	}

	return;
}

static IntcPriorityDegreeType intc_get_mask_priority_degree(CoreIdType coreId)
{
	TargetCoreType *cpu = intc_control[coreId].cpu;
	uint32 *pmr = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_2, SYS_REG_PMR);

	IntcPriorityDegreeType degree;
	for (degree = IntcPriorityDegree_0; degree < IntcPriorityDegree_Num; degree++) {
		if (((*pmr) & (1U << (uint32)degree)) != 0) {
			return degree;
		}
	}
	return IntcPriorityDegree_Num;
}

static void intc_set_user_intr_state_priority_degree_masked (CoreIdType coreId, bool flag)
{
	TargetCoreType *cpu = intc_control[coreId].cpu;
	uint32 *icsr = cpu_get_sysreg2(&cpu->reg.sys, SYS_GRP_2, SYS_REG_ICSR);

	if (flag == TRUE) {
		*icsr |= (uint32)0x0001;
	}
	else {
		*icsr &= (uint32)0xFFFFFFFE;
	}
	return;
}

static void intc_set_mask_priority(CoreIdType coreId, IntcPriorityDegreeType degree)
{
	//TODO maybe no use...
	return;
}


static void intc_set_user_intr_priority_degree(CoreIdType coreId, uint32 intno, IntcPriorityDegreeType degree)
{
	//TODO maybe no use...
	return;
}



