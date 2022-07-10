// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: ui_settings

#include "ui_res.h"
#include "ui_helpers.h"
// #include "lvgl_project.h"
#include "stdio.h"
#include "ui_control.h"

extern lv_indev_t *lv_keypad_device_object;
///////////////////// VARIABLES ////////////////////
lv_obj_t *ui_screen_settings;
lv_obj_t *ui_panel_back;
lv_obj_t *ui_image_bakc;
lv_obj_t *ui_label_back;
lv_obj_t *ui_panel_menus_first;
lv_obj_t *ui_panel_person_center;
lv_obj_t *ui_image_person_center;
lv_obj_t *ui_label_person_center;
lv_obj_t *ui_panel_wifi_set;
lv_obj_t *ui_image_wifi_set;
lv_obj_t *ui_label_wifi_set;
lv_obj_t *ui_panel_bluetooth;
lv_obj_t *ui_image_bluetooth;
lv_obj_t *ui_label_bluetooth;
lv_obj_t *ui_panel_lock_screen;
lv_obj_t *ui_image_lock_screen;
lv_obj_t *ui_label_lock_screen;
lv_obj_t *ui_panel_menus_two;
lv_obj_t *ui_panel_volume_adjust;
lv_obj_t *ui_image_volume_adjust;
lv_obj_t *ui_label_volume_adjust;
lv_obj_t *ui_panel_pronounce_set;
lv_obj_t *ui_image_pronounce_set;
lv_obj_t *ui_label_pronounce_set;
lv_obj_t *ui_panel_ota_upgrade;
lv_obj_t *ui_image_ota_upgrade;
lv_obj_t *ui_label_ota_upgrade;
lv_obj_t *ui_panel_binding;
lv_obj_t *ui_image_binding;
lv_obj_t *ui_label_binding;
lv_obj_t *ui_panel_menus_three;
lv_obj_t *ui_panel_about;
lv_obj_t *ui_image_about;
lv_obj_t *ui_label_about;
lv_obj_t *ui_img_arrow_up;
lv_obj_t *ui_img_arrow_down;

static int8_t page_index = 0;
///////////////////// TEST LVGL SETTINGS ////////////////////

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
static void show_panel_menus_by_index(int8_t index) {
    switch (index) {
        case 1:
            lv_obj_add_flag(ui_panel_menus_first, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_panel_menus_three, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_panel_menus_two, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_img_arrow_up, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_img_arrow_down, LV_OBJ_FLAG_HIDDEN);
            break;
        case 2:
            lv_obj_add_flag(ui_panel_menus_first, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_panel_menus_two, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_panel_menus_three, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_img_arrow_up, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_img_arrow_down, LV_OBJ_FLAG_HIDDEN);
            break;

        default:
            lv_obj_clear_flag(ui_panel_menus_first, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_panel_menus_two, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_panel_menus_three, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_img_arrow_up, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_img_arrow_down, LV_OBJ_FLAG_HIDDEN);
            break;
    }
}

static void ui_screen_settings_callback(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t *ta          = lv_event_get_target(e);

    if (event == LV_EVENT_KEY) {
        uint32_t *key = (uint32_t *)lv_event_get_param(e);
        switch (*key) {
            case LV_KEY_UP:
                printf("ui_screen_settings LV_KEY_UP <A>\r\n");
                if (page_index > 0 && page_index <= 2) {
                    page_index--;
                }
                show_panel_menus_by_index(page_index);
                break;
            case LV_KEY_ENTER:
                printf("ui_screen_settings LV_KEY_ENTER <B>\r\n");
                break;
            case LV_KEY_DOWN:
                printf("ui_screen_settings LV_KEY_DOWN <C>\r\n");
                if (page_index >= 0 && page_index < 2) {
                    page_index++;
                }
                show_panel_menus_by_index(page_index);
                break;
            case LV_KEY_RIGHT:
                printf("ui_screen_settings LV_KEY_RIGHT <PLAY>\r\n");
                break;
            case LV_KEY_LEFT:
                printf("ui_screen_settings LV_KEY_LEFT <POWER>\r\n");
                break;
            default:
                break;
        }
    }
}

static void event_go_back(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if (event == LV_EVENT_PRESSED) {
        lv_scr_load_anim(ui_screen_home, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, true);

        // lv_disp_load_scr(ui_screen_home);
        // lv_obj_del(ui_screen_settings);
        // ui_screen_settings = NULL;
    }
}

///////////////////// SCREENS ////////////////////
void ui_screen_settings_screen_init(void) {
    // ui_screen_settings

    ui_screen_settings = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_screen_settings, LV_OBJ_FLAG_SCROLLABLE);

    // ui_panel_back

    ui_panel_back = lv_obj_create(ui_screen_settings);

    lv_obj_set_width(ui_panel_back, 86);
    lv_obj_set_height(ui_panel_back, 34);

    lv_obj_set_x(ui_panel_back, 0);
    lv_obj_set_y(ui_panel_back, 0);

    lv_obj_clear_flag(ui_panel_back, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_panel_back, event_go_back, LV_EVENT_ALL, NULL);

    // ui_image_bakc

    ui_image_bakc = lv_img_create(ui_panel_back);
    lv_img_set_src(ui_image_bakc, &ic_back);

    lv_obj_set_width(ui_image_bakc, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_bakc, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_bakc, -2);
    lv_obj_set_y(ui_image_bakc, 0);

    lv_obj_set_align(ui_image_bakc, LV_ALIGN_LEFT_MID);

    lv_obj_add_flag(ui_image_bakc, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_bakc, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_back

    ui_label_back = lv_label_create(ui_panel_back);

    lv_obj_set_width(ui_label_back, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_back, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_back, 18);
    lv_obj_set_y(ui_label_back, 0);

    lv_obj_set_align(ui_label_back, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_label_back, "设置");

    lv_obj_set_style_text_font(ui_label_back, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_menus_first

    ui_panel_menus_first = lv_obj_create(ui_screen_settings);

    lv_obj_set_width(ui_panel_menus_first, 280);
    lv_obj_set_height(ui_panel_menus_first, 114);

    lv_obj_set_x(ui_panel_menus_first, 0);
    lv_obj_set_y(ui_panel_menus_first, 0);

    lv_obj_set_align(ui_panel_menus_first, LV_ALIGN_BOTTOM_LEFT);

    lv_obj_clear_flag(ui_panel_menus_first, LV_OBJ_FLAG_SCROLLABLE);

    // ui_panel_person_center

    ui_panel_person_center = lv_obj_create(ui_panel_menus_first);

    lv_obj_set_width(ui_panel_person_center, 126);
    lv_obj_set_height(ui_panel_person_center, 42);

    lv_obj_set_x(ui_panel_person_center, 0);
    lv_obj_set_y(ui_panel_person_center, -10);

    lv_obj_clear_flag(ui_panel_person_center, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_panel_person_center, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_person_center,
                                  lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_person_center, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_person_center, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_person_center

    ui_image_person_center = lv_img_create(ui_panel_person_center);
    lv_img_set_src(ui_image_person_center, &img_settings_personal);

    lv_obj_set_width(ui_image_person_center, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_person_center, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_person_center, -34);
    lv_obj_set_y(ui_image_person_center, 0);

    lv_obj_set_align(ui_image_person_center, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_person_center, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_person_center, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_person_center

    ui_label_person_center = lv_label_create(ui_panel_person_center);

    lv_obj_set_width(ui_label_person_center, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_person_center, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_person_center, 14);
    lv_obj_set_y(ui_label_person_center, 0);

    lv_obj_set_align(ui_label_person_center, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_person_center, "个人中心");

    lv_obj_set_style_text_font(ui_label_person_center, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_wifi_set

    ui_panel_wifi_set = lv_obj_create(ui_panel_menus_first);

    lv_obj_set_width(ui_panel_wifi_set, 126);
    lv_obj_set_height(ui_panel_wifi_set, 42);

    lv_obj_set_x(ui_panel_wifi_set, 136);
    lv_obj_set_y(ui_panel_wifi_set, -10);

    lv_obj_clear_flag(ui_panel_wifi_set, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_panel_wifi_set, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_wifi_set, lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_wifi_set, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_wifi_set, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_wifi_set

    ui_image_wifi_set = lv_img_create(ui_panel_wifi_set);
    lv_img_set_src(ui_image_wifi_set, &img_settings_wifi_set);

    lv_obj_set_width(ui_image_wifi_set, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_wifi_set, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_wifi_set, -34);
    lv_obj_set_y(ui_image_wifi_set, 0);

    lv_obj_set_align(ui_image_wifi_set, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_wifi_set, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_wifi_set, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_wifi_set

    ui_label_wifi_set = lv_label_create(ui_panel_wifi_set);

    lv_obj_set_width(ui_label_wifi_set, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_wifi_set, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_wifi_set, 14);
    lv_obj_set_y(ui_label_wifi_set, 0);

    lv_obj_set_align(ui_label_wifi_set, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_wifi_set, "WiFi设置");

    lv_obj_set_style_text_font(ui_label_wifi_set, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_bluetooth

    ui_panel_bluetooth = lv_obj_create(ui_panel_menus_first);

    lv_obj_set_width(ui_panel_bluetooth, 126);
    lv_obj_set_height(ui_panel_bluetooth, 42);

    lv_obj_set_x(ui_panel_bluetooth, 0);
    lv_obj_set_y(ui_panel_bluetooth, 44);

    lv_obj_clear_flag(ui_panel_bluetooth, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_panel_bluetooth, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_bluetooth, lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_bluetooth, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_bluetooth, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_bluetooth

    ui_image_bluetooth = lv_img_create(ui_panel_bluetooth);
    lv_img_set_src(ui_image_bluetooth, &img_settings_bluetooth);

    lv_obj_set_width(ui_image_bluetooth, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_bluetooth, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_bluetooth, -34);
    lv_obj_set_y(ui_image_bluetooth, 0);

    lv_obj_set_align(ui_image_bluetooth, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_bluetooth, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_bluetooth, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_bluetooth

    ui_label_bluetooth = lv_label_create(ui_panel_bluetooth);

    lv_obj_set_width(ui_label_bluetooth, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_bluetooth, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_bluetooth, 14);
    lv_obj_set_y(ui_label_bluetooth, 0);

    lv_obj_set_align(ui_label_bluetooth, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_bluetooth, "蓝牙连接");

    lv_obj_set_style_text_font(ui_label_bluetooth, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_lock_screen

    ui_panel_lock_screen = lv_obj_create(ui_panel_menus_first);

    lv_obj_set_width(ui_panel_lock_screen, 126);
    lv_obj_set_height(ui_panel_lock_screen, 42);

    lv_obj_set_x(ui_panel_lock_screen, 136);
    lv_obj_set_y(ui_panel_lock_screen, 44);

    lv_obj_clear_flag(ui_panel_lock_screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_panel_lock_screen, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_lock_screen, lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_lock_screen, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_lock_screen, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_lock_screen

    ui_image_lock_screen = lv_img_create(ui_panel_lock_screen);
    lv_img_set_src(ui_image_lock_screen, &img_settings_lock_screen);

    lv_obj_set_width(ui_image_lock_screen, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_lock_screen, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_lock_screen, -34);
    lv_obj_set_y(ui_image_lock_screen, 0);

    lv_obj_set_align(ui_image_lock_screen, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_lock_screen, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_lock_screen, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_lock_screen

    ui_label_lock_screen = lv_label_create(ui_panel_lock_screen);

    lv_obj_set_width(ui_label_lock_screen, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_lock_screen, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_lock_screen, 14);
    lv_obj_set_y(ui_label_lock_screen, 0);

    lv_obj_set_align(ui_label_lock_screen, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_lock_screen, "锁屏时间");

    lv_obj_set_style_text_font(ui_label_lock_screen, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_menus_two

    ui_panel_menus_two = lv_obj_create(ui_screen_settings);

    lv_obj_set_width(ui_panel_menus_two, 280);
    lv_obj_set_height(ui_panel_menus_two, 114);

    lv_obj_set_x(ui_panel_menus_two, 0);
    lv_obj_set_y(ui_panel_menus_two, 0);

    lv_obj_set_align(ui_panel_menus_two, LV_ALIGN_BOTTOM_LEFT);

    lv_obj_clear_flag(ui_panel_menus_two, LV_OBJ_FLAG_SCROLLABLE);

    // ui_panel_volume_adjust

    ui_panel_volume_adjust = lv_obj_create(ui_panel_menus_two);

    lv_obj_set_width(ui_panel_volume_adjust, 126);
    lv_obj_set_height(ui_panel_volume_adjust, 42);

    lv_obj_set_x(ui_panel_volume_adjust, 0);
    lv_obj_set_y(ui_panel_volume_adjust, -10);

    lv_obj_clear_flag(ui_panel_volume_adjust, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_panel_volume_adjust, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_volume_adjust,
                                  lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_volume_adjust, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_volume_adjust, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_volume_adjust

    ui_image_volume_adjust = lv_img_create(ui_panel_volume_adjust);
    lv_img_set_src(ui_image_volume_adjust, &img_settings_volume);

    lv_obj_set_width(ui_image_volume_adjust, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_volume_adjust, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_volume_adjust, -34);
    lv_obj_set_y(ui_image_volume_adjust, 0);

    lv_obj_set_align(ui_image_volume_adjust, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_volume_adjust, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_volume_adjust, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_volume_adjust

    ui_label_volume_adjust = lv_label_create(ui_panel_volume_adjust);

    lv_obj_set_width(ui_label_volume_adjust, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_volume_adjust, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_volume_adjust, 14);
    lv_obj_set_y(ui_label_volume_adjust, 0);

    lv_obj_set_align(ui_label_volume_adjust, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_volume_adjust, "音量调节");

    lv_obj_set_style_text_font(ui_label_volume_adjust, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_pronounce_set

    ui_panel_pronounce_set = lv_obj_create(ui_panel_menus_two);

    lv_obj_set_width(ui_panel_pronounce_set, 126);
    lv_obj_set_height(ui_panel_pronounce_set, 42);

    lv_obj_set_x(ui_panel_pronounce_set, 136);
    lv_obj_set_y(ui_panel_pronounce_set, -10);

    lv_obj_clear_flag(ui_panel_pronounce_set, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_panel_pronounce_set, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_pronounce_set,
                                  lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_pronounce_set, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_pronounce_set, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_pronounce_set

    ui_image_pronounce_set = lv_img_create(ui_panel_pronounce_set);
    lv_img_set_src(ui_image_pronounce_set, &img_settings_pronounce);

    lv_obj_set_width(ui_image_pronounce_set, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_pronounce_set, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_pronounce_set, -34);
    lv_obj_set_y(ui_image_pronounce_set, 0);

    lv_obj_set_align(ui_image_pronounce_set, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_pronounce_set, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_pronounce_set, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_pronounce_set

    ui_label_pronounce_set = lv_label_create(ui_panel_pronounce_set);

    lv_obj_set_width(ui_label_pronounce_set, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_pronounce_set, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_pronounce_set, 14);
    lv_obj_set_y(ui_label_pronounce_set, 0);

    lv_obj_set_align(ui_label_pronounce_set, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_pronounce_set, "发音设置");

    lv_obj_set_style_text_font(ui_label_pronounce_set, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_ota_upgrade

    ui_panel_ota_upgrade = lv_obj_create(ui_panel_menus_two);

    lv_obj_set_width(ui_panel_ota_upgrade, 126);
    lv_obj_set_height(ui_panel_ota_upgrade, 42);

    lv_obj_set_x(ui_panel_ota_upgrade, 0);
    lv_obj_set_y(ui_panel_ota_upgrade, 44);

    lv_obj_clear_flag(ui_panel_ota_upgrade, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_panel_ota_upgrade, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_ota_upgrade, lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_ota_upgrade, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_ota_upgrade, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_ota_upgrade

    ui_image_ota_upgrade = lv_img_create(ui_panel_ota_upgrade);
    lv_img_set_src(ui_image_ota_upgrade, &img_settings_ota);

    lv_obj_set_width(ui_image_ota_upgrade, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_ota_upgrade, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_ota_upgrade, -34);
    lv_obj_set_y(ui_image_ota_upgrade, 0);

    lv_obj_set_align(ui_image_ota_upgrade, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_ota_upgrade, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_ota_upgrade, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_ota_upgrade

    ui_label_ota_upgrade = lv_label_create(ui_panel_ota_upgrade);

    lv_obj_set_width(ui_label_ota_upgrade, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_ota_upgrade, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_ota_upgrade, 14);
    lv_obj_set_y(ui_label_ota_upgrade, 0);

    lv_obj_set_align(ui_label_ota_upgrade, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_ota_upgrade, "版本更新");

    lv_obj_set_style_text_font(ui_label_ota_upgrade, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_binding

    ui_panel_binding = lv_obj_create(ui_panel_menus_two);

    lv_obj_set_width(ui_panel_binding, 126);
    lv_obj_set_height(ui_panel_binding, 42);

    lv_obj_set_x(ui_panel_binding, 136);
    lv_obj_set_y(ui_panel_binding, 44);

    lv_obj_clear_flag(ui_panel_binding, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_panel_binding, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_binding, lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_binding, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_binding, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_binding

    ui_image_binding = lv_img_create(ui_panel_binding);
    lv_img_set_src(ui_image_binding, &img_settings_binding);

    lv_obj_set_width(ui_image_binding, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_binding, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_binding, -34);
    lv_obj_set_y(ui_image_binding, 0);

    lv_obj_set_align(ui_image_binding, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_binding, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_binding, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_binding

    ui_label_binding = lv_label_create(ui_panel_binding);

    lv_obj_set_width(ui_label_binding, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_binding, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_binding, 0);
    lv_obj_set_y(ui_label_binding, 0);

    lv_obj_set_align(ui_label_binding, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_binding, "绑定");

    lv_obj_set_style_text_font(ui_label_binding, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_menus_three

    ui_panel_menus_three = lv_obj_create(ui_screen_settings);

    lv_obj_set_width(ui_panel_menus_three, 280);
    lv_obj_set_height(ui_panel_menus_three, 114);

    lv_obj_set_x(ui_panel_menus_three, 0);
    lv_obj_set_y(ui_panel_menus_three, 0);

    lv_obj_set_align(ui_panel_menus_three, LV_ALIGN_BOTTOM_LEFT);

    lv_obj_clear_flag(ui_panel_menus_three, LV_OBJ_FLAG_SCROLLABLE);

    // ui_panel_about

    ui_panel_about = lv_obj_create(ui_panel_menus_three);

    lv_obj_set_width(ui_panel_about, 126);
    lv_obj_set_height(ui_panel_about, 42);

    lv_obj_set_x(ui_panel_about, 0);
    lv_obj_set_y(ui_panel_about, -10);

    lv_obj_clear_flag(ui_panel_about, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_panel_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_about, lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_about, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_about, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_about

    ui_image_about = lv_img_create(ui_panel_about);
    lv_img_set_src(ui_image_about, &img_settings_about);

    lv_obj_set_width(ui_image_about, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_about, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_about, -34);
    lv_obj_set_y(ui_image_about, 0);

    lv_obj_set_align(ui_image_about, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_about, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_about, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_about

    ui_label_about = lv_label_create(ui_panel_about);

    lv_obj_set_width(ui_label_about, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_about, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_about, 0);
    lv_obj_set_y(ui_label_about, 0);

    lv_obj_set_align(ui_label_about, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_about, "关于");

    lv_obj_set_style_text_font(ui_label_about, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_img_arrow_up

    ui_img_arrow_up = lv_img_create(ui_screen_settings);
    lv_img_set_src(ui_img_arrow_up, &ic_arrow_up);

    lv_obj_set_width(ui_img_arrow_up, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_img_arrow_up, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_img_arrow_up, -4);
    lv_obj_set_y(ui_img_arrow_up, -14);

    lv_obj_set_align(ui_img_arrow_up, LV_ALIGN_RIGHT_MID);

    lv_obj_add_flag(ui_img_arrow_up,
                    LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_img_arrow_up, LV_OBJ_FLAG_SCROLLABLE);

    // ui_img_arrow_down

    ui_img_arrow_down = lv_img_create(ui_screen_settings);
    lv_img_set_src(ui_img_arrow_down, &ic_arrow_down);

    lv_obj_set_width(ui_img_arrow_down, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_img_arrow_down, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_img_arrow_down, -4);
    lv_obj_set_y(ui_img_arrow_down, 14);

    lv_obj_set_align(ui_img_arrow_down, LV_ALIGN_RIGHT_MID);

    lv_obj_add_flag(ui_img_arrow_down, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_img_arrow_down, LV_OBJ_FLAG_SCROLLABLE);

    //物理按键
    lv_group_t *group = lv_group_create();
    lv_indev_set_group(lv_keypad_device_object, group);
    lv_group_add_obj(group, ui_screen_settings);
    lv_group_set_editing(group, false);

    lv_obj_add_event_cb(ui_screen_settings, ui_screen_settings_callback,
                        LV_EVENT_ALL, NULL);

    //
    show_panel_menus_by_index(0);
}

void load_settings_screen(void) {
    // lv_disp_t * dispp = lv_disp_get_default();
    // lv_theme_t * theme = lv_theme_default_init(dispp,
    // lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
    //                                            false, LV_FONT_DEFAULT);
    // lv_disp_set_theme(dispp, theme);
    ui_screen_settings_screen_init();
    // lv_disp_load_scr(ui_screen_settings);
    // lv_scr_load_anim(ui_screen_settings, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
    _ui_screen_change(ui_screen_settings, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    // lv_scr_load(ui_screen_settings);
}
