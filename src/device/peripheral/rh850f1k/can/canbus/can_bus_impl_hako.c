#include "can_types.h"
#include "can_config.h"
#include "canbus/can_bus.h"
#include "cpuemu_ops.h"
#include "mpu_ops.h"
#include "hako_client.h"
#include "can_msgs/pdu_ctype_HakoCan.h"
#include <string.h>

static char* hako_asset_name = NULL;

static void can_bus_operation_impl_hako_init(void);
static bool can_bus_operation_impl_hako_rx_is_arrived(CanChannelIdType cid);
static bool can_bus_operation_impl_hako_rx_receive(CanChannelIdType cid, CanDataType *data);
static Std_ReturnType can_bus_operation_impl_hako_tx_start_send(CanChannelIdType cid, CanDataType *data);
static bool can_bus_operation_impl_hako_tx_is_done(CanChannelIdType cid);

CanBusOperationType can_bus_operation_impl_hako = {
	.init = can_bus_operation_impl_hako_init,
	.rx.is_arrived = can_bus_operation_impl_hako_rx_is_arrived,
	.rx.receive = can_bus_operation_impl_hako_rx_receive,
	.tx.start_send = can_bus_operation_impl_hako_tx_start_send,
	.tx.is_done = can_bus_operation_impl_hako_tx_is_done,
};
static 	uint32 rx_queue_max = 256U;

typedef struct {
	uint32 count;
	uint32 roff;
	uint32 woff;
	CanDataType  *bufferp;
} CanBusRosRxBufferType;

static CanBusRosRxBufferType can_bus_hako_rx_buffer[CAN_CHANNEL_NUM];
static void can_bus_hako_rx_buffer_init(void)
{
	CanChannelIdType cid;

	for (cid = 0; cid < CAN_CHANNEL_NUM; cid++) {
		can_bus_hako_rx_buffer[cid].count = 0;
		can_bus_hako_rx_buffer[cid].roff = 0;
		can_bus_hako_rx_buffer[cid].woff = 0;
		can_bus_hako_rx_buffer[cid].bufferp = malloc(sizeof(CanDataType) * rx_queue_max);
		ASSERT(can_bus_hako_rx_buffer[cid].bufferp != NULL);
	}
}
static bool can_bus_hako_rx_is_arrived(CanChannelIdType cid)
{
	if (can_bus_hako_rx_buffer[cid].count > 0) {
		return TRUE;
	}
	return FALSE;
}

static bool can_bus_hako_rx_buffer_put(CanChannelIdType cid, CanDataType *data)
{
	if (can_bus_hako_rx_buffer[cid].count >= rx_queue_max) {
		return FALSE;
	}
	can_bus_hako_rx_buffer[cid].bufferp[can_bus_hako_rx_buffer[cid].woff] = *data;
	can_bus_hako_rx_buffer[cid].count++;
	can_bus_hako_rx_buffer[cid].woff++;
	if (can_bus_hako_rx_buffer[cid].woff >= rx_queue_max) {
		can_bus_hako_rx_buffer[cid].woff = 0;
	}
	return TRUE;
}
static bool can_bus_hako_rx_buffer_get(CanChannelIdType cid, CanDataType *data)
{
	if (can_bus_hako_rx_buffer[cid].count <= 0) {
		return FALSE;
	}
	*data = can_bus_hako_rx_buffer[cid].bufferp[can_bus_hako_rx_buffer[cid].roff];
	can_bus_hako_rx_buffer[cid].count--;
	can_bus_hako_rx_buffer[cid].roff++;
	if (can_bus_hako_rx_buffer[cid].roff >= rx_queue_max) {
		can_bus_hako_rx_buffer[cid].roff = 0;
	}
	return TRUE;
}


typedef enum {
	HakoTopicType_PUB = 0,
	HakoTopicType_SUB
} HakoTopicType;

static uint32 hako_pub_req_num = 0;

static uint32 hako_sub_req_num = 0;

typedef struct {
	int					pdu_channel;
	CanChannelIdType 	cid;
	uint32				ide;
	uint32				rtr;
	uint32				dlc;
	uint32				canid;
} CanBusHakoTopicType;

static CanBusHakoTopicType *can_bus_hako_pub_topic;
static CanBusHakoTopicType *can_bus_hako_sub_topic;

static void device_parse_can_config(HakoTopicType type, const char* fmt_num, const char* fmt_topic)
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
	if (type == HakoTopicType_PUB) {
		hako_pub_req_num = param_num;
		can_bus_hako_pub_topic = malloc(sizeof(CanBusHakoTopicType) * hako_pub_req_num);
		ASSERT(can_bus_hako_pub_topic != NULL);
	}
	else {
		hako_sub_req_num = param_num;
		//sub_req_table = malloc(sizeof(AthrillMrosDevSubReqType) * param_num);
		//ASSERT(sub_req_table != NULL);
		can_bus_hako_sub_topic = malloc(sizeof(CanBusHakoTopicType) * hako_sub_req_num);
		ASSERT(can_bus_hako_sub_topic != NULL);
	}
	for (i = 0; i < param_num; i++) {
		snprintf(parameter, sizeof(parameter), fmt_topic, i);
		err = cpuemu_get_devcfg_string(parameter, &param_value);
		if (err != STD_E_OK) {
			printf("not found param=%s\n", parameter);
			return;
		}
		/*
		 * topic_name:channel<ch>/CAN_IDE<ide>_RTR<rtr>_DLC<dlc>_<canid>/<pdu_channel>
		 * topic_data: <0><1><2><3><4><5><6><7>
		 */
		if (type == HakoTopicType_PUB) {
			int n = sscanf(param_value, "channel%u/CAN_IDE%u_RTR%u_DLC%u_0x%x/%d",
					&can_bus_hako_pub_topic[i].cid,
					&can_bus_hako_pub_topic[i].ide,
					&can_bus_hako_pub_topic[i].rtr,
					&can_bus_hako_pub_topic[i].dlc,
					&can_bus_hako_pub_topic[i].canid,
					&can_bus_hako_pub_topic[i].pdu_channel
					);
			if (n != 6U) {
				printf("ERROR: can not parse param(%s). format:channel<ch>/CAN_IDE<ide>_RTR<rtr>_DLC<dlc>_<canid>\n", param_value);
			}
			err = hako_client_create_pdu_channel(can_bus_hako_pub_topic[i].pdu_channel, sizeof(Hako_HakoCan));
			if (err != 0) {
				printf("ERROR: can not create pdu channel: %d\n", can_bus_hako_pub_topic[i].cid);
			}
		}
		else {
			int n = sscanf(param_value, "channel%u/CAN_IDE%u_RTR%u_DLC%u_0x%x/%d",
					&can_bus_hako_sub_topic[i].cid,
					&can_bus_hako_sub_topic[i].ide,
					&can_bus_hako_sub_topic[i].rtr,
					&can_bus_hako_sub_topic[i].dlc,
					&can_bus_hako_sub_topic[i].canid,
					&can_bus_hako_pub_topic[i].pdu_channel
					);
			if (n != 6U) {
				printf("ERROR: can not parse param(%s). format:channel<ch>/CAN_IDE<ide>_RTR<rtr>_DLC<dlc>_<canid>\n", param_value);
			}
		}
	}
}

static void can_bus_operation_impl_hako_init(void)
{
	int err;

	(void)cpuemu_get_devcfg_string("DEBUG_FUNC_HAKO_ASSET_NAME", &hako_asset_name);
	err = hako_client_init(hako_asset_name);
	if (err != 0) {
		printf("ERROR: hako_client_init() error=%d\n", err);
		exit(1);
	}

	(void)cpuemu_get_devcfg_value("DEBUG_FUNC_CAN_RX_QUEUE_SIZE ", &rx_queue_max);
	can_bus_hako_rx_buffer_init();

	device_parse_can_config(HakoTopicType_PUB, "DEBUG_FUNC_HAKO_PUB_NUM", "DEBUG_FUNC_HAKO_PUB_%d");
	device_parse_can_config(HakoTopicType_SUB, "DEBUG_FUNC_HAKO_SUB_NUM", "DEBUG_FUNC_HAKO_SUB_%d");

	return;
}

static bool can_bus_operation_impl_hako_rx_is_arrived(CanChannelIdType cid)
{
	if (   (hako_client_is_pdu_created() == 0) 
		&& (hako_client_is_simulation_mode() == 0)
		) {
		int i;
		for (i = 0; i < hako_sub_req_num; i++) {
			if (can_bus_hako_sub_topic[i].cid != cid) {
				continue;
			}
			else if (hako_client_pdu_is_dirty(can_bus_hako_sub_topic[i].pdu_channel) != 0) {
				continue;
			}
			Hako_HakoCan can_msg;
			if (hako_client_read_pdu(hako_asset_name, can_bus_hako_sub_topic[i].pdu_channel, (char *)&can_msg, sizeof(Hako_HakoCan) == 0)) {
				CanDataType data;
				data.rtr = can_msg.head.rtr;
				data.ide = can_msg.head.ide;
				data.dlc = can_msg.head.dlc;
				data.id = can_msg.head.canid;
				memcpy(&data.data[0], &can_msg.body.data[0], 8);
				can_bus_hako_rx_buffer_put(cid, &data);
			}
		}
		hako_client_notify_read_pdu_done(hako_asset_name);
	}
	return can_bus_hako_rx_is_arrived(cid);
}

static bool can_bus_operation_impl_hako_rx_receive(CanChannelIdType cid, CanDataType *data)
{
	return can_bus_hako_rx_buffer_get(cid, data);
}
static Std_ReturnType can_bus_operation_impl_hako_tx_start_send(CanChannelIdType cid, CanDataType *data)
{
	uint32 i;
	for (i = 0; i < hako_pub_req_num; i++) {
		if (can_bus_hako_pub_topic[i].cid != cid) {
			continue;
		}
		if (can_bus_hako_pub_topic[i].ide != data->ide) {
			continue;
		}
		if (can_bus_hako_pub_topic[i].rtr != data->rtr) {
			continue;
		}
		if (can_bus_hako_pub_topic[i].dlc != data->dlc) {
			continue;
		}
		if (can_bus_hako_pub_topic[i].canid != data->id) {
			continue;
		}
		break;
	}
	if (i == hako_pub_req_num) {
		printf("ERROR: ROS Unknown can data send(ide=%u rtr=%u dlc=%u id=0x%x\n", data->ide, data->rtr, data->dlc, data->id);
		return STD_E_NOENT;
	}
	if ((hako_client_is_simulation_mode() == 0) || (hako_client_is_pdu_sync_mode(hako_asset_name) == 0)) {
		Hako_HakoCan can_msg;
		can_msg.head.rtr = data->rtr;
		can_msg.head.ide = data->ide;
		can_msg.head.dlc = data->dlc;
		can_msg.head.channel = cid;
		can_msg.head.canid = data->id;
		int j;
		for (j = 0; j < 8; j++) {
			can_msg.body.data[0] = data->data[j];
		}
		(void)hako_client_write_pdu(hako_asset_name, can_bus_hako_pub_topic[i].pdu_channel, (const char*)&can_msg, sizeof(Hako_HakoCan));
		hako_client_notify_write_pdu_done(hako_asset_name);
	}
	else {
		//not ready
		printf("ERROR: Can not write pdu data\n");
	}
	return STD_E_OK;
}

static bool can_bus_operation_impl_hako_tx_is_done(CanChannelIdType cid)
{
	return TRUE;
}



