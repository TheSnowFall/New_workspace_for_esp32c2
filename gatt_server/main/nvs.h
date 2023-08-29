/*
 * nvs.h
 *
 *  Created on: Aug 29, 2023
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

void  nvs_init(void);

esp_err_t nvs_write_data(  char* key , char* my_input, uint8_t length );
esp_err_t nvs_read_data(char* key );
esp_err_t store_wifi_Credential(char* ssid,  char* pass , uint8_t len_ssid , uint8_t len_pass);




#endif /* MAIN_NVS_H_ */
