

#ifndef MAIN_MY_BT_PROFILE_BASED_HANDLER_H_
#define MAIN_MY_BT_PROFILE_BASED_HANDLER_H_

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


#include "my_bt_event_handler.h" // I guess this need to be removed

#define GATTS_TAG "MY_GATTS_DEMO_TAG"

#define PROFILE_NUM 2
#define PROFILE_A_APP_ID 0
#define PROFILE_B_APP_ID 1


#define GATTS_SERVICE_UUID_TEST_A   0x00FFu
#define GATTS_CHAR_UUID_TEST_A      0xFF01u
#define GATTS_DESCR_UUID_TEST_A     0x3333u
#define GATTS_NUM_HANDLE_TEST_A     4u

#define GATTS_SERVICE_UUID_TEST_B   0x00EEu
#define GATTS_CHAR_UUID_TEST_B      0xEE01u
#define GATTS_DESCR_UUID_TEST_B     0x2222u
#define GATTS_NUM_HANDLE_TEST_B     4u

#define TEST_DEVICE_NAME            "Erfan's_GATTS_Tryout"



#define PREPARE_BUF_MAX_SIZE 1024u
#define GATTS_DEMO_CHAR_VAL_LEN_MAX 0x40u


 uint8_t adv_config_done = 0u;

#define adv_config_flag      (1u << 0)
#define scan_rsp_config_flag (1u << 1)




 uint8_t char1_str[] = { 0x11u, 0x22u, 0x33u };
 esp_gatt_char_prop_t a_property = 0u;
 esp_gatt_char_prop_t b_property = 0u;


 prepare_type_env_t a_prepare_write_env;
 prepare_type_env_t b_prepare_write_env;



esp_attr_value_t gatts_demo_char1_val = {
											.attr_max_len = GATTS_DEMO_CHAR_VAL_LEN_MAX,
											.attr_len = sizeof(char1_str),
											.attr_value =char1_str,
                                         };


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



static uint8_t adv_service_uuid128[32] = {
		/* LSB <--------------------------------------------------------------------------------> MSB */
		//first uuid, 16bit, [12],[13] is the value
		0xFBu, 0x34u, 0x9Bu, 0x5Fu, 0x80u, 0x00u, 0x00u, 0x80u, 0x00u, 0x10u,
		0x00u, 0x00u, 0xEEu, 0x00u, 0x00u, 0x00u,
		/* second uuid, 32bit, [12], [13], [14], [15] is the value */
		0xFBu, 0x34u, 0x9Bu, 0x5Fu, 0x80u, 0x00u, 0x00u, 0x80u, 0x00u, 0x10u,
		0x00u, 0x00u, 0xFFu, 0x00u, 0x00u, 0x00u };

		// The length of adv data must be less than 31 bytes
		//  uint8_t test_manufacturer[TEST_MANUFACTURER_DATA_LEN] =  {0x12, 0x23, 0x45, 0x56};


		// scan response data
		  esp_ble_adv_data_t scan_rsp_data = { .set_scan_rsp = true,
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

		  esp_ble_adv_params_t adv_params = { .adv_int_min = 0x20u,
												   .adv_int_max =0x40u,
												   .adv_type = ADV_TYPE_IND,
												   .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
												   //.peer_addr            =
												   //.peer_addr_type       =
												   .channel_map = ADV_CHNL_ALL, .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
												};






//adv data

esp_ble_adv_data_t adv_data = { 	   .set_scan_rsp = false,
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







void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
void gatts_profile_b_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);









 struct gatts_profile_inst gl_profile_tab[PROFILE_NUM] = {
		[PROFILE_A_APP_ID] = { .gatts_cb = gatts_profile_a_event_handler,
		                       .gatts_if = ESP_GATT_IF_NONE, /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
		                      },

		[PROFILE_B_APP_ID] = { .gatts_cb = gatts_profile_b_event_handler, /* This demo does not implement, similar as profile A */
							   .gatts_if = ESP_GATT_IF_NONE, /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
		                     },

                                                                   };







#endif /* MAIN_MY_BT_PROFILE_BASED_HANDLER_H_ */
