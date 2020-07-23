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
#ifndef _OP_CODEID_H_
#define _OP_CODEID_H_

typedef enum {
	OpCodeId_ABSF_S_F = 0,
	OpCodeId_ADD_1,
	OpCodeId_ADD_2,
	OpCodeId_ADDF_S_F,
	OpCodeId_ADDI_6,
	OpCodeId_ADF_11,
	OpCodeId_AND_1,
	OpCodeId_ANDI_6,
	OpCodeId_BINS_9,
	OpCodeId_BSH_12,
	OpCodeId_BSW_12,
	OpCodeId_Bcond_3,
	OpCodeId_Bcond_7,
	OpCodeId_CALLT_2,
	OpCodeId_CAXI_11,
	OpCodeId_CEILF_SL_F,
	OpCodeId_CEILF_SUL_F,
	OpCodeId_CEILF_SUW_F,
	OpCodeId_CEILF_SW_F,
	OpCodeId_CLR1_8,
	OpCodeId_CLR1_9,
	OpCodeId_CMOV_12,
	OpCodeId_CMOV_11,
	OpCodeId_CMOVF_S_F,
	OpCodeId_CMP_1,
	OpCodeId_CMP_2,
	OpCodeId_CMPF_S_F,
	OpCodeId_CTRET_10,
	OpCodeId_CVTF_DS_F,
	OpCodeId_CVTF_HS_F,
	OpCodeId_CVTF_LS_F,
	OpCodeId_CVTF_SD_F,
	OpCodeId_CVTF_SH_F,
	OpCodeId_CVTF_SL_F,
	OpCodeId_CVTF_SUL_F,
	OpCodeId_CVTF_SUW_F,
	OpCodeId_CVTF_SW_F,
	OpCodeId_CVTF_ULS_F,
	OpCodeId_CVTF_UWS_F,
	OpCodeId_CVTF_WS_F,
	OpCodeId_DBTRAP_1,
	OpCodeId_DI_10,
	OpCodeId_DISPOSE_13,
	OpCodeId_DIV_11,
	OpCodeId_DIVF_S_F,
	OpCodeId_DIVH_1,
	OpCodeId_DIVH_11,
	OpCodeId_DIVHU_11,
	OpCodeId_DIVQ_11,
	OpCodeId_DIVQU_11,
	OpCodeId_DIVU_11,
	OpCodeId_EI_10,
	OpCodeId_EIRET_10,
	OpCodeId_FERET_10,
	OpCodeId_FETRAP_1,
	OpCodeId_FLOORF_SL_F,
	OpCodeId_FLOORF_SUL_F,
	OpCodeId_FLOORF_SUW_F,
	OpCodeId_FLOORF_SW_F,
	OpCodeId_FMAF_S_F,
	OpCodeId_FMSF_S_F,
	OpCodeId_FNMAF_S_F,
	OpCodeId_FNMSF_S_F,
	OpCodeId_HALT_10,
	OpCodeId_HSH_12,
	OpCodeId_HSW_12,
	OpCodeId_JARL_6,
	OpCodeId_JARL_5,
	OpCodeId_JARL_11,
	OpCodeId_JMP_1,
	OpCodeId_JMP_6,
	OpCodeId_JR_6,
	OpCodeId_JR_5,
	OpCodeId_LDL_W_7,
	OpCodeId_LDSR_9,
	OpCodeId_LD_B_7,
	OpCodeId_LD_B_14,
	OpCodeId_LD_BU_7,
	OpCodeId_LD_BU_14,
	OpCodeId_LD_DW_14,
	OpCodeId_LD_H_7,
	OpCodeId_LD_H_14,
	OpCodeId_LD_HU_14,
	OpCodeId_LD_HU_7,
	OpCodeId_LD_W_7,
	OpCodeId_LD_W_14,
	OpCodeId_LOOP_7,
	OpCodeId_MAC_11,
	OpCodeId_MACU_11,
	OpCodeId_MAXF_S_F,
	OpCodeId_MINF_S_F,
	OpCodeId_MOV_1,
	OpCodeId_MOV_2,
	OpCodeId_MOV_6,
	OpCodeId_MOVEA_6,
	OpCodeId_MOVHI_6,
	OpCodeId_MUL_11,
	OpCodeId_MUL_12,
	OpCodeId_MULF_D_F,
	OpCodeId_MULF_S_F,
	OpCodeId_MULH_1,
	OpCodeId_MULH_2,
	OpCodeId_MULHI_6,
	OpCodeId_MULU_11,
	OpCodeId_MULU_12,
	OpCodeId_NEGF_S_F,
	OpCodeId_NOP_1,
	OpCodeId_NOT_1,
	OpCodeId_NOT1_8,
	OpCodeId_NOT1_9,
	OpCodeId_OR_1,
	OpCodeId_ORI_6,
	OpCodeId_POPSP_11,
	OpCodeId_PREPARE_13,
	OpCodeId_PUSHSP_11,
	OpCodeId_RECIPF_S_F,
	OpCodeId_RETI_10,
	OpCodeId_RIE_1,
	OpCodeId_RIE_10,
	OpCodeId_ROTL_7,
	OpCodeId_ROUNDF_SL_F,
	OpCodeId_ROUNDF_SUL_F,
	OpCodeId_ROUNDF_SUW_F,
	OpCodeId_ROUNDF_SW_F,
	OpCodeId_RSQRTF_S_F,
	OpCodeId_SAR_2,
	OpCodeId_SAR_9,
	OpCodeId_SAR_11,
	OpCodeId_SASF_9,
	OpCodeId_SATADD_1,
	OpCodeId_SATADD_2,
	OpCodeId_SATADD_11,
	OpCodeId_SATSUB_1,
	OpCodeId_SATSUB_11,
	OpCodeId_SATSUBI_6,
	OpCodeId_SATSUBR_1,
	OpCodeId_SBF_11,
	OpCodeId_SCH0L_9,
	OpCodeId_SCH0R_9,
	OpCodeId_SCH1L_9,
	OpCodeId_SCH1R_9,
	OpCodeId_SET1_8,
	OpCodeId_SET1_9,
	OpCodeId_SETF_9,
	OpCodeId_SHL_2,
	OpCodeId_SHL_9,
	OpCodeId_SHL_11,
	OpCodeId_SHR_2,
	OpCodeId_SHR_9,
	OpCodeId_SHR_11,
	OpCodeId_SLD_B_4,
	OpCodeId_SLD_BU_4,
	OpCodeId_SLD_H_4,
	OpCodeId_SLD_HU_4,
	OpCodeId_SLD_W_4,
	OpCodeId_SNOOZE_10,
	OpCodeId_SQRTF_S_F,
	OpCodeId_SST_B_4,
	OpCodeId_SST_H_4,
	OpCodeId_SST_W_4,
	OpCodeId_STC_W_7,
	OpCodeId_STSR_9,
	OpCodeId_ST_B_7,
	OpCodeId_ST_B_14,
	OpCodeId_ST_DW_14,
	OpCodeId_ST_H_7,
	OpCodeId_ST_H_14,
	OpCodeId_ST_W_7,
	OpCodeId_ST_W_14,
	OpCodeId_SUB_1,
	OpCodeId_SUBF_S_F,
	OpCodeId_SUBR_1,
	OpCodeId_SWITCH_1,
	OpCodeId_SXB_1,
	OpCodeId_SXH_1,
	OpCodeId_SYNCE_1,
	OpCodeId_SYNCI_1,
	OpCodeId_SYNCM_1,
	OpCodeId_SYNCP_1,
	OpCodeId_SYSCALL_10,
	OpCodeId_TRAP_10,
	OpCodeId_TRFSR_F,
	OpCodeId_TRNCF_SL_F,
	OpCodeId_TRNCF_SUL_F,
	OpCodeId_TRNCF_SUW_F,
	OpCodeId_TRNCF_SW_F,
	OpCodeId_TST_1,
	OpCodeId_TST1_8,
	OpCodeId_TST1_9,
	OpCodeId_XOR_1,
	OpCodeId_XORI_6,
	OpCodeId_ZXB_1,
	OpCodeId_ZXH_1,
	OpCodeId_Num,
} OpCodeId;

#endif /* _OP_CODEID_H_ */
