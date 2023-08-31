/*
 * my_bt_event_handler.h
 *
 *  Created on: Aug 31, 2023
 *      Author: MN
 */

#ifndef MAIN_MY_BT_EVENT_HANDLER_H_
#define MAIN_MY_BT_EVENT_HANDLER_H_

#include "my_bt_profile_based_handler.h" // I guess this need to be removed



typedef struct {
				uint8_t *prepare_buf;
				int prepare_len;
			   } prepare_type_env_t;



void example_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);

void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);






#endif /* MAIN_MY_BT_EVENT_HANDLER_H_ */
