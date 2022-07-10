#ifndef __low_play_h__
#define __low_play_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * 必须要定义的结构体，应用开始播音时会使用这个结构体来传递音频参数
 * channel和rate是必须的
 */
typedef struct low_play_cfg_struct
{
    int32_t channel;    //音频声道
    uint32_t rate;      //采样率
}low_play_cfg_t;

/*
 * 硬件播音时使用的结构体，根据平台自己实现
 */
typedef struct low_play_struct
{
    int32_t channel;
    int32_t rate;
}low_play_t;

/*
 * 平台播音硬件初始化
 * retval:
 * 0:success
 *-1:failed
 */
int32_t low_play_init(void);

/*
 * 平台播音硬件逆处理
 * retval:
 * 0:success
 *-1:failed
 */
int32_t low_play_term(void);

/*
 * 启动播音
 * cfg[in]:
 *  播音参数
 * retval:
 * 非NULL:指向播音对象的句柄
 * NULL:failed
 */
low_play_t *low_play_start(low_play_cfg_t *cfg);

/*
 * 停止播音
 * play[in]:
 *  要停止的播音对象
 * retval:
 *  0:success
 * -1:failed
 */
int32_t low_play_stop(low_play_t *play);

/*
 * 写pcm数据到播音对象
 * play[in]:
 *  启动播音时返回的播音对象
 * pcm[in]:
 *  pcm数据
 * sample[in]:
 *  pcm的sample数，现在pcm是int16型的
 * retval:
 *  0:success
 * -1:failed
 */
int32_t low_play_pcm_write(low_play_t *play, int16_t *pcm, int32_t sample);

/*
 * 设置播音音量
 * vol:音量等级，0-99
 * retval:
 *  0:success
 * -1:failed
 */
int32_t low_play_vol_set(low_play_t *play ,int32_t vol);

/*
 * 播音暂停
 * retval:
 *  0:success
 * -1:failed
 */
int32_t low_play_pause(low_play_t *play);

/*
 * 播音继续
 * retval:
 *  0:success
 * -1:failed
 */
int32_t low_play_resume(low_play_t *play);

/*
 * 音频数据发给low_play了，但有可能还有数据没有播掉
 * 通过这个接口来查询到底有没有全部播掉
 * retval:
 * 0:数据还没有全部播掉
 * 1:发送给low_play的数据已经被全播放掉了
 */
int32_t low_play_flash_over_chk(low_play_t *play);

#endif

