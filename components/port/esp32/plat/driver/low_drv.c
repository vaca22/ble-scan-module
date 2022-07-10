#include "driver/i2s.h"
#include "low_drv.h"
#include "audio_hal.h"
#include "board.h"

 typedef struct {
    uint8_t             channel;
    int16_t             sample_rate;
    int16_t             user;
    audio_hal_handle_t  codec_hal;
    bool                inited;
 } low_drv_t;
 
static low_drv_t low_drv;

#define AUDIO_CODEC_DEFAULT_CONFIG(){                   \
        .adc_input  = AUDIO_HAL_ADC_INPUT_LINE1,        \
        .dac_output = AUDIO_HAL_DAC_OUTPUT_ALL,         \
        .codec_mode = AUDIO_HAL_CODEC_MODE_BOTH,        \
        .i2s_iface = {                                  \
            .mode = AUDIO_HAL_MODE_SLAVE,               \
            .fmt = AUDIO_HAL_I2S_NORMAL,                \
            .samples = AUDIO_HAL_48K_SAMPLES,           \
            .bits = AUDIO_HAL_BIT_LENGTH_16BITS,        \
        },                                              \
};

extern audio_hal_func_t AUDIO_CODEC_ES8311_DEFAULT_HANDLE;

bool _codec_init(bool init)
{
    if (init) {
        audio_hal_codec_config_t audio_codec_cfg = AUDIO_CODEC_DEFAULT_CONFIG();
        if (low_drv.codec_hal) {
            return true;
        }
        low_drv.codec_hal = audio_hal_init(&audio_codec_cfg, &AUDIO_CODEC_ES8311_DEFAULT_HANDLE);
        if (low_drv.codec_hal) {
            audio_hal_ctrl_codec(low_drv.codec_hal, AUDIO_HAL_CODEC_MODE_BOTH, AUDIO_HAL_CTRL_START);
        }
        return (low_drv.codec_hal != NULL);
    }
    else {
        if (low_drv.codec_hal == NULL) {
            return true;
        }
        int ret = audio_hal_deinit(low_drv.codec_hal);
        low_drv.codec_hal = NULL;
        return (ret == 0);
    }
}

static int _i2s_int(void) {
    if (low_drv.inited) {
        return 0;
    }
    if (_codec_init(true) == false) {
        return -1;
    }
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX), 
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2 | ESP_INTR_FLAG_IRAM,
        .dma_buf_count = 3,
        .dma_buf_len = 300,
        .use_apll = true,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0   
    };
    esp_err_t ret = i2s_driver_install(I2S_NUM_0 + LOW_I2S_PORT, &i2s_config, 0, NULL);
    if (ret == ESP_OK) {
        low_drv.inited = true;
    }
#if SOC_I2S_SUPPORTS_ADC_DAC
    if ((config->i2s_config.mode & I2S_MODE_DAC_BUILT_IN) != 0) {
        i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
    } else
#endif
    {
        i2s_pin_config_t i2s_pin_cfg = {0};
        get_i2s_pins(I2S_NUM_0 + LOW_I2S_PORT, &i2s_pin_cfg);
        i2s_set_pin(I2S_NUM_0 + LOW_I2S_PORT, &i2s_pin_cfg);
    }
    //i2s_mclk_gpio_select(I2S_NUM_0 + LOW_I2S_PORT, GPIO_NUM_0);
    return (ret == ESP_OK) ? 0: -1;
}

int low_i2s_init(low_i2s_mode_t mode) {
    int ret = _i2s_int();
    if (ret != 0) {
        return ret;
    }
    low_drv.user++;
    return 0;
}

int low_i2s_deinit(low_i2s_mode_t mode) {
    if (low_drv.inited) {
       low_drv.user--;
       printf("Still have user %d\n", low_drv.user);
       if (low_drv.user == 0) {
            i2s_zero_dma_buffer(I2S_NUM_0 + LOW_I2S_PORT);
            esp_err_t ret = i2s_driver_uninstall(I2S_NUM_0 + LOW_I2S_PORT);
            _codec_init(false);
            low_drv.inited = false;
            return ret;
       }
    }
    return 0;
}

int low_i2s_start(low_i2s_mode_t mode, uint8_t channel, uint16_t sample_rate) {
    if (low_drv.inited == false) {
        return -1;
    }
    i2s_zero_dma_buffer(I2S_NUM_0 + LOW_I2S_PORT);
    esp_err_t ret = i2s_set_clk(I2S_NUM_0 + LOW_I2S_PORT, sample_rate, I2S_BITS_PER_SAMPLE_16BIT, channel);
    printf("Set channel %d samplerate %d\n", channel, sample_rate);
    return ret;
}

int low_i2s_stop(low_i2s_mode_t mode) {
    if (low_drv.inited == false) {
        return -1;
    }
    esp_err_t ret = i2s_stop(I2S_NUM_0 + LOW_I2S_PORT);
    return ret;
}

int low_i2s_move(low_i2s_mode_t mode, void* sample_data, int len) {
    if (low_drv.inited == false) {
        return -1;
    }
    if (mode == LOW_I2S_MODE_PLAY) {
        size_t written = 0;
        esp_err_t ret = i2s_write(I2S_NUM_0 + LOW_I2S_PORT, sample_data, len, &written, portMAX_DELAY);
        if (ret == ESP_OK) {
            return written;
        }
        return ret;
    }
    else {
        size_t readed = 0;
        esp_err_t ret = i2s_read(I2S_NUM_0 + LOW_I2S_PORT, sample_data, len, &readed, portMAX_DELAY);
        if (ret == ESP_OK) {
            return readed;
        }
        return ret;
    }
}

int low_codec_set_vol(uint8_t vol) {
    if (low_drv.inited == false) {
        return -1;
    }
    return audio_hal_set_volume(low_drv.codec_hal, vol);
}
