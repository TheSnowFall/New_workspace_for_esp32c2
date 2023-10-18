/*
 * notification.h
 *
 *  Created on: Sep 11, 2023
 *      Author: MN
 */

#ifndef MAIN_NOTIFICATION_H_
#define MAIN_NOTIFICATION_H_





#define MSG_CONFIG_DATA_STORED_SUCCESS "{\"config\":\"Data Stored Successfully\"}"
#define MSG_WIFI_CONNECTED "{\"wifi\":\"Connected\"}"
#define CONFIG_MODEL_SRT "{\"model\":\"SLA_HPWH_BL602_1.0\"}"
// #define MSG_CONFIG_DATA_BROKEN "{\"config\":\"DataBroken\"}"
// #define MSG_CONFIG_DATA_CAN_NOT_STORE "{\"config\":\"Data Can Not Store\"}"
// #define MSG_WIFI_NOT_CONNECT "{\"wifi\":\"Not Connected\"}"
// #define MSG_SERVER_CONNECTED "{\"Server\":\"Connected\"}"
// #define MSG_SERVER_NOT_CONNECTED "{\"Server\":\"Not Connected\"}"
#define MSG_MQTT_CONNECTED "{\"mqtt\":\"Connected\"}"
// #define MSG_MQTT_NOT_CONNECTED "{\"mqtt\":\"Connected\"}"




#define NVS_resp_write_ok 1
#define WiFi_resp_conn_ok 2
#define BLE_resp_conn_ok  3



void notification_for(uint8_t response_flag);



#endif /* MAIN_NOTIFICATION_H_ */
