
#include "stdio.h"
#include "eink_1680.h"
#include "driver/gpio.h"
#include "plat_os.h"

#define EPD_CS   21               //GPIO 21
#define EPD_DC   22               //GPIO 22
#define EPD_SDA  33               //GPIO 33
#define EPD_SCL  32               //GPIO 32
#define EPD_RST  27               //GPIO 27
#define EPD_BUSY 34               //GPIO 34

#define GPIO_OUTPUT_PIN_SEL ((1ULL<<EPD_CS)   |     \
                             (1ULL<<EPD_DC)   |     \
                             (1ULL<<EPD_SDA)  |     \
                             (1ULL<<EPD_SCL)  |     \
                             (1ULL<<EPD_RST))
#define GPIO_INPUT_PIN_SEL   (1ULL<<EPD_BUSY)

#define nCS_H  gpio_set_level(EPD_CS,  1)
#define nCS_L  gpio_set_level(EPD_CS,  0)
#define nDC_H  gpio_set_level(EPD_DC,  1)
#define nDC_L  gpio_set_level(EPD_DC,  0)
#define SDA_H  gpio_set_level(EPD_SDA, 1)
#define SDA_L  gpio_set_level(EPD_SDA, 0)
#define SCL_H  gpio_set_level(EPD_SCL, 1)
#define SCL_L  gpio_set_level(EPD_SCL, 0)
#define RST_H  gpio_set_level(EPD_RST, 1)
#define RST_L  gpio_set_level(EPD_RST, 0)
#define BUSY_STATE   gpio_get_level(EPD_BUSY)

const uint8_t image_1[192] = {
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0XFF,0XFF,
0X3F,0XFF,0XFF,0XFC,0XFF,0XFF,0X3F,0XFF,0XFF,0XF0,0X00,0X00,0X3F,0XFF,0XFF,0XF0,
0X00,0X00,0X3F,0XFF,0XFF,0XF0,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0X3F,0XFF,
0XFF,0XFF,0XFF,0XFF,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
};

const uint8_t image_2[192] = {
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,0X1F,0XFF,
0XFF,0XFE,0X1F,0XFE,0X1F,0XFF,0XFF,0XFC,0X1F,0XFC,0X1F,0XFF,0XFF,0XFC,0X3F,0XF8,
0X1F,0XFF,0XFF,0XFC,0X7F,0XF0,0X1F,0XFF,0XFF,0XFC,0XFF,0XE1,0X9F,0XFF,0XFF,0XFC,
0XFF,0X83,0X9F,0XFF,0XFF,0XFC,0XFF,0X07,0X9F,0XFF,0XFF,0XFC,0X7C,0X0F,0X9F,0XFF,
0XFF,0XFC,0X00,0X3F,0X9F,0XFF,0XFF,0XFE,0X00,0X7F,0X9F,0XFF,0XFF,0XFE,0X00,0XFF,
0X9F,0XFF,0XFF,0XFF,0X87,0XFF,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
};

const unsigned char image_3[192] = { 
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,0XFF,
0XFF,0XFF,0X3F,0XF8,0XFF,0XFF,0XFF,0XFE,0X1F,0XF0,0X7F,0XFF,0XFF,0XFC,0X1F,0XF0,
0X3F,0XFF,0XFF,0XF8,0X7F,0XFC,0X3F,0XFF,0XFF,0XF8,0XFC,0XFE,0X3F,0XFF,0XFF,0XF9,
0XFC,0XFF,0X1F,0XFF,0XFF,0XF9,0XFC,0XFF,0X1F,0XFF,0XFF,0XF9,0XF8,0X7F,0X1F,0XFF,
0XFF,0XF8,0XF8,0X7E,0X3F,0XFF,0XFF,0XF8,0X00,0X08,0X3F,0XFF,0XFF,0XFC,0X02,0X00,
0X7F,0XFF,0XFF,0XFC,0X03,0X00,0X7F,0XFF,0XFF,0XFF,0X0F,0X81,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};

const unsigned char image_4[192] = { 
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XE1,0XFF,0XFF,0XFF,0XFF,0XFF,0X81,0XFF,0XFF,0XFF,0XFF,0XFF,0X01,0XFF,0XFF,
0XFF,0XFF,0XFC,0X01,0XFF,0XFF,0XFF,0XFF,0XF8,0X31,0XFF,0XFF,0XFF,0XFF,0XF0,0X71,
0XFF,0XFF,0XFF,0XFF,0XC1,0XF1,0XFF,0XFF,0XFF,0XFF,0X83,0XF1,0XFF,0XFF,0XFF,0XFE,
0X07,0XF1,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X1F,0XFF,0XFF,0XFC,0X00,0X00,0X1F,0XFF,
0XFF,0XFC,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};

const unsigned char image_5[192] = { 
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0XF8,0XFF,0XFF,
0XFF,0XFF,0XC0,0XF0,0X7F,0XFF,0XFF,0XF8,0X00,0XF0,0X3F,0XFF,0XFF,0XF8,0X00,0XFC,
0X3F,0XFF,0XFF,0XF8,0X11,0XFF,0X1F,0XFF,0XFF,0XF9,0XF3,0XFF,0X1F,0XFF,0XFF,0XF9,
0XF3,0XFF,0X1F,0XFF,0XFF,0XF9,0XF3,0XFF,0X1F,0XFF,0XFF,0XF9,0XF1,0XFE,0X1F,0XFF,
0XFF,0XF9,0XF0,0XFE,0X3F,0XFF,0XFF,0XF9,0XF8,0X30,0X3F,0XFF,0XFF,0XF9,0XF8,0X00,
0X7F,0XFF,0XFF,0XF9,0XFC,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X01,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};

const unsigned char image_12345[960] = {
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0XFF,0XFF,
0X3F,0XFF,0XFF,0XFC,0XFF,0XFF,0X3F,0XFF,0XFF,0XF0,0X00,0X00,0X3F,0XFF,0XFF,0XF0,
0X00,0X00,0X3F,0XFF,0XFF,0XF0,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0X3F,0XFF,
0XFF,0XFF,0XFF,0XFF,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0XFF,0X1F,0XFF,
0XFF,0XFE,0X1F,0XFE,0X1F,0XFF,0XFF,0XFC,0X1F,0XFC,0X1F,0XFF,0XFF,0XFC,0X3F,0XF8,
0X1F,0XFF,0XFF,0XFC,0X7F,0XF0,0X1F,0XFF,0XFF,0XFC,0XFF,0XE1,0X9F,0XFF,0XFF,0XFC,
0XFF,0X83,0X9F,0XFF,0XFF,0XFC,0XFF,0X07,0X9F,0XFF,0XFF,0XFC,0X7C,0X0F,0X9F,0XFF,
0XFF,0XFC,0X00,0X3F,0X9F,0XFF,0XFF,0XFE,0X00,0X7F,0X9F,0XFF,0XFF,0XFE,0X00,0XFF,
0X9F,0XFF,0XFF,0XFF,0X87,0XFF,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,0XFF,
0XFF,0XFF,0X3F,0XF8,0XFF,0XFF,0XFF,0XFE,0X1F,0XF0,0X7F,0XFF,0XFF,0XFC,0X1F,0XF0,
0X3F,0XFF,0XFF,0XF8,0X7F,0XFC,0X3F,0XFF,0XFF,0XF8,0XFC,0XFE,0X3F,0XFF,0XFF,0XF9,
0XFC,0XFF,0X1F,0XFF,0XFF,0XF9,0XFC,0XFF,0X1F,0XFF,0XFF,0XF9,0XF8,0X7F,0X1F,0XFF,
0XFF,0XF8,0XF8,0X7E,0X3F,0XFF,0XFF,0XF8,0X00,0X08,0X3F,0XFF,0XFF,0XFC,0X02,0X00,
0X7F,0XFF,0XFF,0XFC,0X03,0X00,0X7F,0XFF,0XFF,0XFF,0X0F,0X81,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XE1,0XFF,0XFF,0XFF,0XFF,0XFF,0X81,0XFF,0XFF,0XFF,0XFF,0XFF,0X01,0XFF,0XFF,
0XFF,0XFF,0XFC,0X01,0XFF,0XFF,0XFF,0XFF,0XF8,0X31,0XFF,0XFF,0XFF,0XFF,0XF0,0X71,
0XFF,0XFF,0XFF,0XFF,0XC1,0XF1,0XFF,0XFF,0XFF,0XFF,0X83,0XF1,0XFF,0XFF,0XFF,0XFE,
0X07,0XF1,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X1F,0XFF,0XFF,0XFC,0X00,0X00,0X1F,0XFF,
0XFF,0XFC,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF, 
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0XF8,0XFF,0XFF,
0XFF,0XFF,0XC0,0XF0,0X7F,0XFF,0XFF,0XF8,0X00,0XF0,0X3F,0XFF,0XFF,0XF8,0X00,0XFC,
0X3F,0XFF,0XFF,0XF8,0X11,0XFF,0X1F,0XFF,0XFF,0XF9,0XF3,0XFF,0X1F,0XFF,0XFF,0XF9,
0XF3,0XFF,0X1F,0XFF,0XFF,0XF9,0XF3,0XFF,0X1F,0XFF,0XFF,0XF9,0XF1,0XFE,0X1F,0XFF,
0XFF,0XF9,0XF0,0XFE,0X3F,0XFF,0XFF,0XF9,0XF8,0X30,0X3F,0XFF,0XFF,0XF9,0XF8,0X00,
0X7F,0XFF,0XFF,0XF9,0XFC,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X01,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};


static void eink_1680_gpio_config(void);
static void eink_1680_driver_init(void);
static void eink_1680_write_command(uint8_t command_buf);
static void eink_1680_write_data(uint8_t data_buf);
static void eink_1680_enter_deepsleep(void);   
static void eink_1680_reset(void);
static void eink_1680_read_busy(void);

static uint8_t eink_brash_counter = 0;

void eink_1680_init(void)
{
    printf("eink 1680 init\n");
    eink_1680_gpio_config();
    eink_1680_reset();                //hardware reset
    eink_1680_write_command(0x12);    //softeare reset
    eink_1680_read_busy();
    eink_1680_driver_init();
}

void eink_1680_flush(uint8_t *buf)
{
    if(eink_brash_counter++ < 10)    //10 times quick brash, 1 time slow brash
    {
        eink_1680_reset();
        eink_1680_read_busy();
        /*****************************Quick Brash****************************/
        eink_1680_write_command(0x4E);    //make initial settings for the X
        eink_1680_write_data(0x00);       //X count = 0 
        
        eink_1680_write_command(0x4F);    //make initial settings for the Y
        eink_1680_write_data(0x27);       
        eink_1680_write_data(0x01);       //Y count 0x127 = 295
        
        eink_1680_write_command(0x24);    //Pixel data will be written into the BW RAM
         
        for(uint32_t col = 0; col < 296; col++)    //Y range = 295-0+1 = 296    
        {
            for(uint8_t row = 0; row < 19; row++)  //X range = 18-0+1=19
            {
                eink_1680_write_data(*buf++);
            }         
        }

        eink_1680_write_command(0x22);     //Display update sequence option
        eink_1680_write_data(0xFF);        //0xFF = Quick brash
        eink_1680_write_command(0x20);     //Active display updata sequence
        os_thread_sleep(1);
        eink_1680_read_busy();
        /***********************************************************************/
        eink_1680_enter_deepsleep();
    }
    else
    {
        eink_1680_reset();
        eink_1680_read_busy();
        /*****************************Slow Brash*******************************/
        eink_1680_write_command(0x4E);    //make initial settings for the X
        eink_1680_write_data(0x00);       //X count = 0 
        
        eink_1680_write_command(0x4F);    //make initial settings for the Y
        eink_1680_write_data(0x27);       
        eink_1680_write_data(0x01);       //Y count 0x127 = 295
        
        eink_1680_write_command(0x24);    //Pixel data will be written into the BW RAM
         
        for(uint32_t col = 0; col < 296; col++)    //Y range = 295-0+1 = 296    
        {
            for(uint8_t row = 0; row < 19; row++)  //X range = 18-0+1=19
            {
                eink_1680_write_data(*buf++);
            }         
        }

        eink_1680_write_command(0x4E);    //make initial settings for the X
        eink_1680_write_data(0x00);       //X count = 0 
        
        eink_1680_write_command(0x4F);    //make initial settings for the Y
        eink_1680_write_data(0x27);       
        eink_1680_write_data(0x01);       //Y count 0x127 = 295
        
        eink_1680_write_command(0x26);    //Pixel data will be written into the BW RAM
         
        for(uint32_t col = 0; col < 296; col++)    //Y range = 295-0+1 = 296    
        {
            for(uint8_t row = 0; row < 19; row++)  //X range = 18-0+1=19
            {
                eink_1680_write_data(*buf++);
            }         
        }

        eink_1680_write_command(0x22);     //Display update sequence option
        eink_1680_write_data(0xF7);        //0xF7 = Slow brash
        eink_1680_write_command(0x20);     //Active display updata sequence
        os_thread_sleep(1);
        eink_1680_read_busy(); 
        eink_brash_counter = 0;
        /***************************************************************************/
        eink_1680_enter_deepsleep();
    }
}

static void eink_1680_gpio_config(void)
{
    printf("eink 1680 gpio config\n");
    gpio_config_t io_conf = {};

    /*********************Output Pin Set*********************/
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set. GPIO 21/22/27/32/33
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode   
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
    
    /**********************Input Pin Set*********************/
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //bit mask of the pins that you want to set. GPIO 34
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //disable pull-down mode   
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}

static void eink_1680_write_command(uint8_t command_buf)
{
    uint8_t temp_buf;
    temp_buf = command_buf;

    nCS_H;
    nCS_L;
    SCL_L;
    nDC_L;
    for(uint8_t scnt = 0; scnt < 8; scnt++)
    {
        if(temp_buf&0x80)  
            SDA_H;
        else
            SDA_L;
        SCL_H;              //SCLK rising edge
        SCL_L;
        temp_buf = temp_buf << 1;
    }
}

static void eink_1680_write_data(uint8_t data_buf)
{
    uint8_t temp_buf;
    temp_buf = data_buf;

    SCL_L;
    nDC_H;
    for(uint8_t scnt = 0; scnt < 8; scnt++)
    {
        if(temp_buf&0x80)  
            SDA_H;
        else
            SDA_L;
        SCL_H;               //SCLK rising edge
        SCL_L;
        temp_buf = temp_buf << 1;
    }
}

static void eink_1680_driver_init(void)
{
    printf("eink 1680 driver init\n");
    eink_1680_write_command(0x01);  //set gate driver
    eink_1680_write_data(0x27);
    eink_1680_write_data(0x01);
    eink_1680_write_data(0x00);

    eink_1680_write_command(0x11);  //data entry mode setting
    eink_1680_write_data(0x01);     //01 : Y decrement   X increment

    eink_1680_write_command(0x44);  //set RAM x address    start/end position
    eink_1680_write_data(0x00);     //X start position 0
    eink_1680_write_data(0x12);     //X end position 0x12 = 18

    eink_1680_write_command(0x45);  //set RAM y address    start/end position
    eink_1680_write_data(0x27);     
    eink_1680_write_data(0x01);     //Y start position 0x127 = 295
    eink_1680_write_data(0x00);     
    eink_1680_write_data(0x00);     //Y end position 0

    eink_1680_write_command(0x3C);   //set panel board
    eink_1680_write_data(0x05);      //GS1 --> gs1

    eink_1680_write_command(0x21);   //display update control
    eink_1680_write_data(0x00);
    eink_1680_write_data(0x80);      //Available source from S8 to S167
    eink_1680_write_command(0x18);   //sense temperature control
    eink_1680_write_data(0x80);      //internal temperature sensor
}

static void eink_1680_enter_deepsleep(void)   //To exit deep sleep mode, send hardware reset to IC
{
    printf("eink 1680 enter deep sleep\n");
    eink_1680_write_command(0x10);
    eink_1680_write_data(0x01);              
    nCS_H;
}

static void eink_1680_reset(void)
{
    printf("eink 1680 reset\n");
    RST_L;
    os_thread_sleep(1);
    RST_H;
    os_thread_sleep(1);
}

static void eink_1680_read_busy(void)
{
    while(1)
    {
        if(BUSY_STATE == 0)    //When busy is high, writing command/data to IC is not allowed 
            break;
    }
}


