// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: M01_ui_wifi

#include "ui_res.h"
#include "ui_helpers.h"
#include "stdio.h"
#include "ui_control.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_ScreenWifi;
lv_obj_t * ui_PanelBack;
lv_obj_t * ui_Image1;
lv_obj_t * ui_Label1;
lv_obj_t * ui_Switch1;
lv_obj_t * ui_ImageRefresh;

lv_obj_t * list_wifi;

///////////////////// TEST LVGL SETTINGS ////////////////////

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////


static void event_back(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    // lv_obj_t *ta = lv_event_get_target(e);
    if(event == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_screen_boot_guide, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, false);
    }
}


static void list_event_handler(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
	if (event == LV_EVENT_CLICKED) {
		printf("Clicked: %s\n", lv_list_get_btn_text(list_wifi, ta));
	}

}
///////////////////// SCREENS ////////////////////
void ui_ScreenWifi_screen_init(void)
{

    // ui_ScreenWifi

    ui_ScreenWifi = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_ScreenWifi, LV_OBJ_FLAG_SCROLLABLE);

    // ui_PanelBack

    ui_PanelBack = lv_obj_create(ui_ScreenWifi);

    lv_obj_set_width(ui_PanelBack, 100);
    lv_obj_set_height(ui_PanelBack, 34);

    lv_obj_set_x(ui_PanelBack, 0);
    lv_obj_set_y(ui_PanelBack, 0);

    lv_obj_clear_flag(ui_PanelBack, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_PanelBack, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_PanelBack, event_back, LV_EVENT_ALL, NULL);

    // ui_Image1

    ui_Image1 = lv_img_create(ui_PanelBack);
    lv_img_set_src(ui_Image1, &ic_back);

    lv_obj_set_width(ui_Image1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Image1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Image1, -3);
    lv_obj_set_y(ui_Image1, 0);

    lv_obj_set_align(ui_Image1, LV_ALIGN_LEFT_MID);

    lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_Image1, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Label1

    ui_Label1 = lv_label_create(ui_PanelBack);

    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label1, 11);
    lv_obj_set_y(ui_Label1, 0);

    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label1, "WiFi设置");

    lv_obj_set_style_text_font(ui_Label1, &ui_font_FontRegular, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Switch1

    ui_Switch1 = lv_switch_create(ui_ScreenWifi);

    lv_obj_set_width(ui_Switch1, 42);
    lv_obj_set_height(ui_Switch1, 24);

    lv_obj_set_x(ui_Switch1, -40);
    lv_obj_set_y(ui_Switch1, 8);

    lv_obj_set_align(ui_Switch1, LV_ALIGN_TOP_RIGHT);

    lv_obj_set_style_bg_color(ui_Switch1, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Switch1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_ImageRefresh

    ui_ImageRefresh = lv_img_create(ui_ScreenWifi);
    lv_img_set_src(ui_ImageRefresh, &ic_refresh);

    lv_obj_set_width(ui_ImageRefresh, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ImageRefresh, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_ImageRefresh, -7);
    lv_obj_set_y(ui_ImageRefresh, 12);

    lv_obj_set_align(ui_ImageRefresh, LV_ALIGN_TOP_RIGHT);

    lv_obj_add_flag(ui_ImageRefresh, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ImageRefresh, LV_OBJ_FLAG_SCROLLABLE);


    // wifi list
    list_wifi = lv_list_create(ui_ScreenWifi);
	lv_obj_set_size(list_wifi, 296, 200);
	lv_obj_align(list_wifi, LV_ALIGN_TOP_LEFT, 0, 38);
	//16.2 添加按钮到列表控件
	lv_obj_t * list_btn;
	list_btn = lv_list_add_btn(list_wifi, LV_SYMBOL_FILE, "New");
	lv_obj_add_event_cb(list_btn, list_event_handler, LV_EVENT_ALL, NULL);
	list_btn = lv_list_add_btn(list_wifi, LV_SYMBOL_DIRECTORY, "Open");
	lv_obj_add_event_cb(list_btn, list_event_handler, LV_EVENT_ALL, NULL);
	list_btn = lv_list_add_btn(list_wifi, LV_SYMBOL_CLOSE, "Delete");
	lv_obj_add_event_cb(list_btn, list_event_handler, LV_EVENT_ALL, NULL);
	list_btn = lv_list_add_btn(list_wifi, LV_SYMBOL_EDIT, "Edit");
	lv_obj_add_event_cb(list_btn, list_event_handler, LV_EVENT_ALL, NULL);
	list_btn = lv_list_add_btn(list_wifi, NULL, "Save");
	lv_obj_add_event_cb(list_btn, list_event_handler, LV_EVENT_ALL, NULL);

    // /*Register a button input device*/
    // lv_indev_drv_t indev_drv;
    // lv_indev_drv_init(&indev_drv);
    // indev_drv.type = LV_INDEV_TYPE_BUTTON;
    // indev_drv.read_cb = button_read;
    // lv_indev_t *indev_button = lv_indev_drv_register(&indev_drv);

    // /*Assign buttons to points on the screen*/
    // static const lv_point_t btn_points[2] = {
    //         {10, 10},   /*Button 0 -> x:10; y:10*/
    //         {40, 100},  /*Button 1 -> x:40; y:100*/
    // };
    // lv_indev_set_button_points(indev_button, btn_points);

    // lv_group_t * g = lv_group_create();
    // lv_group_add_obj(g, list_wifi);
    // lv_indev_set_group(indev_button, g);
    
}

void ui_load_wifi_screen()
{
    // lv_disp_t * dispp = lv_disp_get_default();
    // lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
    //                                            false, LV_FONT_DEFAULT);
    // lv_disp_set_theme(dispp, theme);
    ui_ScreenWifi_screen_init();
    lv_disp_load_scr(ui_ScreenWifi);
    // lv_scr_load_anim(ui_ScreenWifi, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, false);
}

