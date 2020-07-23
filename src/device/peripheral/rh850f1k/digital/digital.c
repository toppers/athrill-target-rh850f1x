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
#include "inc/digital.h"
#include "device.h"
#include "std_errno.h"
#include "mpu_types.h"
#include "mpu_ops.h"
#include <stdio.h>

static uint8 prev_digital_value;

void device_init_digital(MpuAddressRegionType *region)
{
	prev_digital_value = 0xffU;
	(void)mpu_put_data8(0U, DIGITAL_REGADD_DSW + 0, 0xff);
	(void)mpu_put_data8(0U, DIGITAL_REGADD_DSW + 1, 0xff);
	(void)mpu_put_data8(0U, DIGITAL_REGADD_DSW + 2, 0xff);
	(void)mpu_put_data8(0U, DIGITAL_REGADD_DSW + 2, 0xff);
	(void)mpu_put_data8(0U, DIGITAL_REGADD_PSW, 0x3);
	return;
}

void device_supply_clock_digital(DeviceClockType *dev_clock)
{
	Std_ReturnType err;
	uint8 *datap;
	uint8 data;

	err = mpu_get_pointer(0U, DIGITAL_REGADD_PSW, &datap);
	if (err != STD_E_OK) {
		printf("ERROR: device_supply_clock_digital() can not read DIGITAL_REGADD_PSW\n");
		return;
	}
	data = *datap;
	if ((!DIGITAL_IS_BITSET(prev_digital_value, DIGITAL_BITPOS_PSW1))
			& (DIGITAL_IS_BITSET(data, DIGITAL_BITPOS_PSW1))) {
		device_raise_int(DIGITAL_PSW1_INTNO);
	}
	else if ((!DIGITAL_IS_BITSET(prev_digital_value, DIGITAL_BITPOS_PSW2))
			& (DIGITAL_IS_BITSET(data, DIGITAL_BITPOS_PSW2))) {
		device_raise_int(DIGITAL_PSW2_INTNO);
	}
	prev_digital_value = data;
	return;
}


