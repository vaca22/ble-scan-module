
#ifndef __oled1306_h__
#define __oled1306_h__

void oled1306_init(void);

void oled1306_flush(uint8_t *buf);

void oled1306_fill_with(uint8_t data);

#endif

