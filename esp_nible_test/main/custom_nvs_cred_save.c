#include "custom_nvs_cred_save.h"
//#include "notification.h"



esp_err_t NVS_store_key(char *key, char *value, size_t length) {
	nvs_handle_t my_handle;
	esp_err_t err;

	// Open
	err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
	if (err != ESP_OK)
		return err;

	// Read the size of memory space required for blob

	size_t required_size = 0;  // value will default to 0, if not set yet in NVS
	err = nvs_get_blob(my_handle, key, NULL, &required_size);

	if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
		return err;

	err = nvs_set_blob(my_handle, key, value, length);

	// Commit
	err = nvs_commit(my_handle);
	if (err != ESP_OK)
		return err;

	// Close
	nvs_close(my_handle);
//	notification_for(NVS_resp_write_ok);
	return ESP_OK;
}

esp_err_t NVS_read_key(char *key, char* return_buff) {

	nvs_handle_t my_handle;
	esp_err_t err;

	// Open
	err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
	if (err != ESP_OK)
		return err;

	// Read the size of memory space required for blob

	size_t required_size = 0;  // value will default to 0, if not set yet in NVS
	err = nvs_get_blob(my_handle, key, NULL, &required_size);
	if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
		return err;
	// Help me vai to print not getting it how to set the logic

	char *value_read = malloc(required_size);

	err = nvs_get_blob(my_handle, key, value_read, &required_size);
	if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
		return err;

//	printf("[NVS.C][NVS_READ_KEY] Reading Value from NVS for the key: %s and the Value : %.*s\n", key,
//			(int) required_size, value_read);

	memcpy(return_buff,value_read,required_size);

//
//	if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
//		return err;

	free(value_read);
	// Close
	nvs_close(my_handle);
	return ESP_OK;
}

esp_err_t NVS_set_wifi_credential(char *ssid, char *pass, uint8_t ssid_len, uint8_t pass_len) {
	esp_err_t wifi_cred_saving_error;
	wifi_cred_saving_error = NVS_store_key(WIFI_SSID_KEY, ssid, ssid_len);
	if (wifi_cred_saving_error == ESP_OK) {
		printf("[NVS.C][NVS_SET_WIFI_CRED.] SSID  : %s LEN: %d\n", ssid, ssid_len);

	} else {
		printf("[NVS.C][NVS_SET_WIFI_CRED.] ERROR STORING SSID : %s\n", ssid);
		return wifi_cred_saving_error;
	}
	wifi_cred_saving_error = NVS_store_key(WIFI_PASS_KEY, pass, pass_len);
	if (wifi_cred_saving_error == ESP_OK) {
		printf("[NVS.C][NVS_SET_WIFI_CRED.] PASSWORD : %s LEN: %d\n", pass, pass_len);
	} else {
		printf("[NVS.C][NVS_SET_WIFI_CRED.] ERROR STORING PASSWORD : %s\n", pass);
		return wifi_cred_saving_error;
	}

	return ESP_OK;
}

esp_err_t NVS_get_wifi_credential(char *ssid, char *pass) {

	esp_err_t wifi_cred_getting_error = NVS_read_key(WIFI_SSID_KEY,ssid);

	if (wifi_cred_getting_error == ESP_OK) {
		printf("[NVS.C][NVS_GET_WIFI_CRED.] Reading SSID from NVS is successful\n");
	} else {
		printf("[NVS.C][NVS_GET_WIFI_CRED.] Reading SSID from NVS is unsuccessful\n");
	}

	wifi_cred_getting_error = NVS_read_key(WIFI_PASS_KEY,pass);

	if (wifi_cred_getting_error == ESP_OK) {
		printf("[NVS.C][NVS_GET_WIFI_CRED.] Reading pass from NVS is successful\n");
	} else {
		printf("[NVS.C][NVS_GET_WIFI_CRED.] Reading pass from NVS is unsuccessful\n");
	}

	printf("[NVS.C][NVS_GET_WIFI_CRED.] Reading Data From NVS: \n\t\t<%s> :<%s>\n\t\t<%s> :<%s>\n", WIFI_SSID_KEY, ssid,WIFI_PASS_KEY,pass);


	return ESP_OK;
}

esp_err_t NVS_set_mqtt_credential(char *topic, char *uuid, uint8_t topic_len, uint8_t uuid_len) {
	esp_err_t mqtt_cred_saving_error = NVS_store_key(MQTT_TOPIC_KEY, topic, topic_len);
	if (mqtt_cred_saving_error == ESP_OK) {
		printf("[NVS.C][NVS_SET_MQTT_CRED.] TOPIC  : %s LEN: %d\n", topic, topic_len);

	} else {
		printf("[NVS.C][NVS_SET_MQTT_CRED.] ERROR STORING TOPIC : %s\n", topic);
		return mqtt_cred_saving_error;
	}
	mqtt_cred_saving_error = NVS_store_key(MQTT_UUID_KEY, uuid, uuid_len);
	if (mqtt_cred_saving_error == ESP_OK) {
		printf("[NVS.C][NVS_SET_MQTT_CRED.] UUID  : %s LEN: %d\n", uuid, uuid_len);
	} else {
		printf("[NVS.C][NVS_SET_MQTT_CRED.] ERROR STORING UUID : %s\n", uuid);
		return mqtt_cred_saving_error;
	}

	return ESP_OK;
}

esp_err_t NVS_get_mqtt_credential(char *topic, char *uuid) {

	esp_err_t mqtt_cred_getting_error = NVS_read_key(MQTT_TOPIC_KEY,topic);

	if (mqtt_cred_getting_error == ESP_OK) {
		printf("[NVS.C][NVS_GET_MQTT_CRED.] Reading TOPIC from NVS is successful\n");
	} else {
		printf("[NVS.C][NVS_GET_MQTT_CRED.] Reading TOPIC from NVS is unsuccessful\n");
	}

	mqtt_cred_getting_error = NVS_read_key(MQTT_UUID_KEY,uuid);

	if (mqtt_cred_getting_error == ESP_OK) {
		printf("[NVS.C][NVS_GET_MQTT_CRED.] Reading uuid from NVS is successful\n");
	} else {
		printf("[NVS.C][NVS_GET_MQTT_CRED.] Reading uuid from NVS is unsuccessful\n");
	}

	printf("[NVS.C][NVS_GET_MQTT_CRED.] Reading Data From NVS: \n\t\t<%s> :<%s>\n\t\t<%s> :<%s>\n", MQTT_TOPIC_KEY, topic,MQTT_UUID_KEY,uuid);


	return ESP_OK;
}
