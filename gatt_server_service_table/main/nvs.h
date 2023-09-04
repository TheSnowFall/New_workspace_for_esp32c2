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

void  nvs_init(void);
esp_err_t save_led_states(uint8_t led_state);
esp_err_t nvs_variable_save(char* value,  char* key , size_t length );
esp_err_t value_read(char* value,  char* key );


#endif /* MAIN_NVS_H_ */



