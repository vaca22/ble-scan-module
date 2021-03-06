#include <stdio.h>
#include <esp_err.h>
#include <nvs_flash.h>
#include <hal/gpio_hal.h>
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>
#include <esp_log.h>
#include "ble_master.h"
#include "wifi-lgh.h"
#include "system-lgh.h"

#define GPIO_OUTPUT_IO_LED    3
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_LED))

void setIo32() {
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    gpio_set_level(GPIO_OUTPUT_IO_LED, 0);
}
void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    start_scan();
//    wifi_init_sta();
    setIo32();
    int64_t aa= lc_low_system_timestamp_get();
    ESP_LOGE("na","%lld",aa);
    while (1){
        gpio_set_level(GPIO_OUTPUT_IO_LED, 0);
        vTaskDelay(200);
        gpio_set_level(GPIO_OUTPUT_IO_LED, 1);
        vTaskDelay(200);
    }
}
