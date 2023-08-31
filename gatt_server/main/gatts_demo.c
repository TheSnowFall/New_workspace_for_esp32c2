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

#include "my_bt_profile_based_handler.h"
#include "my_bt_event_handler.h"
#include "gap_handler.h"
#include "gatt_handler.h"


#define GATTS_TAG "MY_GATTS_DEMO_TAG"

///Declare the   function


  //void characteristic_write_handler(uint8_t *value, uint16_t len, uint8_t *storage);

  uint8_t value1[50] = "";
  uint8_t value2[50] = "";








#define TEST_MANUFACTURER_DATA_LEN  17u





// Global variable to store the received data

char received_buffer[PREPARE_BUF_MAX_SIZE];
int received_buffer_length = 0;







//  uint8_t raw_scan_rsp_data[] = {
//    0x0fu, 0x09u, 0x45u, 0x53u, 0x50u, 0x5fu, 0x47u, 0x41u, 0x54u, 0x54u, 0x53u, 0x5fu, 0x44u,
//    0x45u, 0x4du, 0x4fu
//};





/* One gatt-based profile one app_id and one gatts_if, this array will store the gatts_if returned by ESP_GATTS_REG_EVT */







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



//  void characteristic_write_handler(uint8_t *value, uint16_t len, uint8_t *storage) {
//
//    memcpy(storage, value, len);
//    storage[len] = '\0'; // Null-terminate the string
//    printf("Received Value: %s\n", storage);
//}
