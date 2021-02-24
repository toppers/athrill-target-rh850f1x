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
#include "inc/timer.h"
#include "cpuemu_ops.h"
#include <stdio.h>
#include <limits.h>

typedef enum {
	TIMER_STATE_STOP,
	TIMER_STATE_READY,
	TIMER_STATE_RUNNING
} TimerStateType;

typedef enum {
	TIMER_MODE_FREERUN,
	TIMER_MODE_INTERVAL
} TimerModeType;

typedef struct {
	uint32 				cnt;
	uint32				reg_cnt;
	TimerStateType 		state;
	TimerModeType 		mode;
	uint32 				compare0;
	uint32 				compare0_intno;
	uint64 				start_clock;
	uint32 				fd;
} TimerDeviceType;

static Std_ReturnType timer_get_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data);
static Std_ReturnType timer_get_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data);
static Std_ReturnType timer_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data);
static Std_ReturnType timer_put_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data);
static Std_ReturnType timer_put_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data);
static Std_ReturnType timer_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data);
static Std_ReturnType timer_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data);

MpuAddressRegionOperationType	timer_memory_operation = {
		.get_data8 		= 	timer_get_data8,
		.get_data16		=	timer_get_data16,
		.get_data32		=	timer_get_data32,

		.put_data8 		= 	timer_put_data8,
		.put_data16		=	timer_put_data16,
		.put_data32		=	timer_put_data32,

		.get_pointer	= timer_get_pointer,
};
static TimerDeviceType TimerDevice[TAUJ_UNIT_NUM][TAUJ_CH_NUM];
static MpuAddressRegionType *timer_region;

void device_init_timer(MpuAddressRegionType *region)
{
	int i;
	int j;
	uint32 value = 50000;

	(void)cpuemu_get_devcfg_value("DEVICE_CONFIG_TIMER_FD", &value);
	//printf("timer value=%d\n", value);

	timer_region = region;

	for (i = 0; i < TAUJ_UNIT_NUM; i++) {
		for (j = 0; j < TAUJ_CH_NUM; j++) {
			TimerDevice[i][j].cnt = 0;
			TimerDevice[i][j].reg_cnt = 0xFFFFFFFF;
			TimerDevice[i][j].state = TIMER_STATE_STOP;
			TimerDevice[i][j].mode = TIMER_MODE_INTERVAL;
			TimerDevice[i][j].compare0 = 0;
			TimerDevice[i][j].fd = value;
			TimerDevice[i][j].start_clock = 0;
			TimerDevice[i][j].compare0_intno = TAUJ_INTNO(i, j);
		}
	}

	return;
}

static void device_timer_do_update(DeviceClockType *device, int n, int ch)
{
	TimerDeviceType *timer = &(TimerDevice[n][ch]);

	if (timer->state == TIMER_STATE_READY) {
		timer->state = TIMER_STATE_RUNNING;
		if ((timer->start_clock == 0U) || (timer->mode == TIMER_MODE_INTERVAL)) {
			timer->start_clock = device->clock;
		}
		//printf("device_timer_do_update:n=%d ch=%d compare=%d\n", n, ch, timer->compare0);
	}

	timer->cnt = (uint32)((device->clock - timer->start_clock) / (uint64)timer->fd);
	if (timer->state == TIMER_STATE_RUNNING) {
		if (timer->mode == TIMER_MODE_INTERVAL) {
			timer->reg_cnt = timer->compare0 - timer->cnt;
			//static int interval = 0;
			//if ((interval++) >= 0) { printf("cnt=%u\n", timer->cnt); interval = 0;}
			if (timer->cnt >= timer->compare0) {
				//printf("raise INT:n=%d ch=%d cnt=%d\n", n, ch, timer->cnt);
				device_raise_int(timer->compare0_intno);
				timer->state = TIMER_STATE_READY;
			}
		}
	}
	return;
}


static void device_timer_do_calc_min_interval(DeviceClockType *device, int n, int ch)
{
	TimerDeviceType *timer = &(TimerDevice[n][ch]);
	uint64 interval;

	if (device->can_skip_clock == FALSE) {
		return;
	}
	if (timer->cnt >= timer->compare0) {
		return;
	}

#ifndef CPUEMU_CLOCK_BUG_FIX
	interval = (timer->compare0 - timer->cnt) * timer->fd;
#else
	{
		uint64 cnt_1 = (timer->cnt / timer->fd);
		uint64 cnt_2 = (timer->cnt % timer->fd);
		interval = ((timer->compare0 - cnt_1) * timer->fd) + cnt_2;
	}
#endif

	if ((interval > 0) && (interval < device->min_intr_interval)) {
		device->min_intr_interval = interval;
		//printf("TIMER clock=%I64u min=%I64u\n", device->clock, device->min_intr_interval);
	}
	return;
}

#ifndef CPUEMU_CLOCK_BUG_FIX
#define INLINE_device_supply_clock_timer(dev_clock, n, ch)	\
do {	\
	if ((dev_clock->clock % TimerDevice[n][ch].fd) == 0) {	\
		device_timer_do_update(dev_clock, n, ch);	\
		device_timer_do_calc_min_interval(dev_clock, n, ch);	\
	}	\
	else {	\
		dev_clock->can_skip_clock = FALSE;	\
	}	\
} while(0)
#else
#define INLINE_device_supply_clock_timer(dev_clock, n, ch)	\
do {	\
	if ((dev_clock->clock % TimerDevice[n][ch].fd) == 0) {	\
		device_timer_do_update(dev_clock, n, ch);	\
	}	\
	else if ((dev_clock)->is_halt == TRUE) {	\
		device_timer_do_calc_min_interval(dev_clock, n, ch);	\
	}	\
} while(0)
#endif

void device_supply_clock_timer(DeviceClockType *dev_clock)
{
	INLINE_device_supply_clock_timer(dev_clock, 0, 0);
	INLINE_device_supply_clock_timer(dev_clock, 0, 1);
	//INLINE_device_supply_clock_timer(dev_clock, 1, 0);
	//INLINE_device_supply_clock_timer(dev_clock, 1, 1);
	return;
}


static Std_ReturnType timer_get_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data)
{
	uint32 off = (addr - region->start);
	*data = *((uint8*)(&region->data[off]));
	return STD_E_OK;
}
static Std_ReturnType timer_get_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data)
{
	uint32 off = (addr - region->start);
	*data = *((uint16*)(&region->data[off]));
	return STD_E_OK;
}

static Std_ReturnType timer_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data)
{
	uint32 off = (addr - region->start);
	uint8 n;
	uint8 ch;
	*data = *((uint32*)(&region->data[off]));

	for (n = 0; n < TAUJ_UNIT_NUM; n++) {
		for (ch = 0; ch < TAUJ_CH_NUM; ch++) {
			if (addr == (TAUJCNT(n, ch) & region->mask)) {
				*data = TimerDevice[n][ch].reg_cnt;
				//printf("%s():n=%d ch=%d cnt=%u\n", __FUNCTION__, n, ch, *data);
				return STD_E_OK;
			}
		}
	}
	return STD_E_OK;
}
static Std_ReturnType timer_put_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data)
{
	uint8 n;
	uint8 ch;
	uint32 off = (addr - region->start);
	*((uint8*)(&region->data[off])) = data;

		//printf("timer_put_data8 core=%d addr=0x%x data=0x%x\n", core_id, addr, data);
	for (n = 0; n < TAUJ_UNIT_NUM; n++) {
		if (addr == (TAUJTS(n) & region->mask)) {
			for (ch = 0; ch < TAUJ_CH_NUM; ch++) {
				if ((data & (1U << ch)) != 0) {
					TimerDevice[n][ch].state = TIMER_STATE_READY;
					//printf("timer[%d][%d] addr=0x%x ready\n", n, ch, addr);
					return STD_E_OK;
				}
			}
		}
		else if (addr == (TAUJTT(n) & region->mask)) {
			for (ch = 0; ch < TAUJ_CH_NUM; ch++) {
				if ((data & (1U << ch)) != 0) {
					TimerDevice[n][ch].state = TIMER_STATE_STOP;
					//printf("timer[%d][%d] addr=0x%x stop\n", n, ch, addr);
					return STD_E_OK;
				}
			}
		}
	}
	return STD_E_OK;
}
static Std_ReturnType timer_put_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data)
{
	uint32 off = (addr - region->start);
	*((uint16*)(&region->data[off])) = data;

	return STD_E_OK;
}
static Std_ReturnType timer_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data)
{
	uint8 n;
	uint8 ch;
	uint32 off = (addr - region->start);
	*((uint32*)(&region->data[off])) = data;

	for (n = 0; n < TAUJ_UNIT_NUM; n++) {
		for (ch = 0; ch < TAUJ_CH_NUM; ch++) {
			if (addr == (TAUJCDR(n, ch) & region->mask)) {
				TimerDevice[n][ch].compare0 = data;
				if (TimerDevice[n][ch].state == TIMER_STATE_RUNNING) {
					TimerDevice[n][ch].state = TIMER_STATE_READY;
				}
				//printf("%s() n=%d ch=%d compare0=%u\n", __FUNCTION__, n, ch, data);
				return STD_E_OK;
			}
		}
	}

	return STD_E_OK;
}
static Std_ReturnType timer_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data)
{
	uint32 off = (addr - region->start);
	*data = &region->data[off];
	return STD_E_OK;
}
