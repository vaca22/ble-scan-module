/**
 * @file lv_indev_port.c
 * lvgl input device port
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_port.h"
#include "button.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void touch_init(void);
static void touch_read(lv_indev_drv_t *drv, lv_indev_data_t *data);
static void lv_keypad_driver_read_callback(lv_indev_drv_t *indev_drv,
                                           lv_indev_data_t *data);
/**********************
 *  STATIC VARIABLES
 **********************/
static chsc6x_t touch_dev;

/**********************
 *      MACROS
 **********************/
lv_indev_t *lv_keypad_device_object = NULL;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_indev_port_init(void) {
    /* touch init */
    touch_init();

    /* button init */
    button_init();

    /* Register an input device when enabled on the menuconfig */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = touch_read;
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);

    static lv_indev_drv_t keypad_driver;
    lv_indev_drv_init(&keypad_driver);
    keypad_driver.type      = LV_INDEV_TYPE_KEYPAD;
    keypad_driver.read_cb   = lv_keypad_driver_read_callback;
    lv_keypad_device_object = lv_indev_drv_register(&keypad_driver);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void touch_init(void) {
    gpio_reset_pin(GPIO_NUM_19);
    gpio_pad_select_gpio(GPIO_NUM_19);
    gpio_set_direction(GPIO_NUM_19, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_19, 0);

    touch_dev.interrupt_pin = TOUCH_CHSC6X_INT;
    touch_dev.power_pin     = TOUCH_CHSC6X_PWR;
    touch_dev.reset_pin     = TOUCH_CHSC6X_RST;
    chsc6x_init_desc(&touch_dev, TOUCH_CHSC6X_ADDR, TOUCH_CHSC6X_I2C,
                     TOUCH_CHSC6X_SDA, TOUCH_CHSC6X_SCL);
    chsc6x_init(&touch_dev);
}

static void touch_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    chsc6x_read_coords(&touch_dev);
    if (touch_dev.coords.is_touched) {
        data->point.x               = touch_dev.coords.x;
        data->point.y               = touch_dev.coords.y;
        data->state                 = LV_INDEV_STATE_PR;
        touch_dev.coords.is_touched = false;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

static void lv_keypad_driver_read_callback(lv_indev_drv_t *indev_drv,
                                           lv_indev_data_t *data) {
    data->state       = (lv_indev_state_t)(button_dev.status ? LV_INDEV_STATE_PR
                                                             : LV_INDEV_STATE_REL);
    button_dev.status = 0;

    switch (button_dev.id) {
        case BUTTON_KEY_A_ID:
            data->key = LV_KEY_UP;
            break;
        case BUTTON_KEY_B_ID:
            data->key = LV_KEY_ENTER;
            break;
        case BUTTON_KEY_C_ID:
            data->key = LV_KEY_DOWN;
            break;
        case BUTTON_KEY_PLAY_ID:
            data->key = LV_KEY_RIGHT;
            break;
        case BUTTON_KEY_PWR_ID:
            data->key = LV_KEY_LEFT;
            break;
    }
}
