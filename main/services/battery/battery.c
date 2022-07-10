/**
 * @file battery.c
 * battery file
 */

/*********************
 *      INCLUDES
 *********************/
#include "battery.h"

/*********************
 *      DEFINES
 *********************/
#define TAG           "battery"
#define DEFAULT_VREF  3600
#define NO_OF_SAMPLES 64  // Multisampling
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static int battery_voltage(void);

/**********************
 *  STATIC VARIABLES
 **********************/
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static esp_adc_cal_characteristics_t *adc_chars;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
esp_err_t battery_init() {
    esp_err_t r;
    gpio_num_t adc_gpio_num;
    r = adc2_pad_get_io_num(BAT_ADC_CHANNEL, &adc_gpio_num);
    assert(r == ESP_OK);

    adc2_config_channel_atten(BAT_ADC_CHANNEL, ADC_ATTEN_11db);
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12,
                             DEFAULT_VREF, adc_chars);

    gpio_config_t io_conf = {0};
    io_conf.intr_type     = GPIO_INTR_DISABLE;
    io_conf.mode          = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask =
        (1ULL << BAT_IN_CHARGE_PIN) | (1ULL << BAT_CHARGE_DONE_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en   = 1;
    gpio_config(&io_conf);

    return ESP_OK;
}

/*
value   GPIO13   GPIO36   status
0x00      0        0      此状态应该不存在 --。
0x01      0        1      充电中/USB线插入
0x02      1        0      充满电/USB线插入
0x03      1        1      未充电/USB线未插入
*/
uint8_t battery_check_status() {
    uint8_t status = ((gpio_get_level(BAT_IN_CHARGE_PIN) << 1) |
                      (gpio_get_level(BAT_CHARGE_DONE_PIN)));

    ESP_LOGI(TAG, "charge status: %d", status);

    return status;
}

// TODO:
uint8_t battery_capacity(void) {
    uint8_t level = 80;
    battery_voltage();
    return level;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/

static int battery_voltage(void) {
    uint32_t adc_reading = 0;
    int read_raw = 0;

    // Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++) {
        adc2_get_raw(BAT_ADC_CHANNEL, width, &read_raw);
        adc_reading = adc_reading + read_raw;
    }
    adc_reading /= NO_OF_SAMPLES;
    uint32_t voltage =
        (uint32_t)(esp_adc_cal_raw_to_voltage(adc_reading, adc_chars) / 0.4);
    ESP_LOGI(TAG, "Raw: %d\tVoltage: %dmV", adc_reading, voltage);

    return voltage;
}