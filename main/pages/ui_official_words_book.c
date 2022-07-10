// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: project_home

#include "ui_res.h"
#include "ui_helpers.h"
#include <stdio.h>
#include "ui_control.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t *ui_panel_back_1;
lv_obj_t *ui_label_back_1;
lv_obj_t *ui_image_back_1;

lv_obj_t *ui_screen_word_book;
lv_obj_t *ui_panel_thesaurus_official;
lv_obj_t *ui_image_thesaurus_official;
lv_obj_t *ui_label_thesaurus_official;
lv_obj_t *ui_panel_thesaurus_raw;
lv_obj_t *ui_image_thesaurus_raw;
lv_obj_t *ui_label_thesaurus_raw;

lv_obj_t *ui_screen_word_book_select;
lv_obj_t *ui_label_no_class_info;
lv_obj_t *ui_btn_class_info_setting;
lv_obj_t *ui_label_class_set;

void ui_screen_word_book_screen_init(void);
void ui_screen_word_book_select_screen_init();

static void ui_event_panel_back_1(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if (event == LV_EVENT_PRESSED) {
        _ui_screen_change(ui_screen_home, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    }
}

static void ui_event_panel_back_2(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if (event == LV_EVENT_PRESSED) {
        ui_screen_word_book_screen_init();
        _ui_screen_change(ui_screen_word_book, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    }
}

static void ui_event_class_set(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if (event == LV_EVENT_PRESSED) {
    }
}

static void ui_event_official_words_book(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if (event == LV_EVENT_PRESSED) {
        ui_screen_word_book_select_screen_init();
        _ui_screen_change(ui_screen_word_book_select, LV_SCR_LOAD_ANIM_FADE_ON,
                          0, 0);
    }
}

void ui_screen_word_book_class_set_screen_init() {

}

void ui_screen_word_book_select_screen_init() {
    // ui_screen_word_book_select
    ui_screen_word_book_select = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_screen_word_book_select, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_color(ui_screen_word_book_select,
                                  lv_color_hex(0xFFFFFF),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_screen_word_book_select, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_back_1
    ui_panel_back_1 = lv_obj_create(ui_screen_word_book_select);
    lv_obj_set_width(ui_panel_back_1, 34);
    lv_obj_set_height(ui_panel_back_1, 34);
    lv_obj_set_x(ui_panel_back_1, 0);
    lv_obj_set_y(ui_panel_back_1, 0);
    lv_obj_clear_flag(ui_panel_back_1, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_panel_back_1, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_panel_back_1, ui_event_panel_back_2, LV_EVENT_ALL,
                        NULL);

    // ui_image_back_1
    ui_image_back_1 = lv_img_create(ui_panel_back_1);
    lv_img_set_src(ui_image_back_1, &ic_back);
    lv_obj_set_width(ui_image_back_1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_back_1, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_image_back_1, -2);
    lv_obj_set_y(ui_image_back_1, 0);
    lv_obj_set_align(ui_image_back_1, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_image_back_1, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_back_1, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *ui_panel_info_set = lv_obj_create(ui_screen_word_book_select);
    lv_obj_set_width(ui_panel_info_set, 159);
    lv_obj_set_height(ui_panel_info_set, 17);
    lv_obj_set_pos(ui_panel_info_set, 69, 47);
    lv_obj_clear_flag(ui_panel_info_set, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_panel_info_set, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);

    // 未设置年级信息提醒
    ui_label_no_class_info = lv_label_create(ui_panel_info_set);
    lv_label_set_text(ui_label_no_class_info, "请设置个人信息");
    lv_obj_set_style_text_font(ui_label_no_class_info, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_label_no_class_info, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *ui_panel_set = lv_obj_create(ui_screen_word_book_select);
    lv_obj_set_width(ui_panel_set, 76);
    lv_obj_set_height(ui_panel_set, 28);
    lv_obj_set_pos(ui_panel_set, 110, 82);
    lv_obj_clear_flag(ui_panel_set, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_panel_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_set, lv_color_hex(0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_set, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_set, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_label_class_set = lv_label_create(ui_panel_set);
    lv_label_set_text(ui_label_class_set, "去设置");
    lv_obj_set_style_text_font(ui_label_class_set, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_label_class_set, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(ui_panel_set, ui_event_class_set, LV_EVENT_ALL,
                        NULL);
}

void ui_screen_word_book_screen_init(void) {
    // ui_screen_word_book
    ui_screen_word_book = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_screen_word_book, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_color(ui_screen_word_book, lv_color_hex(0xFFFFFF),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_screen_word_book, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_thesaurus_official
    ui_panel_thesaurus_official = lv_obj_create(ui_screen_word_book);
    lv_obj_set_width(ui_panel_thesaurus_official, 82);
    lv_obj_set_height(ui_panel_thesaurus_official, 105);
    lv_obj_set_x(ui_panel_thesaurus_official, 17);
    lv_obj_set_y(ui_panel_thesaurus_official, 33);
    lv_obj_clear_flag(ui_panel_thesaurus_official, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_panel_thesaurus_official,
                        ui_event_official_words_book, LV_EVENT_ALL, NULL);

    // ui_image_thesaurus_official
    ui_image_thesaurus_official = lv_img_create(ui_panel_thesaurus_official);
    lv_img_set_src(ui_image_thesaurus_official, &img_home_official_thesaurus);
    lv_obj_set_width(ui_image_thesaurus_official, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_thesaurus_official, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_image_thesaurus_official, 0);
    lv_obj_set_y(ui_image_thesaurus_official, -16);
    lv_obj_set_align(ui_image_thesaurus_official, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_image_thesaurus_official, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_thesaurus_official, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_thesaurus_official
    ui_label_thesaurus_official = lv_label_create(ui_panel_thesaurus_official);
    lv_obj_set_width(ui_label_thesaurus_official, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_thesaurus_official, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_label_thesaurus_official, 1);
    lv_obj_set_y(ui_label_thesaurus_official, 30);
    lv_obj_set_align(ui_label_thesaurus_official, LV_ALIGN_CENTER);
    lv_label_set_text(ui_label_thesaurus_official, "官方词库");
    lv_obj_set_style_text_font(ui_label_thesaurus_official,
                               &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_thesaurus_raw
    ui_panel_thesaurus_raw = lv_obj_create(ui_screen_word_book);
    lv_obj_set_width(ui_panel_thesaurus_raw, 82);
    lv_obj_set_height(ui_panel_thesaurus_raw, 105);
    lv_obj_set_x(ui_panel_thesaurus_raw, 107);
    lv_obj_set_y(ui_panel_thesaurus_raw, 33);
    lv_obj_clear_flag(ui_panel_thesaurus_raw, LV_OBJ_FLAG_SCROLLABLE);

    // ui_image_thesaurus_raw
    ui_image_thesaurus_raw = lv_img_create(ui_panel_thesaurus_raw);
    lv_img_set_src(ui_image_thesaurus_raw, &img_home_raw_thesaurus);
    lv_obj_set_width(ui_image_thesaurus_raw, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_thesaurus_raw, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_image_thesaurus_raw, 0);
    lv_obj_set_y(ui_image_thesaurus_raw, -16);
    lv_obj_set_align(ui_image_thesaurus_raw, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_image_thesaurus_raw, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_thesaurus_raw, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_thesaurus_raw
    ui_label_thesaurus_raw = lv_label_create(ui_panel_thesaurus_raw);
    lv_obj_set_width(ui_label_thesaurus_raw, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_thesaurus_raw, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_label_thesaurus_raw, 1);
    lv_obj_set_y(ui_label_thesaurus_raw, 30);
    lv_obj_set_align(ui_label_thesaurus_raw, LV_ALIGN_CENTER);
    lv_label_set_text(ui_label_thesaurus_raw, "生词本词库");
    lv_obj_set_style_text_font(ui_label_thesaurus_raw, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_back_1
    ui_panel_back_1 = lv_obj_create(ui_screen_word_book);
    lv_obj_set_width(ui_panel_back_1, 86);
    lv_obj_set_height(ui_panel_back_1, 34);
    lv_obj_set_x(ui_panel_back_1, 0);
    lv_obj_set_y(ui_panel_back_1, 0);
    lv_obj_clear_flag(ui_panel_back_1, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_panel_back_1, ui_event_panel_back_1, LV_EVENT_ALL,
                        NULL);
    lv_obj_set_style_radius(ui_panel_back_1, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_back_1
    ui_label_back_1 = lv_label_create(ui_panel_back_1);
    lv_obj_set_width(ui_label_back_1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_back_1, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_label_back_1, 18);
    lv_obj_set_y(ui_label_back_1, 0);
    lv_obj_set_align(ui_label_back_1, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_label_back_1, "单词书");
    lv_obj_set_style_text_font(ui_label_back_1, &ui_font_FontRegular,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_back_1
    ui_image_back_1 = lv_img_create(ui_panel_back_1);
    lv_img_set_src(ui_image_back_1, &ic_back);
    lv_obj_set_width(ui_image_back_1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_back_1, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_image_back_1, -2);
    lv_obj_set_y(ui_image_back_1, 0);
    lv_obj_set_align(ui_image_back_1, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_image_back_1, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_back_1, LV_OBJ_FLAG_SCROLLABLE);
}
