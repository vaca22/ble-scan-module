#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/i2s.h"
#include <string.h>

esp_err_t get_i2c_pins(i2c_port_t port, i2c_config_t *i2c_config)
{
    if (port == I2C_NUM_0) {
        i2c_config->sda_io_num = GPIO_NUM_18;
        i2c_config->scl_io_num = GPIO_NUM_23;
    }
    else if (port == I2C_NUM_1) {
        i2c_config->sda_io_num = GPIO_NUM_13;
        i2c_config->scl_io_num = GPIO_NUM_12;
    }
    else {
        i2c_config->sda_io_num = -1;
        i2c_config->scl_io_num = -1;
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t get_i2s_pins(i2s_port_t port, i2s_pin_config_t *i2s_config)
{
    if (port == I2S_NUM_0) {
        i2s_config->mck_io_num = -1;
        i2s_config->bck_io_num = GPIO_NUM_5;
        i2s_config->ws_io_num = GPIO_NUM_25;
        i2s_config->data_out_num = GPIO_NUM_26;
        i2s_config->data_in_num = GPIO_NUM_35;
    } else if (port == I2S_NUM_1) {
        i2s_config->bck_io_num = GPIO_NUM_32;
        i2s_config->ws_io_num = GPIO_NUM_33;
        i2s_config->data_out_num = -1;
        i2s_config->data_in_num = GPIO_NUM_36;
    } else {
        memset(i2s_config, -1, sizeof(i2s_pin_config_t));
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t i2s_mclk_gpio_select(i2s_port_t i2s_num, gpio_num_t gpio_num)
{
    if (i2s_num >= I2S_NUM_MAX) {
        return ESP_ERR_INVALID_ARG;
    }
    if (gpio_num != GPIO_NUM_0 && gpio_num != GPIO_NUM_1 && gpio_num != GPIO_NUM_3) {
        return ESP_ERR_INVALID_ARG;
    }
    if (i2s_num == I2S_NUM_0) {
        if (gpio_num == GPIO_NUM_0) {
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
            WRITE_PERI_REG(PIN_CTRL, 0xFFF0);
        } else if (gpio_num == GPIO_NUM_1) {
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD_CLK_OUT3);
            WRITE_PERI_REG(PIN_CTRL, 0xF0F0);
        } else {
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_U0RXD_CLK_OUT2);
            WRITE_PERI_REG(PIN_CTRL, 0xFF00);
        }
    } else if (i2s_num == I2S_NUM_1) {
        if (gpio_num == GPIO_NUM_0) {
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
            WRITE_PERI_REG(PIN_CTRL, 0xFFFF);
        } else if (gpio_num == GPIO_NUM_1) {
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD_CLK_OUT3);
            WRITE_PERI_REG(PIN_CTRL, 0xF0FF);
        } else {
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_U0RXD_CLK_OUT2);
            WRITE_PERI_REG(PIN_CTRL, 0xFF0F);
        }
    }
    return ESP_OK;
}

void msleep(int ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}
