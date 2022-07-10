
#include "driver/i2c.h"
#include "driver/i2s.h"

#define LOW_I2S_PORT 0

#define PA_ENABLE_GPIO            GPIO_NUM_19

#define ES8311_MCLK_SOURCE        1   /* 0 From MCLK of esp32   1 From BCLK */

esp_err_t get_i2c_pins(i2c_port_t port, i2c_config_t *i2c_config);

esp_err_t get_i2s_pins(i2s_port_t port, i2s_pin_config_t *i2s_config);

esp_err_t i2s_mclk_gpio_select(i2s_port_t i2s_num, gpio_num_t gpio_num);

void msleep(int ms);