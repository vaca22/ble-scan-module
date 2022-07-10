
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_adc_cal.h"
#include "nvs_flash.h"

#include "port_cfg.h"
#include "low_system.h"

int32_t low_system_init(void)
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
    return 0;
}

void low_system_term(void)
{
    ;
}

void low_system_led_init(void)
{
    ;
}

void low_system_led_on(int32_t led)
{
    ;
}

void low_system_led_off(int32_t led)
{
    ;
}

void low_system_led_set_level(int32_t led, int32_t level)
{
    ;
}

#if ADC_KEY == 0
#define KEY_NUM     3
#define KEY_GPIO1   GPIO_NUM_27
#define KEY_GPIO2   GPIO_NUM_26
#define KEY_GPIO3   GPIO_NUM_25
//#define KEY_GPIO    GPIO_NUM_35
static const int32_t key_buf[] = 
{KEY_GPIO1, KEY_GPIO2, KEY_GPIO3};

void low_system_key_init(void)
{
    int32_t i;
    uint64_t pin_mask = 0;
    gpio_config_t pin_cfg;

    for (i = 0; i < KEY_NUM; i++)
    {
        pin_mask |= 1Ull << (uint64_t)key_buf[i];
    }
    pin_cfg.pin_bit_mask = pin_mask;
    pin_cfg.mode = GPIO_MODE_INPUT;
    pin_cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    pin_cfg.pull_down_en = GPIO_PULLDOWN_ENABLE;
    pin_cfg.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&pin_cfg);
}

uint32_t low_system_key_load(void)
{
    uint32_t key = 0;
    int32_t i;
    for(i = 0; i < KEY_NUM; i++)
    {
        key |= gpio_get_level(key_buf[i]) << i;
    }
    return key;
}
#else

static esp_adc_cal_characteristics_t *adc_chars;

#define ADC_KEY_WIDTH   ADC_WIDTH_BIT_12
#define ADC_KEY_CHANNEL ADC_CHANNEL_3
#define ADC_KEY_ATTEN   ADC_ATTEN_DB_11
#define ADC_KEY_UNIT    ADC_UNIT_1
//#define ADC_KEY_DEF_REF 1100
#define ADC_KEY_DEF_REF 2000
#define GPIO_KEY        GPIO_NUM_38

#define ADC_KEY_NUM 4

const int32_t adc_key_threhold[][2] = 
{
    {1450, 1700},
    {1000, 1300},
    {700, 1000},
    {300, 500},
};

void low_system_key_init(void)
{
    esp_adc_cal_value_t val_type;
    //Check if TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK)
    {
        printf("[low_sys]key_init eFuse Two Point: Supported\n");
    }
    else
    {
        printf("[low_sys]key_init eFuse Two Point: NOT supported\n");
    }
    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK)
    {
        printf("[low_sys]key_init eFuse Vref: Supported\n");
    }
    else
    {
        printf("[low_sys]key_init eFuse Vref: NOT supported\n");
    }
    adc1_config_width(ADC_KEY_WIDTH);
    adc1_config_channel_atten(ADC_KEY_CHANNEL, ADC_KEY_ATTEN);
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    val_type = esp_adc_cal_characterize(ADC_KEY_UNIT, ADC_KEY_ATTEN,
            ADC_KEY_WIDTH, ADC_KEY_DEF_REF, adc_chars);
    if(val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
        printf("[low_sys]key_init charact using two point value\n");
    }
    else if(val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        printf("[low_sys]key_init charact using eFuse Vref\n");
    }
    else
    {
        printf("[low_sys]key_init charact using Default Vref\n");
    }
    gpio_config_t pin_cfg;
    pin_cfg.pin_bit_mask = 1Ull << GPIO_KEY;
    pin_cfg.mode = GPIO_MODE_INPUT;
    pin_cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    pin_cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pin_cfg.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&pin_cfg);
}

uint32_t low_system_key_load(void)
{
    uint32_t key = 0, adc_read, adc_vol, i;
    adc_read = adc1_get_raw(ADC_KEY_CHANNEL);
    adc_vol = esp_adc_cal_raw_to_voltage(adc_read, adc_chars);
    //printf("[low_sys]key_load vol:%dmV\n", adc_vol);
    if(adc_vol > adc_key_threhold[0][1])
    {
        goto low_system_key_load_exit;
    }
    for(i = 0; i < ADC_KEY_NUM; i++)
    {
        if(adc_vol < adc_key_threhold[i][0])
            continue;
        if(adc_vol < adc_key_threhold[i][1])
        {
            break;
        }
    }
    if(i == ADC_KEY_NUM)
    {
        printf("[low_sys]key_load err vol:%dmV\n", adc_vol);
        goto low_system_key_load_exit;
    }
    key |= 1 << i;
low_system_key_load_exit:
    key |= gpio_get_level(GPIO_KEY) << ADC_KEY_NUM;
    return key;
}
#endif

uint32_t low_system_battery_capacity(void)
{
    return 100;
}

int32_t low_system_battery_status(void)
{
    return BAT_DISCHARGING;
}

int32_t low_system_battery_temperature(void)
{
    return 20;
}

static TimerHandle_t timer_handle = NULL;
static int32_t timer_period = 0;
static low_timer_cb timer_cb = NULL;

static void low_sys_timer_cb(TimerHandle_t timer)
{
    if(timer_cb != NULL)
    {
        timer_cb(timer_period);
    }
}

int32_t low_system_timer_start(int32_t dur_ms, low_timer_cb cb)
{
    int32_t timid;
    if(timer_handle != NULL)
    {
        return 0;
    }
    timid = 1;
    timer_period = dur_ms;
    timer_handle = xTimerCreate("low_tim", dur_ms, pdTRUE, (void *)timid, low_sys_timer_cb);
    if(timer_handle)
    {
        timer_cb = cb;
        xTimerStart(timer_handle, 1);
        return 0;
    }
    return 0;
}

void low_system_timer_stop(void)
{
    ;
}

int32_t low_system_pwr_switch_chk(void)
{
    return SYS_PWRSW_ON;
}

int32_t low_system_pwr_button_chk(void)
{
    return SYS_PWRBTN_REL;
}

int32_t low_system_pwr_sleep(int32_t sec)
{
    return SYS_WKUP_SRC_KEY;
}

void low_system_pwr_down(void)
{
    ;
}

void low_system_reboot(void)
{
    ;
}

uint32_t low_system_random_number(void)
{
    return 0;
}

char *low_system_sn_get(void)
{
    return NULL;
}

uint32_t low_system_timestamp_get(void)
{
    return 0;
}

int32_t low_system_timestamp_set(uint32_t stamp)
{
    return 0;
}

