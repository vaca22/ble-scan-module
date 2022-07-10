#include "epaper.h"

#define TAG "epaper-display"

static esp_err_t epaper_gpio_init(epaper_display_t *dev);
static esp_err_t epaper_spi_init(epaper_display_t *dev);
static void epaper_spi_pre_transfer_callback(spi_transaction_t *t);
static esp_err_t epaper_send_cmd(epaper_display_t *dev, const uint8_t cmd);
static esp_err_t epaper_send_byte(epaper_display_t *dev, const uint8_t byte);
static esp_err_t epaper_send_buffer(epaper_display_t *dev, const uint8_t *buf,
                                    int len);
static esp_err_t epaper_initialization_sequence(epaper_display_t *dev);
static esp_err_t epaper_wait_busy(epaper_display_t *dev, int timeout);
static esp_err_t epaper_set_partial_ram_area(epaper_display_t *dev, uint16_t x,
                                             uint16_t y, uint16_t w,
                                             uint16_t h);

esp_err_t epaper_display_constructor(epaper_display_t *dev) {
    assert(dev != NULL);
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "epaper_display_constructor");

    epaper_gpio_init(dev);
    epaper_spi_init(dev);

    epaper_display_reset(dev);
    return epaper_display_init(dev);
}

esp_err_t epaper_display_init(epaper_display_t *dev) {
    assert(dev != NULL);
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "epaper_display_init");

    epaper_initialization_sequence(dev);
    return ESP_OK;
}

esp_err_t epaper_display_set_window(epaper_display_t *dev, uint16_t x1,
                                    uint16_t y1, uint16_t x2, uint16_t y2) {
    assert(dev != NULL);
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "epaper_display_clear");

    x1 = fmax(0, fmin(dev->width, x1));
    x2 = fmax(0, fmin(dev->width - 1, x2));
    y1 = fmax(0, fmin(dev->height, y1));
    y2 = fmax(0, fmin(dev->height - 1, y2));

    ESP_LOGI(TAG, "window: x1: %d y1: %d x2: %d y2: %d", x1, y1, x2, y2);
    return epaper_set_partial_ram_area(dev, x1, y1, x2, y2);
}

esp_err_t epaper_display_clear(epaper_display_t *dev, uint8_t black_value) {
    assert(dev != NULL);
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "epaper_display_clear");

    epaper_display_power_on(dev);
    epaper_display_set_window(dev, 0, 0, dev->width, dev->height);
    epaper_send_cmd(dev, 0x24);
    for (uint32_t i = 0; i < (dev->width * dev->height / 8); i++) {
        epaper_send_byte(dev, black_value);
    }
    epaper_send_cmd(dev, 0x26);
    epaper_display_refresh_quick(dev);
    return ESP_OK;
}

esp_err_t epaper_display_write_buffer(epaper_display_t *dev, uint8_t *buff,
                                      uint16_t len) {
    assert(dev != NULL);
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__,
             "epaper_display_write_buffer");

    epaper_display_power_on(dev);
    epaper_display_set_window(dev, 0, 0, dev->width, dev->height);
    epaper_send_cmd(dev, 0x24);
    epaper_send_buffer(dev, buff, len);
    epaper_send_cmd(dev, 0x26);
    if (dev->refresh_count++ > 20) {
        epaper_display_refresh_slow(dev);
        dev->refresh_count = 0;
    }
    epaper_display_refresh_quick(dev);
    return ESP_OK;
}

esp_err_t epaper_display_refresh_quick(epaper_display_t *dev) {
    assert(dev != NULL);
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__,
             "epaper_display_refresh_quick");

    epaper_send_cmd(dev, 0x22);
    epaper_send_byte(dev, 0xFF);
    epaper_send_cmd(dev, 0x20);
    epaper_wait_busy(dev, dev->full_refresh_time);
    return ESP_OK;
}

esp_err_t epaper_display_refresh_slow(epaper_display_t *dev) {
    assert(dev != NULL);
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__,
             "epaper_display_refresh_slow");

    epaper_send_cmd(dev, 0x22);
    epaper_send_byte(dev, 0xF7);
    epaper_send_cmd(dev, 0x20);
    epaper_wait_busy(dev, dev->full_refresh_time);
    return ESP_OK;
}

esp_err_t epaper_display_power_on(epaper_display_t *dev) {
    assert(dev != NULL);
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "epaper_display_power_on");

    if (!dev->is_power_on) {
        epaper_send_cmd(dev, 0x22);
        epaper_send_byte(dev, 0xf8);
        epaper_send_cmd(dev, 0x20);
        epaper_wait_busy(dev, dev->power_on_time);
    }
    dev->is_power_on = true;
    return ESP_OK;
}

esp_err_t epaper_display_power_off(epaper_display_t *dev) {
    assert(dev != NULL);
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "epaper_display_power_off");

    if (dev->is_power_on) {
        epaper_send_cmd(dev, 0x22);
        epaper_send_byte(dev, 0x83);
        epaper_send_cmd(dev, 0x20);
        epaper_wait_busy(dev, dev->power_off_time);
    }
    dev->is_power_on = false;
    return ESP_OK;
}

esp_err_t epaper_display_power_hibernate(epaper_display_t *dev) {
    assert(dev != NULL);
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__,
             "epaper_display_power_hibernate");

    epaper_display_power_off(dev);
    if (dev->reset_pin != -1) {
        epaper_send_cmd(dev, 0x10);  // deep sleep mode
        epaper_send_byte(dev, 0x1);  // enter deep sleep
        dev->hibernating = true;
    }
    return ESP_OK;
}

esp_err_t epaper_display_reset(epaper_display_t *dev) {
    assert(dev != NULL);
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "epaper_display_reset");

    gpio_set_level(dev->reset_pin, dev->reset_level);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(dev->reset_pin, !dev->reset_level);

    dev->hibernating = false;
    return ESP_OK;
}

/////////////////////////////////////////////////////////////////////

static esp_err_t epaper_gpio_init(epaper_display_t *dev) {
    assert(dev != NULL);

    gpio_pad_select_gpio(dev->reset_pin);
    gpio_set_direction(dev->reset_pin, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(dev->dc_pin);
    gpio_set_direction(dev->dc_pin, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(dev->busy_pin);
    gpio_set_direction(dev->busy_pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(dev->busy_pin,
                       GPIO_PULLUP_ONLY);  // busy_pin == high means busy :)

    return ESP_OK;
}

static esp_err_t epaper_spi_init(epaper_display_t *dev) {
    assert(dev != NULL);

    esp_err_t ret;
    spi_bus_config_t buscfg = {.miso_io_num     = dev->miso_pin,
                               .mosi_io_num     = dev->mosi_pin,
                               .sclk_io_num     = dev->sclk_pin,
                               .quadwp_io_num   = -1,
                               .quadhd_io_num   = -1,
                               .max_transfer_sz = dev->width * dev->height / 8};

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 20 * 1000 * 1000,
        .mode           = 0,
        .spics_io_num   = dev->cs_pin,
        .queue_size     = 10,
        .flags          = (SPI_DEVICE_HALFDUPLEX | SPI_DEVICE_3WIRE),
        .pre_cb         = epaper_spi_pre_transfer_callback,
    };
    ret = spi_bus_initialize(dev->spi_host, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);
    ret = spi_bus_add_device(dev->spi_host, &devcfg, &dev->bus);
    ESP_ERROR_CHECK(ret);

    return ESP_OK;
}

static void epaper_spi_pre_transfer_callback(spi_transaction_t *t) {
    epaper_display_t *dev = (epaper_display_t *)t->user;
    gpio_set_level(dev->dc_pin, dev->dc_level);
}

static esp_err_t epaper_send_cmd(epaper_display_t *dev, const uint8_t cmd) {
    assert(dev != NULL);

    dev->dc_level = dev->cmd_level;
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length    = 8;
    t.tx_buffer = &cmd;
    t.user      = (void *)dev;
    ret         = spi_device_transmit(dev->bus, &t);
    return ret;
}

static esp_err_t epaper_send_byte(epaper_display_t *dev, const uint8_t byte) {
    assert(dev != NULL);

    dev->dc_level = dev->data_level;
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length    = 8;
    t.tx_buffer = &byte;
    t.user      = (void *)dev;
    ret         = spi_device_transmit(dev->bus, &t);
    return ret;
}

static esp_err_t epaper_send_buffer(epaper_display_t *dev, const uint8_t *buf,
                                    int len) {
    assert(dev != NULL);

    dev->dc_level = dev->data_level;
    esp_err_t ret;
    spi_transaction_t t;
    if (len == 0) return ESP_OK;
    memset(&t, 0, sizeof(t));
    t.length    = len * 8;
    t.tx_buffer = buf;
    t.user      = (void *)dev;
    ret         = spi_device_transmit(dev->bus, &t);
    return ret;
}

static esp_err_t epaper_initialization_sequence(epaper_display_t *dev) {
    assert(dev != NULL);

    if (dev->hibernating) epaper_display_reset(dev);

    vTaskDelay(10 / portTICK_PERIOD_MS);  // 10ms according to specs
    epaper_send_cmd(dev, 0x12);           // SWRESET

    vTaskDelay(10 / portTICK_PERIOD_MS);  // 10ms according to specs
    epaper_send_cmd(dev, 0x01);           // Driver output control
    epaper_send_byte(dev, 0x27);
    epaper_send_byte(dev, 0x01);
    epaper_send_byte(dev, 0x00);

    epaper_send_cmd(dev, 0x11);   // data entry mode
    epaper_send_byte(dev, 0x01);  // Y decrrement, X inccrement,

    epaper_send_cmd(dev, 0x3C);  // BorderWavefrom
    epaper_send_byte(dev, 0x05);

    epaper_send_cmd(dev, 0x18);  // Read built-in temperature sensor
    epaper_send_byte(dev, 0x80);

    epaper_send_cmd(dev, 0x21);  //  Display update control
    epaper_send_byte(dev, 0x00);
    epaper_send_byte(dev, 0x80);

    epaper_display_set_window(dev, 0, 0, dev->width, dev->height);

    return ESP_OK;
}

static esp_err_t epaper_set_partial_ram_area(epaper_display_t *dev, uint16_t x1,
                                             uint16_t y1, uint16_t x2,
                                             uint16_t y2) {
    // epaper_send_cmd(dev, 0x11);  // data entry mode
    // epaper_send_byte(dev, 0x01); // Y decrement, X inccrement

    // Set RAM X - address
    epaper_send_cmd(dev, 0x44);
    epaper_send_byte(dev, (uint8_t)(x1 / 8));  // start
    epaper_send_byte(dev, (uint8_t)(x2 / 8));  // end

    // Set Ram Y- adddress
    epaper_send_cmd(dev, 0x45);
    epaper_send_byte(dev, (uint8_t)(y2 % 256));  // end
    epaper_send_byte(dev, (uint8_t)(y2 / 256));  // end
    epaper_send_byte(dev, (uint8_t)(y1 % 256));  // start
    epaper_send_byte(dev, (uint8_t)(y1 / 256));  // start

    // Set RAM X adddress counter
    epaper_send_cmd(dev, 0x4E);
    epaper_send_byte(dev, (uint8_t)((x1) / 8));

    // Set RAM Y adddress counter
    epaper_send_cmd(dev, 0x4F);
    epaper_send_byte(dev, (uint8_t)((y1) % 256));
    epaper_send_byte(dev, (uint8_t)((y1) / 256));

    return ESP_OK;
}

static esp_err_t epaper_wait_busy(epaper_display_t *dev, int timeout) {
    assert(dev != NULL);

    esp_err_t ret = ESP_OK;
    time_t start  = time(NULL);
    while (true) {
        if (gpio_get_level(dev->busy_pin) != dev->busy_level) {
            break;
        }

        if (difftime(time(NULL), start) > timeout) {
            ESP_LOGE(TAG, "epaper_wait_busy timeout %ds", timeout);
            ret = ESP_FAIL;
            break;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    return ret;
}