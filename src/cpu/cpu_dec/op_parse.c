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
#include "cpu_dec/op_parse.h"
#include "cpu_dec/op_parse_private.h"
#include "cpu_dec/op_dec.h"

int op_parse(uint16 code[OP_DECODE_MAX], OpDecodedCodeType *decoded_code, OperationCodeType *optype)
{
	uint16 base_id;
	uint16 sub_id;
	uint32 value;
	int ret;

	value = (((uint32)code[1]) << 16U) | ((uint32)code[0]);

	if (((code[0] & BIT_MASK_10_5) == BIT_UP_10_5)
		&& ((value & BIT_MASK_16) == BIT_DOWN_16)) {
		/*
		 * Extend code
		 */
		base_id = GET_VALUE_BIT_26_23(value);
		sub_id  = GET_VALUE_BIT_22_21(value);
		ret = op_parse_extend_code_table[OP_PARSE_TABLE_INDEX(base_id, sub_id)].parse(code, optype);
	}
	else {
		/*
		 * Base code
		 */
		base_id = GET_VALUE_BIT_10_7(code[0]);
		sub_id  = GET_VALUE_BIT_6_5(code[0]);
		ret = op_parse_base_code_table[OP_PARSE_TABLE_INDEX(base_id, sub_id)].parse(code, optype);
	}
	/*
	 * decode
	 */
	if (ret == 0) {
		ret = OpDecoder[optype->format_id].decode(code, decoded_code);
	}
	return ret;
}
