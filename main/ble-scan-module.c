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
#include "audio.h"
#include "sdcard.h"
#include "lv_api_map.h"
#include "lv_port.h"
#include "led.h"
#include "udp_log.h"


#define LV_TICK_PERIOD_MS 10

SemaphoreHandle_t xGuiSemaphore;

extern void ui_init_root(void);
static void lv_tick_task(void *arg) {
    (void)arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}


static void guiTask(void *pvParameter) {
    (void)pvParameter;
    xGuiSemaphore = xSemaphoreCreateMutex();

    lv_init();

    lv_disp_port_init();
    lv_indev_port_init();

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &lv_tick_task, .name = "periodic_gui"};
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(
            esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    /* UI start from here :) */
    ui_init_root();

    while (1) {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
    }

    vTaskDelete(NULL);
}





void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
//    start_scan();
    wifi_init_sta();
    init_udp_logging();
   // setIo32();
    int64_t aa= lc_low_system_timestamp_get();
    ESP_LOGE("na","%lld",aa);
    ESP_ERROR_CHECK(i2cdev_init());
    sd_card_init();
//    audio_es8311_init();
//    audio_cs5230e_init();
//    audio_cs5230e_enable();
   // xTaskCreatePinnedToCore(audio_echo_test2, "audio_echo_test2", 8192, NULL, 5, NULL, 1);


    xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 6, NULL, 0, NULL, 0);

    startLedTask();

//    while (1){
//        gpio_set_level(GPIO_OUTPUT_IO_LED, 0);
//        vTaskDelay(200);
//        gpio_set_level(GPIO_OUTPUT_IO_LED, 1);
//        vTaskDelay(200);
//    }
}
