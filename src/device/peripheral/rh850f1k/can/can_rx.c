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
#include "can_rx.h"
#include "can_buffer.h"
#include "can_fifo.h"


CanRxRuleNumType 	can_rx_number;
CanRxRuleTableType	can_rx_rule_table;

typedef struct {
	uint32			rule_num;
	CanRxRuleType	*rule[CAN_RX_RULE_TABLE_PAGE_NUM * CAN_RX_PAGE_RULE_NUM];
} CanRxChannelRuleType;

typedef struct {
	CanRxRuleTableType	*rule_table;
	CanChannelIdType	cid;
	CanRxRuleType 		*rule;
	CanDataType 		*received_data;
	bool				filter_result;
	bool				filter_acceptance;
	bool				filter_dlc;
} CanRxProcDataType;

static void can_rx_proc_one(CanRxProcDataType *proc_data);
/*
 * Filter Process
 */
static void can_rx_proc_filter(CanRxProcDataType *proc_data);
static void can_rx_proc_filter_acceptance(CanRxProcDataType *proc_data);
static void can_rx_proc_filter_dlc(CanRxProcDataType *proc_data);

/*
 * Routing Process
 */
static void can_rx_proc_routing(CanRxProcDataType *proc_data);

static CanRxChannelRuleType can_rx_channel_rule[CAN_CHANNEL_NUM];
static CanBusOperationType *canbus_rx_operation;
static CanModeGlobalType *can_rx_mode_global;

void can_rx_init(CanBusOperationType *canbus_op)
{
	can_rx_mode_global = &can_mode_global;
	canbus_rx_operation = canbus_op;

	can_rx_update_rule_table();
	return;
}
void can_rx_update_rule_table(void)
{
	uint32 rid;
	uint32 cid;
	uint32 count;
	rid = 0;
	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		can_rx_channel_rule[cid].rule_num = can_rx_number.channel_rx_rule_num[cid];
		for (count = 0; count < can_rx_channel_rule[cid].rule_num; count++) {
			uint32 pid = CAN_RX_GET_PAGEID(rid);
			uint32 pidx = CAN_RX_GET_PAGEIDX(rid);
			can_rx_channel_rule[cid].rule[count] = &can_rx_rule_table.page[pid].rule[pidx];
			rid++;
		}
	}
}

void can_rx_proc(void)
{
	uint32 cid;
	uint32 count;
	CanRxProcDataType proc_data;
	CanDataType data;

	if (can_rx_mode_global->mode != GlobalMode_RUN) {
		return;
	}

	proc_data.rule_table = &can_rx_rule_table;
	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		if (canbus_rx_operation->rx.is_arrived(cid) == FALSE) {
			continue;
		}

		if (can_rx_mode_global->channel[cid].mode != ChannelMode_COMM) {
			continue;
		}
		can_mode_channel_event(&can_rx_mode_global->channel[cid], CanModeChannelEvent_COMM_RX_START);
		if (can_rx_mode_global->channel[cid].comm_mode != ChannelModeComm_RX) {
			continue;
		}

		(void)canbus_rx_operation->rx.receive(cid, &data);
		proc_data.received_data = &data;
		proc_data.cid = cid;
		for (count = 0; count < can_rx_channel_rule[cid].rule_num; count++) {
			proc_data.rule = can_rx_channel_rule[cid].rule[count];
			can_rx_proc_one(&proc_data);
		}
		can_mode_channel_event(&can_rx_mode_global->channel[cid], CanModeChannelEvent_COMM_RX_DONE);
	}
	return;
}

/********************************************************
 * Private methods
 ********************************************************/
static void can_rx_proc_one(CanRxProcDataType *proc_data)
{
	can_rx_proc_filter(proc_data);
	if (proc_data->filter_result == FALSE) {
		return;
	}

	can_rx_proc_routing(proc_data);
	return;
}

static void can_rx_proc_filter(CanRxProcDataType *proc_data)
{
	proc_data->filter_result = FALSE;
	proc_data->filter_acceptance = FALSE;
	proc_data->filter_dlc = FALSE;

	can_rx_proc_filter_acceptance(proc_data);
	if (proc_data->filter_acceptance == FALSE) {
		return;
	}

	(void)can_rx_proc_filter_dlc(proc_data);
	if (proc_data->filter_dlc == FALSE) {
		return;
	}
	proc_data->filter_result = TRUE;
	return;
}


static void can_rx_proc_filter_acceptance(CanRxProcDataType *proc_data)
{
	if (proc_data->rule->mask.ide != FALSE) {
		if (proc_data->rule->id.ide != proc_data->received_data->ide) {
			return;
		}
	}
	if (proc_data->rule->mask.rtr != FALSE) {
		if (proc_data->rule->id.rtr != proc_data->received_data->rtr) {
			return;
		}
	}
	if (proc_data->rule->mask.canid != 0U) {
		if ((proc_data->rule->id.canid &  proc_data->rule->mask.canid) != (proc_data->received_data->id & proc_data->rule->mask.canid)) {
			return;
		}
	}
	proc_data->filter_acceptance = TRUE;
	return;
}
static void can_rx_proc_filter_dlc(CanRxProcDataType *proc_data)
{
	if (can_rx_mode_global->config.enable_dlc_check == FALSE) {
		proc_data->filter_dlc = TRUE;
	}
	else if (proc_data->rule->filter_buffer.dlc != 0) {
		if (proc_data->received_data->dlc < proc_data->rule->filter_buffer.dlc) {
			can_rx_mode_global->status.dlc_error = TRUE;
			proc_data->filter_dlc = FALSE;
		}
		else {
			proc_data->filter_dlc = TRUE;
		}
	}
	else { /* proc_data->rule->filter_buffer.dlc == 0 */
		proc_data->filter_dlc = TRUE;
	}
	return;
}

static void can_rx_proc_routing(CanRxProcDataType *proc_data)
{
	CanBufferIdType rxfifo_id;
	CanBufferIdxType txrxfifo_id;

	/*
	 * ラベル付加する
	 */
	proc_data->received_data->label = proc_data->rule->filter_buffer.label;
	if (proc_data->rule->filter_buffer.enable_rxbuffer == TRUE) {
		(void)can_rxbuffer_put_rxdata(proc_data->cid, proc_data->rule->filter_buffer.rxbuffer_number, proc_data->received_data);
	}
	if (proc_data->rule->filter_fifo.rx_fifo != 0) {
		for (rxfifo_id = 0; rxfifo_id < CAN_RX_FIFO_NUM; rxfifo_id++) {
			if ( ((1U << rxfifo_id) & proc_data->rule->filter_fifo.rx_fifo) != 0U) {
				(void)can_rxfifo_put_rxdata(proc_data->cid, rxfifo_id, proc_data->received_data);
			}
		}
	}
	if (proc_data->rule->filter_fifo.txrx_fifo != 0) {
		for (txrxfifo_id = 0; txrxfifo_id < CAN_TXRX_CHANNEL_FIFO_NUM; txrxfifo_id++) {
			if ( (CAN_TXRXFIFO_BITMASK(proc_data->cid, txrxfifo_id) & proc_data->rule->filter_fifo.txrx_fifo) != 0U) {
				(void)can_txrxfifo_put_rxdata(proc_data->cid, txrxfifo_id, proc_data->received_data);
			}
		}
	}

	return;
}
