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
#include "mpu.h"
#include "cpu_config.h"
#include "mpu_config.h"


static uint8 memory_data_INTC1[MPU_ADDRESS_REGION_SIZE_INX_INTC1 * CPU_CONFIG_CORE_NUM];
static uint8 memory_data_INTC2[MPU_ADDRESS_REGION_SIZE_INX_INTC2 * CPU_CONFIG_CORE_NUM];
static uint8 memory_data_SERIAL[MPU_ADDRESS_REGION_SIZE_INX_SERIAL];
static uint8 memory_data_CLOCK[MPU_ADDRESS_REGION_SIZE_INX_SERIAL];
static uint8 memory_data_TIMER[MPU_ADDRESS_REGION_SIZE_INX_TIMER];
static uint8 memory_data_PH0[MPU_ADDRESS_REGION_SIZE_INX_PH0];
static uint8 memory_data_PH1[MPU_ADDRESS_REGION_SIZE_INX_PH1];


extern MpuAddressRegionOperationType	serial_memory_operation;
extern MpuAddressRegionOperationType	timer_memory_operation;
extern MpuAddressRegionOperationType	intc_memory_operation;
extern MpuAddressRegionOperationType	clock_memory_operation;
extern MpuAddressRegionOperationType	can_memory_operation;

MpuAddressMapType mpu_address_map = {
		.dynamic_map_num = 0,
		.dynamic_map = NULL,
		.map = {
				/*
				 * INDEX 2:DEVICE(割込みコントローラ(INTC1)
				 */
				{
						.type		= DEVICE,
						.is_malloc	= FALSE,
						.permission	= MPU_ADDRESS_REGION_PERM_ALL,
						.start		= 0xFFFEEA00,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_INTC1,
						.mask		= MPU_ADDRESS_REGION_MASK_PH,
						.data		= memory_data_INTC1,
						.ops		= &intc_memory_operation
				},
				/*
				 * INDEX 2:DEVICE(割込みコントローラ(INTC2))
				 */
				{
						.type		= DEVICE,
						.is_malloc	= FALSE,
						.permission	= MPU_ADDRESS_REGION_PERM_ALL,
						.start		= 0xFFFFB000,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_INTC2,
						.mask		= MPU_ADDRESS_REGION_MASK_PH,
						.data		= memory_data_INTC2,
						.ops		= &intc_memory_operation
				},
				/*
				 * TIMER
				 */
				{
						.type		= DEVICE,
						.is_malloc	= FALSE,
						.permission	= MPU_ADDRESS_REGION_PERM_ALL,
						.start		= 0xFFE50000,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_TIMER,
						.mask		= MPU_ADDRESS_REGION_MASK_PH,
						.data		= memory_data_TIMER,
						.ops		= &timer_memory_operation
				},

				/*
				 * SERIAL
				 */
				{
						.type		= DEVICE,
						.is_malloc	= FALSE,
						.permission	= MPU_ADDRESS_REGION_PERM_ALL,
						.start		= 0xFFCE2000,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_SERIAL,
						.mask		= MPU_ADDRESS_REGION_MASK_PH,
						.data		= memory_data_SERIAL,
						.ops		= &serial_memory_operation
				},

				/*
				 * CAN
				 */
#ifdef	OS_LINUX
				{
						.type		= DEVICE,
						.is_malloc	= FALSE,
						.permission	= MPU_ADDRESS_REGION_PERM_ALL,
						.start		= 0xFFD00000,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_CAN,
						.mask		= MPU_ADDRESS_REGION_MASK_PH,
						.data		= memory_data_CAN,
						.ops		= &can_memory_operation
				},
#endif

				/*
				 * CLKC
				 */
				{
						.type		= DEVICE,
						.is_malloc	= FALSE,
						.permission	= MPU_ADDRESS_REGION_PERM_ALL,
						.start		= 0xFFF81100,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_CLOCK,
						.mask		= MPU_ADDRESS_REGION_MASK_PH,
						.data		= memory_data_CLOCK,
						.ops		= &clock_memory_operation
				},

				/*
				 * INDEX :DEVICE(その他内蔵周辺I/O領域)
				 */
				{
						.type		= DEVICE,
						.is_malloc	= FALSE,
						.permission	= MPU_ADDRESS_REGION_PERM_ALL,
						.start		= 0xFFF80000,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_PH0,
						.mask		= MPU_ADDRESS_REGION_MASK_PH,
						.data		= memory_data_PH0,
						.ops		= &default_memory_operation
				},
				/*
				 * INDEX :DEVICE(プログラマブル周辺I/O領域1)
				 */
				{
						.type		= DEVICE,
						.is_malloc	= FALSE,
						.permission	= MPU_ADDRESS_REGION_PERM_ALL,
						.start		= 0xFFA00004,
						.size		= MPU_ADDRESS_REGION_SIZE_INX_PH1,
						.mask		= MPU_ADDRESS_REGION_MASK_PH,
						.data		= memory_data_PH1,
						.ops		= &default_memory_operation
				},

		}
};
