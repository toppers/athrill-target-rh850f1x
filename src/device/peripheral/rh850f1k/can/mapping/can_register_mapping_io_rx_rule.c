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
#include "can_register_mapping_io.h"
#include "can_register.h"
#include "can_register_mapping_reset.h"
#include "can_rule.h"

void can_register_reset_rx_rule(CanRegisterResetType type)
{
	CanChannelIdType cid;
	uint32 pid;
	uint32 rid;

	if (type == CanRegisterResetType_MCU_RESET) {
		can_rx_rule_table.ctrl.enable_write = FALSE;
		can_rx_rule_table.ctrl.page_number = 0U;
		for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
			can_rx_number.channel_rx_rule_num[cid]  = 0U;
		}

		for (pid = 0; pid < CAN_RX_RULE_TABLE_PAGE_NUM; pid++) {
			for (rid = 0; rid < CAN_RX_PAGE_RULE_NUM; rid++) {
				can_rx_rule_table.page[pid].rule[rid].id.ide = 0U;
				can_rx_rule_table.page[pid].rule[rid].id.rtr = 0U;
				can_rx_rule_table.page[pid].rule[rid].id.canid = 0U;
				can_rx_rule_table.page[pid].rule[rid].mask.ide = 0U;
				can_rx_rule_table.page[pid].rule[rid].mask.rtr = 0U;
				can_rx_rule_table.page[pid].rule[rid].mask.canid = 0U;
				can_rx_rule_table.page[pid].rule[rid].filter_buffer.dlc = 0U;
				can_rx_rule_table.page[pid].rule[rid].filter_buffer.label = 0U;
				can_rx_rule_table.page[pid].rule[rid].filter_buffer.enable_rxbuffer = 0U;
				can_rx_rule_table.page[pid].rule[rid].filter_buffer.rxbuffer_number = 0U;
				can_rx_rule_table.page[pid].rule[rid].filter_fifo.txrx_fifo = 0U;
				can_rx_rule_table.page[pid].rule[rid].filter_fifo.rx_fifo = 0U;
			}
		}
	}
	else if (type == CanRegisterResetType_GLOBAL_RESET) {
		// nothing
	}
	else if (type == CanRegisterResetType_CHANNEL_RESET) {
		// nothing
	}

}

void can_register_mapping_io_RSCANnGAFLECTR(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;

	if (io_type == CanRegisterIo_Read) {
		// nothing to do
		// モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
	}
	else { /* CanRegisterIo_Write */
		//モデルへの同期処理
		can_register_read_data32(CAN_REG_RSCANnGAFLECTR, &data);
		//AFLDAE
		CAN_REG_UPDATE_MODEL_FLAG(data, 8U, can_rx_rule_table.ctrl.enable_write);
		//AFLPN [4:0]
		can_rx_rule_table.ctrl.page_number = CAN_REG_GET_MASK_DATA(data, 0x01FU);
	}
}
void can_register_mapping_io_RSCANnGAFLCFG0(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint8 *array = (uint8*)&data;

	if (io_type == CanRegisterIo_Read) {
		// nothing to do
		// モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
	}
	else { /* CanRegisterIo_Write */
		//モデルへの同期処理
		can_register_read_data32(CAN_REG_RSCANnGAFLCFG0, &data);
		can_rx_number.channel_rx_rule_num[0] = array[3];
		can_rx_number.channel_rx_rule_num[1] = array[2];
		can_rx_number.channel_rx_rule_num[2] = array[1];
		can_rx_number.channel_rx_rule_num[3] = array[0];
		can_rx_update_rule_table();
	}
}
void can_register_mapping_io_RSCANnGAFLCFG1(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint8 *array = (uint8*)&data;

	if (io_type == CanRegisterIo_Read) {
		// nothing to do
		// モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
	}
	else { /* CanRegisterIo_Write */
		//モデルへの同期処理
		can_register_read_data32(CAN_REG_RSCANnGAFLCFG1, &data);
		can_rx_number.channel_rx_rule_num[4] = array[3];
		can_rx_number.channel_rx_rule_num[5] = array[2];
		can_rx_update_rule_table();
	}
}

void can_register_mapping_io_RSCANnGAFLIDj(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnGAFLIDj(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		// nothing to do
		// モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
	}
	else { /* CanRegisterIo_Write */
		//モデルへの同期処理
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		// 31 GAFLIDE
		CAN_REG_UPDATE_MODEL_FLAG(data, 31, can_rx_rule_table.page[can_rx_rule_table.ctrl.page_number].rule[index].id.ide);
		// 30 GAFLRTR
		CAN_REG_UPDATE_MODEL_FLAG(data, 30, can_rx_rule_table.page[can_rx_rule_table.ctrl.page_number].rule[index].id.rtr);
		// 28 ～ 0 GAFLID[28:0]
		can_rx_rule_table.page[can_rx_rule_table.ctrl.page_number].rule[index].id.canid = CAN_REG_GET_MASK_DATA(data, 0x1FFFFFFF);
	}
}
void can_register_mapping_io_RSCANnGAFLMj(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnGAFLMj(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		// nothing to do
		// モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
	}
	else { /* CanRegisterIo_Write */
		//モデルへの同期処理
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		// 31 GAFLIDEM
		CAN_REG_UPDATE_MODEL_FLAG(data, 31, can_rx_rule_table.page[can_rx_rule_table.ctrl.page_number].rule[index].mask.ide);
		// 30 GAFLRTRM
		CAN_REG_UPDATE_MODEL_FLAG(data, 30, can_rx_rule_table.page[can_rx_rule_table.ctrl.page_number].rule[index].mask.rtr);
		// 28 ～ 0 GAFLIDM[28:0]
		can_rx_rule_table.page[can_rx_rule_table.ctrl.page_number].rule[index].mask.canid = CAN_REG_GET_MASK_DATA(data, 0x1FFFFFFF);
	}
}
void can_register_mapping_io_RSCANnGAFLP0j(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnGAFLP0j(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		// nothing to do
		// モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
	}
	else { /* CanRegisterIo_Write */
		//モデルへの同期処理
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		// 31 ～ 28 GAFLDLC [3:0]
		can_rx_rule_table.page[can_rx_rule_table.ctrl.page_number].rule[index].filter_buffer.dlc = (data >> 28U);
		// 27 ～ 16 GAFLPTR[11:0]
		can_rx_rule_table.page[can_rx_rule_table.ctrl.page_number].rule[index].filter_buffer.label = ((data >> 16U) & 0xFFF);
		// 15 GAFLRMV
		CAN_REG_UPDATE_MODEL_FLAG(data, 15, can_rx_rule_table.page[can_rx_rule_table.ctrl.page_number].rule[index].filter_buffer.enable_rxbuffer);
		// 14 ～ 8 GAFLRMDP[6:0]
		can_rx_rule_table.page[can_rx_rule_table.ctrl.page_number].rule[index].filter_buffer.rxbuffer_number = CAN_REG_GET_MASK_DATA((data >> 8U), 0x0000007F);
	}
}

void can_register_mapping_io_RSCANnGAFLP1j(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnGAFLP1j(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		// nothing to do
		// モデル側は参照のみで自発的な更新はしないためモデル情報を取得する必要はない
	}
	else { /* CanRegisterIo_Write */
		//モデルへの同期処理
		can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
		// 25 ～ 8 GAFLFDP[25:8]
		can_rx_rule_table.page[can_rx_rule_table.ctrl.page_number].rule[index].filter_fifo.txrx_fifo = CAN_REG_GET_MASK_DATA((data >> 8U), 0x0003FFFF);/* 18bits */
		// 7 ～ 0 GAFLFDP [7:0]
		can_rx_rule_table.page[can_rx_rule_table.ctrl.page_number].rule[index].filter_fifo.rx_fifo = CAN_REG_GET_MASK_DATA((data >> 0U), 0x000000FF);
	}
}
void can_register_mapping_io_RX_RULE(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 addr = CAN_REG_GET_ALIGNED_ADDR(arg->address);

	switch (addr & 0x0000000F) {
	case 0x0:
		can_register_mapping_io_RSCANnGAFLIDj(io_type, arg);
		break;
	case 0x04:
		can_register_mapping_io_RSCANnGAFLMj(io_type, arg);
		break;
	case 0x8:
		can_register_mapping_io_RSCANnGAFLP0j(io_type, arg);
		break;
	case 0xC:
		can_register_mapping_io_RSCANnGAFLP1j(io_type, arg);
		break;
	default:
		break;
	}
	return;
}

