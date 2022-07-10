
#ifndef __low_rec_h__
#define __low_rec_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * 必须要定义的结构体，应用启动录音时会使用这个结构体传递录音参数
 * channel和rate是必须的
 */
typedef struct low_rec_cfg_struct
{
    int32_t		channel;    //录音声道数,目前只会是1
    int32_t		rate;       //录音的采样率
}low_rec_cfg_t;

/*
 * 必须实现的录音对象, 具体实现由平台决定
 */
typedef struct low_rec_obj_struct
{
    int32_t channel;
    int32_t rate;
}low_rec_obj_t;


/*
 * 录音模块初始化
 * retval:
 * 0:success
 *-1:failed
 */
int32_t low_rec_init(void);

/*
 * 录音模块逆处理
 * retval:
 * 0:success
 *-1:failed
 */
int32_t low_rec_term(void);

/*
 * 启动录音
 * cfg[in]:
 *  录音参数，目前只有通道和采样率
 * retval:
 *  非NULL:录音对象
 *  NULL:failed
 */
low_rec_obj_t* low_rec_start(low_rec_cfg_t *cfg);

/*
 * 停止录音
 * rec[in]:
 *  启动录音时返回的录音对象
 * retval:
 *  0:success
 * -1:failed
 */
int32_t low_rec_stop(low_rec_obj_t* rec);

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
int32_t low_rec_pcm_read(low_rec_obj_t* rec, int16_t *pcm, int32_t sample);

/*
 * 后根据需要可能要增加的录音控制,目前暂不使用
 */
int32_t low_rec_ctrl(low_rec_obj_t* rec, int32_t type, void* parm);

#endif

