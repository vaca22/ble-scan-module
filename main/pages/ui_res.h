/**
 * @file ui_res.h
 * @author xm
 * @brief
 * @version 0.1
 * @date 2022-06-24
 *
 * @copyright Copyright (c) 2022
 *
 * 图片字体资源
 */

#ifndef UI_RES_H
#define UI_RES_H

#ifdef __cplusplus
extern "C"
{
#endif

#if __has_include("lvgl.h")
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

// 从SD卡中加载图片资源
#define DECODE_IMAGE_FROM_EXTERNAL_STORAGE 0
#if DECODE_IMAGE_FROM_EXTERNAL_STORAGE == 1
#define RES_IMAGE_PATH "/sdcard/imgs/"
    //
    const char *filename_ic_arrow_down = RES_IMAGE_PATH "ic_arrow_down";
    const char *filename_ic_arrow_right = RES_IMAGE_PATH "ic_arrow_right";
    const char *filename_ic_arrow_up = RES_IMAGE_PATH "ic_arrow_up";
    const char *filename_ic_back = RES_IMAGE_PATH "ic_back";
    const char *filename_ic_bar_battery_0 = RES_IMAGE_PATH "ic_bar_battery_0";
    const char *filename_ic_bar_battery_20 = RES_IMAGE_PATH "ic_bar_battery_20";
    const char *filename_ic_bar_battery_40 = RES_IMAGE_PATH "ic_bar_battery_40";
    const char *filename_ic_bar_battery_60 = RES_IMAGE_PATH "ic_bar_battery_60";
    const char *filename_ic_bar_battery_80 = RES_IMAGE_PATH "ic_bar_battery_80";
    const char *filename_ic_bar_battery_100 = RES_IMAGE_PATH "ic_bar_battery_100";
    const char *filename_ic_charge_battery_0 = RES_IMAGE_PATH "ic_charge_battery_0";
    const char *filename_ic_charge_battery_20 = RES_IMAGE_PATH "ic_charge_battery_20";
    const char *filename_ic_charge_battery_40 = RES_IMAGE_PATH "ic_charge_battery_40";
    const char *filename_ic_charge_battery_60 = RES_IMAGE_PATH "ic_charge_battery_60";
    const char *filename_ic_charge_battery_80 = RES_IMAGE_PATH "ic_charge_battery_80";
    const char *filename_ic_charge_battery_100 = RES_IMAGE_PATH "ic_charge_battery_100";
    const char *filename_ic_delete = RES_IMAGE_PATH "ic_delete";
    const char *filename_ic_list_item_book = RES_IMAGE_PATH "ic_list_item_book";
    const char *filename_ic_list_item_wifi = RES_IMAGE_PATH "ic_list_item_wifi";
    const char *filename_ic_locked = RES_IMAGE_PATH "ic_locked";
    const char *filename_ic_mute = RES_IMAGE_PATH "ic_mute";
    const char *filename_ic_refresh = RES_IMAGE_PATH "ic_refresh";
    const char *filename_ic_reporting = RES_IMAGE_PATH "ic_reporting";
    const char *filename_ic_right = RES_IMAGE_PATH "ic_right";
    const char *filename_ic_speaker = RES_IMAGE_PATH "ic_speaker";
    const char *filename_ic_switch_off = RES_IMAGE_PATH "ic_switch_off";
    const char *filename_ic_switch_on = RES_IMAGE_PATH "ic_switch_on";
    const char *filename_ic_tag_selected = RES_IMAGE_PATH "ic_tag_selected";
    const char *filename_ic_testing = RES_IMAGE_PATH "ic_testing";
    const char *filename_ic_voice_input = RES_IMAGE_PATH "ic_voice_input";
    const char *filename_ic_volume_adjust_lower = RES_IMAGE_PATH "ic_volume_adjust_lower";
    const char *filename_ic_ic_volume_adjust_plus = RES_IMAGE_PATH "ic_volume_adjust_plus";
    const char *filename_img_boot_start = RES_IMAGE_PATH "img_boot_start";
    const char *filename_img_gesture_guideg = RES_IMAGE_PATH "img_gesture_guide";
    const char *filename_img_home_dictation = RES_IMAGE_PATH "img_home_dictation";
    const char *filename_img_home_official_thesaurus = RES_IMAGE_PATH "img_home_official_thesaurus";
    const char *filename_img_home_raw_thesaurus = RES_IMAGE_PATH "img_home_raw_thesaurus";
    const char *filename_img_home_settings = RES_IMAGE_PATH "img_home_settings";
    const char *filename_img_home_word_book = RES_IMAGE_PATH "img_home_word_book";
    const char *filename_img_settings_about = RES_IMAGE_PATH "img_settings_about";
    const char *filename_img_settings_binding = RES_IMAGE_PATH "img_settings_binding";
    const char *filename_img_settings_bluetooth = RES_IMAGE_PATH "img_settings_bluetooth";
    const char *filename_img_settings_lock_screen = RES_IMAGE_PATH "img_settings_lock_screen";
    const char *filename_img_settings_ota = RES_IMAGE_PATH "img_settings_ota";
    const char *filename_img_settings_personal = RES_IMAGE_PATH "img_settings_personal";
    const char *filename_img_settings_pronounce = RES_IMAGE_PATH "img_settings_pronounce";
    const char *filename_img_settings_volume = RES_IMAGE_PATH "img_settings_volume";
    const char *filename_img_settings_wifi_set = RES_IMAGE_PATH "img_settings_wifi_set";

#else
LV_IMG_DECLARE(ic_arrow_down);
LV_IMG_DECLARE(ic_arrow_right);
LV_IMG_DECLARE(ic_arrow_up);
LV_IMG_DECLARE(ic_back);
LV_IMG_DECLARE(ic_bar_battery_0);
LV_IMG_DECLARE(ic_bar_battery_20);
LV_IMG_DECLARE(ic_bar_battery_40);
LV_IMG_DECLARE(ic_bar_battery_60);
LV_IMG_DECLARE(ic_bar_battery_80);
LV_IMG_DECLARE(ic_bar_battery_100);
LV_IMG_DECLARE(ic_charge_battery_0);
LV_IMG_DECLARE(ic_charge_battery_20);
LV_IMG_DECLARE(ic_charge_battery_40);
LV_IMG_DECLARE(ic_charge_battery_60);
LV_IMG_DECLARE(ic_charge_battery_80);
LV_IMG_DECLARE(ic_charge_battery_100);
LV_IMG_DECLARE(ic_delete);
LV_IMG_DECLARE(ic_list_item_book);
LV_IMG_DECLARE(ic_list_item_wifi);
LV_IMG_DECLARE(ic_locked);
LV_IMG_DECLARE(ic_mute);
LV_IMG_DECLARE(ic_refresh);
LV_IMG_DECLARE(ic_reporting);
LV_IMG_DECLARE(ic_right);
LV_IMG_DECLARE(ic_speaker);
LV_IMG_DECLARE(ic_switch_off);
LV_IMG_DECLARE(ic_switch_on);
LV_IMG_DECLARE(ic_tag_selected);
LV_IMG_DECLARE(ic_testing);
LV_IMG_DECLARE(ic_voice_input);
LV_IMG_DECLARE(ic_volume_adjust_lower);
LV_IMG_DECLARE(ic_volume_adjust_plus);
LV_IMG_DECLARE(img_boot_start);
LV_IMG_DECLARE(img_gesture_guide);
LV_IMG_DECLARE(img_home_dictation);
LV_IMG_DECLARE(img_home_official_thesaurus);
LV_IMG_DECLARE(img_home_raw_thesaurus);
LV_IMG_DECLARE(img_home_settings);
LV_IMG_DECLARE(img_home_word_book);
LV_IMG_DECLARE(img_settings_about);
LV_IMG_DECLARE(img_settings_binding);
LV_IMG_DECLARE(img_settings_bluetooth);
LV_IMG_DECLARE(img_settings_lock_screen);
LV_IMG_DECLARE(img_settings_ota);
LV_IMG_DECLARE(img_settings_personal);
LV_IMG_DECLARE(img_settings_pronounce);
LV_IMG_DECLARE(img_settings_volume);
LV_IMG_DECLARE(img_settings_wifi_set);
#endif

    LV_FONT_DECLARE(ui_font_FontRegular);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
