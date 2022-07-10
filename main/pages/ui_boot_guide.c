// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: M01_boot_guider

#include "ui_res.h"
#include "ui_helpers.h"
#include "lv_qrcode.h"


///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_screen_boot_guide;
lv_obj_t * ui_title_welcome;
lv_obj_t * ui_btn_network;
lv_obj_t * ui_Label1;
lv_obj_t * ui_screen_app_down;
lv_obj_t * ui_panel_back_1;
lv_obj_t * ui_Image_back_1;
lv_obj_t * ui_label_down_app;
lv_obj_t * ui_btn_next;
lv_obj_t * ui_label_next;
lv_obj_t * ui_screen_network_by_app;
lv_obj_t * ui_panel_back_2;
lv_obj_t * ui_img_back_2;
lv_obj_t * ui_panel_btn_manual;
lv_obj_t * ui_label_manual;
lv_obj_t * ui_Label_network_by_app;
lv_obj_t * ui_screen_device_bind;
lv_obj_t * ui_panel_back_3;
lv_obj_t * ui_image_back_3;
lv_obj_t * ui_label_bind;
lv_obj_t * ui_label_scan_to_bind;
lv_obj_t * ui_screen_bind_result;
lv_obj_t * ui_label_bind_status;
lv_obj_t * ui_label_bind_user;


extern void ui_load_wifi_screen(void);
///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
static void ui_event_btn_network(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if(event == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_screen_app_down, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, false);
    }
}
static void ui_event_panel_back_1(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if(event == LV_EVENT_CLICKED) {
        _ui_screen_change(ui_screen_boot_guide, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    }
}
static void ui_event_btn_next(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if(event == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_screen_network_by_app, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, false);
    }
}
static void ui_event_panelback2(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if(event == LV_EVENT_CLICKED) {
        _ui_screen_change(ui_screen_app_down, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    }
}
static void ui_event_panel_back_3(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if(event == LV_EVENT_CLICKED) {
        _ui_screen_change(ui_screen_boot_guide, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    }
}

static void event_manual_network(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if(event == LV_EVENT_CLICKED) {
       ui_load_wifi_screen();
    }
}

///////////////////// SCREENS ////////////////////
void ui_screen_boot_guide_screen_init(void)
{

    // ui_screen_boot_guide

    ui_screen_boot_guide = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_screen_boot_guide, LV_OBJ_FLAG_SCROLLABLE);

    // ui_title_welcome

    ui_title_welcome = lv_label_create(ui_screen_boot_guide);

    lv_obj_set_width(ui_title_welcome, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_title_welcome, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_title_welcome, 0);
    lv_obj_set_y(ui_title_welcome, -20);

    lv_obj_set_align(ui_title_welcome, LV_ALIGN_CENTER);

    lv_label_set_text(ui_title_welcome, "欢迎使用阿尔法蛋AI单词宝");

    lv_obj_set_style_text_font(ui_title_welcome, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_btn_network

    ui_btn_network = lv_obj_create(ui_screen_boot_guide);

    lv_obj_set_width(ui_btn_network, 118);
    lv_obj_set_height(ui_btn_network, 39);

    lv_obj_set_x(ui_btn_network, 0);
    lv_obj_set_y(ui_btn_network, 23);

    lv_obj_set_align(ui_btn_network, LV_ALIGN_CENTER);

    lv_obj_clear_flag(ui_btn_network, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_btn_network, ui_event_btn_network, LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(ui_btn_network, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_btn_network, lv_color_hex(0x474747), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btn_network, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label1

    ui_Label1 = lv_label_create(ui_btn_network);

    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label1, 0);
    lv_obj_set_y(ui_Label1, 0);

    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label1, "联网使用");

    lv_obj_set_style_text_color(ui_Label1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label1, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

}
void ui_screen_app_down_screen_init(void)
{

    // ui_screen_app_down

    ui_screen_app_down = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_screen_app_down, LV_OBJ_FLAG_SCROLLABLE);

    // ui_panel_back_1

    ui_panel_back_1 = lv_obj_create(ui_screen_app_down);

    lv_obj_set_width(ui_panel_back_1, 48);
    lv_obj_set_height(ui_panel_back_1, 34);

    lv_obj_set_x(ui_panel_back_1, 0);
    lv_obj_set_y(ui_panel_back_1, 0);

    lv_obj_clear_flag(ui_panel_back_1, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_panel_back_1, ui_event_panel_back_1, LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(ui_panel_back_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Image_back_1

    ui_Image_back_1 = lv_img_create(ui_panel_back_1);
    lv_img_set_src(ui_Image_back_1, &ic_back);

    lv_obj_set_width(ui_Image_back_1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Image_back_1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Image_back_1, -3);
    lv_obj_set_y(ui_Image_back_1, 0);

    lv_obj_set_align(ui_Image_back_1, LV_ALIGN_LEFT_MID);

    lv_obj_add_flag(ui_Image_back_1, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_Image_back_1, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_down_app

    ui_label_down_app = lv_label_create(ui_screen_app_down);

    lv_obj_set_width(ui_label_down_app, 130);
    lv_obj_set_height(ui_label_down_app, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_down_app, 21);
    lv_obj_set_y(ui_label_down_app, 54);

    lv_label_set_text(ui_label_down_app, "扫码下载\"阿尔法蛋\" APP");

    lv_obj_set_style_text_letter_space(ui_label_down_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui_label_down_app, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_label_down_app, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_btn_next

    ui_btn_next = lv_obj_create(ui_screen_app_down);

    lv_obj_set_width(ui_btn_next, 90);
    lv_obj_set_height(ui_btn_next, 33);

    lv_obj_set_x(ui_btn_next, 51);
    lv_obj_set_y(ui_btn_next, 97);

    lv_obj_clear_flag(ui_btn_next, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_btn_next, ui_event_btn_next, LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(ui_btn_next, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_btn_next, lv_color_hex(0x474747), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btn_next, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_next

    ui_label_next = lv_label_create(ui_btn_next);

    lv_obj_set_width(ui_label_next, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_next, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_next, 0);
    lv_obj_set_y(ui_label_next, 0);

    lv_obj_set_align(ui_label_next, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_next, "下一步");

    lv_obj_set_style_text_color(ui_label_next, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_label_next, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_label_next, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);


    lv_obj_t * qr = lv_qrcode_create(ui_screen_app_down, 100, lv_color_hex(0x474747), lv_color_hex(0xFFFFFF));

    /*Set data*/
    const char * data = "www.baidu.com";
    lv_qrcode_update(qr, data, strlen(data));

    lv_obj_set_x(qr, -24);
    lv_obj_set_y(qr, 8);
    lv_obj_set_align(qr, LV_ALIGN_RIGHT_MID);

    /*Add a border with bg_color*/
    // lv_obj_set_style_border_color(qr, bg_color, 0);
    // lv_obj_set_style_border_width(qr, 5, 0);

}
void ui_screen_network_by_app_screen_init(void)
{

    // ui_screen_network_by_app

    ui_screen_network_by_app = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_screen_network_by_app, LV_OBJ_FLAG_SCROLLABLE);

    // ui_panel_back_2

    ui_panel_back_2 = lv_obj_create(ui_screen_network_by_app);

    lv_obj_set_width(ui_panel_back_2, 48);
    lv_obj_set_height(ui_panel_back_2, 34);

    lv_obj_set_x(ui_panel_back_2, 0);
    lv_obj_set_y(ui_panel_back_2, 0);

    lv_obj_clear_flag(ui_panel_back_2, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_panel_back_2, ui_event_panelback2, LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(ui_panel_back_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_img_back_2

    ui_img_back_2 = lv_img_create(ui_panel_back_2);
    lv_img_set_src(ui_img_back_2, &ic_back);

    lv_obj_set_width(ui_img_back_2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_img_back_2, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_img_back_2, -3);
    lv_obj_set_y(ui_img_back_2, 0);

    lv_obj_set_align(ui_img_back_2, LV_ALIGN_LEFT_MID);

    lv_obj_add_flag(ui_img_back_2, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_img_back_2, LV_OBJ_FLAG_SCROLLABLE);

    // ui_panel_btn_manual

    ui_panel_btn_manual = lv_obj_create(ui_screen_network_by_app);

    lv_obj_set_width(ui_panel_btn_manual, 58);
    lv_obj_set_height(ui_panel_btn_manual, 34);

    lv_obj_set_x(ui_panel_btn_manual, 0);
    lv_obj_set_y(ui_panel_btn_manual, 0);

    lv_obj_set_align(ui_panel_btn_manual, LV_ALIGN_TOP_RIGHT);

    lv_obj_clear_flag(ui_panel_btn_manual, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_panel_btn_manual, event_manual_network, LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(ui_panel_btn_manual, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_manual

    ui_label_manual = lv_label_create(ui_panel_btn_manual);

    lv_obj_set_width(ui_label_manual, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_manual, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_manual, 0);
    lv_obj_set_y(ui_label_manual, 0);

    lv_obj_set_align(ui_label_manual, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_manual, "手动");

    lv_obj_set_style_text_color(ui_label_manual, lv_color_hex(0x474747), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_label_manual, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_label_manual, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label_network_by_app

    ui_Label_network_by_app = lv_label_create(ui_screen_network_by_app);

    lv_obj_set_width(ui_Label_network_by_app, 130);
    lv_obj_set_height(ui_Label_network_by_app, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label_network_by_app, 21);
    lv_obj_set_y(ui_Label_network_by_app, 54);

    lv_label_set_text(ui_Label_network_by_app, "请使用阿尔法蛋APP 扫描联网");

    lv_obj_set_style_text_letter_space(ui_Label_network_by_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui_Label_network_by_app, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label_network_by_app, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);


    lv_obj_t * qr = lv_qrcode_create(ui_screen_network_by_app, 100, lv_color_hex(0x474747), lv_color_hex(0xFFFFFF));

    /*Set data*/
    const char * data = "www.baidu.com";
    lv_qrcode_update(qr, data, strlen(data));

    lv_obj_set_x(qr, -24);
    lv_obj_set_y(qr, 8);
    lv_obj_set_align(qr, LV_ALIGN_RIGHT_MID);

}



void ui_screen_device_bind_screen_init(void)
{

    // ui_screen_device_bind

    ui_screen_device_bind = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_screen_device_bind, LV_OBJ_FLAG_SCROLLABLE);

    // ui_panel_back_3

    ui_panel_back_3 = lv_obj_create(ui_screen_device_bind);

    lv_obj_set_width(ui_panel_back_3, 80);
    lv_obj_set_height(ui_panel_back_3, 34);

    lv_obj_set_x(ui_panel_back_3, 0);
    lv_obj_set_y(ui_panel_back_3, 0);

    lv_obj_clear_flag(ui_panel_back_3, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_panel_back_3, ui_event_panel_back_3, LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(ui_panel_back_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_back_3

    ui_image_back_3 = lv_img_create(ui_panel_back_3);
    lv_img_set_src(ui_image_back_3, &ic_back);

    lv_obj_set_width(ui_image_back_3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_back_3, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_back_3, -3);
    lv_obj_set_y(ui_image_back_3, 0);

    lv_obj_set_align(ui_image_back_3, LV_ALIGN_LEFT_MID);

    lv_obj_add_flag(ui_image_back_3, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_back_3, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_bind

    ui_label_bind = lv_label_create(ui_panel_back_3);

    lv_obj_set_width(ui_label_bind, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_bind, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_bind, 11);
    lv_obj_set_y(ui_label_bind, 0);

    lv_obj_set_align(ui_label_bind, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_bind, "绑定");

    lv_obj_set_style_text_decor(ui_label_bind, LV_TEXT_DECOR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_label_bind, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_scan_to_bind

    ui_label_scan_to_bind = lv_label_create(ui_screen_device_bind);

    lv_obj_set_width(ui_label_scan_to_bind, 102);
    lv_obj_set_height(ui_label_scan_to_bind, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_scan_to_bind, 36);
    lv_obj_set_y(ui_label_scan_to_bind, 6);

    lv_obj_set_align(ui_label_scan_to_bind, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_label_scan_to_bind, "使用\"阿尔法蛋\" APP扫码绑定");

    lv_obj_set_style_text_letter_space(ui_label_scan_to_bind, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui_label_scan_to_bind, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_label_scan_to_bind, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

}
void ui_screen_bind_result_screen_init(void)
{

    // ui_screen_bind_result

    ui_screen_bind_result = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_screen_bind_result, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_bind_status

    ui_label_bind_status = lv_label_create(ui_screen_bind_result);

    lv_obj_set_width(ui_label_bind_status, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_bind_status, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_bind_status, 0);
    lv_obj_set_y(ui_label_bind_status, -20);

    lv_obj_set_align(ui_label_bind_status, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_bind_status, "已被绑定");

    lv_obj_set_style_text_font(ui_label_bind_status, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_bind_user

    ui_label_bind_user = lv_label_create(ui_screen_bind_result);

    lv_obj_set_width(ui_label_bind_user, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_bind_user, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_bind_user, 0);
    lv_obj_set_y(ui_label_bind_user, 13);

    lv_obj_set_align(ui_label_bind_user, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_bind_user, "管理员:  155****5555");

    lv_obj_set_style_text_font(ui_label_bind_user, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

}

void ui_init(void)
{
    // lv_disp_t * dispp = lv_disp_get_default();
    // lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
    //                                            false, LV_FONT_DEFAULT);
    // lv_disp_set_theme(dispp, theme);
    ui_screen_boot_guide_screen_init();
    ui_screen_app_down_screen_init();
    ui_screen_network_by_app_screen_init();
    ui_screen_device_bind_screen_init();
    ui_screen_bind_result_screen_init();
    lv_disp_load_scr(ui_screen_boot_guide);
}



void load_ui_screen_boot_guide(){
    ui_screen_boot_guide_screen_init();
    ui_screen_app_down_screen_init();
    ui_screen_network_by_app_screen_init();
    ui_screen_device_bind_screen_init();
    ui_screen_bind_result_screen_init();
    // lv_disp_load_scr(ui_screen_boot_guide);

    _ui_screen_change(ui_screen_boot_guide, LV_SCR_LOAD_ANIM_FADE_ON, 0, 3000);
}

