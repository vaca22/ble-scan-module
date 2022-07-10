#ifndef __EPAPER_H__
#define __EPAPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"

typedef struct epaper_display {
    spi_host_device_t spi_host;
    spi_device_handle_t bus;

    gpio_num_t miso_pin;
    gpio_num_t mosi_pin;
    gpio_num_t sclk_pin;
    gpio_num_t cs_pin;

    gpio_num_t dc_pin;
    gpio_num_t reset_pin;
    gpio_num_t busy_pin;

    uint8_t dc_level;
    uint8_t cmd_level;
    uint8_t data_level;
    uint8_t reset_level;
    uint8_t busy_level;

    uint16_t width;
    uint16_t height;
    uint16_t power_on_time;
    uint16_t power_off_time;
    uint16_t full_refresh_time;

    uint8_t refresh_count;
    bool hibernating;
    bool is_power_on;
} epaper_display_t;

esp_err_t epaper_display_constructor(epaper_display_t *dev);
esp_err_t epaper_display_init(epaper_display_t *dev);
esp_err_t epaper_display_set_window(epaper_display_t *dev, uint16_t x1,
                                    uint16_t y1, uint16_t x2, uint16_t y2);
esp_err_t epaper_display_clear(epaper_display_t *dev, uint8_t black_value);
esp_err_t epaper_display_write_buffer(epaper_display_t *dev, uint8_t *buff,
                                      uint16_t len);
esp_err_t epaper_display_refresh_quick(epaper_display_t *dev);
esp_err_t epaper_display_refresh_slow(epaper_display_t *dev);
esp_err_t epaper_display_power_on(epaper_display_t *dev);
esp_err_t epaper_display_power_off(epaper_display_t *dev);
esp_err_t epaper_display_power_hibernate(epaper_display_t *dev);
esp_err_t epaper_display_reset(epaper_display_t *dev);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __EPAPER_H__ */