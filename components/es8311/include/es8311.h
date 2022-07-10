// Copyright 2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "esp_types.h"
#include "esp_err.h"
#include "i2cdev.h"
#include "audio_hal.h"
#include "esxxx_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ES8311 address: CE pin low - 0x18, CE pin high - 0x19 */
#define ES8311_ADDRRES_0 0x18u
#define ES8311_ADDRESS_1 0x19u

#define FROM_MCLK_PIN      0
#define FROM_SCLK_PIN      1
#define ES8311_MCLK_SOURCE FROM_SCLK_PIN
#define INVERT_MCLK        0  // do not invert
#define INVERT_SCLK        0
#define IS_DMIC            0  // Is it a digital microphone

typedef struct {
    i2c_dev_t i2c_dev;
} es8311_dev_t;

typedef enum {
    ES8311_MIC_GAIN_MIN = -1,
    ES8311_MIC_GAIN_0DB,
    ES8311_MIC_GAIN_6DB,
    ES8311_MIC_GAIN_12DB,
    ES8311_MIC_GAIN_18DB,
    ES8311_MIC_GAIN_24DB,
    ES8311_MIC_GAIN_30DB,
    ES8311_MIC_GAIN_36DB,
    ES8311_MIC_GAIN_42DB,
    ES8311_MIC_GAIN_MAX
} es8311_mic_gain_t;

typedef enum es8311_resolution_t {
    ES8311_RESOLUTION_16 = 16,
    ES8311_RESOLUTION_18 = 18,
    ES8311_RESOLUTION_20 = 20,
    ES8311_RESOLUTION_24 = 24,
    ES8311_RESOLUTION_32 = 32
} es8311_resolution_t;

typedef struct es8311_clock_config_t {
    bool mclk_inverted;
    bool sclk_inverted;
    bool mclk_from_mclk_pin;  // true: from MCLK pin (pin no. 2), false: from
                              // SCLK pin (pin no. 6)
    int sample_frequency;     // in Hz
} es8311_clock_config_t;

esp_err_t es8311_init_desc(es8311_dev_t *dev, uint8_t addr, i2c_port_t port,
                           gpio_num_t sda_gpio, gpio_num_t scl_gpio);
esp_err_t ex8311_free_desc(es8311_dev_t *dev);

esp_err_t es8311_codec_init(es8311_dev_t *dev, uint32_t samples);
esp_err_t es8311_set_bits_per_sample(es8311_dev_t *dev,
                                     audio_hal_iface_bits_t bits);
esp_err_t es8311_config_fmt(es8311_dev_t *dev, es_i2s_fmt_t fmt);
esp_err_t es8311_codec_ctrl_state(es8311_dev_t *dev,
                                  audio_hal_codec_mode_t mode,
                                  audio_hal_ctrl_t ctrl_state);
esp_err_t es8311_start(es8311_dev_t *dev, es_module_t mode);
esp_err_t es8311_stop(es8311_dev_t *dev, es_module_t mode);
esp_err_t es8311_suspend(es8311_dev_t *dev);
esp_err_t es8311_codec_set_voice_volume(es8311_dev_t *dev, int volume);
esp_err_t es8311_codec_get_voice_volume(es8311_dev_t *dev, int *volume);
esp_err_t es8311_set_voice_mute(bool enable);
esp_err_t es8311_get_voice_mute(es8311_dev_t *dev, int *mute);
esp_err_t es8311_set_mic_gain(es8311_dev_t *dev, es8311_mic_gain_t gain_db);
esp_err_t es8311_set_channel(es8311_dev_t *dev, int is_right);
void es8311_read_all(es8311_dev_t *dev);

#ifdef __cplusplus
}
#endif
