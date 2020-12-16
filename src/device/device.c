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
#include "device.h"
#include "cpuemu_ops.h"
#include "device_ex_serial_ops.h"
#include "concrete_executor/target/dbg_target_serial.h"
#include <stdio.h>
#include "std_device_ops.h"
#include "athrill_device.h"
#ifdef SERIAL_FIFO_ENABLE
#include "serial_fifo.h"
#endif /* SERIAL_FIFO_ENABLE */

#ifdef CONFIG_STAT_PERF
ProfStatType cpuemu_dev_timer_prof;
ProfStatType cpuemu_dev_serial_prof;
ProfStatType cpuemu_dev_intr_prof;

#define CPUEMU_DEV_TIMER_PROF_START()	profstat_start(&cpuemu_dev_timer_prof)
#define CPUEMU_DEV_TIMER_PROF_END()		profstat_end(&cpuemu_dev_timer_prof)
#define CPUEMU_DEV_SERIAL_PROF_START()	profstat_start(&cpuemu_dev_serial_prof)
#define CPUEMU_DEV_SERIAL_PROF_END()		profstat_end(&cpuemu_dev_serial_prof)
#define CPUEMU_DEV_INTR_PROF_START()	profstat_start(&cpuemu_dev_intr_prof)
#define CPUEMU_DEV_INTR_PROF_END()		profstat_end(&cpuemu_dev_intr_prof)
#else
#define CPUEMU_DEV_TIMER_PROF_START()
#define CPUEMU_DEV_TIMER_PROF_END()
#define CPUEMU_DEV_SERIAL_PROF_START()
#define CPUEMU_DEV_SERIAL_PROF_END()
#define CPUEMU_DEV_INTR_PROF_START()
#define CPUEMU_DEV_INTR_PROF_END()
#endif /* CONFIG_STAT_PERF */

static DeviceExSerialOpType device_ex_serial_op = {
		.putchar = dbg_serial_putchar,
		.getchar = dbg_serial_getchar,
		.flush = NULL,
};
static DeviceExSerialOpType device_ex_serial_tcp_op = {
		.putchar = dbg_serial_putchar_tcp,
		.getchar = dbg_serial_getchar_tcp,
		.flush = NULL,
};

static void device_serial_init(CpuType *cpu, DeviceClockType *dev_clock)
{
	Std_ReturnType ret;
	uint32 value;


	device_init_serial(&mpu_address_map.map[MPU_ADDRESS_REGION_INX_SERIAL]);
	ret = cpuemu_get_devcfg_value("DEVICE_CONFIG_SERIAL_CH1_SERVER_PORTNO", &value);
	if (ret != STD_E_OK) {
		device_ex_serial_register_ops(1U, &device_ex_serial_op);
	}
	else {
		ret = dbg_serial_init_tcp(2U);
		ASSERT(ret == STD_E_OK);
		device_ex_serial_register_ops(1U, &device_ex_serial_tcp_op);
		printf("device_serial_init: server-portno=%u\n", value);
		ret = dbg_serial_open_tcp(1U, value);
		ASSERT(ret == STD_E_OK);
	}
	return;
}

static uint32 enable_mros_can = 0;
void device_init(CpuType *cpu, DeviceClockType *dev_clock)
{

	dev_clock->clock = 0;
	dev_clock->intclock = 0;
	dev_clock->min_intr_interval = DEVICE_CLOCK_MAX_INTERVAL;
	dev_clock->can_skip_clock = FALSE;

	device_init_clock(&mpu_address_map.map[MPU_ADDRESS_REGION_INX_CLOCK]);
	device_init_intc(cpu, &mpu_address_map.map[MPU_ADDRESS_REGION_INX_INTC1]);
	device_init_timer(&mpu_address_map.map[MPU_ADDRESS_REGION_INX_TIMER]);
	device_init_digital(NULL);

	device_serial_init(cpu, dev_clock);

	cpuemu_get_devcfg_value("DEBUG_FUNC_ENABLE_CAN", &enable_mros_can);
	if (enable_mros_can != 0) {
		device_init_can(&mpu_address_map.map[MPU_ADDRESS_REGION_INX_CAN]);
	}
#ifdef SERIAL_FIFO_ENABLE
	athrill_device_init_serial_fifo();
#endif /* SERIAL_FIFO_ENABLE */

#ifdef EXDEV_ENABLE
	device_init_athrill_exdev();
#endif /* EXDEV_ENABLE */
	return;
}

void device_supply_clock(DeviceClockType *dev_clock)
{
	dev_clock->min_intr_interval = DEVICE_CLOCK_MAX_INTERVAL;
	dev_clock->can_skip_clock = TRUE;

	CPUEMU_DEV_TIMER_PROF_START();
	device_supply_clock_digital(dev_clock);
	CPUEMU_DEV_TIMER_PROF_END();

	CPUEMU_DEV_TIMER_PROF_START();
	device_supply_clock_timer(dev_clock);
	CPUEMU_DEV_TIMER_PROF_END();

	CPUEMU_DEV_SERIAL_PROF_START();
	device_supply_clock_serial(dev_clock);
	CPUEMU_DEV_SERIAL_PROF_END();

	if (enable_mros_can != 0) {
		CPUEMU_DEV_SERIAL_PROF_START();
		device_supply_clock_can(dev_clock);
		CPUEMU_DEV_SERIAL_PROF_END();
	}
#ifdef SERIAL_FIFO_ENABLE
	CPUEMU_DEV_INTR_PROF_START();
	athrill_device_supply_clock_serial_fifo(dev_clock);
	CPUEMU_DEV_INTR_PROF_END();
#endif /* SERIAL_FIFO_ENABLE */

#ifdef EXDEV_ENABLE
	device_supply_clock_exdev(dev_clock);
#endif /* EXDEV_ENABLE */
	CPUEMU_DEV_INTR_PROF_START();
	device_supply_clock_intc(dev_clock);
	CPUEMU_DEV_INTR_PROF_END();
	return;
}

void device_raise_int(uint16 intno)
{
	intc_raise_intr(intno);
}


