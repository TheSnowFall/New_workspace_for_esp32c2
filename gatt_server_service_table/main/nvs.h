/*
 * nvs.h
 *
 *  Created on: Sep 4, 2023
 *      Author: MN
 */

#ifndef MAIN_NVS_H_
#define MAIN_NVS_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "string.h"

#define STORAGE_NAMESPACE "storage"
#define WIFI_SSID_KEY "WSSID"
#define WIFI_PASS_KEY "WPASS"

void  nvs_init(void);
esp_err_t save_led_states(uint8_t led_state);
esp_err_t NVS_store_key(char* key,  char* value , size_t length );
esp_err_t NVS_read_key(char *key, char* return_buff);
esp_err_t NVS_set_wifi_credential(char *ssid, char *pass, uint8_t ssid_len, uint8_t pass_len);
esp_err_t NVS_get_wifi_credential(char* ssid, char* pass );


#endif /* MAIN_NVS_H_ */



