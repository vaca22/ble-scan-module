
#ifndef __low_vi_h__
#define __low_vi_h__

#include "stdint.h"

/*
 * 获取摄像头frame接口
 * 根据方案确定是否要实现
 */

#define Y_FRAME_SIZE_320X240    (320 * 240)

struct buffer {
	void *start[3];
	int length[3];
};

typedef struct vi_yframe_struct
{
    int32_t width;
    int32_t height;
    struct buffer *buffers;
//    uint16_t buf[Y_FRAME_SIZE_320X240]; //rgb565
}vi_frame_t;

/*
 * 初始化摄像头
 */
int32_t low_vi_init(void);

/*
 * 
 */
int32_t low_vi_term(void);

/*
 * 获取一帧数据
 * retval:
 *  1/2/3:读到的数据帧数
 *  0:没有读到数据
 * -1:failed
 */
int32_t low_vi_get_frame(vi_frame_t *frame);

/*
 * 释放一帧数据
 * buf:
 *  &vi_frame.buf
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_vi_free_buf(vi_frame_t *buf);

#define VI_FLIP_H       0 //水平方向,镜像翻转
#define VI_FLIP_V       1 //垂直方向
#define VI_FLIP_RESET   2 //旋转复原
/*
 * 控制摄像头数据翻转
 * flip: 
 *  VI_FLIP_H/VI_FLIP_V
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_vi_ctrl_flip(int32_t flip);

/*
 * 关闭sensor据流,摄像头进入低功耗状态
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_vi_stream_off(void);

/*
 * 重启sensor数据流
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_vi_stream_on(void);

#endif

