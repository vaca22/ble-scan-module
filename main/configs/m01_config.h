/**
 * @file config.h
 * M01 project configuration
 */
#ifndef __M01_CONFIG_h__
#define __M01_CONFIG_h__

/*********************
 *      DEFINES
 *********************/

#define DEBUG_TEST 1

// 墨水屏相关
#define EPAPER_SPI_HOST  HSPI_HOST
#define EPAPER_MISO_PIN  -1
#define EPAPER_MOSI_PIN  4
#define EPAPER_SCLK_PIN  17
#define EPAPER_CS_PIN    21
#define EPAPER_DC_PIN    22
#define EPAPER_RESET_PIN 27
#define EPAPER_BUSY_PIN  34

#define EPAPER_HOR_RES_MAX    296
#define EPAPER_VER_RES_MAX    152
#define EPAPER_DISP_BUFF_SIZE ((EPAPER_HOR_RES_MAX * EPAPER_VER_RES_MAX) / 8)

// 触摸相关
#define TOUCH_CHSC6X_I2C  0
#define TOUCH_CHSC6X_ADDR 0x2E
#define TOUCH_CHSC6X_SDA  18
#define TOUCH_CHSC6X_SCL  23
#define TOUCH_CHSC6X_RST  27
#define TOUCH_CHSC6X_INT  37
#define TOUCH_CHSC6X_PWR  -1

// 按键
#define BUTTON_KEY_PWR      38
#define BUTTON_KEY_PWR_ID   4
#define BUTTON_ADC_CHN      ADC_CHANNEL_3
#define BUTTON_KEY_A_ID     0
#define BUTTON_KEY_A_MIN    700
#define BUTTON_KEY_A_MAX    950
#define BUTTON_KEY_B_ID     1
#define BUTTON_KEY_B_MIN    1100
#define BUTTON_KEY_B_MAX    1300
#define BUTTON_KEY_C_ID     2
#define BUTTON_KEY_C_MIN    300
#define BUTTON_KEY_C_MAX    500
#define BUTTON_KEY_PLAY_ID  3
#define BUTTON_KEY_PLAY_MIN 1500
#define BUTTON_KEY_PLAY_MAX 1700

// CodeC
#define AUDIO_ES8311_I2C  0
#define AUDIO_ES8311_ADDR 0x18
#define AUDIO_ES8311_SDA  18
#define AUDIO_ES8311_SCL  23
#define AUDIO_I2S_NUM     0
#define AUDIO_I2S_MCK_PIN -1
#define AUDIO_I2S_BCK_PIN 5
#define AUDIO_I2S_WS_PIN  25
#define AUDIO_I2S_DO_PIN  26
#define AUDIO_I2S_DI_PIN  35

// AMP
#define AUDIO_CS5230E_PWR_PIN 19

// Battery
#define BAT_ADC_CHANNEL     ADC2_CHANNEL_5
#define BAT_IN_CHARGE_PIN   13
#define BAT_CHARGE_DONE_PIN 36

// LVGL相关
#define LVGL_DISP_BUF_SIZE (EPAPER_HOR_RES_MAX * 10)

#endif /*__M01_CONFIG_h__*/
