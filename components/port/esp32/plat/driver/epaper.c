#include <stdlib.h>
#include "epaper.h"

extern epaper_impl_t my_epaper_impl;

static epaper_impl_t* epaper_impl;

int epaper_init(epaper_cfg_t* cfg) {
    if (cfg->type == EPAPER_MY_EPAPER) {
        epaper_impl = &my_epaper_impl;
    }
    if (epaper_impl) {
        return epaper_impl->init(cfg);
    }
    return -1;

}

int epaper_deinit() {
    if (epaper_impl == NULL) {
        return -1;
    }
    return epaper_impl->deinit();
}

int epaper_draw(uint8_t* black, uint8_t* red) {
    if (epaper_impl == NULL) {
        return -1;
    }
    return epaper_impl->draw(black, red);
}

int epaper_clear() {
    if (epaper_impl == NULL) {
        return -1;
    }
    return epaper_impl->clear();
}

int epaper_sleep() {
    if (epaper_impl == NULL) {
        return -1;
    }
    return epaper_impl->sleep();
}

int epaper_wakeup() {
     if (epaper_impl == NULL) {
        return -1;
    }
    return epaper_impl->wakeup();
}
