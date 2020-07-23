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
#ifndef _CPU_REGISTER_H_
#define _CPU_REGISTER_H_

#include "cpu_dec/op_dec.h"
#include "std_types.h"

#include "mpu/target/target_mpu_common.h"

#define CPU_GREG_NUM			(32U)

#define CPU_COMMON_SYSREG_NUM	(4U)
#define CPU_SYSBNK_NUM			(3U)

typedef enum {
	SYS_GRP_CPU = 0,
	SYS_GRP_1,
	SYS_GRP_2,
	SYS_GRP_3,
	SYS_GRP_4,
	SYS_GRP_5,
	SYS_GRP_6,
	SYS_GRP_7,
	SYS_GRP_NUM,
} SysGrpuType;

typedef enum {
	SYS_GRP_CPU_BNK_0 = 0,
	SYS_GRP_CPU_BNK_1,
	SYS_GRP_CPU_BNK_2,
	SYS_GRP_CPU_BNK_NUM,
} SysGrpCpuBnkType;

typedef enum {
	SYS_REG_EIPC = 0,
	SYS_REG_EIPSW,
	SYS_REG_FEPC,
	SYS_REG_FEPSW,
	SYS_SEL0_RESERVE_4,
	SYS_REG_PSW,
	SYS_REG_FPSR,
	SYS_REG_FPEPC,
	SYS_REG_FPST,
	SYS_REG_FPCC,
	SYS_REG_FPCFG,
	SYS_REG_FPEC,
	SYS_SEL0_RESERVE_12,
	SYS_REG_EIIC,
	SYS_REG_FEIC,
	SYS_SEL0_RESERVE_15,
	SYS_REG_CTPC,
	SYS_REG_CTPSW,
	SYS_SEL0_RESERVE_18,
	SYS_SEL0_RESERVE_19,
	SYS_REG_CTBP,
	SYS_SEL0_RESERVE_21,
	SYS_SEL0_RESERVE_22,
	SYS_SEL0_RESERVE_23,
	SYS_SEL0_RESERVE_24,
	SYS_SEL0_RESERVE_25,
	SYS_SEL0_RESERVE_26,
	SYS_SEL0_RESERVE_27,
	SYS_REG_EIWR,
	SYS_REG_FEWR,
	SYS_SEL0_RESERVE_30,
	SYS_SEL0_RESERVE_31,
	SYS_REG_NUM,
} SysSel0RegisterType;

#define SYS_REG_ECR	SYS_REG_EIIC
#define CPU_SYSREG_NUM SYS_REG_NUM

typedef enum {
	SYS_REG_MCFG0 = 0,
	SYS_SEL1_RESERVE1,
	SYS_REG_RBASE,
	SYS_REG_EBASE,
	SYS_REG_INTBP,
	SYS_REG_MCTL,
	SYS_REG_PID,
	SYS_SEL1_RESERVE7,
	SYS_SEL1_RESERVE8,
	SYS_SEL1_RESERVE9,
	SYS_SEL1_RESERVE10,
	SYS_REG_SCCFG,
	SYS_REG_SCBP,
	SYS_SEL1_RESERVE13,
	SYS_SEL1_RESERVE14,
	SYS_SEL1_RESERVE15,
	SYS_SEL1_RESERVE16,
	SYS_SEL1_RESERVE17,
	SYS_SEL1_RESERVE18,
	SYS_SEL1_RESERVE19,
	SYS_SEL1_RESERVE20,
	SYS_SEL1_RESERVE21,
	SYS_SEL1_RESERVE22,
	SYS_SEL1_RESERVE23,
	SYS_SEL1_RESERVE24,
	SYS_SEL1_RESERVE25,
	SYS_SEL1_RESERVE26,
	SYS_SEL1_RESERVE27,
	SYS_SEL1_RESERVE28,
	SYS_SEL1_RESERVE29,
	SYS_SEL1_RESERVE30,
	SYS_SEL1_RESERVE31,
	SYS_SEL1_REG_NUM,
} SysSel1RegisterType;


typedef enum {
	SYS_REG_HTCFG0 = 0,
	SYS_SEL2_RESERVE1,
	SYS_SEL2_RESERVE2,
	SYS_SEL2_RESERVE3,
	SYS_SEL2_RESERVE4,
	SYS_SEL2_RESERVE5,
	SYS_REG_MEA,
	SYS_REG_ASID,
	SYS_REG_MEI,
	SYS_SEL2_RESERVE9,
	SYS_REG_ISPR,
	SYS_REG_PMR,
	SYS_REG_ICSR,
	SYS_REG_INTCFG,
	SYS_SEL2_RESERVE14,
	SYS_SEL2_RESERVE15,
	SYS_SEL2_RESERVE16,
	SYS_SEL2_RESERVE17,
	SYS_SEL2_RESERVE18,
	SYS_SEL2_RESERVE19,
	SYS_SEL2_RESERVE20,
	SYS_SEL2_RESERVE21,
	SYS_SEL2_RESERVE22,
	SYS_SEL2_RESERVE23,
	SYS_SEL2_RESERVE24,
	SYS_SEL2_RESERVE25,
	SYS_SEL2_RESERVE26,
	SYS_SEL2_RESERVE27,
	SYS_SEL2_RESERVE28,
	SYS_SEL2_RESERVE29,
	SYS_SEL2_RESERVE30,
	SYS_SEL2_RESERVE31,
	SYS_SEL2_REG_NUM,
} SysSel2RegisterType;


typedef enum {
	SYS_SEL3_RESERVE0 = 0,
	SYS_SEL3_RESERVE1,
	SYS_SEL3_RESERVE2,
	SYS_SEL3_RESERVE3,
	SYS_SEL3_RESERVE4,
	SYS_SEL3_RESERVE5,
	SYS_SEL3_RESERVE6,
	SYS_SEL3_RESERVE7,
	SYS_SEL3_RESERVE8,
	SYS_SEL3_RESERVE9,
	SYS_SEL3_RESERVE10,
	SYS_SEL3_RESERVE11,
	SYS_SEL3_RESERVE12,
	SYS_SEL3_RESERVE13,
	SYS_SEL3_RESERVE14,
	SYS_SEL3_RESERVE15,
	SYS_SEL3_RESERVE16,
	SYS_SEL3_RESERVE17,
	SYS_SEL3_RESERVE18,
	SYS_SEL3_RESERVE19,
	SYS_SEL3_RESERVE20,
	SYS_SEL3_RESERVE21,
	SYS_SEL3_RESERVE22,
	SYS_SEL3_RESERVE23,
	SYS_SEL3_RESERVE24,
	SYS_SEL3_RESERVE25,
	SYS_SEL3_RESERVE26,
	SYS_SEL3_RESERVE27,
	SYS_SEL3_RESERVE28,
	SYS_SEL3_RESERVE29,
	SYS_SEL3_RESERVE30,
	SYS_SEL3_RESERVE31,
	SYS_SEL3_REG_NUM,
} SysSel3RegisterType;


typedef enum {
	SYS_SEL4_RESERVE0 = 0,
	SYS_SEL4_RESERVE1,
	SYS_SEL4_RESERVE2,
	SYS_SEL4_RESERVE3,
	SYS_SEL4_RESERVE4,
	SYS_SEL4_RESERVE5,
	SYS_SEL4_RESERVE6,
	SYS_SEL4_RESERVE7,
	SYS_SEL4_RESERVE8,
	SYS_SEL4_RESERVE9,
	SYS_SEL4_RESERVE10,
	SYS_SEL4_RESERVE11,
	SYS_SEL4_RESERVE12,
	SYS_SEL4_RESERVE13,
	SYS_SEL4_RESERVE14,
	SYS_SEL4_RESERVE15,
	SYS_SEL4_RESERVE16,
	SYS_SEL4_RESERVE17,
	SYS_SEL4_RESERVE18,
	SYS_SEL4_RESERVE19,
	SYS_SEL4_RESERVE20,
	SYS_SEL4_RESERVE21,
	SYS_SEL4_RESERVE22,
	SYS_SEL4_RESERVE23,
	SYS_SEL4_RESERVE24,
	SYS_SEL4_RESERVE25,
	SYS_SEL4_RESERVE26,
	SYS_SEL4_RESERVE27,
	SYS_SEL4_RESERVE28,
	SYS_SEL4_RESERVE29,
	SYS_SEL4_RESERVE30,
	SYS_SEL4_RESERVE31,
	SYS_SEL4_REG_NUM,
} SysSel4RegisterType;


typedef enum {
	SYS_REG_MPM = 0,
	SYS_REG_MPRC,
	SYS_SEL5_RESERVE2,
	SYS_SEL5_RESERVE3,
	SYS_REG_MPBRGN,
	SYS_REG_MPTRGN,
	SYS_SEL5_RESERVE6,
	SYS_SEL5_RESERVE7,
	SYS_REG_MCA,
	SYS_REG_MCS,
	SYS_REG_MCC,
	SYS_REG_MCR,
	SYS_SEL5_RESERVE12,
	SYS_SEL5_RESERVE13,
	SYS_SEL5_RESERVE14,
	SYS_SEL5_RESERVE15,
	SYS_SEL5_RESERVE16,
	SYS_SEL5_RESERVE17,
	SYS_SEL5_RESERVE18,
	SYS_SEL5_RESERVE19,
	SYS_SEL5_RESERVE20,
	SYS_SEL5_RESERVE21,
	SYS_SEL5_RESERVE22,
	SYS_SEL5_RESERVE23,
	SYS_SEL5_RESERVE24,
	SYS_SEL5_RESERVE25,
	SYS_SEL5_RESERVE26,
	SYS_SEL5_RESERVE27,
	SYS_SEL5_RESERVE28,
	SYS_SEL5_RESERVE29,
	SYS_SEL5_RESERVE30,
	SYS_SEL5_RESERVE31,
	SYS_SEL5_REG_NUM,
} SysSel5RegisterType;


typedef enum {
	SYS_REG_MPLA0 = 0,
	SYS_REG_MPUA0,
	SYS_REG_MPAT0,
	SYS_SEL6_RESERVE3,
	SYS_REG_MPLA1,
	SYS_REG_MPUA1,
	SYS_REG_MPAT1,
	SYS_SEL6_RESERVE7,
	SYS_REG_MPLA2,
	SYS_REG_MPUA2,
	SYS_REG_MPAT2,
	SYS_SEL6_RESERVE11,
	SYS_REG_MPLA3,
	SYS_REG_MPUA3,
	SYS_REG_MPAT3,
	SYS_SEL6_RESERVE15,
	SYS_REG_MPLA4,
	SYS_REG_MPUA4,
	SYS_REG_MPAT4,
	SYS_SEL6_RESERVE19,
	SYS_REG_MPLA5,
	SYS_REG_MPUA5,
	SYS_REG_MPAT5,
	SYS_SEL6_RESERVE23,
	SYS_REG_MPLA6,
	SYS_REG_MPUA6,
	SYS_REG_MPAT6,
	SYS_SEL6_RESERVE27,
	SYS_REG_MPLA7,
	SYS_REG_MPUA7,
	SYS_REG_MPAT7,
	SYS_SEL6_RESERVE31,
	SYS_SEL6_REG_NUM,
} SysSel6RegisterType;


typedef enum {
	SYS_REG_MPLA8 = 0,
	SYS_REG_MPUA8,
	SYS_REG_MPAT8,
	SYS_SEL7_RESERVE3,
	SYS_REG_MPLA9,
	SYS_REG_MPUA9,
	SYS_REG_MPAT9,
	SYS_SEL7_RESERVE7,
	SYS_REG_MPLA10,
	SYS_REG_MPUA10,
	SYS_REG_MPAT10,
	SYS_SEL7_RESERVE11,
	SYS_REG_MPLA11,
	SYS_REG_MPUA11,
	SYS_REG_MPAT11,
	SYS_SEL7_RESERVE15,
	SYS_REG_MPLA12,
	SYS_REG_MPUA12,
	SYS_REG_MPAT12,
	SYS_SEL7_RESERVE19,
	SYS_REG_MPLA13,
	SYS_REG_MPUA13,
	SYS_REG_MPAT13,
	SYS_SEL7_RESERVE23,
	SYS_REG_MPLA14,
	SYS_REG_MPUA14,
	SYS_REG_MPAT14,
	SYS_SEL7_RESERVE27,
	SYS_REG_MPLA15,
	SYS_REG_MPUA15,
	SYS_REG_MPAT15,
	SYS_SEL7_RESERVE31,
	SYS_SEL7_REG_NUM,
} SysSel7RegisterType;

typedef enum {
	CpuMemoryAccess_NONE = 0,
	CpuMemoryAccess_READ,
	CpuMemoryAccess_WRITE,
	CpuMemoryAccess_EXEC,
} CpuMemoryAccessType;

typedef enum {
	ExceptionId_RESET = 0,
	ExceptionId_FENMI,
	ExceptionId_SYSERR1,
	ExceptionId_FEINT,
	ExceptionId_FPI,
	ExceptionId_UserIntr,
	ExceptionId_MIP,
	ExceptionId_SYSERR2,
	ExceptionId_RIE,
	ExceptionId_UCPOP,
	ExceptionId_PIE,
	ExceptionId_MAE,
	ExceptionId_MDP,
	ExceptionId_FPP,
	ExceptionId_SYSCALL,
	ExceptionId_FETRAP,
	ExceptionId_TRAP0,
	ExceptionId_TRAP1,
	ExceptionId_Num,
} ExceptionIdType;

#define CPU_REG_UINT_MAX	0xFFFFFFFFULL
#define CPU_REG_PLUS_MAX	2147483647LL
#define CPU_REG_MINUS_MAX	-2147483648LL

#define CPU_REG_SP		(3)
#define CPU_REG_EP		(30)
#define CPU_REG_LP		(31)


typedef struct {
	uint32 r[SYS_REG_NUM];
} CpuSystemRegisterDataType;

typedef struct {
	uint32						current_grp;
	uint32						current_bnk;
	CpuSystemRegisterDataType	grp[SYS_GRP_NUM][CPU_SYSBNK_NUM];
} CpuSystemRegisterType;

typedef struct {
	uint32 pc;
	sint32 r[CPU_GREG_NUM];
	CpuSystemRegisterType	sys;
} CpuRegisterType;


static inline uint32 *cpu_get_sysreg(CpuSystemRegisterType *sys, uint32 regid) {
	return &sys->grp[sys->current_grp][sys->current_bnk].r[regid];
}

static inline uint32 *cpu_get_sysreg2(CpuSystemRegisterType *sys, uint32 selid, uint32 regid) {
	return &sys->grp[selid][sys->current_bnk].r[regid];
}

static inline CpuSystemRegisterDataType *sys_get_cpu_base(CpuRegisterType *reg) {
	return &reg->sys.grp[SYS_GRP_CPU][SYS_GRP_CPU_BNK_0];
}

typedef struct {
	bool				is_code_exception;
	ExceptionIdType		exception_type;
} CodeExceptionType;

typedef struct {
	uint32					real_elaps;
	CoreIdType				core_id;
	CpuRegisterType 		reg;
	bool					is_halt;
	uint16 					*current_code;
	OpDecodedCodeType		*decoded_code;
	CodeExceptionType		current_code_exception;
	bool					fatal_error;
	TargetMpuType			mpu;
} TargetCoreType;

extern uint32 cpu_get_pc(const TargetCoreType *core);
extern uint32 cpu_get_sp(const TargetCoreType *core);

extern void fpu_sync_sysreg(TargetCoreType *cpu, uint32 regid, uint32 selid);

#define SYS_ISSET_BIT(data32p, bitpos)                  ( (*(data32p)) & (1U << (bitpos)) )
#define SYS_SET_BIT(data32p, bitpos)    \
do {    \
        *(data32p) |= (1U << (bitpos)); \
} while (0)
#define SYS_CLR_BIT(data32p, bitpos)    \
do {    \
        *(data32p) &= ~(1U << (bitpos));        \
} while (0)

#define SYS_GET_DATA2(data32p, bitpos)                  ( ( (uint8)((*(data32p)) >> (bitpos)) ) & 0x0003 )
#define SYS_SET_DATA2(data32p, bitpos, data8)   \
do { \
        uint32 _org_data = *(data32p);  \
        uint32 _new_data = _org_data & ~((uint32)(0x0003) << (bitpos)); \
        _new_data |= ( ((uint32)(data8)) << (bitpos)); \
        *(data32p) = _new_data;  \
} while(0)

#define SYS_GET_DATA5(data32p, bitpos)                  ( ( (uint8)((*(data32p)) >> (bitpos)) ) & 0x001F )
#define SYS_SET_DATA5(data32p, bitpos, data8)   \
do { \
        uint32 _org_data = *(data32p);  \
        uint32 _new_data = _org_data & ~((uint32)(0x001F) << (bitpos)); \
        _new_data |= ( ((uint32)(data8)) << (bitpos)); \
        *(data32p) = _new_data;  \
} while(0)

#define SYS_GET_DATA6(data32p, bitpos)                  ( ( (uint8)((*(data32p)) >> (bitpos)) ) & 0x003F )
#define SYS_SET_DATA6(data32p, bitpos, data8)   \
do { \
        uint32 _org_data = *(data32p);  \
        uint32 _new_data = _org_data & ~((uint32)(0x003F) << (bitpos)); \
        _new_data |= ( ((uint32)(data8)) << (bitpos)); \
        *(data32p) = _new_data;  \
} while(0)

#define SYS_GET_DATA8(data32p, bitpos)                  ( (uint8)((*(data32p)) >> (bitpos)) )
#define SYS_SET_DATA8(data32p, bitpos, data8)   \
do { \
        uint32 _org_data = *(data32p);  \
        uint32 _new_data = _org_data & ~((uint32)(0xFFFF) << (bitpos)); \
        _new_data |= ( ((uint32)(data8)) << (bitpos)); \
        *(data32p) = _new_data;  \
} while(0)

#define SYS_REG_EIPC_GET()	(sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPC] & 0xFFFFFFFE)
#define SYS_REG_FEPC_GET()	(sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPC] & 0xFFFFFFFE)


#endif /* _CPU_REGISTER_H_ */
