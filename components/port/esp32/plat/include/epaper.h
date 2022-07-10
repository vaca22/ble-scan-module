#ifndef E_PAPER_H
#define E_PAPER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    EPAPER_MY_EPAPER,
} epaper_type_t;

typedef struct {
    epaper_type_t type;
    int8_t       mosi_pin;
    int8_t       sclk_pin;
    int8_t       cs_pin;
    int           freq;
    int8_t       reset_pin;
    int8_t       dc_pin;
    int8_t       busy_pin;
    uint16_t      width;
    uint16_t      height;
} epaper_cfg_t;

typedef struct {
    int (*init)(epaper_cfg_t* cfg);
    int (*draw)(uint8_t* black, uint8_t* red);
    int (*clear)();
    int (*sleep)();
    int (*wakeup)();
    int (*deinit)();
} epaper_impl_t;

int epaper_init(epaper_cfg_t* cfg);

int epaper_deinit();

int epaper_draw(uint8_t* black, uint8_t* red);

int epaper_clear();

int epaper_sleep();

int epaper_wakeup();

#endif