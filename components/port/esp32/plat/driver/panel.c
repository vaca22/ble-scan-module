#include "panel.h"
#include <stdlib.h>

extern panel_impl_t panel_chsc6x;

static panel_impl_t* panel_drv;

int panel_init(panel_config_t* cfg) {
    if (cfg->panel_type == PANEL_TYPE_CHSC6X) {
        panel_drv = &panel_chsc6x;
    }
    if (panel_drv == NULL) {
        return -1;
    }
    return panel_drv->init(cfg);
}

int panel_read_coords(int32_t x[], int32_t y[]) {
    if (panel_drv == NULL) {
        return -1;
    }
    return panel_drv->coords(x, y);
}

int panel_power_on(bool power) {
    if (panel_drv == NULL) {
        return -1;
    }
    return panel_drv->power_on(power);
}

int panel_deinit(void) {
    if (panel_drv == NULL) {
        return -1;
    }
    return panel_drv->deinit();
}
