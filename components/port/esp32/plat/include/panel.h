#ifndef PANEL_H
#define PANEL_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    PANEL_TYPE_CHSC6X,
} panel_type_t;

typedef struct {
    panel_type_t panel_type;
    int8_t      power_gpio;
    int8_t      reset_gpio;
    int8_t      int_gpio;
    int8_t      i2c_port;
} panel_config_t;

typedef struct {
    int (*init)(panel_config_t* cfg);
    int (*coords)(int32_t x[], int32_t y[]);
    int (*power_on)(bool power);
    int (*deinit)(void);
} panel_impl_t;

int panel_init(panel_config_t* cfg);

int panel_read_coords(int32_t x[], int32_t y[]);

int panel_power_on(bool power);

int panel_deinit(void);

#endif