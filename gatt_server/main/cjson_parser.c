#include "cjson_parser.h"


uint8_t BLE_packet_parser(uint8_t* buffer){

    printf("Data Received From BLE:\n");

    cJSON *root = cJSON_Parse((char*)buffer);
    if (root == NULL) {
        printf("Error parsing JSON.\n");
        return 0;
    }

    cJSON *ssid_item = cJSON_GetObjectItem(root, "ssid");
    cJSON *pass_item = cJSON_GetObjectItem(root, "pass");


    if (ssid_item && pass_item) {
        printf("SSID: %s\n", ssid_item->valuestring);
        printf("Password: %s\n", pass_item->valuestring);
    } else {
        printf("JSON format is incorrect.\n");
    }

    cJSON_Delete(root);

    return 0;
}


