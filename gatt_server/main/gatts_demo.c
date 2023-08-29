//MISRA C Applied
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

#include <inttypes.h>
#include "sdkconfig.h"

#include "nvs.h"

#define GATTS_TAG "MY_GATTS_DEMO_TAG"

#define APPLICATION_PROFILE_A 1
#define APPLICATION_PROFILE_B 0




///Declare the static function
static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void gatts_profile_b_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void characteristic_write_handler(uint8_t *value, uint16_t len, uint8_t *storage, uint8_t profile);

static uint8_t value1[50] = "";
static uint8_t value2[50] = "";



#define APPLICATION_PROFILE_A 1
#define APPLICATION_PROFILE_B 0

#define GATTS_SERVICE_UUID_TEST_A   0x00FFu
#define GATTS_CHAR_UUID_TEST_A      0xFF01u
#define GATTS_DESCR_UUID_TEST_A     0x3333u
#define GATTS_NUM_HANDLE_TEST_A     4u

#define GATTS_SERVICE_UUID_TEST_B   0x00EEu
#define GATTS_CHAR_UUID_TEST_B      0xEE01u
#define GATTS_DESCR_UUID_TEST_B     0x2222u
#define GATTS_NUM_HANDLE_TEST_B     4u

#define TEST_DEVICE_NAME            "Erfan's_GATTS_Tryout"
#define TEST_MANUFACTURER_DATA_LEN  17u

#define GATTS_DEMO_CHAR_VAL_LEN_MAX 0x40u

#define PREPARE_BUF_MAX_SIZE 1024u

// Global variable to store the received data

char received_buffer[PREPARE_BUF_MAX_SIZE];
int received_buffer_length = 0;

static uint8_t char1_str[] = { 0x11u, 0x22u, 0x33u };
static esp_gatt_char_prop_t a_property = 0u;
static esp_gatt_char_prop_t b_property = 0u;

static esp_attr_value_t gatts_demo_char1_val = { .attr_max_len =
GATTS_DEMO_CHAR_VAL_LEN_MAX, .attr_len = sizeof(char1_str), .attr_value =
		char1_str, };

static uint8_t adv_config_done = 0u;
#define adv_config_flag      (1u << 0)
#define scan_rsp_config_flag (1u << 1)

#ifdef CONFIG_SET_RAW_ADV_DATA
static uint8_t raw_adv_data[] = {
    0x02u, 0x01u, 0x06u,
    0x02u, 0x0au, 0xebu, 0x03u, 0x03u, 0xabu, 0xcdu
};

//static uint8_t raw_scan_rsp_data[] = {
//    0x0fu, 0x09u, 0x45u, 0x53u, 0x50u, 0x5fu, 0x47u, 0x41u, 0x54u, 0x54u, 0x53u, 0x5fu, 0x44u,
//    0x45u, 0x4du, 0x4fu
//};

static uint8_t raw_scan_rsp_data[] = {
    0x0Fu, 0x09u, 0x54u, 0x75u, 0x73u, 0x68u, 0x61u, 0x72u, 0x27u, 0x73u,
    0x5Fu, 0x47u, 0x41u, 0x54u, 0x54u, 0x53u, 0x5Fu, 0x54u, 0x72u, 0x79u,
    0x6Fu, 0x75u, 0x74u
};


static uint8_t raw_scan_rsp_data[] = {
    0x0Fu, 0x09u, 0x45u, 0x53u, 0x50u, 0x5Fu, 0x47u, 0x41u, 0x54u, 0x54u, 0x53u, 0x5Fu, 0x44u,
    0x45u, 0x4Du, 0x4Fu
};
#else

//application uuid
static uint8_t adv_service_uuid128[32] = {
		/* LSB <--------------------------------------------------------------------------------> MSB */
		//first uuid, 16bit, [12],[13] is the value
		0xFBu, 0x34u, 0x9Bu, 0x5Fu, 0x80u, 0x00u, 0x00u, 0x80u, 0x00u, 0x10u,
		0x00u, 0x00u, 0xEEu, 0x00u, 0x00u, 0x00u,
		/* second uuid, 32bit, [12], [13], [14], [15] is the value */
		0xFBu, 0x34u, 0x9Bu, 0x5Fu, 0x80u, 0x00u, 0x00u, 0x80u, 0x00u, 0x10u,
		0x00u, 0x00u, 0xFFu, 0x00u, 0x00u, 0x00u };

// The length of adv data must be less than 31 bytes
//static uint8_t test_manufacturer[TEST_MANUFACTURER_DATA_LEN] =  {0x12, 0x23, 0x45, 0x56};
//adv data
static esp_ble_adv_data_t adv_data = { .set_scan_rsp = false,
									   .include_name =true,
									   .include_txpower = false,
									   .min_interval = 0x0006u, //slave connection min interval, Time = min_interval * 1.25 msec
									   .max_interval = 0x0010u, //slave connection max interval, Time = max_interval * 1.25 msec
									   .appearance = 0x00u,
									   .manufacturer_len = 0u, //TEST_MANUFACTURER_DATA_LEN,
									   .p_manufacturer_data = NULL, //&test_manufacturer[0],
									   .service_data_len = 0u,
									   .p_service_data = NULL,
									   .service_uuid_len =sizeof(adv_service_uuid128),
									   .p_service_uuid =adv_service_uuid128,
									   .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
									};

// scan response data
static esp_ble_adv_data_t scan_rsp_data = { .set_scan_rsp = true,
											.include_name = true,
											.include_txpower = true,
											//.min_interval = 0x0006u,
											//.max_interval = 0x0010u,
											.appearance = 0x00u,
											.manufacturer_len = 0u, //TEST_MANUFACTURER_DATA_LEN,
											.p_manufacturer_data = NULL, //&test_manufacturer[0],
											.service_data_len = 0u, .p_service_data = NULL,
											.service_uuid_len =sizeof(adv_service_uuid128),
											.p_service_uuid =adv_service_uuid128,
											.flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
										};

#endif /* CONFIG_SET_RAW_ADV_DATA */

static esp_ble_adv_params_t adv_params = { .adv_int_min = 0x20u,
										   .adv_int_max =0x40u,
										   .adv_type = ADV_TYPE_IND,
										   .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
										   //.peer_addr            =
										   //.peer_addr_type       =
										   .channel_map = ADV_CHNL_ALL, .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
										};

#define PROFILE_NUM 2
#define PROFILE_A_APP_ID 0
#define PROFILE_B_APP_ID 1

struct gatts_profile_inst {
	esp_gatts_cb_t gatts_cb;
	uint16_t gatts_if;
	uint16_t app_id;
	uint16_t conn_id;
	uint16_t service_handle;
	esp_gatt_srvc_id_t service_id;
	uint16_t char_handle;
	esp_bt_uuid_t char_uuid;
	esp_gatt_perm_t perm;
	esp_gatt_char_prop_t property;
	uint16_t descr_handle;
	esp_bt_uuid_t descr_uuid;
};

/* One gatt-based profile one app_id and one gatts_if, this array will store the gatts_if returned by ESP_GATTS_REG_EVT */
static struct gatts_profile_inst gl_profile_tab[PROFILE_NUM] = {
		[PROFILE_A_APP_ID] = { .gatts_cb = gatts_profile_a_event_handler,
		                       .gatts_if = ESP_GATT_IF_NONE, /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
		                      },

		[PROFILE_B_APP_ID] = { .gatts_cb = gatts_profile_b_event_handler, /* This demo does not implement, similar as profile A */
							   .gatts_if = ESP_GATT_IF_NONE, /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
		                     },

                                                                   };

typedef struct {
				uint8_t *prepare_buf;
				int prepare_len;
			   } prepare_type_env_t;

static prepare_type_env_t a_prepare_write_env;
static prepare_type_env_t b_prepare_write_env;

void example_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);

void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {


	switch (event) {
#ifdef CONFIG_SET_RAW_ADV_DATA
    case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
        adv_config_done &= (~adv_config_flag);
        if (adv_config_done==0){
            esp_ble_gap_start_advertising(&adv_params);
        }
        break;
    case ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT:
        adv_config_done &= (~scan_rsp_config_flag);
        if (adv_config_done==0){
            esp_ble_gap_start_advertising(&adv_params);
        }
        break;
#else
	case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
		adv_config_done &= (~adv_config_flag);
		if (adv_config_done == 0) {
			esp_ble_gap_start_advertising(&adv_params);
		}
		break;
	case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
		adv_config_done &= (~scan_rsp_config_flag);
		if (adv_config_done == 0) {
			esp_ble_gap_start_advertising(&adv_params);
		}
		break;
#endif
	case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
		//advertising start complete event to indicate advertising start successfully or failed
		if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
			ESP_LOGE(GATTS_TAG, "Advertising start failed\n");
		}
		break;
	case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
		if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS) {
			ESP_LOGE(GATTS_TAG, "Advertising stop failed\n");
		} else {
			ESP_LOGI(GATTS_TAG, "Stop adv successfully\n");
		}
		break;
	case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
		ESP_LOGI(GATTS_TAG,
				"update connection params status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
				param->update_conn_params.status,
				param->update_conn_params.min_int,
				param->update_conn_params.max_int,
				param->update_conn_params.conn_int,
				param->update_conn_params.latency,
				param->update_conn_params.timeout);
		break;
	default:
		break;
	}
}

void example_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param) {
	esp_gatt_status_t status = ESP_GATT_OK;
	if (param->write.need_rsp) {
		if (param->write.is_prep) {
			if (prepare_write_env->prepare_buf == NULL) {
				prepare_write_env->prepare_buf = (uint8_t*) malloc(
				PREPARE_BUF_MAX_SIZE * sizeof(uint8_t));
				prepare_write_env->prepare_len = 0;
				if (prepare_write_env->prepare_buf == NULL) {
					ESP_LOGE(GATTS_TAG, "Gatt_server prep no mem\n");
					status = ESP_GATT_NO_RESOURCES;
				}
			} else {
				if (param->write.offset > PREPARE_BUF_MAX_SIZE) {
					status = ESP_GATT_INVALID_OFFSET;
				} else if ((param->write.offset + param->write.len)
						> PREPARE_BUF_MAX_SIZE) {
					status = ESP_GATT_INVALID_ATTR_LEN;
				}

			}

			esp_gatt_rsp_t *gatt_rsp = (esp_gatt_rsp_t*) malloc(
					sizeof(esp_gatt_rsp_t));

			gatt_rsp->attr_value.len = param->write.len;
			gatt_rsp->attr_value.handle = param->write.handle;
			gatt_rsp->attr_value.offset = param->write.offset;
			gatt_rsp->attr_value.auth_req = ESP_GATT_AUTH_REQ_NONE;
			memcpy(gatt_rsp->attr_value.value, param->write.value,
					param->write.len);
			esp_err_t response_err = esp_ble_gatts_send_response(gatts_if,
					param->write.conn_id, param->write.trans_id, status,
					gatt_rsp);
			if (response_err != ESP_OK) {
				ESP_LOGE(GATTS_TAG, "Send response error\n");
			}
			free(gatt_rsp);
			if (status != ESP_GATT_OK) {
				return;
			}
			memcpy(prepare_write_env->prepare_buf + param->write.offset, param->write.value, param->write.len);
			prepare_write_env->prepare_len += param->write.len;

		} else {
			esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, NULL);
		}
	}
}

void example_exec_write_event_env(prepare_type_env_t *prepare_write_env,
		esp_ble_gatts_cb_param_t *param) {
	if (param->exec_write.exec_write_flag == ESP_GATT_PREP_WRITE_EXEC) {
		esp_log_buffer_hex(GATTS_TAG, prepare_write_env->prepare_buf, prepare_write_env->prepare_len);
	} else {
		ESP_LOGI(GATTS_TAG, "ESP_GATT_PREP_WRITE_CANCEL");
	}
	if (prepare_write_env->prepare_buf) {
		free(prepare_write_env->prepare_buf);
		prepare_write_env->prepare_buf = NULL;
	}
	prepare_write_env->prepare_len = 0;
}

static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
	switch (event) {

	case ESP_GATTS_REG_EVT:

		ESP_LOGI(GATTS_TAG, "REGISTER_APP_EVT, status %d, app_id %d\n",param->reg.status, param->reg.app_id);
		gl_profile_tab[PROFILE_A_APP_ID].service_id.is_primary = true;
		gl_profile_tab[PROFILE_A_APP_ID].service_id.id.inst_id = 0x00;
		gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
		gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_TEST_A;

		esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(
		TEST_DEVICE_NAME);
		if (set_dev_name_ret) {
			ESP_LOGE(GATTS_TAG, "set device name failed, error code = %x",
					set_dev_name_ret);
		}
#ifdef CONFIG_SET_RAW_ADV_DATA
        esp_err_t raw_adv_ret = esp_ble_gap_config_adv_data_raw(raw_adv_data, sizeof(raw_adv_data));
        if (raw_adv_ret){
            ESP_LOGE(GATTS_TAG, "config raw adv data failed, error code = %x ", raw_adv_ret);
        }
        adv_config_done |= adv_config_flag;
        esp_err_t raw_scan_ret = esp_ble_gap_config_scan_rsp_data_raw(raw_scan_rsp_data, sizeof(raw_scan_rsp_data));
        if (raw_scan_ret){
            ESP_LOGE(GATTS_TAG, "config raw scan rsp data failed, error code = %x", raw_scan_ret);
        }
        adv_config_done |= scan_rsp_config_flag;
#else
		//config adv data
		esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
		if (ret) {
			ESP_LOGE(GATTS_TAG, "config adv data failed, error code = %x", ret);
		}
		adv_config_done |= adv_config_flag;
		//config scan response data
		ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
		if (ret) {
			ESP_LOGE(GATTS_TAG,
					"config scan response data failed, error code = %x", ret);
		}
		adv_config_done |= scan_rsp_config_flag;

#endif
		esp_ble_gatts_create_service(gatts_if,
				&gl_profile_tab[PROFILE_A_APP_ID].service_id,
				GATTS_NUM_HANDLE_TEST_A);
		break;
	case ESP_GATTS_READ_EVT: {
		ESP_LOGI(GATTS_TAG,
				"GATT_READ_EVT, conn_id %d, trans_id %" PRIu32 ", handle %d\n",
				param->read.conn_id, param->read.trans_id, param->read.handle);
		esp_gatt_rsp_t rsp;
		memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
		rsp.attr_value.handle = param->read.handle;
		rsp.attr_value.len = 4;
		rsp.attr_value.value[0] = 0xDE;
		rsp.attr_value.value[1] = 0xED;
		rsp.attr_value.value[2] = 0xBE;
		rsp.attr_value.value[3] = 0xEF;
		esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, &rsp);
		break;
	}

	case ESP_GATTS_WRITE_EVT: {
		ESP_LOGI(GATTS_TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %" PRIu32 ", handle %d", param->write.conn_id, param->write.trans_id, param->write.handle);
		if (!param->write.is_prep) {
			ESP_LOGI(GATTS_TAG, "GATT_WRITE_EVT, value len %d, value :", param->write.len);
			characteristic_write_handler(param->write.value, param->write.len, value1, APPLICATION_PROFILE_A);
			esp_log_buffer_hex(GATTS_TAG, param->write.value, param->write.len);
			if (gl_profile_tab[PROFILE_A_APP_ID].descr_handle == param->write.handle && param->write.len == 2) { //param->write.len == 2
				uint16_t descr_value = param->write.value[1] << 8  | param->write.value[0];

				if (descr_value == 0x0001) {
					if (a_property & ESP_GATT_CHAR_PROP_BIT_NOTIFY) {
						ESP_LOGI(GATTS_TAG, "notify enable");
						uint8_t notify_data[15];
						for (int i = 0; i < sizeof(notify_data); ++i) {
							notify_data[i] = i % 0xff;
						}
						//the size of notify_data[] need less than MTU size
						esp_ble_gatts_send_indicate(gatts_if,
								param->write.conn_id,
								gl_profile_tab[PROFILE_A_APP_ID].char_handle,
								sizeof(notify_data), notify_data, false);
					}
				} else if (descr_value == 0x0002) {
					if (a_property & ESP_GATT_CHAR_PROP_BIT_INDICATE) {
						ESP_LOGI(GATTS_TAG, "indicate enable");
						uint8_t indicate_data[15];
						for (int i = 0; i < sizeof(indicate_data); ++i) {
							indicate_data[i] = i % 0xFF;
						}
						//the size of indicate_data[] need less than MTU size
						esp_ble_gatts_send_indicate(gatts_if,
								param->write.conn_id,
								gl_profile_tab[PROFILE_A_APP_ID].char_handle,
								sizeof(indicate_data), indicate_data, true);
					}
				} else if (descr_value == 0x0000) {
					ESP_LOGI(GATTS_TAG, "notify/indicate disable ");
				} else {
					ESP_LOGE(GATTS_TAG, "unknown descr value");
					esp_log_buffer_hex(GATTS_TAG, param->write.value, param->write.len);
				}

			}
		}
		example_write_event_env(gatts_if, &a_prepare_write_env, param);
		break;


	}
	case ESP_GATTS_EXEC_WRITE_EVT:
		ESP_LOGI(GATTS_TAG, "ESP_GATTS_EXEC_WRITE_EVT");
		esp_ble_gatts_send_response(gatts_if, param->write.conn_id,
				param->write.trans_id, ESP_GATT_OK, NULL);
		example_exec_write_event_env(&a_prepare_write_env, param);
		break;
	case ESP_GATTS_MTU_EVT:
		ESP_LOGI(GATTS_TAG, "ESP_GATTS_MTU_EVT, MTU %d", param->mtu.mtu);
		break;
	case ESP_GATTS_UNREG_EVT:
		break;
	case ESP_GATTS_CREATE_EVT:
		ESP_LOGI(GATTS_TAG,
				"CREATE_SERVICE_EVT, status %d,  service_handle %d\n",
				param->create.status, param->create.service_handle);
		gl_profile_tab[PROFILE_A_APP_ID].service_handle =
				param->create.service_handle;
		gl_profile_tab[PROFILE_A_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
		gl_profile_tab[PROFILE_A_APP_ID].char_uuid.uuid.uuid16 =
		GATTS_CHAR_UUID_TEST_A;

		esp_ble_gatts_start_service(
				gl_profile_tab[PROFILE_A_APP_ID].service_handle);
		a_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE
				| ESP_GATT_CHAR_PROP_BIT_NOTIFY;
		esp_err_t add_char_ret = esp_ble_gatts_add_char(
				gl_profile_tab[PROFILE_A_APP_ID].service_handle,
				&gl_profile_tab[PROFILE_A_APP_ID].char_uuid,
				ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, a_property,
				&gatts_demo_char1_val, NULL);
		if (add_char_ret) {
			ESP_LOGE(GATTS_TAG, "add char failed, error code =%x",
					add_char_ret);
		}
		break;
	case ESP_GATTS_ADD_INCL_SRVC_EVT:
		break;
	case ESP_GATTS_ADD_CHAR_EVT: {
		uint16_t length = 0;
		const uint8_t *prf_char;

		ESP_LOGI(GATTS_TAG,
				"ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
				param->add_char.status, param->add_char.attr_handle,
				param->add_char.service_handle);
		gl_profile_tab[PROFILE_A_APP_ID].char_handle =
				param->add_char.attr_handle;
		gl_profile_tab[PROFILE_A_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
		gl_profile_tab[PROFILE_A_APP_ID].descr_uuid.uuid.uuid16 =
		ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
		esp_err_t get_attr_ret = esp_ble_gatts_get_attr_value(
				param->add_char.attr_handle, &length, &prf_char);
		if (get_attr_ret == ESP_FAIL) {
			ESP_LOGE(GATTS_TAG, "ILLEGAL HANDLE");
		}

		ESP_LOGI(GATTS_TAG, "the gatts demo char length = %x\n", length);
		for (int i = 0; i < length; i++) {
			ESP_LOGI(GATTS_TAG, "prf_char[%x] =%x\n", i, prf_char[i]);
		}
		esp_err_t add_descr_ret = esp_ble_gatts_add_char_descr(
				gl_profile_tab[PROFILE_A_APP_ID].service_handle,
				&gl_profile_tab[PROFILE_A_APP_ID].descr_uuid,
				ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, NULL, NULL);
		if (add_descr_ret) {
			ESP_LOGE(GATTS_TAG, "add char descr failed, error code =%x",
					add_descr_ret);
		}
		break;
	}
	case ESP_GATTS_ADD_CHAR_DESCR_EVT:
		gl_profile_tab[PROFILE_A_APP_ID].descr_handle =
				param->add_char_descr.attr_handle;
		ESP_LOGI(GATTS_TAG,
				"ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n",
				param->add_char_descr.status, param->add_char_descr.attr_handle,
				param->add_char_descr.service_handle);
		break;
	case ESP_GATTS_DELETE_EVT:
		break;
	case ESP_GATTS_START_EVT:
		ESP_LOGI(GATTS_TAG, "SERVICE_START_EVT, status %d, service_handle %d\n",
				param->start.status, param->start.service_handle);
		break;
	case ESP_GATTS_STOP_EVT:
		break;
	case ESP_GATTS_CONNECT_EVT: {
		esp_ble_conn_update_params_t conn_params = { 0 };
		memcpy(conn_params.bda, param->connect.remote_bda,
				sizeof(esp_bd_addr_t));
		/* For the IOS system, please reference the apple official documents about the ble connection parameters restrictions. */
		conn_params.latency = 0;
		conn_params.max_int = 0x20;    // max_int = 0x20*1.25ms = 40ms
		conn_params.min_int = 0x10;    // min_int = 0x10*1.25ms = 20ms
		conn_params.timeout = 400;    // timeout = 400*10ms = 4000ms
		ESP_LOGI(GATTS_TAG,
				"ESP_GATTS_CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:",
				param->connect.conn_id, param->connect.remote_bda[0],
				param->connect.remote_bda[1], param->connect.remote_bda[2],
				param->connect.remote_bda[3], param->connect.remote_bda[4],
				param->connect.remote_bda[5]);
		gl_profile_tab[PROFILE_A_APP_ID].conn_id = param->connect.conn_id;
		//start sent the update connection parameters to the peer device.
		esp_ble_gap_update_conn_params(&conn_params);
		break;
	}
	case ESP_GATTS_DISCONNECT_EVT:
		ESP_LOGI(GATTS_TAG, "ESP_GATTS_DISCONNECT_EVT, disconnect reason 0x%x",
				param->disconnect.reason);
		esp_ble_gap_start_advertising(&adv_params);
		break;
	case ESP_GATTS_CONF_EVT:
		ESP_LOGI(GATTS_TAG, "ESP_GATTS_CONF_EVT, status %d attr_handle %d",
				param->conf.status, param->conf.handle);
		if (param->conf.status != ESP_GATT_OK) {
			esp_log_buffer_hex(GATTS_TAG, param->conf.value, param->conf.len);
		}
		break;
	case ESP_GATTS_OPEN_EVT:
	case ESP_GATTS_CANCEL_OPEN_EVT:
	case ESP_GATTS_CLOSE_EVT:
	case ESP_GATTS_LISTEN_EVT:
	case ESP_GATTS_CONGEST_EVT:
	default:
		break;
	}
}

static void gatts_profile_b_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {

	switch (event) {
	case ESP_GATTS_REG_EVT:

		ESP_LOGI(GATTS_TAG, "REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
		gl_profile_tab[PROFILE_B_APP_ID].service_id.is_primary = true;
		gl_profile_tab[PROFILE_B_APP_ID].service_id.id.inst_id = 0x00;
		gl_profile_tab[PROFILE_B_APP_ID].service_id.id.uuid.len =
		ESP_UUID_LEN_16;
		gl_profile_tab[PROFILE_B_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_TEST_B;

		esp_ble_gatts_create_service(gatts_if, &gl_profile_tab[PROFILE_B_APP_ID].service_id, GATTS_NUM_HANDLE_TEST_B);
		break;

	case ESP_GATTS_READ_EVT: {
		ESP_LOGI(GATTS_TAG, "GATT_READ_EVT, conn_id %d, trans_id %" PRIu32 ", handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
		esp_gatt_rsp_t rsp;
		memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
		rsp.attr_value.handle = param->read.handle;
		rsp.attr_value.len = 4;
		rsp.attr_value.value[0] = 0xDE;
		rsp.attr_value.value[1] = 0xED;
		rsp.attr_value.value[2] = 0xBE;
		rsp.attr_value.value[3] = 0xEF;
		esp_ble_gatts_send_response(gatts_if, param->read.conn_id,
				param->read.trans_id, ESP_GATT_OK, &rsp);
		break;
	}
	case ESP_GATTS_WRITE_EVT: {

		ESP_LOGI(GATTS_TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %" PRIu32 ", handle %d\n", param->write.conn_id, param->write.trans_id, param->write.handle);
		if (!param->write.is_prep) {
			ESP_LOGI(GATTS_TAG, "GATT_WRITE_EVT, value len %d, value :", param->write.len);
			esp_log_buffer_hex(GATTS_TAG, param->write.value, param->write.len);
			characteristic_write_handler(param->write.value, param->write.len, value2, APPLICATION_PROFILE_B);


			if (gl_profile_tab[PROFILE_B_APP_ID].descr_handle == param->write.handle && param->write.len == 2) { // param->write.len == 2

				uint16_t descr_value = param->write.value[1] << 8 | param->write.value[0];

				if (descr_value == 0x0001) {
					if (b_property & ESP_GATT_CHAR_PROP_BIT_NOTIFY) {
						ESP_LOGI(GATTS_TAG, "notify enable");
						uint8_t notify_data[15];
						for (int i = 0; i < sizeof(notify_data); ++i) {
							notify_data[i] = i % 0xFF;
						}
						//the size of notify_data[] need less than MTU size
						esp_ble_gatts_send_indicate(gatts_if,
								param->write.conn_id,
								gl_profile_tab[PROFILE_B_APP_ID].char_handle,
								sizeof(notify_data), notify_data, false);
					}
				} else if (descr_value == 0x0002) {
					if (b_property & ESP_GATT_CHAR_PROP_BIT_INDICATE) {
						ESP_LOGI(GATTS_TAG, "indicate enable");
						uint8_t indicate_data[15];
						for (int i = 0; i < sizeof(indicate_data); ++i) {
							indicate_data[i] = i % 0xFF;
						}
						//the size of indicate_data[] need less than MTU size
						esp_ble_gatts_send_indicate(gatts_if,
								param->write.conn_id,
								gl_profile_tab[PROFILE_B_APP_ID].char_handle,
								sizeof(indicate_data), indicate_data, true);
					}
				} else if (descr_value == 0x0000) {
					ESP_LOGI(GATTS_TAG, "notify/indicate disable ");
				} else {
					ESP_LOGE(GATTS_TAG, "unknown value");
				}

			}
		}
		example_write_event_env(gatts_if, &b_prepare_write_env, param);
		break;
	}
	case ESP_GATTS_EXEC_WRITE_EVT:
		ESP_LOGI(GATTS_TAG, "ESP_GATTS_EXEC_WRITE_EVT");
		esp_ble_gatts_send_response(gatts_if, param->write.conn_id,
				param->write.trans_id, ESP_GATT_OK, NULL);
		example_exec_write_event_env(&b_prepare_write_env, param);
		break;
	case ESP_GATTS_MTU_EVT:
		ESP_LOGI(GATTS_TAG, "ESP_GATTS_MTU_EVT, MTU %d", param->mtu.mtu);
		break;
	case ESP_GATTS_UNREG_EVT:
		break;
	case ESP_GATTS_CREATE_EVT:
		ESP_LOGI(GATTS_TAG,
				"CREATE_SERVICE_EVT, status %d,  service_handle %d\n",
				param->create.status, param->create.service_handle);
		gl_profile_tab[PROFILE_B_APP_ID].service_handle =
				param->create.service_handle;
		gl_profile_tab[PROFILE_B_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
		gl_profile_tab[PROFILE_B_APP_ID].char_uuid.uuid.uuid16 =
		GATTS_CHAR_UUID_TEST_B;

		esp_ble_gatts_start_service(
				gl_profile_tab[PROFILE_B_APP_ID].service_handle);
		b_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE
				| ESP_GATT_CHAR_PROP_BIT_NOTIFY;
		esp_err_t add_char_ret = esp_ble_gatts_add_char(
				gl_profile_tab[PROFILE_B_APP_ID].service_handle,
				&gl_profile_tab[PROFILE_B_APP_ID].char_uuid,
				ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, b_property,
				NULL, NULL);
		if (add_char_ret) {
			ESP_LOGE(GATTS_TAG, "add char failed, error code =%x",
					add_char_ret);
		}
		break;
	case ESP_GATTS_ADD_INCL_SRVC_EVT:
		break;
	case ESP_GATTS_ADD_CHAR_EVT:
		ESP_LOGI(GATTS_TAG,
				"ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
				param->add_char.status, param->add_char.attr_handle,
				param->add_char.service_handle);

		gl_profile_tab[PROFILE_B_APP_ID].char_handle =
				param->add_char.attr_handle;
		gl_profile_tab[PROFILE_B_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
		gl_profile_tab[PROFILE_B_APP_ID].descr_uuid.uuid.uuid16 =
		ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
		esp_ble_gatts_add_char_descr(
				gl_profile_tab[PROFILE_B_APP_ID].service_handle,
				&gl_profile_tab[PROFILE_B_APP_ID].descr_uuid,
				ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
				NULL, NULL);
		break;
	case ESP_GATTS_ADD_CHAR_DESCR_EVT:
		gl_profile_tab[PROFILE_B_APP_ID].descr_handle =
				param->add_char_descr.attr_handle;
		ESP_LOGI(GATTS_TAG, "ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n", param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);
		break;
	case ESP_GATTS_DELETE_EVT:
		break;
	case ESP_GATTS_START_EVT:
		ESP_LOGI(GATTS_TAG, "SERVICE_START_EVT, status %d, service_handle %d\n",
				param->start.status, param->start.service_handle);
		break;
	case ESP_GATTS_STOP_EVT:
		break;
	case ESP_GATTS_CONNECT_EVT:
		ESP_LOGI(GATTS_TAG,
				"CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:",
				param->connect.conn_id, param->connect.remote_bda[0],
				param->connect.remote_bda[1], param->connect.remote_bda[2],
				param->connect.remote_bda[3], param->connect.remote_bda[4],
				param->connect.remote_bda[5]);
		gl_profile_tab[PROFILE_B_APP_ID].conn_id = param->connect.conn_id;
		break;
	case ESP_GATTS_CONF_EVT:
		ESP_LOGI(GATTS_TAG, "ESP_GATTS_CONF_EVT status %d attr_handle %d",
				param->conf.status, param->conf.handle);
		if (param->conf.status != ESP_GATT_OK) {
			esp_log_buffer_hex(GATTS_TAG, param->conf.value, param->conf.len);
		}
		break;
	case ESP_GATTS_DISCONNECT_EVT:
	case ESP_GATTS_OPEN_EVT:
	case ESP_GATTS_CANCEL_OPEN_EVT:
	case ESP_GATTS_CLOSE_EVT:
	case ESP_GATTS_LISTEN_EVT:
	case ESP_GATTS_CONGEST_EVT:
	default:
		break;
	}
}

static void gatts_event_handler(esp_gatts_cb_event_t event,
		esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
	/* If event is register event, store the gatts_if for each profile */
	if (event == ESP_GATTS_REG_EVT) {
		if (param->reg.status == ESP_GATT_OK) {
			gl_profile_tab[param->reg.app_id].gatts_if = gatts_if;
		} else {
			ESP_LOGI(GATTS_TAG, "Reg app failed, app_id %04x, status %d\n",
					param->reg.app_id, param->reg.status);
			return;
		}
	}

	/* If the gatts_if equal to profile A, call profile A cb handler,
	 * so here call each profile's callback */
	do {
		int idx;
		for (idx = 0; idx < PROFILE_NUM; idx++) {
			if (gatts_if == ESP_GATT_IF_NONE || /* ESP_GATT_IF_NONE, not specify a certain gatt_if, need to call every profile cb function */
			gatts_if == gl_profile_tab[idx].gatts_if) {
				if (gl_profile_tab[idx].gatts_cb) {
					gl_profile_tab[idx].gatts_cb(event, gatts_if, param);
				}
			}
		}
	} while (0);
}

void app_main(void) {

	esp_err_t ret;

	// Initialize NVS.
	ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	//
	ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

	//BT controller is initialized with default settings described in esp_bt.h
	esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
	ret = esp_bt_controller_init(&bt_cfg); //error handling

	if (ret) {
		ESP_LOGE(GATTS_TAG, "%s initialize controller failed: %s\n", __func__,
				esp_err_to_name(ret));
		return;
	}

	// choosing BLE mode over Classic/BTDM/idle
	ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
	if (ret) {
		ESP_LOGE(GATTS_TAG, "%s enable controller failed: %s\n", __func__,
				esp_err_to_name(ret));
		return;
	}
	// blutooth config done

	//Blutooth turned on
	ret = esp_bluedroid_init();
	if (ret) {
		ESP_LOGE(GATTS_TAG, "%s init bluetooth failed: %s\n", __func__,
				esp_err_to_name(ret));
		return;
	}
	// don't know the difference between these two
	ret = esp_bluedroid_enable();
	if (ret) {
		ESP_LOGE(GATTS_TAG, "%s enable bluetooth failed: %s\n", __func__,
				esp_err_to_name(ret));
		return;
	}
	//GATT event handler, in which event what will happen for GATT
	ret = esp_ble_gatts_register_callback(gatts_event_handler);
	if (ret) {
		ESP_LOGE(GATTS_TAG, "gatts register error, error code = %x", ret);
		return;
	}
	//GAP event handler, the GAP event handler defines what will happen in different situation of GAP
	ret = esp_ble_gap_register_callback(gap_event_handler);
	if (ret) {
		ESP_LOGE(GATTS_TAG, "gap register error, error code = %x", ret);
		return;
	}

	//Here 2 application is running inside the service
	//GATT's app register starts for  A app

	ret = esp_ble_gatts_app_register(PROFILE_A_APP_ID);
	if (ret) {
		ESP_LOGE(GATTS_TAG, "gatts app register error, error code = %x", ret);
		return;
	}
	//GATT's app register starts for  B app
	ret = esp_ble_gatts_app_register(PROFILE_B_APP_ID);
	if (ret) {
		ESP_LOGE(GATTS_TAG, "gatts app register error, error code = %x", ret);
		return;
	}
	//how much bits can be transmitted each packet is set here
	esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(500);
	if (local_mtu_ret) {
		ESP_LOGE(GATTS_TAG, "set local  MTU failed, error code = %x",
				local_mtu_ret);
	}

	return;

}



static void characteristic_write_handler(uint8_t *value, uint16_t len, uint8_t *storage, uint8_t profile) {

//    memcpy(storage, value, len);
//    printf("Received Value: %s\n", storage);
//
    if(profile== APPLICATION_PROFILE_A){
    	nvs_write_data((char*)storage, "SSID", len+1);
//    	printf("Saved SSID: %s\n", storage);
//    	storage[len] = '\0';
   }
//
   if(profile == APPLICATION_PROFILE_B){
	   nvs_write_data((char*)storage, "PASS", len+1);
// 	    printf("Saved Password: %s\n", storage);
// 	   storage[len] = '\0';
      }

      nvs_read_data( "SSID" );
  	  nvs_read_data( "PASS" );

     //store_wifi_Credential("sm_deco",  "sales152152" , sizeof("sm_deco") , sizeof("sales152152"));



}