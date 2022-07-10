
#include "port_cfg.h"
#include "low_lcd.h"
#if DISPLAY_1306 == 1
#include "oled1306.h"
#elif DISPLAY_EINK == 1
#include "eink_1680.h"
#endif


int32_t low_lcd_init(void)
{
#if DISPLAY_1306 == 1
    oled1306_init();
#else
    eink_1680_init();
#endif
    return 0;
}

void low_lcd_term(void)
{
    ;
}

uint32_t low_lcd_get_xsize(void)
{
    return 0;
}

uint32_t low_lcd_get_ysize(void)
{
    return 0;
}

void low_lcd_flush(uint8_t *buf)
{
#if DISPLAY_1306 == 1
    oled1306_flush(buf);
#else
    eink_1680_flush(buf);
#endif
}

void low_lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t pixel)
{
    ;
}

void low_lcd_display_off(void)
{
    ;
}

void low_lcd_display_on(void)
{
    ;
}

void low_lcd_set_brightness(uint8_t level)
{
    ;
}

int32_t low_lcd_set_rotate(int32_t rotate)
{
    return 0;
}

int32_t low_lcd_id_get(void)
{
    return 0;
}

