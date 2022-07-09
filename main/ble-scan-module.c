#include <stdio.h>
#include <esp_err.h>
#include <nvs_flash.h>
#include "ble_master.h"
#include "wifi-lgh.h"

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    start_scan();
    wifi_init_sta();
}
