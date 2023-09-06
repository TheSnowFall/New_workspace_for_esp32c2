#include "cjson_parser.h"
#include "nvs.h"
#include "wifi_station.h"

const char *WIFi_TAG = "wifi station";
char *ssid ;
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

		printf("SSID LEN: %d, PASS LEN: %d\n",strlen(ssid), strlen(pass));

		NVS_set_wifi_credential(ssid,pass,strlen(ssid),strlen(pass));
		char ss[64],pas[64];
		NVS_get_wifi_credential(ss,pas);


	}


//        printf("SSID: %s\n", ssid_item->valuestring);
//        printf("Password: %s\n", pass_item->valuestring);
//        printf("UUID: %s\n", uuid_item->valuestring);
//        printf("Topic: %s\n", topic_item->valuestring);
// }
	else {
		printf("JSON format is incorrect.\n");
	}

//        esp_err_t read_my_result = nvs_variable_read( "ssid" );
//
//        if(read_my_result==ESP_OK){
//        	printf("Reading ssid from NVS is successful\n");
//        }
//        else{printf("Reading ssid from NVS is unsuccessful\n");
//        }
//
//         read_my_result = nvs_variable_read( "pass" );
//
//              if(read_my_result==ESP_OK){
//              	printf("Reading pass from NVS is successful\n");
//              }
//              else{printf("Reading pass from NVS is unsuccessful\n");
//              }
//
//
//
//               read_my_result = nvs_variable_read( "topic" );
//
//                    if(read_my_result==ESP_OK){
//                    	printf("Reading topic from NVS is successful\n");
//                    }
//                    else{printf("Reading topic from NVS is unsuccessful\n");
//                    }
//
//
//                 read_my_result = nvs_variable_read( "uuid" );
//
//                          if(read_my_result==ESP_OK){
//                          	printf("Reading uuid from NVS is successful\n");
//                          }
//                          else{printf("Reading uuid from NVS is unsuccessful\n");
//                          }


	cJSON_Delete(root);

//	ESP_LOGI(WIFi_TAG, "ESP_WIFI_MODE_STA");
//	wifi_init_sta();

	return 1;
}

