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
#ifndef _OP_EXEC_H_
#define _OP_EXEC_H_

#include "target_cpu.h"
#include "cpu_dec/op_codeid.h"
#include "cpu_common/cpu_ops.h"
#include "cpu_exec/op_exec_ops.h"

#define OP_EXEC_TABLE_NUM		OpCodeId_Num

typedef struct {
	int clocks;
	int (*exec) (TargetCoreType *cpu);
} OpExecType;

extern OpExecType op_exec_table[OP_EXEC_TABLE_NUM];

//#define op_exec_add_1 op_exec_add_1
//#define op_exec_add_2 op_exec_add_2
#define op_exec_addi_6 op_exec_addi
#define op_exec_adf_11 op_exec_adf_11
#define op_exec_and_1 op_exec_and
#define op_exec_andi_6 op_exec_andi
#define op_exec_bins_9 op_exec_bins_9
#define op_exec_bsh_12 op_exec_bsh_12
#define op_exec_bsw_12 op_exec_bsw_12
#define op_exec_bcond_3 op_exec_bcond_3
#define op_exec_bcond_7 op_exec_bcond_7
#define op_exec_callt_2 NULL /* not supported */
#define op_exec_caxi_11 op_exec_caxi
//#define op_exec_clr1_8 op_exec_clr1_8
//#define op_exec_clr1_9 op_exec_clr1_9
//#define op_exec_cmov_12 op_exec_cmov_12
//#define op_exec_cmov_11 op_exec_cmov_11
//#define op_exec_cmp_1 op_exec_cmp_1
//#define op_exec_cmp_2 op_exec_cmp_2
#define op_exec_ctret_10 NULL /* not supported */
#define op_exec_dbtrap_1 NULL /* not supported */
#define op_exec_di_10 op_exec_diei
#define op_exec_dispose_13 op_exec_dispose
#define op_exec_div_11 op_exec_div
//#define op_exec_divh_1 op_exec_divh_1
//#define op_exec_divh_11 op_exec_divh_11
#define op_exec_divhu_11 op_exec_divhu
#define op_exec_divq_11 op_exec_div
#define op_exec_divqu_11 op_exec_divqu_11
#define op_exec_divu_11 op_exec_divu
#define op_exec_ei_10 op_exec_diei
#define op_exec_eiret_10 op_exec_eiret_10
#define op_exec_feret_10 op_exec_feret_10
#define op_exec_fetrap_1 op_exec_fetrap_1
#define op_exec_halt_10 op_exec_halt
#define op_exec_hsh_12 op_exec_hsh_12
#define op_exec_hsw_12 op_exec_hsw_12
#define op_exec_jarl_6 op_exec_jarl_6
#define op_exec_jarl_5 op_exec_jr
#define op_exec_jarl_11 op_exec_jarl_11
#define op_exec_jmp_1 op_exec_jmp
#define op_exec_jmp_6 op_exec_jmp_6
#define op_exec_jr_6 op_exec_jr_6
#define op_exec_jr_5 op_exec_jr
#define op_exec_ldl_w_7 op_exec_ldhw
#define op_exec_ldsr_9 op_exec_ldsr
#define op_exec_ld_b_7 op_exec_ldb
#define op_exec_ld_b_14 op_exec_ld_b_14
#define op_exec_ld_bu_7 op_exec_ldbu
#define op_exec_ld_bu_14 op_exec_ld_bu_14
#define op_exec_ld_dw_14 op_exec_ld_dw_14
#define op_exec_ld_h_7 op_exec_ldhw
#define op_exec_ld_h_14 op_exec_ld_h_14
#define op_exec_ld_hu_14 op_exec_ld_hu_14
#define op_exec_ld_hu_7 op_exec_ldhu
#define op_exec_ld_w_7 op_exec_ldhw
#define op_exec_ld_w_14 op_exec_ld_w_14
#define op_exec_loop_7 op_exec_loop_7
#define op_exec_mac_11 op_exec_mac_11
#define op_exec_macu_11 op_exec_macu_11
//#define op_exec_mov_1 op_exec_mov_1
//#define op_exec_mov_2 op_exec_mov_2
//#define op_exec_mov_6 op_exec_mov_6
#define op_exec_movea_6 op_exec_movea
#define op_exec_movhi_6 op_exec_movhi
#define op_exec_mul_11 op_exec_mul
//#define op_exec_mul_12 op_exec_mul_12
//#define op_exec_mulh_1 op_exec_mulh_1
//#define op_exec_mulh_2 op_exec_mulh_2
#define op_exec_mulhi_6 op_exec_mulhi
#define op_exec_mulu_11 op_exec_mulu
//#define op_exec_mulu_12 op_exec_mulu_12
#define op_exec_nop_1 op_exec_nop
#define op_exec_not_1 op_exec_not
//#define op_exec_not1_8 op_exec_not1_8
//#define op_exec_not1_9 op_exec_not1_9
#define op_exec_or_1 op_exec_or
#define op_exec_ori_6 op_exec_ori
#define op_exec_popsp_11 op_exec_popsp_11
#define op_exec_prepare_13 op_exec_prepare
#define op_exec_pushsp_11 op_exec_pushsp_11
#define op_exec_reti_10 op_exec_reti
#define op_exec_rie_1 NULL /* TODO */
#define op_exec_rie_10 NULL /* TODO */
#define op_exec_rotl_7 op_exec_rotl_7
//#define op_exec_sar_2 op_exec_sar_2
//#define op_exec_sar_9 op_exec_sar_9
#define op_exec_sar_11 op_exec_sar_11
#define op_exec_sasf_9 op_exec_sasf_9
//#define op_exec_satadd_1 op_exec_satadd_1
//#define op_exec_satadd_2 op_exec_satadd_2
#define op_exec_satadd_11 op_exec_satadd_11
//#define op_exec_satsub_1 op_exec_satsub_1
#define op_exec_satsub_11 op_exec_satsub_11
#define op_exec_satsubi_6 op_exec_satsubi
#define op_exec_satsubr_1 op_exec_satsubr_1
#define op_exec_sbf_11 op_exec_sbf_11
#define op_exec_sch0l_9 op_exec_sch0l_9
#define op_exec_sch0r_9 op_exec_sch0r_9
#define op_exec_sch1l_9 op_exec_sch1l_9
#define op_exec_sch1r_9 op_exec_sch1r_9
//#define op_exec_set1_8 op_exec_set1_8
//#define op_exec_set1_9 op_exec_set1_9
#define op_exec_setf_9 op_exec_setf
//#define op_exec_shl_2 op_exec_shl_2
//#define op_exec_shl_9 op_exec_shl_9
#define op_exec_shl_11 op_exec_shl_11
//#define op_exec_shr_2 op_exec_shr_2
//#define op_exec_shr_9 op_exec_shr_9
#define op_exec_shr_11 op_exec_shr_11
#define op_exec_sld_b_4 op_exec_sldb
#define op_exec_sld_bu_4 op_exec_sldbu
#define op_exec_sld_h_4 op_exec_sldh
#define op_exec_sld_hu_4 op_exec_sldhu
#define op_exec_sld_w_4 op_exec_sldw
#define op_exec_snooze_10 NULL /* not supported */
#define op_exec_sst_b_4 op_exec_sstb
#define op_exec_sst_h_4 op_exec_ssth
#define op_exec_sst_w_4 op_exec_sstw
#define op_exec_stc_w_7 op_exec_st_w_7
#define op_exec_stsr_9 op_exec_stsr
#define op_exec_st_b_7 op_exec_stb
#define op_exec_st_b_14 op_exec_st_b_14
#define op_exec_st_dw_14 op_exec_st_dw_14
#define op_exec_st_h_7 op_exec_sthw
#define op_exec_st_h_14 op_exec_st_h_14
#define op_exec_st_w_7 op_exec_sthw
#define op_exec_st_w_14 op_exec_st_w_14
#define op_exec_sub_1 op_exec_sub
#define op_exec_subr_1 op_exec_subr
#define op_exec_switch_1 op_exec_switch
#define op_exec_sxb_1 op_exec_sxb
#define op_exec_sxh_1 op_exec_sxh
#define op_exec_synce_1 op_exec_synce_1
#define op_exec_synci_1 op_exec_synci_1
#define op_exec_syncm_1 op_exec_syncm_1
#define op_exec_syncp_1 op_exec_syncp_1
#define op_exec_syscall_10 op_exec_syscall_10
#define op_exec_trap_10 op_exec_trap
#define op_exec_tst_1 op_exec_tst
//#define op_exec_tst1_8 op_exec_tst1_8
//#define op_exec_tst1_9 op_exec_tst1_9
#define op_exec_xor_1 op_exec_xor
#define op_exec_xori_6 op_exec_xori
#define op_exec_zxb_1 op_exec_zxb
#define op_exec_zxh_1 op_exec_zxh


#define op_exec_addf_s_F	op_exec_addf_s_F
#define op_exec_absf_s_F	op_exec_absf_s_F
#define op_exec_ceilf_sl_F	NULL /* not supported */
#define op_exec_ceilf_sul_F	NULL /* not supported */
#define op_exec_ceilf_suw_F	NULL /* not supported */
#define op_exec_ceilf_sw_F	NULL /* not supported */
#define op_exec_cmovf_s_F	op_exec_cmovf_s_F
#define op_exec_cmpf_s_F	op_exec_cmpf_s_F
#define op_exec_cvtf_hs_F	NULL /* not supported */
#define op_exec_cvtf_ls_F	op_exec_cvtf_ls_F
#define op_exec_cvtf_sh_F	NULL /* not supported */
#define op_exec_cvtf_sl_F	NULL /* not supported */
#define op_exec_cvtf_sul_F	NULL /* not supported */
#define op_exec_cvtf_suw_F	op_exec_cvtf_suw_F
#define op_exec_cvtf_sw_F	op_exec_cvtf_sw_F
#define op_exec_cvtf_uls_F	op_exec_cvtf_uls_F
#define op_exec_cvtf_uws_F	op_exec_cvtf_uws_F
#define op_exec_cvtf_ws_F	op_exec_cvtf_ws_F
#define op_exec_divf_s_F	op_exec_divf_s_F
#define op_exec_floorf_sl_F	NULL /* not supported */
#define op_exec_floorf_sul_F	op_exec_floorf_sul_F
#define op_exec_floorf_suw_F	NULL /* not supported */
#define op_exec_floorf_sw_F	NULL /* not supported */
#define op_exec_fmaf_s_F	op_exec_fmaf_s_F
#define op_exec_fmsf_s_F	op_exec_fmsf_s_F
#define op_exec_fnmaf_s_F	op_exec_fnmaf_s_F
#define op_exec_fnmsf_s_F	op_exec_fnmsf_s_F
#define op_exec_maxf_s_F	op_exec_maxf_s_F
#define op_exec_minf_s_F	op_exec_minf_s_F
#define op_exec_mulf_s_F	op_exec_mulf_s_F
#define op_exec_negf_s_F	op_exec_negf_s_F
#define op_exec_recipf_s_F	NULL /* not supported */
#define op_exec_roundf_sl_F	NULL /* not supported */
#define op_exec_roundf_sul_F	NULL /* not supported */
#define op_exec_roundf_suw_F	NULL /* not supported */
#define op_exec_roundf_sw_F	NULL /* not supported */
#define op_exec_rsqrtf_s_F	NULL /* not supported */
#define op_exec_sqrtf_s_F	NULL /* not supported */
#define op_exec_subf_s_F	op_exec_subf_s_F
#define op_exec_trfsr_F		op_exec_trfsr_F
#define op_exec_trncf_sl_F	NULL /* not supported */
#define op_exec_trncf_sul_F	op_exec_trncf_sul_F
#define op_exec_trncf_suw_F	op_exec_trncf_suw_F
#define op_exec_trncf_sw_F	op_exec_trncf_sw_F

#define op_exec_mulf_d_F op_exec_mulf_d_F
#define op_exec_cvtf_ds_F op_exec_cvtf_ds_F
#define op_exec_cvtf_sd_F op_exec_cvtf_sd_F


#endif /* _OP_EXEC_H_ */
