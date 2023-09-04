#include "cjson_parser.h"
#include "nvs.h"

uint8_t BLE_packet_parser(uint8_t* buffer){

    printf("Data Received From BLE:\n");

    cJSON *root = cJSON_Parse((char*)buffer);
    if (root == NULL) {
        printf("Error parsing JSON.\n");
        return 0;
    }

    cJSON *ssid_item = cJSON_GetObjectItem(root, "ssid");
    cJSON *pass_item = cJSON_GetObjectItem(root, "pass");
    cJSON *uuid_item = cJSON_GetObjectItem(root, "uuid");
    cJSON *topic_item = cJSON_GetObjectItem(root, "topic");



    if (ssid_item) {
    	 char *ssid = cJSON_GetStringValue(ssid_item);
   	     esp_err_t error_in_parse = nvs_variable_save("ssid", ssid, strlen(ssid));
   	     if (error_in_parse == ESP_OK){
   	    	 printf("SSID is saved. Saved data: %s\n", ssid);
   	     }
   	     else {
   	    	printf("SSID could not be saved\n");
   	     }
    }


    if (pass_item) {
        	 char *pass = cJSON_GetStringValue(pass_item);
       	     esp_err_t error_in_parse = nvs_variable_save("pass", pass, strlen(pass));
       	     if (error_in_parse == ESP_OK){
       	    	 printf("Password is saved. Saved data: %s\n", pass);
       	     }
       	     else {
       	    	printf("Password could not be saved\n");
       	     }
        }
    if (uuid_item) {
        	 char *uuid = cJSON_GetStringValue(uuid_item);
       	     esp_err_t error_in_parse = nvs_variable_save("uuid", uuid, strlen(uuid));
       	     if (error_in_parse == ESP_OK){
       	    	 printf("uuid is saved. Saved data: %s\n", uuid);
       	     }
       	     else {
       	    	printf("uuid could not be saved\n");
       	     }
        }

    if (topic_item) {
        	 char *topic = cJSON_GetStringValue(topic_item);
       	     esp_err_t error_in_parse = nvs_variable_save("topic", topic, strlen(topic));
       	     if (error_in_parse == ESP_OK){
       	    	 printf("topic is saved. Saved data: %s\n", topic);
       	     }
       	     else {
       	    	printf("topic could not be saved\n");
       	     }
        }
//        printf("SSID: %s\n", ssid_item->valuestring);
//        printf("Password: %s\n", pass_item->valuestring);
//        printf("UUID: %s\n", uuid_item->valuestring);
//        printf("Topic: %s\n", topic_item->valuestring);
// }
    else {
        printf("JSON format is incorrect.\n");
    }

    cJSON_Delete(root);

    return 1;
}

