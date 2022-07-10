
#ifndef UI_CONTROL_H
#define UI_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#if __has_include("lvgl.h")
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

extern lv_obj_t *ui_screen_settings;
extern lv_obj_t *ui_screen_home;
extern lv_obj_t *ui_screen_boot_guide;


extern void load_ui_screen_boot_guide(void);
extern void load_screen_home(void);
extern void load_screen_factory(void);
extern void load_settings_screen(void);
extern void load_shutdown_screen(void);
extern void load_screen_study_weekly(void);


void ui_init_root(void);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif