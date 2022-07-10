#ifndef __CHSC6X_H__
#define __CHSC6X_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include "i2cdev.h"

typedef struct {
    int x;
    int y;
    volatile bool is_touched;
} coords_t;

typedef struct {
    i2c_dev_t i2c_dev;
    gpio_num_t interrupt_pin;
    gpio_num_t reset_pin;
    gpio_num_t power_pin;
    coords_t coords;
} chsc6x_t;

esp_err_t chsc6x_init_desc(chsc6x_t *dev, uint8_t addr, i2c_port_t port,
                           gpio_num_t sda_gpio, gpio_num_t scl_gpio);
esp_err_t chsc6x_free_desc(chsc6x_t *dev);

esp_err_t chsc6x_init(chsc6x_t *dev);
esp_err_t chsc6x_reset(chsc6x_t *dev);
esp_err_t chsc6x_check(chsc6x_t *dev);
esp_err_t chsc6x_read_coords(chsc6x_t *dev);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __CHSC6X_H__ */