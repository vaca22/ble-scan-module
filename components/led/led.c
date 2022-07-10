//
// Created by vaca on 7/10/22.
//

#include "led.h"
#include <hal/gpio_hal.h>
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>


#define GPIO_OUTPUT_IO_LED    3
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_LED))

void led_test(void *args) {
    while (1){
        gpio_set_level(GPIO_OUTPUT_IO_LED, 1);
        vTaskDelay(25);
        gpio_set_level(GPIO_OUTPUT_IO_LED, 0);
        vTaskDelay(25);
    }
}
void initLed() {
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
    gpio_set_level(GPIO_OUTPUT_IO_LED, 0);
}


void startLedTask(){
    initLed();
    xTaskCreatePinnedToCore(led_test, "led_test", 4096 , NULL, 0, NULL, 0);
}