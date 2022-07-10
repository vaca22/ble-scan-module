#include "low_play.h"
#include "low_drv.h"

int32_t low_play_init(void)
{
    return low_i2s_init(LOW_I2S_MODE_PLAY);
}

int32_t low_play_term(void)
{
    return low_i2s_deinit(LOW_I2S_MODE_PLAY);
}

low_play_t *low_play_start(low_play_cfg_t *cfg)
{
    low_play_t* player = calloc(1, sizeof(low_play_t));
    if (player) {
        int ret = low_i2s_start(LOW_I2S_MODE_PLAY, (uint8_t)cfg->channel, (uint16_t)cfg->rate);
        if (ret == 0) {
            player->channel = cfg->channel;
            player->rate    = cfg->rate;
            return player;
        }
        free(player);
    }
    return NULL;
}

int32_t low_play_stop(low_play_t *play)
{
    if (play) {
        int ret = low_i2s_stop(LOW_I2S_MODE_PLAY);
        free(play);
        return ret;
    }
    return -1;
}

int32_t low_play_pcm_write(low_play_t *play, int16_t *pcm, int32_t sample)
{
    if (play) {
        int len = sample * sizeof(int16_t);
        int ret = low_i2s_move(LOW_I2S_MODE_PLAY, pcm, len);
        if (ret >= 0) {
            ret /=  sizeof(int16_t);
        }
        if(ret == sample)
        {
            return 0;
        }
    }
    return -1;
}

int32_t low_play_vol_set(low_play_t *play, int32_t vol)
{
    if (play) {
        return low_codec_set_vol((uint8_t)vol);
    }
    return -1;
}

int32_t low_play_pause(low_play_t *play)
{
    if (play) {
        return low_i2s_stop(LOW_I2S_MODE_PLAY);
    }
    return -1;
}

int32_t low_play_resume(low_play_t *play)
{
    if (play) {
        return low_i2s_start(LOW_I2S_MODE_PLAY, (uint8_t)play->channel, (uint16_t)play->rate);
    }
    return -1;
}

int32_t low_play_flash_over_chk(low_play_t *play)
{
    return 1;
}
