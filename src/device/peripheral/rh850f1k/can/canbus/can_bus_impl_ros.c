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
#include "can_types.h"
#include "can_config.h"
#include "canbus/can_bus.h"
#include "athrill_mros_device.h"
#include "cpuemu_ops.h"
#include "mpu_ops.h"

static void can_bus_operation_impl_ros_init(void);
static bool can_bus_operation_impl_ros_rx_is_arrived(CanChannelIdType cid);
static bool can_bus_operation_impl_ros_rx_receive(CanChannelIdType cid, CanDataType *data);
static Std_ReturnType can_bus_operation_impl_ros_tx_start_send(CanChannelIdType cid, CanDataType *data);
static bool can_bus_operation_impl_ros_tx_is_done(CanChannelIdType cid);

CanBusOperationType can_bus_operation_impl_ros = {
	.init = can_bus_operation_impl_ros_init,
	.rx.is_arrived = can_bus_operation_impl_ros_rx_is_arrived,
	.rx.receive = can_bus_operation_impl_ros_rx_receive,
	.tx.start_send = can_bus_operation_impl_ros_tx_start_send,
	.tx.is_done = can_bus_operation_impl_ros_tx_is_done,
};
static 	uint32 rx_queue_max = 256U;

typedef struct {
	uint32 count;
	uint32 roff;
	uint32 woff;
	CanDataType  *bufferp;
} CanBusRosRxBufferType;

static CanBusRosRxBufferType can_bus_ros_rx_buffer[CAN_CHANNEL_NUM];
static void can_bus_ros_rx_buffer_init(void)
{
	CanChannelIdType cid;

	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		can_bus_ros_rx_buffer[cid].count = 0;
		can_bus_ros_rx_buffer[cid].roff = 0;
		can_bus_ros_rx_buffer[cid].woff = 0;
		can_bus_ros_rx_buffer[cid].bufferp = malloc(sizeof(CanDataType) * rx_queue_max);
		ASSERT(can_bus_ros_rx_buffer[cid].bufferp != NULL);
	}
}
static bool can_bus_ros_rx_is_arrived(CanChannelIdType cid)
{
	if (can_bus_ros_rx_buffer[cid].count > 0) {
		return TRUE;
	}
	return FALSE;
}

static bool can_bus_ros_rx_buffer_put(CanChannelIdType cid, CanDataType *data)
{
	if (can_bus_ros_rx_buffer[cid].count >= rx_queue_max) {
		return FALSE;
	}
	can_bus_ros_rx_buffer[cid].bufferp[can_bus_ros_rx_buffer[cid].woff] = *data;
	can_bus_ros_rx_buffer[cid].count++;
	can_bus_ros_rx_buffer[cid].woff++;
	if (can_bus_ros_rx_buffer[cid].woff >= rx_queue_max) {
		can_bus_ros_rx_buffer[cid].woff = 0;
	}
	return TRUE;
}
static bool can_bus_ros_rx_buffer_get(CanChannelIdType cid, CanDataType *data)
{
	ros_topic_callback_lock();
	if (can_bus_ros_rx_buffer[cid].count <= 0) {
		ros_topic_callback_unlock();
		return FALSE;
	}
	*data = can_bus_ros_rx_buffer[cid].bufferp[can_bus_ros_rx_buffer[cid].roff];
	can_bus_ros_rx_buffer[cid].count--;
	can_bus_ros_rx_buffer[cid].roff++;
	if (can_bus_ros_rx_buffer[cid].roff >= rx_queue_max) {
		can_bus_ros_rx_buffer[cid].roff = 0;
	}
	ros_topic_callback_unlock();
	return TRUE;
}


typedef enum {
	mRosTopicType_PUB = 0,
	mRosTopicType_SUB
} mRosTopicType;

static uint32 mros_pub_req_num = 0;
static AthrillMrosDevPubReqType *pub_req_table;

static void topic_callback(const char *data, int datalen);
static uint32 mros_sub_req_num = 0;
static AthrillMrosDevSubReqType *sub_req_table;

typedef struct {
	CanChannelIdType 	cid;
	uint32				ide;
	uint32				rtr;
	uint32				dlc;
	uint32				canid;
} CanBusRosTopicType;

static CanBusRosTopicType *can_bus_ros_pub_topic;
static CanBusRosTopicType *can_bus_ros_sub_topic;

static void device_parse_can_config(mRosTopicType type, const char* fmt_num, const char* fmt_topic)
{
	uint32 i;
	uint32 param_num;
	Std_ReturnType err;
	static char parameter[4096];
	char *param_value;

	err = cpuemu_get_devcfg_value(fmt_num, &param_num);
	if (err != STD_E_OK) {
		return;
	}
	if (type == mRosTopicType_PUB) {
		mros_pub_req_num = param_num;
		pub_req_table = malloc(sizeof(AthrillMrosDevPubReqType) * param_num);
		ASSERT(pub_req_table != NULL);
		can_bus_ros_pub_topic = malloc(sizeof(CanBusRosTopicType) * mros_pub_req_num);
		ASSERT(can_bus_ros_pub_topic != NULL);
	}
	else {
		mros_sub_req_num = param_num;
		sub_req_table = malloc(sizeof(AthrillMrosDevSubReqType) * param_num);
		ASSERT(sub_req_table != NULL);
		can_bus_ros_sub_topic = malloc(sizeof(CanBusRosTopicType) * mros_sub_req_num);
		ASSERT(can_bus_ros_sub_topic != NULL);
	}
	for (i = 0; i < param_num; i++) {
		snprintf(parameter, sizeof(parameter), fmt_topic, i);
		err = cpuemu_get_devcfg_string(parameter, &param_value);
		if (err != STD_E_OK) {
			printf("not found param=%s\n", parameter);
			return;
		}
		/*
		 * topic_name:channel<ch>/CAN_IDE<ide>_RTR<rtr>_DLC<dlc>_<canid>
		 * topic_data: <0><1><2><3><4><5><6><7>
		 */
		if (type == mRosTopicType_PUB) {
			pub_req_table[i].topic_name = param_value;
			pub_req_table[i].pub = NULL;
			int n = sscanf(param_value, "channel%u/CAN_IDE%u_RTR%u_DLC%u_0x%x",
					&can_bus_ros_pub_topic[i].cid,
					&can_bus_ros_pub_topic[i].ide,
					&can_bus_ros_pub_topic[i].rtr,
					&can_bus_ros_pub_topic[i].dlc,
					&can_bus_ros_pub_topic[i].canid);
			if (n != 5U) {
				printf("ERROR: can not parse param(%s). format:channel<ch>/CAN_IDE<ide>_RTR<rtr>_DLC<dlc>_<canid>\n", param_value);
			}
		}
		else {
			sub_req_table[i].topic_name = param_value;
			sub_req_table[i].callback = topic_callback;
			sub_req_table[i].sub = NULL;
			int n = sscanf(param_value, "channel%u/CAN_IDE%u_RTR%u_DLC%u_0x%x",
					&can_bus_ros_sub_topic[i].cid,
					&can_bus_ros_sub_topic[i].ide,
					&can_bus_ros_sub_topic[i].rtr,
					&can_bus_ros_sub_topic[i].dlc,
					&can_bus_ros_sub_topic[i].canid);
			if (n != 5U) {
				printf("ERROR: can not parse param(%s). format:channel<ch>/CAN_IDE<ide>_RTR<rtr>_DLC<dlc>_<canid>\n", param_value);
			}
		}
	}
}

static void can_bus_operation_impl_ros_init(void)
{
	int err;

	set_athrill_task();

	(void)cpuemu_get_devcfg_value("DEBUG_FUNC_CAN_RX_QUEUE_SIZE ", &rx_queue_max);
	can_bus_ros_rx_buffer_init();

	device_parse_can_config(mRosTopicType_PUB, "DEBUG_FUNC_MROS_TOPIC_PUB_NUM", "DEBUG_FUNC_MROS_TOPIC_PUB_%d");
	device_parse_can_config(mRosTopicType_SUB, "DEBUG_FUNC_MROS_TOPIC_SUB_NUM", "DEBUG_FUNC_MROS_TOPIC_SUB_%d");

	err = athrill_mros_device_pub_register(pub_req_table, mros_pub_req_num);
	if (err < 0) {
		printf("error: athrill_mros_device_pub_register()\n");
		return;
	}
	err = athrill_mros_device_sub_register(sub_req_table, mros_sub_req_num);
	if (err < 0) {
		printf("error: athrill_mros_device_sub_register()\n");
		return;
	}
	err = athrill_mros_device_start();
	if (err < 0) {
		printf("error: athrill_mros_device_start()\n");
		return;
	}
	return;
}



static void topic_callback(const char *data, int datalen)
{
	mRosCallbackTopicIdType topic_id = ros_topic_callback_topic_id();
	int i;
	int copylen;
	CanDataType can_data;
	CanChannelIdType cid;

	for (i = 0; i < mros_sub_req_num; i++) {
		if (sub_req_table[i].sub == NULL) {
			continue;
		}
		if (topic_id == sub_req_table[i].sub->topic_id) {
			break;
		}
	}
	if (i == mros_sub_req_num) {
		return;
	}
	cid = can_bus_ros_sub_topic[i].cid;
	can_data.ide = can_bus_ros_sub_topic[i].ide;
	can_data.rtr = can_bus_ros_sub_topic[i].rtr;
	can_data.dlc = can_bus_ros_sub_topic[i].dlc;
	can_data.id = can_bus_ros_sub_topic[i].canid;

	copylen = datalen;
	if (datalen != CAN_RX_BUFFER_DATA_SIZE) {
		printf("ERROR: can rx data size invalid(topic=%s len=%u)\n", sub_req_table[i].topic_name, datalen);
		if (datalen > CAN_RX_BUFFER_DATA_SIZE) {
			copylen = CAN_RX_BUFFER_DATA_SIZE;
		}
	}
	memcpy(can_data.data, data, copylen);
	can_bus_ros_rx_buffer_put(cid, &can_data);
	return;
}


static bool can_bus_operation_impl_ros_rx_is_arrived(CanChannelIdType cid)
{
	return can_bus_ros_rx_is_arrived(cid);
}

static bool can_bus_operation_impl_ros_rx_receive(CanChannelIdType cid, CanDataType *data)
{
	return can_bus_ros_rx_buffer_get(cid, data);
}
static Std_ReturnType can_bus_operation_impl_ros_tx_start_send(CanChannelIdType cid, CanDataType *data)
{
	uint32 i;
	for (i = 0; i < mros_pub_req_num; i++) {
		if (can_bus_ros_pub_topic[i].cid != cid) {
			continue;
		}
		if (can_bus_ros_pub_topic[i].ide != data->ide) {
			continue;
		}
		if (can_bus_ros_pub_topic[i].rtr != data->rtr) {
			continue;
		}
		if (can_bus_ros_pub_topic[i].dlc != data->dlc) {
			continue;
		}
		if (can_bus_ros_pub_topic[i].canid != data->id) {
			continue;
		}
		break;
	}
	if (i == mros_pub_req_num) {
		printf("ERROR: ROS Unknown can data send(ide=%u rtr=%u dlc=%u id=0x%x\n", data->ide, data->rtr, data->dlc, data->id);
		return STD_E_NOENT;
	}
	(void)ros_topic_publish(pub_req_table[i].pub, data->data, data->dlc);
	return STD_E_OK;
}

static bool can_bus_operation_impl_ros_tx_is_done(CanChannelIdType cid)
{
	return TRUE;
}



