/**
 * @file button.c
 * button file
 */

/*********************
 *      INCLUDES
 *********************/
#include "button.h"

/*********************
 *      DEFINES
 *********************/
#define TAG "button"
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void button_press_down_cb(void *arg);
/**********************
 *  STATIC VARIABLES
 **********************/
static button_handle_t g_btns[5] = {0};
/**********************
 *      MACROS
 **********************/
button_dev_t button_dev = {0};
/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void button_init(void) {
    button_config_t cfg = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config =
            {
                .gpio_num     = BUTTON_KEY_PWR,
                .active_level = 0,
            },
    };
    g_btns[0] = iot_button_create(&cfg);
    assert(g_btns[0]);

    cfg.type                           = BUTTON_TYPE_ADC;
    cfg.adc_button_config.adc_channel  = BUTTON_ADC_CHN;
    cfg.adc_button_config.button_index = BUTTON_KEY_A_ID;
    cfg.adc_button_config.min          = BUTTON_KEY_A_MIN;
    cfg.adc_button_config.max          = BUTTON_KEY_A_MAX;
    g_btns[1] = iot_button_create(&cfg);
    assert(g_btns[1]);

    cfg.adc_button_config.button_index = BUTTON_KEY_B_ID;
    cfg.adc_button_config.min          = BUTTON_KEY_B_MIN;
    cfg.adc_button_config.max          = BUTTON_KEY_B_MAX;
    g_btns[2] = iot_button_create(&cfg);
    assert(g_btns[2]);

    cfg.adc_button_config.button_index = BUTTON_KEY_C_ID;
    cfg.adc_button_config.min          = BUTTON_KEY_C_MIN;
    cfg.adc_button_config.max          = BUTTON_KEY_C_MAX;
    g_btns[3] = iot_button_create(&cfg);
    assert(g_btns[3]);

    cfg.adc_button_config.button_index = BUTTON_KEY_PLAY_ID;
    cfg.adc_button_config.min          = BUTTON_KEY_PLAY_MIN;
    cfg.adc_button_config.max          = BUTTON_KEY_PLAY_MAX;
    g_btns[4] = iot_button_create(&cfg);
    assert(g_btns[4]);

    iot_button_register_cb(g_btns[0], BUTTON_PRESS_DOWN, button_press_down_cb);
    iot_button_register_cb(g_btns[1], BUTTON_PRESS_DOWN, button_press_down_cb);
    iot_button_register_cb(g_btns[2], BUTTON_PRESS_DOWN, button_press_down_cb);
    iot_button_register_cb(g_btns[3], BUTTON_PRESS_DOWN, button_press_down_cb);
    iot_button_register_cb(g_btns[4], BUTTON_PRESS_DOWN, button_press_down_cb);
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
// TODO:
static void button_press_down_cb(void *arg) {

    button_dev.status = true;
    if ((int)arg >= (BUTTON_ADC_CHN << 8)) {
        ESP_LOGI(TAG, "ADC button %d was pressed", ADC_BUTTON_SPLIT_INDEX((int)arg));
        button_dev.id = ADC_BUTTON_SPLIT_INDEX((int)arg);
    } else {
        ESP_LOGI(TAG, "GPIO button %d was pressed", (int)arg);
        button_dev.id = BUTTON_KEY_PWR_ID;
    }
}
