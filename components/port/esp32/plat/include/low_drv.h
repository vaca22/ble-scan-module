#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LOW_I2S_MODE_PLAY,
    LOW_I2S_MODE_RECORD
} low_i2s_mode_t;

int low_i2s_init(low_i2s_mode_t mode);

int low_i2s_start(low_i2s_mode_t mode, uint8_t channel, uint16_t sample_rate);

int low_i2s_stop(low_i2s_mode_t mode);

int low_i2s_move(low_i2s_mode_t mode, void* sample_data, int len);

int low_codec_set_vol(uint8_t vol);

int low_i2s_deinit(low_i2s_mode_t mode);


