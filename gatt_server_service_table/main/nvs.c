#include "nvs.h"

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

esp_err_t nvs_variable_save(char *key, char *value,  size_t length) {
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
	return ESP_OK;
}
