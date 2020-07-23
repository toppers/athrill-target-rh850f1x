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
#include "can_register_mapping.h"
#include "can_register_mapping_io_table.h"
#include "can_register.h"
#include <stdio.h>

#if 0 /* for test */
void debug_map_print(void)
{
	CanRegisterMappingType *map;
	int i;
	uint32 end_addr;


	printf("start, end,\n");
	for (i = 0; i < CAN_REGISTER_MAPPING_TABLE_NUM; i++) {
		map = &can_reigster_mapping_table[i];
		if (map->register_group_size != 0) {
			end_addr = map->start_address + map->register_group_size;
		}
		else {
			end_addr = map->start_address + map->register_unit_size;
		}
		printf("0x%x, 0x%x,\n", map->start_address - RSCANn_base, end_addr - RSCANn_base);
	}
}
#endif

static CanRegisterMappingType *search_map(uint32 address, uint32 size)
{
	CanRegisterMappingType *map;
	int i;
	uint32 end_addr;


	for (i = 0; i < CAN_REGISTER_MAPPING_TABLE_NUM; i++) {
		map = &can_reigster_mapping_table[i];
		if (map->register_group_size != 0) {
			end_addr = map->start_address + map->register_group_size;
		}
		else {
			end_addr = map->start_address + map->register_unit_size;
		}
		if (address < map->start_address) {
			continue;
		}
		if (address >= end_addr) {
			continue;
		}
		return map;
	}

	return NULL;
}

void can_register_mapping_write_data(uint32 address, uint32 size)
{
	CanRegisterIoArgType arg;
	CanRegisterMappingType *map = search_map(address, size);
	if (map == NULL) {
		printf("ERROR: can not WRITE HIT CAN address:0x%x %u\n", address, size);
		return;
	}
	arg.address = address;
	arg.size = size;
	map->io(CanRegisterIo_Write, &arg);
	return;
}

void can_register_mapping_read_data(uint32 address, uint32 size)
{
	CanRegisterIoArgType arg;
	CanRegisterMappingType *map = search_map(address, size);
	if (map == NULL) {
		printf("ERROR: can not READ HIT CAN address:0x%x %u\n", address, size);
		return;
	}
	arg.address = address;
	arg.size = size;
	map->io(CanRegisterIo_Read, &arg);
	return;
}
