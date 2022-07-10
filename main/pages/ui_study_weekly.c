// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: Study_Weekly

#include "ui_res.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_screen_study_weekly;
lv_obj_t * ui_panel_back;
lv_obj_t * ui_image_back;
lv_obj_t * ui_label_back_title;
lv_obj_t * ui_panel_content_pass;
lv_obj_t * ui_label_pass_vocabulary_value;
lv_obj_t * ui_label_pass_vocabulary;
lv_obj_t * ui_label_read_vocabulary;
lv_obj_t * ui_label_read_vocabulary_value;
lv_obj_t * ui_label_hearing_vocabulary;
lv_obj_t * ui_label_hearing_vocabulary_value;
lv_obj_t * ui_label_spoken_vocabulary;
lv_obj_t * ui_label_spoken_vocabulary_value;
lv_obj_t * ui_panel_content_btd;
lv_obj_t * ui_label_btd_value;
lv_obj_t * ui_label_btd;
lv_obj_t * ui_label_read_count;
lv_obj_t * ui_label_read_count_value;
lv_obj_t * ui_label_hearing_count;
lv_obj_t * ui_label_hearing_count_value;
lv_obj_t * ui_label_spoken_count;
lv_obj_t * ui_label_spoken_count_value;
lv_obj_t * ui_image_arrow_up;
lv_obj_t * ui_image_arrow_down;

///////////////////// TEST LVGL SETTINGS ////////////////////

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////
void ui_screen_study_weekly_screen_init(void)
{

    // ui_screen_study_weekly

    ui_screen_study_weekly = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_screen_study_weekly, LV_OBJ_FLAG_SCROLLABLE);

    // ui_panel_back

    ui_panel_back = lv_obj_create(ui_screen_study_weekly);

    lv_obj_set_width(ui_panel_back, 100);
    lv_obj_set_height(ui_panel_back, 34);

    lv_obj_set_x(ui_panel_back, 0);
    lv_obj_set_y(ui_panel_back, 0);

    lv_obj_clear_flag(ui_panel_back, LV_OBJ_FLAG_SCROLLABLE);

    // ui_image_back

    ui_image_back = lv_img_create(ui_panel_back);
    lv_img_set_src(ui_image_back, &ic_back);

    lv_obj_set_width(ui_image_back, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_back, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_back, -8);
    lv_obj_set_y(ui_image_back, 0);

    lv_obj_set_align(ui_image_back, LV_ALIGN_LEFT_MID);

    lv_obj_add_flag(ui_image_back, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_back, LV_OBJ_FLAG_SCROLLABLE);

    // ui_label_back_title

    ui_label_back_title = lv_label_create(ui_panel_back);

    lv_obj_set_width(ui_label_back_title, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_back_title, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_back_title, 4);
    lv_obj_set_y(ui_label_back_title, 0);

    lv_obj_set_align(ui_label_back_title, LV_ALIGN_CENTER);

    lv_label_set_text(ui_label_back_title, "学习周报");

    lv_obj_set_style_text_font(ui_label_back_title, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_content_pass

    ui_panel_content_pass = lv_obj_create(ui_screen_study_weekly);

    lv_obj_set_width(ui_panel_content_pass, 265);
    lv_obj_set_height(ui_panel_content_pass, 109);

    lv_obj_set_x(ui_panel_content_pass, -5);
    lv_obj_set_y(ui_panel_content_pass, 13);

    lv_obj_set_align(ui_panel_content_pass, LV_ALIGN_CENTER);

    lv_obj_clear_flag(ui_panel_content_pass, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_panel_content_pass, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_content_pass, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_content_pass, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_content_pass, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_pass_vocabulary_value

    ui_label_pass_vocabulary_value = lv_label_create(ui_panel_content_pass);

    lv_obj_set_width(ui_label_pass_vocabulary_value, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_pass_vocabulary_value, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_pass_vocabulary_value, 24);
    lv_obj_set_y(ui_label_pass_vocabulary_value, 15);

    lv_label_set_text(ui_label_pass_vocabulary_value, "2000");

    // ui_label_pass_vocabulary

    ui_label_pass_vocabulary = lv_label_create(ui_panel_content_pass);

    lv_obj_set_width(ui_label_pass_vocabulary, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_pass_vocabulary, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_pass_vocabulary, 8);
    lv_obj_set_y(ui_label_pass_vocabulary, 44);

    lv_label_set_text(ui_label_pass_vocabulary, "通关词汇量");

    lv_obj_set_style_text_font(ui_label_pass_vocabulary, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_read_vocabulary

    ui_label_read_vocabulary = lv_label_create(ui_panel_content_pass);

    lv_obj_set_width(ui_label_read_vocabulary, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_read_vocabulary, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_read_vocabulary, 118);
    lv_obj_set_y(ui_label_read_vocabulary, 0);

    lv_label_set_text(ui_label_read_vocabulary, "阅读词汇量");

    lv_obj_set_style_text_font(ui_label_read_vocabulary, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_read_vocabulary_value

    ui_label_read_vocabulary_value = lv_label_create(ui_panel_content_pass);

    lv_obj_set_width(ui_label_read_vocabulary_value, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_read_vocabulary_value, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_read_vocabulary_value, 200);
    lv_obj_set_y(ui_label_read_vocabulary_value, 0);

    lv_label_set_text(ui_label_read_vocabulary_value, "1200");

    lv_obj_set_style_text_font(ui_label_read_vocabulary_value, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_hearing_vocabulary

    ui_label_hearing_vocabulary = lv_label_create(ui_panel_content_pass);

    lv_obj_set_width(ui_label_hearing_vocabulary, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_hearing_vocabulary, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_hearing_vocabulary, 118);
    lv_obj_set_y(ui_label_hearing_vocabulary, 0);

    lv_obj_set_align(ui_label_hearing_vocabulary, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_label_hearing_vocabulary, "听力词汇量");

    lv_obj_set_style_text_font(ui_label_hearing_vocabulary, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_hearing_vocabulary_value

    ui_label_hearing_vocabulary_value = lv_label_create(ui_panel_content_pass);

    lv_obj_set_width(ui_label_hearing_vocabulary_value, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_hearing_vocabulary_value, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_hearing_vocabulary_value, 200);
    lv_obj_set_y(ui_label_hearing_vocabulary_value, 0);

    lv_obj_set_align(ui_label_hearing_vocabulary_value, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_label_hearing_vocabulary_value, "1800");

    lv_obj_set_style_text_font(ui_label_hearing_vocabulary_value, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_spoken_vocabulary

    ui_label_spoken_vocabulary = lv_label_create(ui_panel_content_pass);

    lv_obj_set_width(ui_label_spoken_vocabulary, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_spoken_vocabulary, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_spoken_vocabulary, 118);
    lv_obj_set_y(ui_label_spoken_vocabulary, 0);

    lv_obj_set_align(ui_label_spoken_vocabulary, LV_ALIGN_BOTTOM_LEFT);

    lv_label_set_text(ui_label_spoken_vocabulary, "口语词汇量");

    lv_obj_set_style_text_font(ui_label_spoken_vocabulary, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_spoken_vocabulary_value

    ui_label_spoken_vocabulary_value = lv_label_create(ui_panel_content_pass);

    lv_obj_set_width(ui_label_spoken_vocabulary_value, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_spoken_vocabulary_value, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_spoken_vocabulary_value, 200);
    lv_obj_set_y(ui_label_spoken_vocabulary_value, 0);

    lv_obj_set_align(ui_label_spoken_vocabulary_value, LV_ALIGN_BOTTOM_LEFT);

    lv_label_set_text(ui_label_spoken_vocabulary_value, "1000");

    lv_obj_set_style_text_font(ui_label_spoken_vocabulary_value, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_panel_content_btd

    ui_panel_content_btd = lv_obj_create(ui_screen_study_weekly);

    lv_obj_set_width(ui_panel_content_btd, 265);
    lv_obj_set_height(ui_panel_content_btd, 109);

    lv_obj_set_x(ui_panel_content_btd, -5);
    lv_obj_set_y(ui_panel_content_btd, 13);

    lv_obj_set_align(ui_panel_content_btd, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_panel_content_btd, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_panel_content_btd, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_panel_content_btd, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_content_btd, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_content_btd, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_panel_content_btd, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_btd_value

    ui_label_btd_value = lv_label_create(ui_panel_content_btd);

    lv_obj_set_width(ui_label_btd_value, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_btd_value, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_btd_value, 24);
    lv_obj_set_y(ui_label_btd_value, 15);

    lv_label_set_text(ui_label_btd_value, "2000");

    // ui_label_btd

    ui_label_btd = lv_label_create(ui_panel_content_btd);

    lv_obj_set_width(ui_label_btd, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_btd, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_btd, 0);
    lv_obj_set_y(ui_label_btd, 44);

    lv_label_set_text(ui_label_btd, "背诵`速听`跟读");

    lv_obj_set_style_text_font(ui_label_btd, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_read_count

    ui_label_read_count = lv_label_create(ui_panel_content_btd);

    lv_obj_set_width(ui_label_read_count, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_read_count, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_read_count, 118);
    lv_obj_set_y(ui_label_read_count, 0);

    lv_label_set_text(ui_label_read_count, "阅读词次");

    lv_obj_set_style_text_font(ui_label_read_count, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_read_count_value

    ui_label_read_count_value = lv_label_create(ui_panel_content_btd);

    lv_obj_set_width(ui_label_read_count_value, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_read_count_value, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_read_count_value, 200);
    lv_obj_set_y(ui_label_read_count_value, 0);

    lv_label_set_text(ui_label_read_count_value, "1200");

    lv_obj_set_style_text_font(ui_label_read_count_value, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_hearing_count

    ui_label_hearing_count = lv_label_create(ui_panel_content_btd);

    lv_obj_set_width(ui_label_hearing_count, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_hearing_count, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_hearing_count, 118);
    lv_obj_set_y(ui_label_hearing_count, 0);

    lv_obj_set_align(ui_label_hearing_count, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_label_hearing_count, "听力词次");

    lv_obj_set_style_text_font(ui_label_hearing_count, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_hearing_count_value

    ui_label_hearing_count_value = lv_label_create(ui_panel_content_btd);

    lv_obj_set_width(ui_label_hearing_count_value, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_hearing_count_value, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_hearing_count_value, 200);
    lv_obj_set_y(ui_label_hearing_count_value, 0);

    lv_obj_set_align(ui_label_hearing_count_value, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_label_hearing_count_value, "1800");

    lv_obj_set_style_text_font(ui_label_hearing_count_value, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_spoken_count

    ui_label_spoken_count = lv_label_create(ui_panel_content_btd);

    lv_obj_set_width(ui_label_spoken_count, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_spoken_count, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_spoken_count, 118);
    lv_obj_set_y(ui_label_spoken_count, 0);

    lv_obj_set_align(ui_label_spoken_count, LV_ALIGN_BOTTOM_LEFT);

    lv_label_set_text(ui_label_spoken_count, "口语词次");

    lv_obj_set_style_text_font(ui_label_spoken_count, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_label_spoken_count_value

    ui_label_spoken_count_value = lv_label_create(ui_panel_content_btd);

    lv_obj_set_width(ui_label_spoken_count_value, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_label_spoken_count_value, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_label_spoken_count_value, 200);
    lv_obj_set_y(ui_label_spoken_count_value, 0);

    lv_obj_set_align(ui_label_spoken_count_value, LV_ALIGN_BOTTOM_LEFT);

    lv_label_set_text(ui_label_spoken_count_value, "1000");

    lv_obj_set_style_text_font(ui_label_spoken_count_value, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_image_arrow_up

    ui_image_arrow_up = lv_img_create(ui_screen_study_weekly);
    lv_img_set_src(ui_image_arrow_up, &ic_arrow_up);

    lv_obj_set_width(ui_image_arrow_up, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_arrow_up, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_arrow_up, -6);
    lv_obj_set_y(ui_image_arrow_up, -14);

    lv_obj_set_align(ui_image_arrow_up, LV_ALIGN_RIGHT_MID);

    lv_obj_add_flag(ui_image_arrow_up, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_arrow_up, LV_OBJ_FLAG_SCROLLABLE);

    // ui_image_arrow_down

    ui_image_arrow_down = lv_img_create(ui_screen_study_weekly);
    lv_img_set_src(ui_image_arrow_down, &ic_arrow_down);

    lv_obj_set_width(ui_image_arrow_down, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_image_arrow_down, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_image_arrow_down, -6);
    lv_obj_set_y(ui_image_arrow_down, 14);

    lv_obj_set_align(ui_image_arrow_down, LV_ALIGN_RIGHT_MID);

    lv_obj_add_flag(ui_image_arrow_down, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_image_arrow_down, LV_OBJ_FLAG_SCROLLABLE);

}

void load_screen_study_weekly(void)
{
    ui_screen_study_weekly_screen_init();
    lv_disp_load_scr(ui_screen_study_weekly);
}

