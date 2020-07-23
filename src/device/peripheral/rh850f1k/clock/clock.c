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
#include "inc/clock.h"
#include "device.h"
#include "std_errno.h"
#include "mpu_types.h"
#include "cpuemu_ops.h"
#include <stdio.h>

static Std_ReturnType clock_get_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data);
static Std_ReturnType clock_get_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data);
static Std_ReturnType clock_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data);
static Std_ReturnType clock_put_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data);
static Std_ReturnType clock_put_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data);
static Std_ReturnType clock_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data);
static Std_ReturnType clock_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data);

MpuAddressRegionOperationType	clock_memory_operation = {
		.get_data8 		= 	clock_get_data8,
		.get_data16		=	clock_get_data16,
		.get_data32		=	clock_get_data32,

		.put_data8 		= 	clock_put_data8,
		.put_data16		=	clock_put_data16,
		.put_data32		=	clock_put_data32,

		.get_pointer	= clock_get_pointer,
};

static MpuAddressRegionType *clock_region;

void device_init_clock(MpuAddressRegionType *region)
{
	clock_region = region;
	return;
}

static void clock_hook_update_mosce(MpuAddressRegionType *region, CoreIdType core_id, uint32 data)
{
	uint8* moscs_datap;

	(void)clock_get_pointer(region, core_id, MOSCS, (uint8**)&moscs_datap);
	if (MOSCE_ISSTOP(data)) {
		MOSCS_CLR_ACT((uint32*)moscs_datap);
	}

	if (MOSCE_ISSTART(data)) {
		MOSCS_SET_ACT((uint32*)moscs_datap);
	}

	return;
}

static Std_ReturnType clock_get_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data)
{
	uint32 off = (addr - region->start);
	*data = *((uint8*)(&region->data[off]));
	return STD_E_OK;
}
static Std_ReturnType clock_get_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data)
{
	uint32 off = (addr - region->start);
	*data = *((uint16*)(&region->data[off]));
	return STD_E_OK;
}
static Std_ReturnType clock_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data)
{
	uint32 off = (addr - region->start);
	*data = *((uint32*)(&region->data[off]));
	return STD_E_OK;
}
static Std_ReturnType clock_put_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data)
{
	uint32 off = (addr - region->start);
	*((uint8*)(&region->data[off])) = data;
	return STD_E_OK;
}
static Std_ReturnType clock_put_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data)
{
	uint32 off = (addr - region->start);
	*((uint16*)(&region->data[off])) = data;
	return STD_E_OK;
}
static Std_ReturnType clock_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data)
{
	uint8* datap;
	uint32 off = (addr - region->start);
	*((uint32*)(&region->data[off])) = data;

	if (addr ==  MOSCE) {
		clock_hook_update_mosce(region, core_id, data);
	}
	else if (addr == CKSC_PPLLCLKS_CTL) {
		(void)clock_get_pointer(region, core_id, CKSC_PPLLCLKS_ACT, (uint8**)&datap);
		*datap = data;
	}
	else if (addr == CKSC_CPUCLKS_CTL) {
		(void)clock_get_pointer(region, core_id, CKSC_CPUCLKS_ACT, (uint8**)&datap);
		*datap = data;
	}
	else if (addr == CKSC_CPUCLKD_CTL) {
		(void)clock_get_pointer(region, core_id, CKSC_CPUCLKD_ACT, (uint8**)&datap);
		*datap = data;
	}
	else if (addr == CKSC_ATAUJS_CTL) {
		(void)clock_get_pointer(region, core_id, CKSC_ATAUJS_ACT, (uint8**)&datap);
		*datap = data;
	}
	else if (addr == CKSC_ATAUJD_CTL) {
		(void)clock_get_pointer(region, core_id, CKSC_ATAUJD_ACT, (uint8**)&datap);
		*datap = data;
	}
	else if (addr == CKSC_ILINS_CTL) {
		(void)clock_get_pointer(region, core_id, CKSC_ILINS_ACT, (uint8**)&datap);
		*datap = data;
	}
	else if (addr == CKSC_ILIND_CTL) {
		(void)clock_get_pointer(region, core_id, CKSC_ILIND_ACT, (uint8**)&datap);
		*datap = data;
	}
	else {
		/* nop */
	}

	return STD_E_OK;
}
static Std_ReturnType clock_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data)
{
	uint32 off = (addr - region->start);
	*data = &region->data[off];
	return STD_E_OK;
}
