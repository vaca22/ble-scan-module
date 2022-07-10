/**
 * @file setting.h
 * setting file
 */
#ifndef __SETTING_H__
#define __SETTING_H__

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
esp_err_t setting_nvs_init();
esp_err_t setting_nvs_save_str_value(const char *key, const char *value);
esp_err_t setting_nvs_read_str_value(const char *key, char *out_value,
                                     size_t *length);
esp_err_t setting_nvs_save_int_value(const char *key, int16_t value);
esp_err_t setting_nvs_read_int_value(const char *key, int16_t *out_value);
// TODO:
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*__setting_H__*/
