/**
 * @file lv_port.h
 * lvgl port
 */
#ifndef __LV_PORT_H__
#define __LV_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <string.h>

#include "epaper.h"
#include "chsc5448.h"
#include "m01_config.h"

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_disp_port_init(void);
void lv_indev_port_init(void);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*__LV_PORT_H__*/
