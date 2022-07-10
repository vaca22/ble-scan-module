/**
 * @file audio.h
 * audio file
 */
#ifndef __AUDIO_H__
#define __AUDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <string.h>
#include "driver/i2s.h"
#include "esp_log.h"
#include "es8311.h"
#include "m01_config.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

// es8311
esp_err_t audio_es8311_init(void);
esp_err_t audio_es8311_play();
esp_err_t audio_es8311_play_from_file();
esp_err_t audio_es8311_record();
esp_err_t audio_es8311_record_to_file();
esp_err_t audio_es8311_deinit(void);

// cs5230e
esp_err_t audio_cs5230e_init(void);
esp_err_t audio_cs5230e_enable(void);
esp_err_t audio_cs5230e_disable(void);

void audio_echo_test(void *args);
void audio_echo_test2(void *args);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*__audio_H__*/
