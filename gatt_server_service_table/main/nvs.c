#include "nvs.h"
#include "notification.h"

void nvs_init(void) {
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		// NVS partition was truncated and needs to be erased
		// Retry nvs_flash_init
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK(err);
}

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
	notification_for(NVS_resp_write_ok);
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

	printf("[NVS.C][NVS_READ_KEY] Reading Value from NVS for the key: %s and the Value : %.*s\n", key,
			(int) required_size, value_read);

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
	esp_err_t rc;
	rc = NVS_store_key(WIFI_SSID_KEY, ssid, ssid_len);
	if (rc == ESP_OK) {
		printf("[NVS.C][NVS_SET_WIFI_CRED.] SSID STORED SUCCESS: %s LEN: %d\n", ssid, ssid_len);

	} else {
		printf("[NVS.C][NVS_SET_WIFI_CRED.] SSID STORED ERROR: %s\n", ssid);
		return rc;
	}
	rc = NVS_store_key(WIFI_PASS_KEY, pass, pass_len);
	if (rc == ESP_OK) {
		printf("[NVS.C][NVS_SET_WIFI_CRED.] SSID STORED SUCCESS: %s LEN: %d\n", pass, pass_len);
	} else {
		printf("[NVS.C][NVS_SET_WIFI_CRED.] SSID STORED ERROR: %s\n", pass);
		return rc;
	}

	return ESP_OK;
}


esp_err_t NVS_get_wifi_credential(char *ssid, char *pass) {
//char ssid_buff[64]={0,};
//char pass_buff[64]={0,};
	esp_err_t read_my_result_ssid = NVS_read_key(WIFI_SSID_KEY,ssid);

	if (read_my_result_ssid == ESP_OK) {
		printf("[NVS.C][NVS_GET_WIFI_CRED.] Reading SSID from NVS is successful\n");
	} else {
		printf("[NVS.C][NVS_GET_WIFI_CRED.] Reading SSID from NVS is unsuccessful\n");
	}

	read_my_result_ssid = NVS_read_key(WIFI_PASS_KEY,pass);

	if (read_my_result_ssid == ESP_OK) {
		printf("[NVS.C][NVS_GET_WIFI_CRED.] Reading pass from NVS is successful\n");
	} else {
		printf("[NVS.C][NVS_GET_WIFI_CRED.] Reading pass from NVS is unsuccessful\n");
	}

	printf("[NVS.C][NVS_GET_WIFI_CRED.] Reading Data From NVS: \n\t\t<%s> :<%s>\n\t\t<%s> :<%s>\n", WIFI_SSID_KEY, ssid,WIFI_PASS_KEY,pass);


	return ESP_OK;
}

