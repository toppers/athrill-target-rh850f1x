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
#include "can.h"
#include "device.h"
#include "std_errno.h"
#include "mpu_types.h"
#include "cpuemu_ops.h"
#include "mpu_ops.h"
#include "assert.h"
#include "mapping/can_register_mapping_reset.h"
#include "mapping/can_register_mapping_io.h"
#include "canbus/can_bus.h"
#include "can_rx.h"
#include "can_tx.h"
#include "can_interrupt.h"
#include <stdio.h>

static Std_ReturnType can_get_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data);
static Std_ReturnType can_get_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data);
static Std_ReturnType can_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data);
static Std_ReturnType can_put_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data);
static Std_ReturnType can_put_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data);
static Std_ReturnType can_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data);
static Std_ReturnType can_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data);

MpuAddressRegionOperationType	can_memory_operation = {
		.get_data8 		= 	can_get_data8,
		.get_data16		=	can_get_data16,
		.get_data32		=	can_get_data32,

		.put_data8 		= 	can_put_data8,
		.put_data16		=	can_put_data16,
		.put_data32		=	can_put_data32,

		.get_pointer	= can_get_pointer,
};

void can_reset(void)
{
	CanChannelIdType cid;

	can_register_reset_global(CanRegisterResetType_MCU_RESET);
	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		can_register_reset_channel(CanRegisterResetType_MCU_RESET, cid);
	}
	can_register_reset_fifo_status(CanRegisterResetType_MCU_RESET);
	can_register_reset_rx_rule(CanRegisterResetType_MCU_RESET);
	can_register_reset_rx_buffer(CanRegisterResetType_MCU_RESET);
	can_register_reset_rx_fifo(CanRegisterResetType_MCU_RESET);
	can_register_reset_tx_buffer(CanRegisterResetType_MCU_RESET);
	can_register_reset_tx_buffer_status(CanRegisterResetType_MCU_RESET);
	can_register_reset_txrx_fifo(CanRegisterResetType_MCU_RESET);

	can_register_reset_tx_queue(CanRegisterResetType_MCU_RESET);
	can_register_reset_tx_history(CanRegisterResetType_MCU_RESET);
}
static uint32 can_clock_fd = 50000;

void device_init_can(MpuAddressRegionType *region)
{
	char *asset_name = NULL;
	(void)cpuemu_get_devcfg_value("DEVICE_CONFIG_CAN_FD", &can_clock_fd);
	int ret = cpuemu_get_devcfg_string("DEBUG_FUNC_HAKO_ASSET_NAME", &asset_name);

	can_reset();
	if (ret == STD_E_NOENT) {
		can_rx_init(&can_bus_operation_impl_ros);
		can_tx_init(&can_bus_operation_impl_ros);
		can_bus_operation_impl_ros.init();
	}
	else {
		can_rx_init(&can_bus_operation_impl_hako);
		can_tx_init(&can_bus_operation_impl_hako);
		can_bus_operation_impl_hako.init();
	}

	return;
}

void device_supply_clock_can(DeviceClockType *dev_clock)
{
	if ((dev_clock->clock % can_clock_fd) == 0) {
		can_rx_proc();
		can_tx_proc();
		can_tx_done();
		can_intr_run();
	}
#ifdef CPUEMU_CLOCK_BUG_FIX
    else {
        uint64 next_clock = ((dev_clock->clock + (can_clock_fd -1)) / can_clock_fd) * can_clock_fd;
        uint64 interval = next_clock - dev_clock->clock;
        if (interval < dev_clock->min_intr_interval) {
            if (interval == 0) {
                interval = 1;
            }
            dev_clock->min_intr_interval = interval;
        }
	}
#endif
	return;
}

static Std_ReturnType can_get_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data)
{
	uint32 off = (addr - region->start);
	can_register_mapping_read_data(addr, 1U);
	*data = *((uint8*)(&region->data[off]));
	return STD_E_OK;
}
static Std_ReturnType can_get_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data)
{
	uint32 off = (addr - region->start);
	can_register_mapping_read_data(addr, 2U);
	*data = *((uint16*)(&region->data[off]));
	return STD_E_OK;
}
static Std_ReturnType can_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data)
{
	uint32 off = (addr - region->start);
	can_register_mapping_read_data(addr, 4U);
	*data = *((uint32*)(&region->data[off]));
	return STD_E_OK;
}
static Std_ReturnType can_put_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data)
{
	uint32 off = (addr - region->start);

	*((uint8*)(&region->data[off])) = data;
	can_register_mapping_write_data(addr, 1U);

	return STD_E_OK;
}
static Std_ReturnType can_put_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data)
{
	uint32 off = (addr - region->start);
	*((uint16*)(&region->data[off])) = data;
	can_register_mapping_write_data(addr, 2U);
	return STD_E_OK;
}
static Std_ReturnType can_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data)
{
	uint32 off = (addr - region->start);
	*((uint32*)(&region->data[off])) = data;
	can_register_mapping_write_data(addr, 4U);
	return STD_E_OK;
}
static Std_ReturnType can_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data)
{
	uint32 off = (addr - region->start);
	*data = &region->data[off];
	return STD_E_OK;
}

