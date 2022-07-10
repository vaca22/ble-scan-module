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

#include <string.h>
#include "es8311.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"

#include "es8311_reg.h"

/*!< MCLK_DIV_FRE is the frequency division coefficient of LRCLK */
#define MCLK_DIV_FRE \
    128  // 32u // this is valid for 16bit data resolution and MCLK from SCLK
         // pin

#define CHECK(x)                           \
    do {                                   \
        esp_err_t __;                      \
        if ((__ = x) != ESP_OK) return __; \
    } while (0)
#define CHECK_ARG(VAL)                          \
    do {                                        \
        if (!(VAL)) return ESP_ERR_INVALID_ARG; \
    } while (0)

/*
 * Clock coefficient structer
 */
struct _coeff_div {
    uint32_t mclk;     /* mclk frequency */
    uint32_t rate;     /* sample rate */
    uint8_t pre_div;   /* the pre divider with range from 1 to 8 */
    uint8_t pre_multi; /* the pre multiplier with 0: 1x, 1: 2x, 2: 4x, 3: 8x
                          selection */
    uint8_t adc_div;   /* adcclk divider */
    uint8_t dac_div;   /* dacclk divider */
    uint8_t fs_mode;   /* double speed or single speed, =0, ss, =1, ds */
    uint8_t lrck_h;    /* adclrck divider and daclrck divider */
    uint8_t lrck_l;
    uint8_t bclk_div; /* sclk divider */
    uint8_t adc_osr;  /* adc osr */
    uint8_t dac_osr;  /* dac osr */
};

/* codec hifi mclk clock divider coefficients */
static const struct _coeff_div coeff_div[] = {
    // mclk     rate   pre_div  mult  adc_div dac_div fs_mode lrch  lrcl  bckdiv
    // osr
    /* 8k */
    {12288000, 8000, 0x06, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {18432000, 8000, 0x03, 0x02, 0x03, 0x03, 0x00, 0x05, 0xff, 0x18, 0x10,
     0x10},
    {16384000, 8000, 0x08, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {8192000, 8000, 0x04, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {6144000, 8000, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {4096000, 8000, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {3072000, 8000, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {2048000, 8000, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {1536000, 8000, 0x03, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},
    {1024000, 8000, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10, 0x10},

    /* 11.025k */
    {11289600, 11025, 0x04, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {5644800, 11025, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {2822400, 11025, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {1411200, 11025, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},

    /* 12k */
    {12288000, 12000, 0x04, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {6144000, 12000, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {3072000, 12000, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {1536000, 12000, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},

    /* 16k */
    {12288000, 16000, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {18432000, 16000, 0x03, 0x02, 0x03, 0x03, 0x00, 0x02, 0xff, 0x0c, 0x10,
     0x10},
    {16384000, 16000, 0x04, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {8192000, 16000, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {6144000, 16000, 0x03, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {4096000, 16000, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {3072000, 16000, 0x03, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {2048000, 16000, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {1536000, 16000, 0x03, 0x08, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {1024000, 16000, 0x01, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},

    /* 22.05k */
    {11289600, 22050, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {5644800, 22050, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {2822400, 22050, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {1411200, 22050, 0x01, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},

    /* 24k */
    {12288000, 24000, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {18432000, 24000, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {6144000, 24000, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {3072000, 24000, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {1536000, 24000, 0x01, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},

    /* 32k */
    {12288000, 32000, 0x03, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {18432000, 32000, 0x03, 0x04, 0x03, 0x03, 0x00, 0x02, 0xff, 0x0c, 0x10,
     0x10},
    {16384000, 32000, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {8192000, 32000, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {6144000, 32000, 0x03, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {4096000, 32000, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {3072000, 32000, 0x03, 0x08, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {2048000, 32000, 0x01, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {1536000, 32000, 0x03, 0x08, 0x01, 0x01, 0x01, 0x00, 0x7f, 0x02, 0x10,
     0x10},
    {1024000, 32000, 0x01, 0x08, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},

    /* 44.1k */
    {11289600, 44100, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {5644800, 44100, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {2822400, 44100, 0x01, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {1411200, 44100, 0x01, 0x08, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},

    /* 48k */
    {12288000, 48000, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {18432000, 48000, 0x03, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {6144000, 48000, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {3072000, 48000, 0x01, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {1536000, 48000, 0x01, 0x08, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},

    /* 64k */
    {12288000, 64000, 0x03, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {18432000, 64000, 0x03, 0x04, 0x03, 0x03, 0x01, 0x01, 0x7f, 0x06, 0x10,
     0x10},
    {16384000, 64000, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {8192000, 64000, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {6144000, 64000, 0x01, 0x04, 0x03, 0x03, 0x01, 0x01, 0x7f, 0x06, 0x10,
     0x10},
    {4096000, 64000, 0x01, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {3072000, 64000, 0x01, 0x08, 0x03, 0x03, 0x01, 0x01, 0x7f, 0x06, 0x10,
     0x10},
    {2048000, 64000, 0x01, 0x08, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {1536000, 64000, 0x01, 0x08, 0x01, 0x01, 0x01, 0x00, 0xbf, 0x03, 0x18,
     0x18},
    {1024000, 64000, 0x01, 0x08, 0x01, 0x01, 0x01, 0x00, 0x7f, 0x02, 0x10,
     0x10},

    /* 88.2k */
    {11289600, 88200, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {5644800, 88200, 0x01, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {2822400, 88200, 0x01, 0x08, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {1411200, 88200, 0x01, 0x08, 0x01, 0x01, 0x01, 0x00, 0x7f, 0x02, 0x10,
     0x10},

    /* 96k */
    {12288000, 96000, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {18432000, 96000, 0x03, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {6144000, 96000, 0x01, 0x04, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {3072000, 96000, 0x01, 0x08, 0x01, 0x01, 0x00, 0x00, 0xff, 0x04, 0x10,
     0x10},
    {1536000, 96000, 0x01, 0x08, 0x01, 0x01, 0x01, 0x00, 0x7f, 0x02, 0x10,
     0x10},
};

static char *TAG = "ES8311";
static int get_coeff(uint32_t mclk, uint32_t rate);

static int es8311_write_reg(es8311_dev_t *dev, uint8_t reg_addr, uint8_t data) {
    ESP_LOGV(TAG, "%s:%d %s", __FILE__, __LINE__, "es8311_write_reg");
    CHECK_ARG(dev);

    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
    if (i2c_dev_write(&dev->i2c_dev, &reg_addr, 1, &data, 1) != ESP_OK) {
        ESP_LOGE(TAG, "es8311 write reg error");
        return ESP_FAIL;
    }
    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    return ESP_OK;
}

int es8311_read_reg(es8311_dev_t *dev, uint8_t reg_addr) {
    ESP_LOGV(TAG, "%s:%d %s", __FILE__, __LINE__, "es8311_read_reg");
    CHECK_ARG(dev);

    uint8_t data = 0;

    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
    if (i2c_dev_read(&dev->i2c_dev, &reg_addr, 1, &data, 1) != ESP_OK) {
        ESP_LOGE(TAG, "es8311 read reg error");
    }
    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);

    return data;
}

esp_err_t es8311_init_desc(es8311_dev_t *dev, uint8_t addr, i2c_port_t port,
                           gpio_num_t sda_gpio, gpio_num_t scl_gpio) {
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "es8311_init_desc");
    CHECK_ARG(dev);

    dev->i2c_dev.port                 = port;
    dev->i2c_dev.addr                 = addr;
    dev->i2c_dev.cfg.sda_io_num       = sda_gpio;
    dev->i2c_dev.cfg.scl_io_num       = scl_gpio;
    dev->i2c_dev.cfg.master.clk_speed = 100000;

    return i2c_dev_create_mutex(&dev->i2c_dev);
}

esp_err_t ex8311_free_desc(es8311_dev_t *dev) {
    ESP_LOGI(TAG, "%s:%d %s", __FILE__, __LINE__, "ex8311_free_desc");
    CHECK_ARG(dev);

    return i2c_dev_delete_mutex(&dev->i2c_dev);
}

esp_err_t es8311_codec_init(es8311_dev_t *dev, uint32_t samples) {
    uint8_t datmp, regv;
    int coeff;
    esp_err_t ret = ESP_OK;

    // ret |= bsp_i2c_add_device(&i2c_handle, ES8311_ADDR);

    ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG01, 0x30);
    ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG02, 0x00);
    ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG03, 0x10);
    ret |= es8311_write_reg(dev, ES8311_ADC_REG16, 0x24);
    ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG04, 0x10);
    ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG05, 0x00);
    ret |= es8311_write_reg(dev, ES8311_SYSTEM_REG0B, 0x00);
    ret |= es8311_write_reg(dev, ES8311_SYSTEM_REG0C, 0x00);
    ret |= es8311_write_reg(dev, ES8311_SYSTEM_REG10, 0x1F);
    ret |= es8311_write_reg(dev, ES8311_SYSTEM_REG11, 0x7F);
    ret |= es8311_write_reg(dev, ES8311_RESET_REG00, 0x80);
    /*
     * Set Codec into Master or Slave mode
     */
    regv = es8311_read_reg(dev, ES8311_RESET_REG00);

    ESP_LOGI(TAG, "ES8311 in Slave mode");
    regv &= 0xBF;

    ret |= es8311_write_reg(dev, ES8311_RESET_REG00, regv);
    ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG01, 0x3F);
    /*
     * Select clock source for internal mclk
     */

    int es8311_mclk_src = ES8311_MCLK_SOURCE;
    switch (es8311_mclk_src) {
        case FROM_MCLK_PIN:
            regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG01);
            regv &= 0x7F;
            ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG01, regv);
            break;
        case FROM_SCLK_PIN:
            regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG01);
            regv |= 0x80;
            ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG01, regv);
            break;
        default:
            regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG01);
            regv &= 0x7F;
            ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG01, regv);
            break;
    }
    int sample_fre = 0;
    int mclk_fre   = 0;

    sample_fre = samples;
    mclk_fre   = sample_fre * MCLK_DIV_FRE;
    coeff      = get_coeff(mclk_fre, sample_fre);
    if (coeff < 0) {
        ESP_LOGE(TAG, "Unable to configure sample rate %dHz with %dHz MCLK",
                 sample_fre, mclk_fre);
        return ESP_FAIL;
    }
    /*
     * Set clock parammeters
     */
    if (coeff >= 0) {
        regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG02) & 0x07;
        regv |= (coeff_div[coeff].pre_div - 1) << 5;
        datmp = 0;
        switch (coeff_div[coeff].pre_multi) {
            case 1:
                datmp = 0;
                break;
            case 2:
                datmp = 1;
                break;
            case 4:
                datmp = 2;
                break;
            case 8:
                datmp = 3;
                break;
            default:
                break;
        }

        if (ES8311_MCLK_SOURCE == FROM_SCLK_PIN) {
            datmp = 3; /* DIG_MCLK = LRCK * 256 = BCLK * 8 */
        }
        regv |= (datmp) << 3;
        ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG02, regv);

        regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG05) & 0x00;
        regv |= (coeff_div[coeff].adc_div - 1) << 4;
        regv |= (coeff_div[coeff].dac_div - 1) << 0;
        ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG05, regv);

        regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG03) & 0x80;
        regv |= coeff_div[coeff].fs_mode << 6;
        regv |= coeff_div[coeff].adc_osr << 0;
        ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG03, regv);

        regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG04) & 0x80;
        regv |= coeff_div[coeff].dac_osr << 0;
        ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG04, regv);

        regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG07) & 0xC0;
        regv |= coeff_div[coeff].lrck_h << 0;
        ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG07, regv);

        regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG08) & 0x00;
        regv |= coeff_div[coeff].lrck_l << 0;
        ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG08, regv);

        regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG06) & 0xE0;
        if (coeff_div[coeff].bclk_div < 19) {
            regv |= (coeff_div[coeff].bclk_div - 1) << 0;
        } else {
            regv |= (coeff_div[coeff].bclk_div) << 0;
        }
        ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG06, regv);
    }

    /*
     * mclk inverted or not
     */
    if (INVERT_MCLK) {
        regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG01);
        regv |= 0x40;
        ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG01, regv);
    } else {
        regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG01);
        regv &= ~(0x40);
        ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG01, regv);
    }
    /*
     * sclk inverted or not
     */
    if (INVERT_SCLK) {
        regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG06);
        regv |= 0x20;
        ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG06, regv);
    } else {
        regv = es8311_read_reg(dev, ES8311_CLK_MANAGER_REG06);
        regv &= ~(0x20);
        ret |= es8311_write_reg(dev, ES8311_CLK_MANAGER_REG06, regv);
    }

    ret |= es8311_write_reg(dev, ES8311_SYSTEM_REG13, 0x10);
    ret |= es8311_write_reg(dev, ES8311_ADC_REG1B, 0x0A);
    ret |= es8311_write_reg(dev, ES8311_ADC_REG1C, 0x6A);

    return ESP_OK;
}

esp_err_t es8311_set_bits_per_sample(es8311_dev_t *dev,
                                     audio_hal_iface_bits_t bits) {
    esp_err_t ret     = ESP_OK;
    uint8_t adc_iface = 0, dac_iface = 0;
    dac_iface = es8311_read_reg(dev, ES8311_SDPIN_REG09);
    adc_iface = es8311_read_reg(dev, ES8311_SDPOUT_REG0A);
    switch (bits) {
        case AUDIO_HAL_BIT_LENGTH_16BITS:
            dac_iface |= 0x0c;
            adc_iface |= 0x0c;
            break;
        case AUDIO_HAL_BIT_LENGTH_24BITS:
            break;
        case AUDIO_HAL_BIT_LENGTH_32BITS:
            dac_iface |= 0x10;
            adc_iface |= 0x10;
            break;
        default:
            dac_iface |= 0x0c;
            adc_iface |= 0x0c;
            break;
    }
    ret |= es8311_write_reg(dev, ES8311_SDPIN_REG09, dac_iface);
    ret |= es8311_write_reg(dev, ES8311_SDPOUT_REG0A, adc_iface);

    return ret;
}

esp_err_t es8311_config_fmt(es8311_dev_t *dev, es_i2s_fmt_t fmt) {
    esp_err_t ret     = ESP_OK;
    uint8_t adc_iface = 0, dac_iface = 0;
    dac_iface = es8311_read_reg(dev, ES8311_SDPIN_REG09);
    adc_iface = es8311_read_reg(dev, ES8311_SDPOUT_REG0A);
    switch (fmt) {
        case AUDIO_HAL_I2S_NORMAL:
            ESP_LOGI(TAG, "ES8311 in I2S Format");
            dac_iface &= 0xFC;
            adc_iface &= 0xFC;
            break;
        case AUDIO_HAL_I2S_LEFT:
        case AUDIO_HAL_I2S_RIGHT:
            ESP_LOGI(TAG, "ES8311 in LJ Format");
            adc_iface &= 0xFC;
            dac_iface &= 0xFC;
            adc_iface |= 0x01;
            dac_iface |= 0x01;
            break;
        case AUDIO_HAL_I2S_DSP:
            ESP_LOGI(TAG, "ES8311 in DSP-B Format");
            adc_iface &= 0xDC;
            dac_iface &= 0xDC;
            adc_iface |= 0x23;
            dac_iface |= 0x23;
            break;
        default:
            dac_iface &= 0xFC;
            adc_iface &= 0xFC;
            break;
    }
    ret |= es8311_write_reg(dev, ES8311_SDPIN_REG09, dac_iface);
    ret |= es8311_write_reg(dev, ES8311_SDPOUT_REG0A, adc_iface);

    return ret;
}

esp_err_t es8311_codec_ctrl_state(es8311_dev_t *dev,
                                  audio_hal_codec_mode_t mode,
                                  audio_hal_ctrl_t ctrl_state) {
    esp_err_t ret       = ESP_OK;
    es_module_t es_mode = ES_MODULE_MIN;

    switch (mode) {
        case AUDIO_HAL_CODEC_MODE_ENCODE:
            es_mode = ES_MODULE_ADC;
            break;
        case AUDIO_HAL_CODEC_MODE_LINE_IN:
            es_mode = ES_MODULE_LINE;
            break;
        case AUDIO_HAL_CODEC_MODE_DECODE:
            es_mode = ES_MODULE_DAC;
            break;
        case AUDIO_HAL_CODEC_MODE_BOTH:
            es_mode = ES_MODULE_ADC_DAC;
            break;
        default:
            es_mode = ES_MODULE_DAC;
            ESP_LOGW(TAG, "Codec mode not support, default is decode mode");
            break;
    }

    if (ctrl_state == AUDIO_HAL_CTRL_START) {
        ret |= es8311_start(dev, es_mode);
    } else if (ctrl_state == AUDIO_HAL_CTRL_STOP) {
        ESP_LOGW(TAG, "The codec is about to stop");
        ret |= es8311_stop(dev, es_mode);
    }

    return ret;
}

esp_err_t es8311_start(es8311_dev_t *dev, es_module_t mode) {
    esp_err_t ret     = ESP_OK;
    uint8_t adc_iface = 0, dac_iface = 0;

    dac_iface = es8311_read_reg(dev, ES8311_SDPIN_REG09) & 0xBF;
    adc_iface = es8311_read_reg(dev, ES8311_SDPOUT_REG0A) & 0xBF;
    adc_iface |= BIT(6);
    dac_iface |= BIT(6);

    if (mode == ES_MODULE_LINE) {
        ESP_LOGE(TAG, "The codec es8311 doesn't support ES_MODULE_LINE mode");
        return ESP_FAIL;
    }
    if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
        adc_iface &= ~(BIT(6));
    }
    if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
        dac_iface &= ~(BIT(6));
    }

    ret |= es8311_write_reg(dev, ES8311_SDPIN_REG09, dac_iface);
    ret |= es8311_write_reg(dev, ES8311_SDPOUT_REG0A, adc_iface);

    ret |= es8311_write_reg(dev, ES8311_ADC_REG17, 0xBF);
    ret |= es8311_write_reg(dev, ES8311_SYSTEM_REG0E, 0x02);
    ret |= es8311_write_reg(dev, ES8311_SYSTEM_REG12, 0x00);
    ret |= es8311_write_reg(dev, ES8311_SYSTEM_REG14, 0x1A);

    /*
     * pdm dmic enable or disable
     */
    int regv = 0;
    if (IS_DMIC) {
        regv = es8311_read_reg(dev, ES8311_SYSTEM_REG14);
        regv |= 0x40;
        ret |= es8311_write_reg(dev, ES8311_SYSTEM_REG14, regv);
    } else {
        regv = es8311_read_reg(dev, ES8311_SYSTEM_REG14);
        regv &= ~(0x40);
        ret |= es8311_write_reg(dev, ES8311_SYSTEM_REG14, regv);
    }

    ret |= es8311_write_reg(dev, ES8311_SYSTEM_REG0D, 0x01);
    ret |= es8311_write_reg(dev, ES8311_ADC_REG15, 0x40);
    ret |= es8311_write_reg(dev, ES8311_DAC_REG37, 0x48);
    ret |= es8311_write_reg(dev, ES8311_GP_REG45, 0x00);

    return ret;
}

esp_err_t es8311_stop(es8311_dev_t *dev, es_module_t mode) {
    esp_err_t ret = ESP_OK;
    es8311_suspend(dev);
    return ret;
}

esp_err_t es8311_suspend(es8311_dev_t *dev) {
    ESP_LOGI(TAG, "Enter into es8311_suspend()");
    es8311_write_reg(dev, ES8311_DAC_REG32, 0x00);
    es8311_write_reg(dev, ES8311_ADC_REG17, 0x00);
    es8311_write_reg(dev, ES8311_SYSTEM_REG0E, 0xFF);
    es8311_write_reg(dev, ES8311_SYSTEM_REG12, 0x02);
    es8311_write_reg(dev, ES8311_SYSTEM_REG14, 0x00);
    es8311_write_reg(dev, ES8311_SYSTEM_REG0D, 0xFA);
    es8311_write_reg(dev, ES8311_ADC_REG15, 0x00);
    es8311_write_reg(dev, ES8311_DAC_REG37, 0x08);
    es8311_write_reg(dev, ES8311_GP_REG45, 0x01);

    return ESP_OK;
}

esp_err_t es8311_codec_set_voice_volume(es8311_dev_t *dev, int volume) {
    esp_err_t res = ESP_OK;
    if (volume < 0) {
        volume = 0;
    } else if (volume > 100) {
        volume = 100;
    }
    int vol = (volume)*2550 / 1000;
    // ESP_LOGI(TAG, "SET: volume:%d", vol);
    es8311_write_reg(dev, ES8311_DAC_REG32, vol);
    return res;
}

esp_err_t es8311_codec_get_voice_volume(es8311_dev_t *dev, int *volume) {
    esp_err_t res = ESP_OK;
    int regv      = 0;
    regv          = es8311_read_reg(dev, ES8311_DAC_REG32);
    if (regv == ESP_FAIL) {
        *volume = 0;
        res     = ESP_FAIL;
    } else {
        *volume = regv * 100 / 256;
    }
    ESP_LOGI(TAG, "GET: regv:%d volume:%d", regv, *volume);
    return res;
}

esp_err_t es8311_set_voice_mute(bool enable) {
    ESP_LOGI(TAG, "Es8311SetVoiceMute volume:%d", enable);
    // es8311_mute(enable);
    return ESP_OK;
}

esp_err_t es8311_get_voice_mute(es8311_dev_t *dev, int *mute) {
    esp_err_t res = ESP_OK;
    uint8_t reg   = 0;
    res           = es8311_read_reg(dev, ES8311_DAC_REG31);
    if (res != ESP_FAIL) {
        reg = (res & 0x20) >> 5;
    }
    *mute = reg;
    return res;
}

esp_err_t es8311_set_mic_gain(es8311_dev_t *dev, es8311_mic_gain_t gain_db) {
    esp_err_t res = ESP_OK;
    res = es8311_write_reg(dev, ES8311_ADC_REG16, gain_db);  // MIC gain scale
    return res;
}

void es8311_read_all(es8311_dev_t *dev) {
    for (int i = 0; i < 0x4A; i++) {
        uint8_t reg = es8311_read_reg(dev, i);
        ets_printf("REG:%02x, %02x\n", reg, i);
    }
}

esp_err_t es8311_set_channel(es8311_dev_t *dev, int is_right) {
    uint8_t reg_val = es8311_read_reg(dev, ES8311_SDPIN_REG09);
    if (is_right) {
        reg_val |= 0b10000000;
    } else {
        reg_val &= 0b01111111;
    }
    return es8311_write_reg(dev, ES8311_SDPIN_REG09, reg_val);
}

/*
 * look for the coefficient in coeff_div[] table
 */
static int get_coeff(uint32_t mclk, uint32_t rate) {
    for (int i = 0; i < (sizeof(coeff_div) / sizeof(coeff_div[0])); i++) {
        if (coeff_div[i].rate == rate && coeff_div[i].mclk == mclk) {
            return i;
        }
    }

    return -1;
}
