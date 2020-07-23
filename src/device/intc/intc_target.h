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
#ifndef _INTC_TARGET_H_
#define _INTC_TARGET_H_

#include "std_types.h"
#include "std_errno.h"
#include "cpu.h"
#include "intc_config.h"

typedef struct {
	uint32 level;
	uint32 degree_custom;
} ExceptionPriorityType;

typedef struct {
	uint32 	addr;
	uint32 	code;
	bool	is_rbase;
} ExceptionHandlerType;

typedef enum {
	ExceptionLevel_FE = 0,
	ExceptionLevel_EI,
	ExceptionLevel_RESET,
} ExceptionLevelType;

typedef enum {
	ExceptionForm_ABORT = 0,
	ExceptionForm_REEXEC,
	ExceptionForm_COMPLETE,
} ExceptionFormType;

typedef enum {
	ExceptionRaise_SYSTEM = 0,
	ExceptionRaise_USER,
} ExceptionRaiseType;

typedef struct {
	bool ID;
	bool NP;
} ExceptionCanRaiseConditionType;

typedef struct {
	IntcPriorityLevelType (*get_exception_priority_level) (CoreIdType coreId, uint32 exno);
	uint32 (*get_exception_priority_degree) (CoreIdType coreId, uint32 exno, uint32 intno);
	IntcPriorityDegreeType (*get_current_user_intr_priority_degree) (CoreIdType coreId);
	void (*set_current_user_intr_priority_degree) (CoreIdType coreId, uint32 intno, bool flag);

	IntcPriorityDegreeType (*get_mask_priority_degree) (CoreIdType coreId);
	void (*set_mask_priority_degree) (CoreIdType coreId, IntcPriorityDegreeType degree);

	void (*set_user_intr_reqflag)(CoreIdType coreId, uint32 intno, bool flag);
	bool (*isset_user_intr_reqflag)(CoreIdType coreId, uint32 intno);
	IntcPriorityDegreeType (*get_user_intr_priority_degree)(CoreIdType coreId, uint32 intno);
	void (*set_user_intr_priority_degree)(CoreIdType coreId, uint32 intno, IntcPriorityDegreeType degree);

	bool (*is_user_intr_masked) (CoreIdType coreId, uint32 intno);

	void (*set_user_intr_state_priority_degree_masked) (CoreIdType coreId, bool flag);

	uint32 (*get_exception_handler_addr) (CoreIdType core, uint32 exno, uint32 intno);
	uint32 (*get_exception_code) (CoreIdType core, uint32 exno, uint32 intno);

	void (*reset_registers) (CoreIdType coreId);
} IntcControlRegisterOperationType;

extern IntcControlRegisterOperationType intc_control_register_op;

extern Std_ReturnType intc_hook_update_reg8(CoreIdType core_id, uint32 regaddr, uint8* data);
extern Std_ReturnType intc_hook_update_reg16(CoreIdType core_id, uint32 regaddr, uint16* data);
extern Std_ReturnType intc_hook_update_reg32(CoreIdType core_id, uint32 regaddr, uint32* data);
extern MpuAddressRegionType *intc_region;

typedef enum {
	ExceptionUpdatePswOp_CLR = 0,
	ExceptionUpdatePswOp_SET,
	ExceptionUpdatePswOp_SAVE,
} ExceptionUpdatePswOpType;

typedef struct {
	ExceptionUpdatePswOpType UM;
	ExceptionUpdatePswOpType ID;
	ExceptionUpdatePswOpType NP;
	ExceptionUpdatePswOpType EP;
	ExceptionUpdatePswOpType EBV;
} ExceptionUpdatePswInfoType;

typedef struct {
	const char* 					name;
	ExceptionPriorityType			priority;
	ExceptionHandlerType			handler;
	ExceptionLevelType				level;
	//ExceptionFormType				form;
	//ExceptionRaiseType				raise_type;
	ExceptionCanRaiseConditionType	cond;
	ExceptionUpdatePswInfoType		info;
} ExceptionType;

extern ExceptionType exception_config[ExceptionId_Num];

extern Std_ReturnType intc_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data);

#endif /* _INTC_TARGET_H_ */
