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
#include "can_buffer.h"
#include "can_register_mapping_reset.h"

void can_register_reset_tx_buffer_status(CanRegisterResetType type)
{
	if (type == CanRegisterResetType_MCU_RESET) {
		// nothing
	}
	else if (type == CanRegisterResetType_GLOBAL_RESET) {
		// nothing
	}
	else if (type == CanRegisterResetType_CHANNEL_RESET) {
		// nothing
	}

}
void can_register_mapping_io_RSCANnTMTRSTSy(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 p = 0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnTMTRSTSy(0), 4U);
	CanChannelIdType cid;
	CanBufferIdxType bid;

	if (io_type == CanRegisterIo_Read) {
		for (cid = (index * 2U); cid < ((index * 2U) + 2U); cid++) {
			for (bid = 0; bid < CAN_TX_CHANNEL_BUFFER_NUM; bid++) {
				CAN_REG_UPDATE_REG_BIT(can_tx_buffer_control.buf[cid][bid].ctrl.req_tx, p, data);
				p++;
			}
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}

	return;
}

void can_register_mapping_io_RSCANnTMTCSTSy(CanRegisterIoType io_type, CanRegisterIoArgType *arg)
{
	uint32 data = 0x0;
	uint32 p = 0;
	uint32 index = CAN_REG_GROUP_INDEX(arg->address, CAN_REG_RSCANnTMTCSTSy(0), 4U);
	CanChannelIdType cid;
	CanBufferIdxType bid;

	if (io_type == CanRegisterIo_Read) {
		for (cid = (index * 2U); cid < ((index * 2U) + 2U); cid++) {
			for (bid = 0; bid < CAN_TX_CHANNEL_BUFFER_NUM; bid++) {
				if ((can_tx_buffer_control.buf[cid][bid].status.result & CAN_TX_BUFFER_RESULT_OK) != 0U) {
					data |= (1U << (p));
				}
				else {
					data &= ~(1U << (p));
				}
				p++;
			}
		}
		can_register_write_data32(CAN_REG_GET_ALIGNED_ADDR(arg->address), data);
	}
	else { /* CanRegisterIo_Write */
		// nothing to do
		// ユーザ側は参照のみ
	}

	return;
}
