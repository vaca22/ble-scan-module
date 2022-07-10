#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"

#include "esp_timer.h"
#include "panel.h"
#include "i2c_bus.h"
#include "board.h"
#include "esp_log.h"

#define TAG "CHSC6X"

#define _release(handle) {BaseType_t _t; xSemaphoreGiveFromISR(handle, &_t); }

#define _lock(handle, time) xSemaphoreTake(handle, time)

#define CHSC6X_I2C_ID 0x5c

static i2c_bus_handle_t  i2c_handle;
static panel_config_t    chsc6x_config;
static bool              chsc6x_inited = false;  
static uint64_t          touch_event_time = 0;      

static int chsc6x_read_bytes_u16addr_sub(unsigned char id, unsigned short adr, unsigned char *rxbuf, unsigned short lenth)
{
    adr = (adr >> 8) | ((adr & 0xFF) << 8);
    int ret = i2c_bus_read_bytes(i2c_handle, id, (uint8_t*)&adr, sizeof(adr), rxbuf, lenth);
    return ret;
}

static int chsc6x_write_bytes_u16addr_sub(unsigned char id, unsigned short adr, unsigned char *rxbuf, unsigned short lenth)
{
    adr = (adr >> 8) | ((adr & 0xFF) << 8);
    int ret = i2c_bus_write_bytes(i2c_handle, id, (uint8_t*)&adr, sizeof(adr), rxbuf, lenth);
    return ret;
}

static void chsc6x_tp_reset(void)
{
    if (chsc6x_config.reset_gpio != -1) {
        gpio_set_level(chsc6x_config.reset_gpio, 0);
        msleep(50);
        gpio_set_level(chsc6x_config.reset_gpio, 1);
        msleep(50);
    }
}

static void chsc6x_resume(void)
{
    chsc6x_tp_reset();
    ESP_LOGI(TAG, "touch_resumed\n");
}

static void chsc6x_suspend(void)
{
    unsigned char buft[1] = {0};
    int ret = chsc6x_write_bytes_u16addr_sub(CHSC6X_I2C_ID, 0xa503, buft, 0);
    if(ret == 0) {
        ESP_LOGI(TAG, "touch_suspend OK \r\n");
    } else{
        ESP_LOGI(TAG, "touch_suspend failed \r\n");
    }
}

static int i2c_init(i2c_port_t port)
{
    int res = 0;
    i2c_config_t es_i2c_cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };
    res = get_i2c_pins(port, &es_i2c_cfg);
    if (res != 0) {
        return res;
    }
    i2c_handle = i2c_bus_create(port, &es_i2c_cfg);
    return i2c_handle ? 0 : -1;
}

static void IRAM_ATTR touch_isr_handler(void* arg) {
    touch_event_time = esp_timer_get_time() / 1000;
}

static void chsc6x_enable_int(bool enable) {
    if (chsc6x_config.int_gpio < 0) {
        return;
    }
    if (enable) {
        gpio_config_t io_config = {
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = 1,
            .pin_bit_mask = (uint64_t)1 << chsc6x_config.int_gpio,
        };
        io_config.intr_type = GPIO_INTR_NEGEDGE;
        gpio_config(&io_config);
        gpio_set_intr_type(chsc6x_config.int_gpio, GPIO_INTR_NEGEDGE);
        gpio_install_isr_service(0);
        gpio_isr_handler_add(chsc6x_config.int_gpio, touch_isr_handler, NULL);
        int v = gpio_get_level(chsc6x_config.int_gpio);
        printf("gpio %02x level %d\n", chsc6x_config.int_gpio, v);
    }
    else {
        gpio_set_intr_type(chsc6x_config.int_gpio, GPIO_INTR_DISABLE);
        gpio_isr_handler_remove(chsc6x_config.int_gpio);
    }
}

static int chsc6x_check() {
    unsigned char regData[3] = {1,1,1};
    if (chsc6x_read_bytes_u16addr_sub(CHSC6X_I2C_ID, 0x01, regData, 3)) {
        ESP_LOGE(TAG, "Read addr 0x1 fail\n");
        return -1;
    }
    return 0;
}

static int chsc6x_init(panel_config_t* cfg)
{
    int ret = 0;
    chsc6x_config = *cfg;

    gpio_config_t  io_conf;
    memset(&io_conf, 0, sizeof(io_conf));
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = BIT64(chsc6x_config.reset_gpio);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    ret = i2c_init(cfg->i2c_port);
    if (ret != 0) {
        ESP_LOGE(TAG, "init I2S port %d fail", cfg->i2c_port);
        return ret;
    }
    chsc6x_tp_reset();
    ret = chsc6x_check();
    if (ret != 0) {
        ESP_LOGE(TAG, "Fail to check CHSC6x");
        return ret;
    }
    chsc6x_enable_int(true);
    chsc6x_inited = true;
    return 0;
}

static int chsc6x_read_coords(int32_t x[], int32_t y[]) {
    if (chsc6x_inited == false) {
        return -1;
    }
    uint64_t cur_event_time = esp_timer_get_time() / 1000;
    //touch happen and within 1s
    if (cur_event_time < touch_event_time + 1000) {
        uint8_t data[11];
        uint8_t adr = 0x02;
        data[0] = 0;
        i2c_bus_read_bytes(i2c_handle, CHSC6X_I2C_ID, &adr, sizeof(adr), data, sizeof(data));
        if (data[0] && data[0] < 2) {
            x[0] = ((data[1] << 8) | data[2]) & 0x0fff;
            y[0] = ((data[3] << 8) | data[4]) & 0x0fff;
            if (x[0] > 152 || y[0] > 296) {
                return 0;
            }
            if (data[0] == 2) {
                x[1] = ((data[7] << 8) | data[8]) & 0x0fff;
                y[1] = ((data[9] << 8) | data[10]) & 0x0fff;
            }
            return data[0];
        }
    }
    return 0;
}

static int chsc6x_power(bool power) {
    if (chsc6x_inited == false) {
        return -1;
    }
    if (power) {
        chsc6x_resume();
    }
    else {
        chsc6x_suspend();
    }
    return 0;
}

static int chsc6x_deinit() {
    if (chsc6x_inited == false) {
        return -1;
    }
    chsc6x_enable_int(false);
    return 0;
}

panel_impl_t panel_chsc6x = {
    .init = chsc6x_init,
    .coords = chsc6x_read_coords,
    .power_on = chsc6x_power,
    .deinit = chsc6x_deinit,
};
