#include "cjson_parser.h"

#include "custom_nvs_cred_save.h"
//#include "wifi_station.h"

const char *WIFi_TAG = "wifi station";

char *ssid;
char *pass;
char *uuid;
char *topic;

void BLE_packet_parser(char *buffer) {



	cJSON *root = cJSON_Parse((char*) buffer);
	if (root == NULL) {
		printf("[cJSON_PARSER.C][BLE_PACKET_PARSER] Error parsing JSON.\n");

	}

	cJSON *ssid_item = 	cJSON_GetObjectItem(root, "ssid");
	cJSON *pass_item =	cJSON_GetObjectItem(root, "pass");
	cJSON *uuid_item = 	cJSON_GetObjectItem(root, "uuid");
	cJSON *topic_item = cJSON_GetObjectItem(root, "topic");

	if (ssid_item && pass_item && uuid_item && topic_item) {


		ssid = 	cJSON_GetObjectItem(root, "ssid")->valuestring;
		pass = 	cJSON_GetObjectItem(root, "pass")->valuestring;
		uuid = 	cJSON_GetObjectItem(root, "uuid")->valuestring;
		topic = cJSON_GetObjectItem(root, "topic")->valuestring;

//		printf(
//				"[cJSON_PARSER.C][BLE_PACKET_PARSER] All Credentials Below:  \n  SSID : %s, \n  PASS : %s,  \n  UUID : %s,  \n Topic : %s \n",ssid,pass,uuid,topic );




		esp_err_t nvs_set_wifi_cred_error= NVS_set_wifi_credential(ssid, pass, strlen(ssid), strlen(pass));
		esp_err_t nvs_set_mqtt_cred_error= NVS_set_mqtt_credential(topic,uuid, strlen(topic), strlen(uuid));
		if(nvs_set_wifi_cred_error == ESP_OK && nvs_set_mqtt_cred_error == ESP_OK){
			printf("[cJSON_PARSER.C][BLE_PACKET_PARSER] WIFI & MQTT Credentials Saved . \n \n");
			//wifi_init_sta();
		}
		else {
			printf("[cJSON_PARSER.C][BLE_PACKET_PARSER] WIFI & MQTT Credentials could not be saved \n \n");

		}


 }
	else {
		printf(
				"[cJSON_PARSER.C][BLE_PACKET_PARSER] See the commas and brackets of JSON.\n");
	}



	cJSON_Delete(root);




}

