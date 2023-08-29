#include "nvs.h"


void  nvs_init(void){
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		// NVS partition was truncated and needs to be erased
		// Retry nvs_flash_init
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK(err);
}



esp_err_t  nvs_write_data ( char* key , char* credential,  uint8_t length ) {

	nvs_handle_t my_handle;
	esp_err_t err;
	printf("[W] VALUE: %s , KEY: %s , LENGTH: %d ,\n",credential,key,length );
	// Open
	err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
	if (err != ESP_OK)
		return err;

		// Read the size of memory space required for blob

	size_t required_size = 0;  // credential will default to 0, if not set yet in NVS
	err = nvs_get_blob(my_handle, key, NULL, &required_size);
	printf("[W] Previous Value size: %d\n",required_size);

	printf("[W] Require Size : %d - To Save: %s\n",length,credential);

	// Help me vai to print not getting it how to set the logic


	if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
		return err;

	err = nvs_set_blob(my_handle, key, credential, length);

	// Commit
	err = nvs_commit(my_handle);
	if (err != ESP_OK)
		return err;

	// Close
	nvs_close(my_handle);
	return ESP_OK;
}


esp_err_t  nvs_read_data( char* key ) {

	nvs_handle_t my_handle;
	esp_err_t err;

	// Open
	err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
	if (err != ESP_OK)
		return err;


		// Read the size of memory space required for blob

	size_t required_size = 0;  // credential will default to 0, if not set yet in NVS
	err = nvs_get_blob(my_handle, key, NULL, &required_size);
	if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
		return err;
	printf("[R] Require Size : %d -  To read: %s\n",required_size,key );

	char* credential_received = malloc(required_size );


	err = nvs_get_blob(my_handle, key, credential_received, &required_size);
	if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
		return err;

	printf("KEY: %s VALUE: %s\n",key, credential_received);

//
//	if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
//		return err;

	// Close
	nvs_close(my_handle);
	return ESP_OK;
}



esp_err_t  store_wifi_Credential(char* ssid,  char* pass , uint8_t len_ssid , uint8_t len_pass) {

	esp_err_t err;

	err = nvs_write_data( ssid, "SSID", len_ssid );

	  if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND){

            printf("[NVS] [WIFI Credential] SSID Storing ERROR\n");
	  		return err;
	  }


	  err =nvs_write_data(pass, "PASS"  ,  len_pass );

	  if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND){
		  printf("[NVS] [WIFI Credential] PASS Storing ERROR\n");
	  		return err;
	  }


	  nvs_read_data( "SSID" );
	  nvs_read_data( "PASS" );




	  return ESP_OK;


}



