// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: project_home

#include "ui_res.h"
#include "ui_helpers.h"
#include <stdio.h>
#include "ui_control.h"

extern lv_indev_t *lv_keypad_device_object;

///////////////////// VARIABLES ////////////////////
lv_obj_t *ui_screen_home;
lv_obj_t *ui_Panel_word_book;
lv_obj_t *ui_image_word_book;
lv_obj_t *ui_label_word_book;
lv_obj_t *ui_panel_speak_dictation;
lv_obj_t *ui_image_speak_dictation;
lv_obj_t *ui_label_speak_dictation;
lv_obj_t *ui_panel_settings;
lv_obj_t *ui_image_settings;
lv_obj_t *ui_label_settings;
lv_obj_t *ui_label_electricity_percent;
lv_obj_t *ui_image_electricity;

extern lv_obj_t *ui_screen_word_book;
extern void ui_screen_word_book_screen_init(void);
///////////////////// TEST LVGL SETTINGS ////////////////////

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
static void ui_event_Panel_word_book(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if (event == LV_EVENT_PRESSED) {
        ui_screen_word_book_screen_init();
        _ui_screen_change(ui_screen_word_book, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    }
}
static void ui_event_panel_settings(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if (event == LV_EVENT_PRESSED) {
        load_settings_screen();
    }
}

static void ui_event_panel_back_2(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if (event == LV_EVENT_CLICKED) {
        _ui_screen_change(ui_screen_home, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    }
}
static void ui_event_PanelSettingMain(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
}

static void ui_screen_home_callback(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t *ta          = lv_event_get_target(e);

    if (event == LV_EVENT_KEY) {
        uint32_t *key = (uint32_t *)lv_event_get_param(e);
        switch (*key) {
            case LV_KEY_UP:
                printf("ui_screen_home LV_KEY_UP <A>\r\n");
                break;
            case LV_KEY_ENTER:
                printf("ui_screen_home LV_KEY_ENTER <B>\r\n");
                break;
            case LV_KEY_DOWN:
                printf("ui_screen_home LV_KEY_DOWN <C>\r\n");
                break;
            case LV_KEY_RIGHT:
                printf("ui_screen_home LV_KEY_RIGHT <PLAY>\r\n");
                break;
            case LV_KEY_LEFT:
                printf("ui_screen_home LV_KEY_LEFT <POWER>\r\n");
                break;
            default:
                break;
        }
    }
}

///////////////////// SCREENS ////////////////////
void ui_screen_factory_screen_init(void) {
    // ui_screen_home

    ui_screen_home = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_screen_home, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_border_color(ui_screen_home, lv_color_hex(0x000000),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_screen_home, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_screen_home, 0,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Panel_word_book
    ui_Panel_word_book = lv_obj_create(ui_screen_home);

    lv_obj_set_width(ui_Panel_word_book, 82);
    lv_obj_set_height(ui_Panel_word_book, 105);

    lv_obj_set_x(ui_Panel_word_book, 17);
    lv_obj_set_y(ui_Panel_word_book, 33);

    lv_obj_clear_flag(ui_Panel_word_book, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_Panel_word_book, ui_event_Panel_word_book,
                        LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(ui_Panel_word_book, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel_word_book, lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel_word_book, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel_word_book, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_word_book
    ui_image_word_book = lv_img_create(ui_Panel_word_book);
    lv_img_set_src(ui_image_word_book, &img_home_word_book);

    lv_obj_set_width(ui_image_word_book, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_word_book, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_word_book, 0);
    lv_obj_set_y(ui_image_word_book, -16);

    lv_obj_set_align(ui_image_word_book, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_word_book, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_word_book, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_bg_color(ui_image_word_book, lv_color_hex(0xFFFFFF),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_image_word_book, 255,
                            LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_word_book
    ui_label_word_book = lv_label_create(ui_Panel_word_book);
    lv_obj_set_width(ui_label_word_book, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_word_book, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_label_word_book, 1);
    lv_obj_set_y(ui_label_word_book, 30);

    lv_obj_set_align(ui_label_word_book, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_word_book, "单词本");

    lv_obj_set_style_text_font(ui_label_word_book, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_speak_dictation

    ui_panel_speak_dictation = lv_obj_create(ui_screen_home);

    lv_obj_set_width(ui_panel_speak_dictation, 82);
    lv_obj_set_height(ui_panel_speak_dictation, 105);

    lv_obj_set_x(ui_panel_speak_dictation, 107);
    lv_obj_set_y(ui_panel_speak_dictation, 33);

    lv_obj_clear_flag(ui_panel_speak_dictation, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_panel_speak_dictation, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_speak_dictation,
                                  lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_speak_dictation, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_speak_dictation, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_speak_dictation

    ui_image_speak_dictation = lv_img_create(ui_panel_speak_dictation);
    lv_img_set_src(ui_image_speak_dictation, &img_home_dictation);

    lv_obj_set_width(ui_image_speak_dictation, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_speak_dictation, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_speak_dictation, 0);
    lv_obj_set_y(ui_image_speak_dictation, -16);

    lv_obj_set_align(ui_image_speak_dictation, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_speak_dictation, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_speak_dictation, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_speak_dictation

    ui_label_speak_dictation = lv_label_create(ui_panel_speak_dictation);

    lv_obj_set_width(ui_label_speak_dictation, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_speak_dictation, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_speak_dictation, 1);
    lv_obj_set_y(ui_label_speak_dictation, 30);

    lv_obj_set_align(ui_label_speak_dictation, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_speak_dictation, "报听写");

    lv_obj_set_style_text_font(ui_label_speak_dictation, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_settings

    ui_panel_settings = lv_obj_create(ui_screen_home);

    lv_obj_set_width(ui_panel_settings, 82);
    lv_obj_set_height(ui_panel_settings, 105);

    lv_obj_set_x(ui_panel_settings, -17);
    lv_obj_set_y(ui_panel_settings, 33);

    lv_obj_set_align(ui_panel_settings, LV_ALIGN_TOP_RIGHT);

    lv_obj_clear_flag(ui_panel_settings, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_panel_settings, ui_event_panel_settings,
                        LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(ui_panel_settings, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_settings, lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_settings, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_settings, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_settings

    ui_image_settings = lv_img_create(ui_panel_settings);
    lv_img_set_src(ui_image_settings, &img_home_settings);

    lv_obj_set_width(ui_image_settings, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_settings, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_settings, 0);
    lv_obj_set_y(ui_image_settings, -16);

    lv_obj_set_align(ui_image_settings, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_settings, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_settings, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_settings

    ui_label_settings = lv_label_create(ui_panel_settings);

    lv_obj_set_width(ui_label_settings, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_settings, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_settings, 1);
    lv_obj_set_y(ui_label_settings, 30);

    lv_obj_set_align(ui_label_settings, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_settings, "设置");

    lv_obj_set_style_text_font(ui_label_settings, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_electricity_percent

    ui_label_electricity_percent = lv_label_create(ui_screen_home);

    lv_obj_set_width(ui_label_electricity_percent, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_electricity_percent, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_electricity_percent, 89);
    lv_obj_set_y(ui_label_electricity_percent, -61);

    lv_obj_set_align(ui_label_electricity_percent, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_electricity_percent, "100%");

    lv_obj_set_style_text_font(ui_label_electricity_percent,
                               &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_electricity

    ui_image_electricity = lv_img_create(ui_screen_home);
    lv_img_set_src(ui_image_electricity, &ic_bar_battery_100);

    lv_obj_set_width(ui_image_electricity, 20);
    lv_obj_set_height(ui_image_electricity, 12);

    lv_obj_set_x(ui_image_electricity, 124);
    lv_obj_set_y(ui_image_electricity, -62);

    lv_obj_set_align(ui_image_electricity, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_image_electricity, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_electricity, LV_OBJ_FLAG_SCROLLABLE);

    // TODO: 物理按键
    lv_group_t *group = lv_group_create();
    lv_indev_set_group(lv_keypad_device_object, group);
    lv_group_add_obj(group, ui_screen_home);
    lv_group_set_editing(group, false);

    lv_obj_add_event_cb(ui_screen_home, ui_screen_home_callback, LV_EVENT_ALL,
                        NULL);
}

void load_screen_factory(void) {
    ui_screen_factory_screen_init();
    _ui_screen_change(ui_screen_home, LV_SCR_LOAD_ANIM_FADE_ON, 0, 1000);
}
