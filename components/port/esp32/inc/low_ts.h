
#ifndef __low_ts_h__
#define __low_ts_h__

#include <stdint.h>

/*
 * 屏幕旋转方向枚举,目前不用实现
 */
enum
{
    LOW_TS_ROTATE_0 = 0,
    LOW_TS_ROTATE_90,         //0独旋转
    LOW_TS_ROTATE_180,
    LOW_TS_ROTATE_270,
}low_ts_rotate;

/*
 * 初始化触摸模块
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_ts_init(void);

/*
 * 读取当前触摸按键的状态,返回触摸的点数,目前暂不需要多点触摸，返回单点即可
 *  x[]:存放触摸的x坐标
 *  y[]:存放触摸的y坐村
 * retval:
 *  返回触摸的点数
 *  0:没有触摸
 *  1:有1个触摸 
 *  2:有2个触摸
 */
int32_t low_ts_load(int32_t x[], int32_t y[]);

/*
 * 配置触摸方向旋转，该方案不需要实现
 * rotate[in]:
 *  low_ts_rotate枚举类型
 * retval:
 *  0: success
 *  -1: fail
 */
int32_t low_ts_set_rotate(int32_t rotate);

/*
 * 关闭触摸
 */
void low_ts_off(void);

/*
 * 打开触摸
 */
void low_ts_on(void);

/*
 * 控制忽略检测到的触摸座标,暂时不实现
 * pass:
 *  1:触摸将被忽略掉
 *  0:触摸不会被忽略掉
 * delay:
 *  延时执行的时间,单位为ms
 */
void low_ts_bypass_set(int32_t pass, uint32_t delay);

#endif

