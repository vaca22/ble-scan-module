#include "low_rec.h"
#include "low_drv.h"
/*
 * 录音模块初始化
 * retval:
 * 0:success
 *-1:failed
 */
int32_t low_rec_init(void)
{
    return low_i2s_init(LOW_I2S_MODE_RECORD);
}

/*
 * 录音模块逆处理
 * retval:
 * 0:success
 *-1:failed
 */
int32_t low_rec_term(void)
{
    return low_i2s_deinit(LOW_I2S_MODE_RECORD);
}

/*
 * 启动录音
 * cfg[in]:
 *  录音参数，目前只有通道和采样率
 * retval:
 *  非NULL:录音对象
 *  NULL:failed
 */
low_rec_obj_t* low_rec_start(low_rec_cfg_t *cfg)
{
    low_rec_obj_t* rec = (low_rec_obj_t*)calloc(1, sizeof(low_rec_obj_t));
    if (rec) {
        int ret = low_i2s_start(LOW_I2S_MODE_RECORD, (uint8_t)cfg->channel, (uint16_t)cfg->rate);
        if (ret == 0) {
            rec->channel = cfg->channel;
            rec->rate    = cfg->rate;
            return rec;
        }
        free(rec);
    }
    return NULL;
}

/*
 * 停止录音
 * rec[in]:
 *  启动录音时返回的录音对象
 * retval:
 *  0:success
 * -1:failed
 */
int32_t low_rec_stop(low_rec_obj_t* rec)
{
    if (rec) {
        int ret = low_i2s_stop(LOW_I2S_MODE_RECORD);
        free(rec);
        return ret;
    }
    return 0;
}

/*
 * 读录音数据，只有录音数据够的情况下才会返回录音数据,不够的话直接返回0
 * rec[in]:
 *  启动录音时返回的录音对象
 * pcm:
 *  存放录音数据
 * sample:
 *  本次想要读的录音sample数
 * retval:
 *  0:录音数据不够
 *  sample:录音数据够
 */
int32_t low_rec_pcm_read(low_rec_obj_t* rec, int16_t *pcm, int32_t sample)
{
    if (rec) {
        int len = sample * rec->channel * sizeof(int16_t);
        int ret = low_i2s_move(LOW_I2S_MODE_RECORD, pcm, len);
        if (ret >= 0) {
            ret /=  rec->channel * sizeof(int16_t);
        }
        return ret;
    }
    return -1;
}

/*
 * 后根据需要可能要增加的录音控制,目前暂不使用
 */
int32_t low_rec_ctrl(low_rec_obj_t* rec, int32_t type, void* parm)
{
    return 0;
}
