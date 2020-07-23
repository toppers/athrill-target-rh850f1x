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
#ifndef _OP_FORMAT_H_
#define _OP_FORMAT_H_

#include "std_types.h"

#define OP_CODE_FORMAT_NUM	15
typedef enum {
	OP_CODE_FORMAT_1 = 0,
	OP_CODE_FORMAT_2,
	OP_CODE_FORMAT_3,
	OP_CODE_FORMAT_4,
	OP_CODE_FORMAT_5,
	OP_CODE_FORMAT_6,
	OP_CODE_FORMAT_7,
	OP_CODE_FORMAT_8,
	OP_CODE_FORMAT_9,
	OP_CODE_FORMAT_10,
	OP_CODE_FORMAT_11,
	OP_CODE_FORMAT_12,
	OP_CODE_FORMAT_13,
	OP_CODE_FORMAT_14,
	OP_CODE_FORMAT_F,
	OP_CODE_FORMAT_UNKNOWN,
} OpCodeFormatId;


/*
 * reg-reg命令形式
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint16 reg1;	/* 4-0 */
	uint16 reg2;	/* 15-11 */
} OpCodeFormatType1;
/*
 * imm-reg命令形式
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint32 imm;		/* 4-0 */
	uint16 reg2;	/* 15-11 */
} OpCodeFormatType2;
/*
 * 条件分岐命令形式
 */
typedef struct {
	uint16 opcode;	/* 10-7 */
	uint32 disp;	/* 15-11, 6-4 */
	uint16 cond;	/* 3-0 */
} OpCodeFormatType3;
/*
 * ロード／ストア命令16ビット形式
 */
typedef struct {
	uint16 opcode;	/* 10-7 */
	uint32 disp;	/* 6-1 */
	uint16 reg2;	/* 15-11 */
	uint32 gen;		/* 0 */
} OpCodeFormatType4_1;
typedef struct {
	uint16 opcode;	/* 10-4 */
	uint32 disp;	/* 3-0 */
	uint16 reg2;	/* 15-11 */
} OpCodeFormatType4_2;

/*
 * ジャンプ命令形式
 */
typedef struct {
	uint16 opcode;	/* 10-6 */
	uint32 disp;	/* 5-0, 31-17 */
	uint16 reg2;	/* 15-11 */
} OpCodeFormatType5;
/*
 * 3オペランド命令形式
 */
typedef struct {
	uint16 opcode;	/* 10-4 */
	uint32 imm;		/* 31-16 */
	uint16 reg1;	/* 4-0 */
	uint16 reg2;	/* 15-11 */
} OpCodeFormatType6;

/*
 * ロード／ストア命令32ビット形式
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint32 disp;	/* 31-17 */
	uint16 reg1;	/* 4-0 */
	uint16 reg2;	/* 15-11 */
	uint32 gen;		/* 16 */
} OpCodeFormatType7;

/*
 * ビット操作命令形式
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint16 sub;		/* 15-14 */
	uint16 bit;		/* 13-11 */
	uint16 reg1;	/* 4-0 */
	sint16 disp;	/* 31-16 */
} OpCodeFormatType8;
/*
 * 拡張命令形式1
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint16 sub;		/* 26-21 */
	uint16 gen;		/* 4-0 */
	uint16 reg2;	/* 15-11 */
	uint16 rfu2;		/* 31-27 */
	uint16 rfu1;		/* 20-17 */
} OpCodeFormatType9;
/*
 * 拡張命令形式2
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint16 gen1;	/* 15-13 */
	uint32 gen2;	/* 4-0 */
	uint16 sub;		/* 26-21 */
	uint16 rfu1;		/* 12-11*/
	uint16 rfu2;		/* 20-17 */
	uint16 rfu3;		/* 31-27 */
} OpCodeFormatType10;
/*
 * 拡張命令形式3
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint16 reg1;	/* 4-0 */
	uint16 reg2;	/* 15-11 */
	uint16 reg3;	/* 31-27 */
	uint16 sub1;	/* 26-21 */
	uint16 sub2;	/* 17 */
	uint16 rfu;		/* 20-18 */
} OpCodeFormatType11;
/*
 * 拡張命令形式4
 */
typedef struct {
	uint16 opcode;	/* 10-5 */
	uint16 reg2;	/* 15-11 */
	uint16 reg3;	/* 31-27 */
	uint32 imml;		/* 4-0 */
	uint32 immh;		/* 22-18 */
	uint16 sub1;	/* 26-23 */
	uint16 sub2;	/* 17 */
} OpCodeFormatType12;
/*
 * スタック操作命令形式1
 */
typedef struct {
	uint16 opcode;	/* 10-6 */
	uint32 imm;		/* 5-1 */
	uint8 list[32U];	/* 0, 31-21 */
	uint16 gen;		/* 20-16 */
	uint16 rfu;		/* 15-11 */
} OpCodeFormatType13;

/*
 * ロード／ストア命令48ビット形式（Format XIV）
 */
typedef struct {
	uint16 sub1;	/* 15-11 */
	uint16 opcode;	/* 10-5 */
	uint16 reg1;	/* 4-0 */
	uint16 reg3;	/* 31-27 */
	uint32 disp_low;	/* 26-20 */
	uint16 sub2;	/* 19-16 */
	uint32 disp_high;	/* 47-32 */
} OpCodeFormatType14;

/*
 * 浮動小数点演算命令(Format F:I)
 */
typedef struct {
	uint16 reg2;		/* 15-11 */
	uint16 opcode;		/* 10-5 */
	uint16 reg1;		/* 4-0 */
	uint16 reg3;		/* 31-27 */
	uint32 subopcode;	/* 26-16 */
} OpCodeFormatType_f;

#endif /* _OP_FORMAT_H_ */
