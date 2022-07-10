// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: project_home

#include "ui_res.h"
#include "ui_helpers.h"
#include <stdio.h>
#include <esp_ota_ops.h>
#include <esp_log.h>
#include "ui_control.h"

extern lv_indev_t *lv_keypad_device_object;

///////////////////// VARIABLES ////////////////////
lv_obj_t *ui_screen_factory;
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
        _ui_screen_change(ui_screen_factory, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0);
    }
}
static void ui_event_PanelSettingMain(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
}

static void ui_screen_factory_callback(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t *ta          = lv_event_get_target(e);

    if (event == LV_EVENT_KEY) {
        uint32_t *key = (uint32_t *)lv_event_get_param(e);
        switch (*key) {
            case LV_KEY_UP:
                esp_ota_mark_app_invalid_rollback_and_reboot();
                printf("ui_screen_factory LV_KEY_UP <A>\r\n");
                break;
            case LV_KEY_ENTER:
                esp_ota_mark_app_invalid_rollback_and_reboot();
                printf("ui_screen_factory LV_KEY_ENTER <B>\r\n");
                break;
            case LV_KEY_DOWN:
                esp_ota_mark_app_invalid_rollback_and_reboot();
                printf("ui_screen_factory LV_KEY_DOWN <C>\r\n");
                break;
            case LV_KEY_RIGHT:
                esp_ota_mark_app_invalid_rollback_and_reboot();
                printf("ui_screen_factory LV_KEY_RIGHT <PLAY>\r\n");
                break;
            case LV_KEY_LEFT:
                esp_ota_mark_app_invalid_rollback_and_reboot();
                printf("ui_screen_factory LV_KEY_LEFT <POWER>\r\n");
                break;
            default:
                break;
        }
    }
}


static lv_obj_t * list1;
static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        ESP_LOGE("good","Clicked: %s", lv_list_get_btn_text(list1, obj));
    } }

///////////////////// SCREENS ////////////////////
void ui_screen_factory_screen_init(void) {
    // ui_screen_factory

    ui_screen_factory = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_screen_factory, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_border_color(ui_screen_factory, lv_color_hex(0x000000),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_screen_factory, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_screen_factory, 0,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);


    lv_obj_t * arc = lv_arc_create(ui_screen_factory);
    lv_obj_set_size(arc, 50, 50);
    lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 0, 270);
    lv_arc_set_value(arc, 40);
    lv_obj_center(arc);
    lv_obj_set_style_border_color(arc, lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(arc, ui_event_Panel_word_book,
                        LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(arc, 0,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(arc, lv_color_hex(0x474747),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(arc, 255,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(arc, 1,
                                  LV_PART_MAIN | LV_STATE_DEFAULT);


    ui_Panel_word_book = lv_obj_create(ui_screen_factory);

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




    list1 = lv_list_create(ui_screen_factory);
    lv_obj_set_size(list1, 180, 100);
    lv_obj_center(list1);
/*Add buttons to the list*/
    lv_obj_t * btn;
    lv_list_add_text(list1, "File");
    btn = lv_list_add_btn(list1, LV_SYMBOL_FILE, "New");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_DIRECTORY, "Open");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_SAVE, "Save");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_CLOSE, "Delete");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);


    btn = lv_list_add_btn(list1, LV_SYMBOL_EDIT, "Edit");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    lv_list_add_text(list1, "Connectivity");
    btn = lv_list_add_btn(list1, LV_SYMBOL_BLUETOOTH, "Bluetooth");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_list_add_text(list1, "Exit");
    btn = lv_list_add_btn(list1, LV_SYMBOL_OK, "Apply");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_CLOSE, "Close");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);




    // TODO: 物理按键
    lv_group_t *group = lv_group_create();
    lv_indev_set_group(lv_keypad_device_object, group);
    lv_group_add_obj(group, ui_screen_factory);
    lv_group_set_editing(group, false);

    lv_obj_add_event_cb(ui_screen_factory, ui_screen_factory_callback, LV_EVENT_ALL,
                        NULL);
}

void load_screen_factory(void) {
    ui_screen_factory_screen_init();
    _ui_screen_change(ui_screen_factory, LV_SCR_LOAD_ANIM_FADE_ON, 0, 1000);
}
