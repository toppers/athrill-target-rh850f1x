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
#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "std_types.h"
#include "cpu.h"
#include "mpu_config.h"
#include "mpu.h"
#include "mpu_ops.h"
#include "std_device_ops.h"

/*
 * デバイス初期化関数
 */
extern void device_init_intc(CpuType *cpu, MpuAddressRegionType *region);
extern void device_init_timer(MpuAddressRegionType *region);
extern void device_init_serial(MpuAddressRegionType *region);
extern void device_init_can(MpuAddressRegionType *region);
extern void device_init_clock(MpuAddressRegionType *region);
extern void device_init_digital(MpuAddressRegionType *region);

/*
 * デバイスクロック供給
 */
extern void device_supply_clock_intc(DeviceClockType *dev_clock);
extern void device_supply_clock_timer(DeviceClockType *dev_clock);
extern void device_supply_clock_serial(DeviceClockType *dev_clock);
extern void device_supply_clock_can(DeviceClockType *dev_clock);
extern void device_supply_clock_digital(DeviceClockType *dev_clock);

/*
 * 割込み関数
 */
extern void device_raise_int(uint16 intno);

/*
 * メモリアクセス関数
 */
static inline Std_ReturnType device_get_data8(uint32 addr, uint8 *data)
{
	Std_ReturnType err;
	uint8 *datap;

	err = mpu_get_pointer(0U, addr, (uint8**)&datap);
	if (err != STD_E_OK) {
		return err;
	}
	*data = *datap;
	return STD_E_OK;
}

static inline Std_ReturnType device_get_data16(uint32 addr, uint16 *data)
{
	Std_ReturnType err;
	uint16 *datap;

	err = mpu_get_pointer(0U, addr, (uint8**)&datap);
	if (err != STD_E_OK) {
		return err;
	}
	*data = *datap;
	return STD_E_OK;
}

static inline Std_ReturnType device_put_data8(uint32 addr, uint8 data)
{
	Std_ReturnType err;
	uint8 *datap;

	err = mpu_get_pointer(0U, addr, (uint8**)&datap);
	if (err != STD_E_OK) {
		return err;
	}
	*datap = data;
	return STD_E_OK;
}

/*
 * 割込みコントローラAPI
 */
extern void intc_clr_fe(CoreIdType core_id);
extern void intc_clr_ei(CoreIdType core_id);

extern int intc_raise_exception(CoreIdType coreId, ExceptionIdType exno);

#endif /* _DEVICE_H_ */
