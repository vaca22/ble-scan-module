/**
 * @file lv_disp_port.c
 * lvgl display port
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_port.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void epaper_disp_init(void);
static void epaper_color_convert(uint8_t *mono, const lv_area_t *area,
                                 lv_color_t *color_p);
static void epaper_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area,
                              lv_color_t *color_p);

/**********************
 *  STATIC VARIABLES
 **********************/
static uint8_t epaper_disp_buff[EPAPER_DISP_BUFF_SIZE] = {0};
static epaper_display_t epaper_dev;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_disp_port_init(void) {
    /* epaper init */
    epaper_disp_init();

    static lv_disp_draw_buf_t disp_buf;
    lv_color_t *buf1 = heap_caps_malloc(LVGL_DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf1 != NULL);

    /* Initialize the working buffer depending on the selected display.
     * NOTE: buf2 == NULL when using monochrome displays. */
    lv_disp_draw_buf_init(&disp_buf, buf1, NULL, LVGL_DISP_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res  = EPAPER_HOR_RES_MAX;
    disp_drv.ver_res  = EPAPER_VER_RES_MAX;
    disp_drv.flush_cb = epaper_disp_flush;

    // TODO:
#ifdef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    disp_drv.rounder_cb = disp_driver_rounder;
    disp_drv.set_px_cb  = disp_driver_set_px;
#endif

    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void epaper_disp_init(void) {
    epaper_dev.spi_host  = EPAPER_SPI_HOST;
    epaper_dev.miso_pin  = EPAPER_MISO_PIN;
    epaper_dev.mosi_pin  = EPAPER_MOSI_PIN;
    epaper_dev.sclk_pin  = EPAPER_SCLK_PIN;
    epaper_dev.cs_pin    = EPAPER_CS_PIN;
    epaper_dev.dc_pin    = EPAPER_DC_PIN;
    epaper_dev.reset_pin = EPAPER_RESET_PIN;
    epaper_dev.busy_pin  = EPAPER_BUSY_PIN;

    epaper_dev.reset_level = 0;
    epaper_dev.busy_level  = 1;
    epaper_dev.data_level  = 1;
    epaper_dev.cmd_level   = 0;

    epaper_dev.width  = EPAPER_VER_RES_MAX;
    epaper_dev.height = EPAPER_HOR_RES_MAX;

    epaper_dev.power_on_time     = 1;
    epaper_dev.power_off_time    = 1;
    epaper_dev.full_refresh_time = 18;
    epaper_dev.is_power_on       = true;
    epaper_dev.hibernating       = false;
    epaper_dev.refresh_count     = 0;

    epaper_display_constructor(&epaper_dev);
    epaper_display_clear(&epaper_dev, 0x0);
    epaper_display_power_on(&epaper_dev);
}

static void epaper_color_convert(uint8_t *mono, const lv_area_t *area,
                                 lv_color_t *color_p) {
    for (size_t y = area->y1; y <= area->y2; y++) {
        for (size_t x = area->x1; x <= area->x2; x++) {
            if (lv_color_to1(*color_p) != 0) {
                epaper_disp_buff[x * EPAPER_VER_RES_MAX / 8 + (y / 8)] |=
                    (1 << (7 - (y % 8)));
            } else {
                epaper_disp_buff[x * EPAPER_VER_RES_MAX / 8 + (y / 8)] &=
                    ~(1 << (7 - (y % 8)));
            }
            color_p++;
        }
    }
}

static void epaper_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area,
                              lv_color_t *color_p) {
    epaper_color_convert(epaper_disp_buff, area, color_p);
    if (lv_disp_flush_is_last(disp_drv)) {
        epaper_display_write_buffer(&epaper_dev, epaper_disp_buff,
                                    EPAPER_DISP_BUFF_SIZE);
    }
    /* Inform the graphics library that you are ready with the flushing */
    lv_disp_flush_ready(disp_drv);
}