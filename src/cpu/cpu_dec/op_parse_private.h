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
#ifndef _OP_PARSE_PRIVATE_H_
#define _OP_PARSE_PRIVATE_H_

#define BIT_MASK_6_5		( (1U <<  6U) | (1U <<  5U) )
#define BIT_MASK_22_21		( (1U << 22U) | (1U << 21U) )

#define BIT_MASK_10_7		( (1U << 10U) | (1U <<  9U) | (1U <<  8U) | (1U <<  7U) )
#define BIT_MASK_26_23		( (1U << 26U) | (1U << 25U) | (1U << 24U) | (1U << 23U) )

#define BIT_MASK_10_5		( (1U << 10U) | (1U << 9U) | (1U << 8U) | (1U << 7U) | (1U << 6U) | (1U << 5U) )
#define BIT_MASK_16			( (1U << 16U) )


/*
 * 31 ==> 15
 * 30 ==> 14
 * 29 ==> 13
 * 28 ==> 12
 * 27 ==> 11
 */
#define BIT_MASK_31_27		( (1U << 15U) | (1U << 14U) | (1U << 13U) | (1U << 12U) | (1U << 11U) )
#define BIT_MASK_15_11		( (1U << 15U) | (1U << 14U) | (1U << 13U) | (1U << 12U) | (1U << 11U) )
#define BIT_MASK_4_0		( (1U <<  4U) | (1U <<  3U) | (1U <<  2U) | (1U <<  1U) | (1U <<  0U) )



#define BIT_UP_10_5			( (1U << 10U) | (1U << 9U) | (1U << 8U) | (1U << 7U) | (1U << 6U) | (1U << 5U) )
#define BIT_UP_16			( (1U << 16U) )
#define BIT_DOWN_16			( (0U << 16U) )


#define GET_VALUE_BIT_10_7(data)	( ( (data) & BIT_MASK_10_7 ) >> 7U )
#define GET_VALUE_BIT_26_23(data)	( ( (data) & BIT_MASK_26_23 ) >> 23U )

#define GET_VALUE_BIT_6_5(data)		( ( (data) & BIT_MASK_6_5 ) >> 5U )
#define GET_VALUE_BIT_22_21(data)	( ( (data) & BIT_MASK_22_21 ) >> 21U )

#define GET_VALUE_BIT_31_27(data1)	( ( (data1) & BIT_MASK_31_27 ) >> 11U )
#define GET_VALUE_BIT_15_11(data)	( ( (data) & BIT_MASK_15_11 ) >> 11U )
#define GET_VALUE_BIT_4_0(data)		( ( (data) & BIT_MASK_4_0 ) >> 0U )

#define GET_REG1(data)	GET_VALUE_BIT_4_0(data)
#define GET_REG2(data)	GET_VALUE_BIT_15_11(data)
#define GET_REG3(data1)	GET_VALUE_BIT_31_27(data1)

#define OP_PARSE_CODE_TABLE_NUM		64U
#define OP_PARSE_TABLE_INDEX(base_id, sub_id)	( ((base_id) << 2U) | (sub_id) )

typedef struct {
	int (*parse) (uint16 code[OP_DECODE_MAX], OperationCodeType *optype);
} OpParserType;

extern OpParserType op_parse_extend_code_table[OP_PARSE_CODE_TABLE_NUM];
extern OpParserType op_parse_base_code_table[OP_PARSE_CODE_TABLE_NUM];

#endif /* _OP_PARSE_PRIVATE_H_ */
