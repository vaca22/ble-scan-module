// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: ui_shutdown

#include "ui_res.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_screen_shutdown;
lv_obj_t * ui_label_shutdown_tip;
lv_obj_t * ui_panel_btn_cancel;
lv_obj_t * ui_image_arrow_right_1;
lv_obj_t * ui_label_btn_cancel;
lv_obj_t * ui_panel_btn_reboot;
lv_obj_t * ui_image_arrow_right_2;
lv_obj_t * ui_label_btn_reboot;
lv_obj_t * ui_panel_btn_shutdown;
lv_obj_t * ui_image_arrow_right_3;
lv_obj_t * ui_label_btn_shutdown;
lv_obj_t * ui_screen_shutting_down;
lv_obj_t * ui_label_shutting_down;

///////////////////// TEST LVGL SETTINGS ////////////////////

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////
void ui_screen_shutdown_screen_init(void)
{

    // ui_screen_shutdown

    ui_screen_shutdown = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_screen_shutdown, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_shutdown_tip

    ui_label_shutdown_tip = lv_label_create(ui_screen_shutdown);

    lv_obj_set_width(ui_label_shutdown_tip, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_shutdown_tip, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_shutdown_tip, 0);
    lv_obj_set_y(ui_label_shutdown_tip, 0);

    lv_obj_set_align(ui_label_shutdown_tip, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_shutdown_tip, "请确认关机");

    lv_obj_set_style_text_font(ui_label_shutdown_tip, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_btn_cancel

    ui_panel_btn_cancel = lv_obj_create(ui_screen_shutdown);

    lv_obj_set_width(ui_panel_btn_cancel, 80);
    lv_obj_set_height(ui_panel_btn_cancel, 50);

    lv_obj_set_x(ui_panel_btn_cancel, 0);
    lv_obj_set_y(ui_panel_btn_cancel, 0);

    lv_obj_set_align(ui_panel_btn_cancel, LV_ALIGN_TOP_RIGHT);

    lv_obj_clear_flag(ui_panel_btn_cancel, LV_OBJ_FLAG_SCROLLABLE);

    // ui_image_arrow_right_1

    ui_image_arrow_right_1 = lv_img_create(ui_panel_btn_cancel);
    lv_img_set_src(ui_image_arrow_right_1, &ic_arrow_right);

    lv_obj_set_width(ui_image_arrow_right_1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_arrow_right_1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_arrow_right_1, 8);
    lv_obj_set_y(ui_image_arrow_right_1, 0);

    lv_obj_set_align(ui_image_arrow_right_1, LV_ALIGN_RIGHT_MID);

    lv_obj_add_flag(ui_image_arrow_right_1, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_arrow_right_1, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_btn_cancel

    ui_label_btn_cancel = lv_label_create(ui_panel_btn_cancel);

    lv_obj_set_width(ui_label_btn_cancel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_btn_cancel, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_btn_cancel, 0);
    lv_obj_set_y(ui_label_btn_cancel, 0);

    lv_obj_set_align(ui_label_btn_cancel, LV_ALIGN_RIGHT_MID);

    lv_label_set_text(ui_label_btn_cancel, "取消");

    lv_obj_set_style_text_font(ui_label_btn_cancel, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_btn_reboot

    ui_panel_btn_reboot = lv_obj_create(ui_screen_shutdown);

    lv_obj_set_width(ui_panel_btn_reboot, 80);
    lv_obj_set_height(ui_panel_btn_reboot, 50);

    lv_obj_set_x(ui_panel_btn_reboot, 0);
    lv_obj_set_y(ui_panel_btn_reboot, 0);

    lv_obj_set_align(ui_panel_btn_reboot, LV_ALIGN_RIGHT_MID);

    lv_obj_clear_flag(ui_panel_btn_reboot, LV_OBJ_FLAG_SCROLLABLE);

    // ui_image_arrow_right_2

    ui_image_arrow_right_2 = lv_img_create(ui_panel_btn_reboot);
    lv_img_set_src(ui_image_arrow_right_2, &ic_arrow_right);

    lv_obj_set_width(ui_image_arrow_right_2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_arrow_right_2, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_arrow_right_2, 8);
    lv_obj_set_y(ui_image_arrow_right_2, 0);

    lv_obj_set_align(ui_image_arrow_right_2, LV_ALIGN_RIGHT_MID);

    lv_obj_add_flag(ui_image_arrow_right_2, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_arrow_right_2, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_btn_reboot

    ui_label_btn_reboot = lv_label_create(ui_panel_btn_reboot);

    lv_obj_set_width(ui_label_btn_reboot, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_btn_reboot, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_btn_reboot, 0);
    lv_obj_set_y(ui_label_btn_reboot, 0);

    lv_obj_set_align(ui_label_btn_reboot, LV_ALIGN_RIGHT_MID);

    lv_label_set_text(ui_label_btn_reboot, "重启");

    lv_obj_set_style_text_font(ui_label_btn_reboot, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_btn_shutdown

    ui_panel_btn_shutdown = lv_obj_create(ui_screen_shutdown);

    lv_obj_set_width(ui_panel_btn_shutdown, 80);
    lv_obj_set_height(ui_panel_btn_shutdown, 50);

    lv_obj_set_x(ui_panel_btn_shutdown, 0);
    lv_obj_set_y(ui_panel_btn_shutdown, 0);

    lv_obj_set_align(ui_panel_btn_shutdown, LV_ALIGN_BOTTOM_RIGHT);

    lv_obj_clear_flag(ui_panel_btn_shutdown, LV_OBJ_FLAG_SCROLLABLE);

    // ui_image_arrow_right_3

    ui_image_arrow_right_3 = lv_img_create(ui_panel_btn_shutdown);
    lv_img_set_src(ui_image_arrow_right_3, &ic_arrow_right);

    lv_obj_set_width(ui_image_arrow_right_3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_arrow_right_3, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_arrow_right_3, 8);
    lv_obj_set_y(ui_image_arrow_right_3, 0);

    lv_obj_set_align(ui_image_arrow_right_3, LV_ALIGN_RIGHT_MID);

    lv_obj_add_flag(ui_image_arrow_right_3, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_arrow_right_3, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_btn_shutdown

    ui_label_btn_shutdown = lv_label_create(ui_panel_btn_shutdown);

    lv_obj_set_width(ui_label_btn_shutdown, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_btn_shutdown, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_btn_shutdown, 0);
    lv_obj_set_y(ui_label_btn_shutdown, 0);

    lv_obj_set_align(ui_label_btn_shutdown, LV_ALIGN_RIGHT_MID);

    lv_label_set_text(ui_label_btn_shutdown, "关机");

    lv_obj_set_style_text_font(ui_label_btn_shutdown, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

}
void ui_screen_shutting_down_screen_init(void)
{

    // ui_screen_shutting_down

    ui_screen_shutting_down = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_screen_shutting_down, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_shutting_down

    ui_label_shutting_down = lv_label_create(ui_screen_shutting_down);

    lv_obj_set_width(ui_label_shutting_down, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_shutting_down, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_shutting_down, 0);
    lv_obj_set_y(ui_label_shutting_down, 0);

    lv_obj_set_align(ui_label_shutting_down, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_shutting_down, "正在关机...");

    lv_obj_set_style_text_font(ui_label_shutting_down, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

}

void load_shutdown_screen(void)
{
    // lv_disp_t * dispp = lv_disp_get_default();
    // lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
    //                                            false, LV_FONT_DEFAULT);
    // lv_disp_set_theme(dispp, theme);
    ui_screen_shutdown_screen_init();
    ui_screen_shutting_down_screen_init();
    lv_disp_load_scr(ui_screen_shutdown);
}

