/**
 * @file audio.c
 * audio file
 */

/*********************
 *      INCLUDES
 *********************/
#include "audio.h"

/*********************
 *      DEFINES
 *********************/
#define TAG           "Audio"
#define RECV_BUF_SIZE 1024 * 2
#define SAMPLE_RATE   16000
#define MCLK_MULTIPLE I2S_MCLK_MULTIPLE_128
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static esp_err_t audio_i2s_init();
/**********************
 *  STATIC VARIABLES
 **********************/
static es8311_dev_t audio_dev;
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
esp_err_t audio_es8311_init(void) {
    audio_i2s_init();

    es8311_init_desc(&audio_dev, AUDIO_ES8311_ADDR, AUDIO_ES8311_I2C,
                     AUDIO_ES8311_SDA, AUDIO_ES8311_SCL);

    es8311_codec_init(&audio_dev, SAMPLE_RATE);
    es8311_config_fmt(&audio_dev, AUDIO_HAL_I2S_NORMAL);
    es8311_set_bits_per_sample(&audio_dev, AUDIO_HAL_BIT_LENGTH_16BITS);
    es8311_codec_set_voice_volume(&audio_dev, 75);
    es8311_codec_ctrl_state(&audio_dev, AUDIO_HAL_CODEC_MODE_BOTH,
                            AUDIO_HAL_CTRL_START);

    return ESP_OK;
}

// TODO:
esp_err_t audio_es8311_play() {
    return ESP_OK;
}

// TODO:
esp_err_t audio_es8311_play_from_file() {
    return ESP_OK;
}

// TODO:
esp_err_t audio_es8311_record() {
    return ESP_OK;
}

// TODO:
esp_err_t audio_es8311_record_to_file() {
    return ESP_OK;
}

// TODO:
esp_err_t audio_es8311_deinit(void) {
    return ESP_OK;
}

/**********************************************/
esp_err_t audio_cs5230e_init(void) {
    gpio_config_t io_conf = {0};
    io_conf.intr_type     = GPIO_INTR_DISABLE;
    io_conf.mode          = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask  = (1ULL << AUDIO_CS5230E_PWR_PIN);
    io_conf.pull_down_en  = 0;
    io_conf.pull_up_en    = 0;
    return gpio_config(&io_conf);
}

esp_err_t audio_cs5230e_enable(void) {
    return gpio_set_level(AUDIO_CS5230E_PWR_PIN, 1);
}
esp_err_t audio_cs5230e_disable(void) {
    return gpio_set_level(AUDIO_CS5230E_PWR_PIN, 0);
}

void audio_echo_test(void *args) {
    int *mic_data = malloc(RECV_BUF_SIZE);
    if (!mic_data) {
        ESP_LOGE(TAG, "[echo] No memory for read data buffer");
        abort();
    }

    esp_err_t ret           = ESP_OK;
    uint8_t flag            = 0;
    size_t bytes_read       = 0;
    size_t bytes_write      = 0;
    FILE *file              = NULL;
    uint32_t total_size     = 0;
    size_t file_bytes_read  = 0;
    size_t file_bytes_write = 0;
    ESP_LOGI(TAG, "[echo] Echo start");

    while (1) {
        if (flag == 0) {
            ESP_LOGI(TAG, "start to recording");
            file = fopen("/sdcard/test.pcm", "w");
            if (file == NULL) {
                ESP_LOGE(TAG, "open test.pcm faild");
                flag = 0;
            } else {
                audio_cs5230e_disable();
                es8311_set_mic_gain(&audio_dev, ES8311_MIC_GAIN_12DB);
                flag = 1;
            }
        }

        if (flag == 1) {
            memset(mic_data, 0, RECV_BUF_SIZE);
            ret = i2s_read(AUDIO_I2S_NUM, mic_data, RECV_BUF_SIZE, &bytes_read,
                           100);

            if (ret != ESP_OK) {
                ESP_LOGE(TAG, "i2s read error");
                abort();
            }

            file_bytes_write = fwrite((uint8_t *)mic_data, 1, bytes_read, file);
            total_size += file_bytes_write;

            // ESP_LOGI(TAG, "当前写入大小:%d 已写入:%d.%dk", file_bytes_write,
            //          total_size / 1024, total_size % 1024);

            if (total_size > 1 * 1024 * 128) {
                flag = 2;
                ESP_LOGI(TAG, "结束 已写入:%d.%dk", total_size / 1024,
                         total_size % 1024);
                total_size       = 0;
                file_bytes_write = 0;
                fclose(file);
                i2s_zero_dma_buffer(AUDIO_I2S_NUM);
                audio_cs5230e_disable();
            }
        }

        if (flag == 2) {
            ESP_LOGI(TAG, "start play recording");
            file = fopen("/sdcard/test.pcm", "r");
            if (file == NULL) {
                ESP_LOGE(TAG, "open test.pcm faild");
                flag = 0;
            } else {
                audio_cs5230e_enable();
                es8311_codec_set_voice_volume(&audio_dev, 85);
                flag = 3;
            }
        }

        if (flag == 3) {
            memset(mic_data, 0, RECV_BUF_SIZE);

            file_bytes_read =
                fread((uint8_t *)mic_data, 1, RECV_BUF_SIZE, file);

            ret = i2s_write(AUDIO_I2S_NUM, mic_data, RECV_BUF_SIZE,
                            &bytes_write, 100);
            if (ret != ESP_OK) {
                ESP_LOGE(TAG, "i2s write error");
                abort();
            }
            total_size += file_bytes_read;
            // ESP_LOGI(TAG, "当前读取大小:%d 已读取:%d.%dk", file_bytes_read,
            //          total_size / 1024, total_size % 1024);

            if (file_bytes_read < RECV_BUF_SIZE) {
                flag = 0;
                ESP_LOGI(TAG, "结束 已读取:%d.%dk", total_size / 1024,
                         total_size % 1024);
                total_size      = 0;
                file_bytes_read = 0;
                fclose(file);
                i2s_zero_dma_buffer(AUDIO_I2S_NUM);
                audio_cs5230e_disable();
            }
        }
    }
    vTaskDelete(NULL);
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
static esp_err_t audio_i2s_init() {
    i2s_config_t i2s_cfg = {
        .mode                 = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX,
        .sample_rate          = SAMPLE_RATE,
        .bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format       = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .tx_desc_auto_clear   = true,
        .dma_buf_count        = 8,
        .dma_buf_len          = 64,
        .use_apll             = false,
        .mclk_multiple        = MCLK_MULTIPLE,
        .intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
    };

    i2s_driver_install(AUDIO_I2S_NUM, &i2s_cfg, 0, NULL);

    i2s_pin_config_t i2s_pin_cfg = {.mck_io_num   = AUDIO_I2S_MCK_PIN,
                                    .bck_io_num   = AUDIO_I2S_BCK_PIN,
                                    .ws_io_num    = AUDIO_I2S_WS_PIN,
                                    .data_out_num = AUDIO_I2S_DO_PIN,
                                    .data_in_num  = AUDIO_I2S_DI_PIN};
    return i2s_set_pin(AUDIO_I2S_NUM, &i2s_pin_cfg);
}
