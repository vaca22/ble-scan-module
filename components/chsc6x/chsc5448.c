#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_idf_lib_helpers.h>

#include "chsc5448.h"

#define I2C_FREQ_HZ 400000  // 400kHz

#define CHECK(x)                           \
    do {                                   \
        esp_err_t __;                      \
        if ((__ = x) != ESP_OK) return __; \
    } while (0)
#define CHECK_ARG(VAL)                          \
    do {                                        \
        if (!(VAL)) return ESP_ERR_INVALID_ARG; \
    } while (0)

static const char *TAG = "chsc6x";

esp_err_t chsc6x_init_desc(chsc6x_t *dev, uint8_t addr, i2c_port_t port,
                           gpio_num_t sda_gpio, gpio_num_t scl_gpio) {
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "chsc6x_init_desc");
    CHECK_ARG(dev);

    dev->i2c_dev.port           = port;
    dev->i2c_dev.addr           = addr;
    dev->i2c_dev.cfg.sda_io_num = sda_gpio;
    dev->i2c_dev.cfg.scl_io_num = scl_gpio;
#if HELPER_TARGET_IS_ESP32
    dev->i2c_dev.cfg.master.clk_speed = I2C_FREQ_HZ;
#endif

    return i2c_dev_create_mutex(&dev->i2c_dev);
}

esp_err_t chsc6x_free_desc(chsc6x_t *dev) {
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "chsc6x_free_desc");
    CHECK_ARG(dev);

    return i2c_dev_delete_mutex(&dev->i2c_dev);
}

static void IRAM_ATTR gpio_isr_handler(void *arg) {
    // chsc6x_t *dev          = (chsc6x_t *)arg;
    // dev->coords.is_touched = true;
}

esp_err_t chsc6x_init(chsc6x_t *dev) {
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "chsc6x_init");
    CHECK_ARG(dev);

    gpio_config_t io_conf = {};

    if (dev->power_pin != -1) {
        io_conf.intr_type    = GPIO_INTR_DISABLE;
        io_conf.mode         = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = (1ULL << dev->power_pin);
        io_conf.pull_down_en = 0;
        io_conf.pull_up_en   = 0;
        gpio_config(&io_conf);
    }

    if (dev->reset_pin != -1) {
        io_conf.intr_type    = GPIO_INTR_DISABLE;
        io_conf.mode         = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = (1ULL << dev->reset_pin);
        io_conf.pull_down_en = 0;
        io_conf.pull_up_en   = 0;
        gpio_config(&io_conf);
    }

    if (dev->interrupt_pin != -1) {
        io_conf.intr_type    = GPIO_INTR_NEGEDGE;
        io_conf.mode         = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = (1ULL << dev->interrupt_pin);
        io_conf.pull_down_en = 0;
        io_conf.pull_up_en   = 1;
        gpio_config(&io_conf);
        gpio_install_isr_service(0);
        gpio_isr_handler_add(dev->interrupt_pin, gpio_isr_handler, (void *)dev);
    }

    chsc6x_reset(dev);

    return chsc6x_check(dev);
}

esp_err_t chsc6x_reset(chsc6x_t *dev) {
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "chsc6x_reset");
    CHECK_ARG(dev);

    if (dev->reset_pin != -1) {
        gpio_set_level(dev->reset_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_set_level(dev->reset_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    return ESP_OK;
}

esp_err_t chsc6x_check(chsc6x_t *dev) {
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "chsc6x_check");
    CHECK_ARG(dev);

    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
    uint8_t temp_reg[2] = {0x00, 0x01};
    if (i2c_dev_write(&dev->i2c_dev, NULL, 0, temp_reg, 2) != ESP_OK) {
        ESP_LOGE(TAG, "check write reg 0x00 0x01 error");
        return ESP_FAIL;
    }

    uint8_t temp_data[3] = {0};
    if (i2c_dev_read(&dev->i2c_dev, NULL, 0, temp_data, 3) != ESP_OK) {
        ESP_LOGE(TAG, "check read status error");
        return ESP_FAIL;
    }

    uint8_t chsc6x_id_reg[4] = {0x20, 0x00, 0x00, 0x18};
    if (i2c_dev_write(&dev->i2c_dev, NULL, 0, chsc6x_id_reg, 4) != ESP_OK) {
        ESP_LOGE(TAG, "check write reg 0x20, 0x00, 0x00, 0x18 error");
        return ESP_FAIL;
    }

    uint8_t chsc6x_id_data[4] = {0};
    if (i2c_dev_read(&dev->i2c_dev, NULL, 0, chsc6x_id_data, 4) != ESP_OK) {
        ESP_LOGE(TAG, "check read id error");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "check id %x %x %x %x", chsc6x_id_data[0], chsc6x_id_data[1],
             chsc6x_id_data[2], chsc6x_id_data[3]);
    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    return ESP_OK;
}

esp_err_t chsc6x_read_coords(chsc6x_t *dev) {
    // ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "chsc6x_read_coords");
    CHECK_ARG(dev);

    static uint8_t chsc6x_coords_reg[4]    = {0x20, 0x00, 0x00, 0x2c};
    static uint8_t chsc6x_coords_data[256] = {0};

    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
    if (i2c_dev_write(&dev->i2c_dev, NULL, 0, chsc6x_coords_reg, 4) != ESP_OK) {
        ESP_LOGE(TAG, "read_coords write coords error");
    }

    memset((char *)chsc6x_coords_data, 0, sizeof(chsc6x_coords_data));
    if (i2c_dev_read(&dev->i2c_dev, NULL, 0, chsc6x_coords_data, 256) !=
        ESP_OK) {
        ESP_LOGE(TAG, "read_coords read coords error");
    }

    if (chsc6x_coords_data[1] == 1) {
        dev->coords.is_touched = true;
        uint8_t y_hight_4bit = chsc6x_coords_data[5] / 16;
        uint8_t x_hight_4bit = chsc6x_coords_data[5] % 16;

        dev->coords.y =
            (unsigned int)(x_hight_4bit << 8 | chsc6x_coords_data[2]);
        dev->coords.x =
            296 - (unsigned int)(y_hight_4bit << 8 | chsc6x_coords_data[3]);

        ESP_LOGV(TAG, "x: %d y: %d is_touched: %s interrupt: %d", dev->coords.x,
                 dev->coords.y, dev->coords.is_touched ? "true" : "false",
                 gpio_get_level(dev->interrupt_pin));
    }

    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    return ESP_OK;
}