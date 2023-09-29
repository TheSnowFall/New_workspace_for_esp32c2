#include "cjson_parser.h"
#include "nvs.h"
#include "wifi_station.h"

const char *WIFi_TAG = "wifi station";

char *ssid;
char *pass;

uint8_t BLE_packet_parser(uint8_t *buffer) {

	printf("Data Received From BLE:\n");

	cJSON *root = cJSON_Parse((char*) buffer);
	if (root == NULL) {
		printf("Error parsing JSON.\n");
		return 0;
	}

	cJSON *ssid_item = cJSON_GetObjectItem(root, "ssid");
	cJSON *pass_item = cJSON_GetObjectItem(root, "pass");
//	cJSON *uuid_item = cJSON_GetObjectItem(root, "uuid");
//	cJSON *topic_item = cJSON_GetObjectItem(root, "topic");

	if (ssid_item && pass_item) {

//    	 esp_wifi_ssid = ssid_item ;
		ssid = cJSON_GetObjectItem(root, "ssid")->valuestring;
		pass = cJSON_GetObjectItem(root, "pass")->valuestring;

		printf(
				"[cJSON_PARSER.C][BLE_PACKET_PARSER]SSID LEN: %d, PASS LEN: %d\n",
				strlen(ssid), strlen(pass));

		NVS_set_wifi_credential(ssid, pass, strlen(ssid), strlen(pass));

//		esp_err_t nvs_set_wifi_cred= NVS_set_wifi_credential(ssid, pass, strlen(ssid), strlen(pass));
//		if(ESP_OK){
//
//		}

	}

//        printf("SSID: %s\n", ssid_item->valuestring);
//        printf("Password: %s\n", pass_item->valuestring);
//        printf("UUID: %s\n", uuid_item->valuestring);
//        printf("Topic: %s\n", topic_item->valuestring);
// }
	else {
		printf(
				"[cJSON_PARSER.C][BLE_PACKET_PARSER] JSON format is incorrect.\n");
	}



	cJSON_Delete(root);


//	if(check_init_starting_status == true){
//
//
//		esp_err_t result_discon = discon_wifi();
//			if (result_discon == ESP_OK) {
//			    // WiFi stopped successfully.
//			} else {
//			    // Handle the error, e.g., print an error message or take appropriate action.
//			}
//
//
//
//	esp_err_t result_stop = stop_wifi();
//	if (result_stop == ESP_OK) {
//	    // WiFi stopped successfully.
//	} else {
//	    // Handle the error, e.g., print an error message or take appropriate action.
//	}
//
//
//
//
//	esp_err_t result_deinit = deinit_wifi();
//	if (result_deinit == ESP_OK) {
//	    // WiFi stopped successfully.
//	} else {
//	    // Handle the error, e.g., print an error message or take appropriate action.
//	}
//
//	}

	ESP_LOGI(WIFi_TAG, "ESP_WIFI_MODE_STA");
	wifi_init_sta();

	return 1;
}

