#include "notification.h"

//esp_gatt_if_t gatts_if = ESP_GATT_IF_NONE;
    //uint16_t conn_id = ;
  //  uint16_t attr_handle =;


//esp_err_t esp_ble_gatts_send_indicate(esp_gatt_if_t gatts_if, uint16_t conn_id, uint16_t attr_handle,
//                                                  uint16_t value_len, uint8_t *value, bool need_confirm)



void notification_for(uint8_t response_flag){

	if (response_flag == BLE_resp_conn_ok ){

		//esp_err_t esp_ble_gatts_send_indicate(esp_gatt_if_t gatts_if, uint16_t conn_id, uint16_t attr_handle,
		//


			}
	if (response_flag == WiFi_resp_conn_ok){

		//esp_err_t esp_ble_gatts_send_indicate(esp_gatt_if_t gatts_if, uint16_t conn_id, uint16_t attr_handle,
		printf("[NOTIFICATION.C][NOTIFICATION_FOR] WiFi response check\n ");
				}
	if (response_flag == NVS_resp_write_ok ){

		//esp_err_t esp_ble_gatts_send_indicate(esp_gatt_if_t gatts_if, uint16_t conn_id, uint16_t attr_handle,
		//
				}

}









