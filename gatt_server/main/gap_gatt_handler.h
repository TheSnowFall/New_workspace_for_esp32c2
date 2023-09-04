/*
 * gatt_handler.h
 *
 *  Created on: Aug 31, 2023
 *      Author: MN
 */

#ifndef MAIN_GAP_GATT_HANDLER_H_
#define MAIN_GAP_GATT_HANDLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

#include <inttypes.h>
#include "sdkconfig.h"
#include "my_bt_event_handler.h"



#define GATTS_TAG "MY_GATTS_DEMO_TAG"


void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);

void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

#endif /* MAIN_GAP_GATT_HANDLER_H_ */
