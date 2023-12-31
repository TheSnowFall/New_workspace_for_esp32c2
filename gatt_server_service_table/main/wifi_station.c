#include "wifi_station.h"
#include "nvs.h"
#include "notification.h"

//const char *WIFi_TAG = "wifi station" ;

EventGroupHandle_t s_wifi_event_group;


bool check_init_starting_status = false;

static int s_retry_num = 0;

void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id,
		void *event_data) {
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT
			&& event_id == WIFI_EVENT_STA_DISCONNECTED) {
		if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
			esp_wifi_connect();
			s_retry_num++;
			ESP_LOGI(WIFi_TAG, "retry to connect to the AP");
		} else {
			xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
		}
		ESP_LOGI(WIFi_TAG, "connect to the AP fail");
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t *event = (ip_event_got_ip_t*) event_data;
		ESP_LOGI(WIFi_TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
		s_retry_num = 0;
		xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
	}
}

void wifi_init_sta(void) {

//	if (!check_init_starting_status){

	s_wifi_event_group = xEventGroupCreate();

	ESP_ERROR_CHECK(esp_netif_init());

	ESP_ERROR_CHECK(esp_event_loop_create_default());

	check_init_starting_status = true;

//	}
	esp_netif_create_default_wifi_sta();



	wifi_init_config_t cfg =
					WIFI_INIT_CONFIG_DEFAULT();
					ESP_ERROR_CHECK(esp_wifi_init(&cfg));

					esp_event_handler_instance_t instance_any_id;
					esp_event_handler_instance_t instance_got_ip;
					ESP_ERROR_CHECK(
							esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
					ESP_ERROR_CHECK(
							esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

					/*
					 * NVS SSID AND PASS Read
					 * Data allaready saved in NVS from BLE Rechive event in CJSON
					 *
					 */
	uint8_t ss[64]= {0,},
	pas[64] = { 0, };

	NVS_get_wifi_credential((char*) ss, (char*) pas);
	printf(
			"[WIFI_ST.C][WIFI_INIT_STA] Reading Data From NVS: \n\t\t<%s> :<%s>\n",
			ss, pas);

	wifi_config_t wifi_config = { .sta = { .ssid = "", .password = "",
	/* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
	 * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
	 * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
	 * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
	 */
	.threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD, .sae_pwe_h2e =
			WPA3_SAE_PWE_BOTH, }, };

	memcpy(wifi_config.sta.ssid, ss, sizeof(ss));
	memcpy(wifi_config.sta.password, pas, sizeof(pas));

	printf(
			"[WIFI_ST.C][WIFI_INIT_STA] Reading Data From NVS: \n\t\t<%s> :<%s>\n\t\t<%s> :<%s>\n",
			ss, wifi_config.sta.ssid, pas, wifi_config.sta.password);

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());

	ESP_LOGI(WIFi_TAG, "wifi_init_sta finished.");
//	check_init_starting_status = true;
//
//	}


	/* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
	 * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
	EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
	WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
	pdFALSE,
	pdFALSE,
	portMAX_DELAY);

	/* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
	 * happened. */
	if (bits & WIFI_CONNECTED_BIT) {
		ESP_LOGI(WIFi_TAG, "connected to ap SSID:%s password:%s",
				EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);

	} else if (bits & WIFI_FAIL_BIT) {
		ESP_LOGI(WIFi_TAG, "Failed to connect to SSID:%s, password:%s",
				EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
	} else {
		ESP_LOGE(WIFi_TAG, "UNEXPECTED EVENT");
	}

	notification_for(WiFi_resp_conn_ok);


//	if(!check_init_starting_status){
//		check_init_starting_status == true ;
//	}
//
//	else{
//		check_init_starting_status == false ;
//	}


}

esp_err_t stop_wifi(void) {
	esp_err_t wifi_stop_result =  esp_wifi_stop();

	if (wifi_stop_result == ESP_OK) {
		return ESP_OK; // WiFi stopped successfully.
	} else {
		return wifi_stop_result; // Return the error code.
	}
}

esp_err_t discon_wifi(void) {
	esp_err_t wifi_discon_result = esp_wifi_disconnect();

	if (wifi_discon_result == ESP_OK) {
		return ESP_OK; // WiFi stopped successfully.
	} else {
		return wifi_discon_result; // Return the error code.
	}
}

esp_err_t deinit_wifi(void) {
	esp_err_t wifi_deinit_result = esp_wifi_deinit();

	if (wifi_deinit_result == ESP_OK) {
		return ESP_OK; // WiFi stopped successfully.
	} else {
		return wifi_deinit_result; // Return the error code.
	}
}


