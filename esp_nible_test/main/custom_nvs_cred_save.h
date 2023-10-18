/*
 * custom_nvs_cred_save.h
 *
 *  Created on: Sep 4, 2023
 *      Author: MN
 */

#ifndef MAIN_CUSTOM_NVS_CRED_SAVE_H_
#define MAIN_CUSTOM_NVS_CRED_SAVE_H_



//#include <stdio.h>
//#include <stdbool.h>
//#include <unistd.h>
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "freertos/queue.h"
//#include "driver/gpio.h"
//#include "esp_log.h"
//#include "sdkconfig.h"
//#include "esp_system.h"
#include "nvs_flash.h"
#include "string.h"

#define STORAGE_NAMESPACE "storage"
#define WIFI_SSID_KEY "W_SSID"
#define WIFI_PASS_KEY "W_PASS"
#define MQTT_TOPIC_KEY "M_TOPIC"
#define MQTT_UUID_KEY "M_UUID"

void  nvs_init(void);


esp_err_t NVS_store_key(char* key,  char* value , size_t length );
esp_err_t NVS_read_key(char *key, char* return_buff);

esp_err_t NVS_set_wifi_credential(char *ssid, char *pass, uint8_t ssid_len, uint8_t pass_len);
esp_err_t NVS_get_wifi_credential(char* ssid, char* pass );

esp_err_t NVS_get_mqtt_credential(char *topic, char *uuid);
esp_err_t NVS_set_mqtt_credential(char *topic, char *uuid, uint8_t topic_len, uint8_t uuid_len);


#endif /* MAIN_CUSTOM_NVS_CRED_SAVE_H_ */



