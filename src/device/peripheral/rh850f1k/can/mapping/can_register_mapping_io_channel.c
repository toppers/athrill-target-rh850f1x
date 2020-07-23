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
#include "can_mode_global.h"
#include "can_register_mapping_reset.h"

void can_register_reset_channel(CanRegisterResetType type, CanChannelIdType cid)
{
	if (type == CanRegisterResetType_MCU_RESET) {
		can_mode_global.channel[cid].cid = cid;
		can_mode_global.channel[cid].ctrl.stop = TRUE;
		can_mode_global.channel[cid].ctrl.select_mode = CAN_MODE_CHANNEL_MODE_RESET;
		can_mode_global.channel[cid].mode = ChannelMode_STOP;
		can_mode_global.channel[cid].comm_mode = ChannelModeComm_IDLE;
		can_mode_global.channel[cid].status.can_comm = FALSE;
		can_mode_global.channel[cid].status.receiving = FALSE;
		can_mode_global.channel[cid].status.sending = FALSE;
		can_mode_global.channel[cid].status.mode_stop = TRUE;
		can_mode_global.channel[cid].status.mode_wait = FALSE;
		can_mode_global.channel[cid].status.mode_reset = TRUE;
	}
	else if (type == CanRegisterResetType_GLOBAL_RESET) {
		// nothing
	}
	else if (type == CanRegisterResetType_CHANNEL_RESET) {
		can_mode_global.channel[cid].mode = ChannelMode_STOP;
		can_mode_global.channel[cid].ctrl.select_mode = CAN_MODE_CHANNEL_MODE_RESET;
		can_mode_global.channel[cid].status.can_comm = FALSE;
		can_mode_global.channel[cid].status.receiving = FALSE;
		can_mode_global.channel[cid].status.sending = FALSE;
		can_mode_global.channel[cid].status.mode_wait = FALSE;
	}

}

void can_register_mapping_io_CHANNEL(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 addr = CAN_REG_GET_ALIGNED_ADDR(arg->address);

	switch (addr & 0x0000000F) {
	case 0x0:
		can_register_mapping_io_RSCANnCmCFG(io_type, arg);
		break;
	case 0x04:
		can_register_mapping_io_RSCANnCmCTR(io_type, arg);
		break;
	case 0x8:
		can_register_mapping_io_RSCANnCmSTS(io_type, arg);
		break;
	case 0xC:
		can_register_mapping_io_RSCANnCmERFL(io_type, arg);
		break;
	default:
		break;
	}
	return;
}
void can_register_mapping_io_RSCANnCmCFG(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	//not supported
	return;
}
void can_register_mapping_io_RSCANnCmCTR(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 current_cslpr;
	uint32 current_chmdc;
	static uint32 old_cslpr = 0x01; // reset value
	static uint32 old_chmdc = 0x01; // reset value
	uint32 data;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnCmCTR(0), 16U);

	can_register_read_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), &data);
	current_cslpr = CAN_REG_GET_MASK_DATA(data >> 2U, 0x1);
	current_chmdc = CAN_REG_GET_MASK_DATA(data, 0x3);

	if (io_type == CanRegisterIo_Write) {
		// 2 CSLPR
		if (current_cslpr != old_cslpr) {
			if (current_cslpr != 0U) {
				can_mode_channel_event(&can_mode_global.channel[index], CanModeChannelEvent_SET_STOP);
			}
			else {
				can_mode_channel_event(&can_mode_global.channel[index], CanModeChannelEvent_CLR_STOP);
			}
		}
		// 1、 0 CHMDC[1:0]
		if (current_chmdc != old_chmdc) {
			if (current_chmdc == CAN_MODE_CHANNEL_MODE_COMM) {
				can_mode_channel_event(&can_mode_global.channel[index], CanModeChannelEvent_SET_COMM);
			}
			else if (current_chmdc == CAN_MODE_CHANNEL_MODE_RESET) {
				can_mode_channel_event(&can_mode_global.channel[index], CanModeChannelEvent_SET_RESET);
			}
			else if (current_chmdc == CAN_MODE_CHANNEL_MODE_WAIT) {
				can_mode_channel_event(&can_mode_global.channel[index], CanModeChannelEvent_SET_WAIT);
			}
		}
		old_chmdc = current_chmdc;
		old_cslpr = current_cslpr;
	}
	// 2 CSLPR
	CAN_REG_UPDATE_REG_BIT(can_mode_global.channel[index].ctrl.stop, 2U, data);
	// 1、 0 CHMDC[1:0]
	data &= ~0x3;
	data |= can_mode_global.channel[index].ctrl.select_mode;
	can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	return;
}
void can_register_mapping_io_RSCANnCmSTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnCmSTS(0), 16U);

	if (io_type == CanRegisterIo_Read) {
		// 7 COMSTS
		CAN_REG_UPDATE_REG_BIT(can_mode_global.channel[index].status.can_comm, 7U, data);
		// 6 RECSTS
		CAN_REG_UPDATE_REG_BIT(can_mode_global.channel[index].status.receiving, 6U, data);
		// 5 TRMSTS
		CAN_REG_UPDATE_REG_BIT(can_mode_global.channel[index].status.sending, 5U, data);
		// 2 CSLPSTS
		CAN_REG_UPDATE_REG_BIT(can_mode_global.channel[index].status.mode_stop, 2U, data);
		// 1 CHLTSTS
		CAN_REG_UPDATE_REG_BIT(can_mode_global.channel[index].status.mode_wait, 1U, data);
		// 0 CRSTSTS
		CAN_REG_UPDATE_REG_BIT(can_mode_global.channel[index].status.mode_reset, 0U, data);
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else {
		// nothing to do read only.
	}
	return;
}
void can_register_mapping_io_RSCANnCmERFL(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	//not supported
	return;
}
