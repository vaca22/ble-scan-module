/**
 * @file button.h
 * hardware button
 */
#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"

#include "iot_button.h"
#include "m01_config.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    uint8_t status;
    uint8_t id;
} button_dev_t;
/**********************
 * GLOBAL PROTOTYPES
 **********************/
void button_init(void);
/**********************
 *      MACROS
 **********************/
extern button_dev_t button_dev;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*__BUTTON_H__*/
