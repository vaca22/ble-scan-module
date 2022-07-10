
#include "ui_helpers.h"
#include "ui_control.h"
#include "stdio.h"
#include "ui_res.h"

#define DEVICE_BIND_STATUS 0



void ui_init_root(void)
{
    // 初始化一些LVGL默认配置
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_GREY), lv_palette_main(LV_PALETTE_GREY),
                                              false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    // 开机画面
    lv_obj_t *img1 = lv_img_create(lv_scr_act());
    lv_img_set_src(img1, &img_boot_start);
    lv_obj_set_align(img1, LV_ALIGN_CENTER);
    lv_obj_set_size(img1, 186, 42);
    
    printf("load_ui_screen_boot_guide\n");
    //
    // load_ui_screen_boot_guide();
    load_screen_factory();

    // // 判断绑定状态
    // #ifdef DEVICE_BIND_STATUS == 0
    //     // 未绑定 -> 弹出开机引导
    //     load_ui_screen_boot_guide();
    // // #else

    // #endif
    

}