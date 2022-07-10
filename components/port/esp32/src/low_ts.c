#include "driver/gpio.h"
#include "low_ts.h"
#include "panel.h"

int32_t low_ts_init(void) {
    panel_config_t cfg = {
        .panel_type = PANEL_TYPE_CHSC6X,
        .power_gpio = -1,
        .reset_gpio = GPIO_NUM_17,
        .int_gpio   = GPIO_NUM_37,
        .i2c_port = 0,
    };
    int ret = panel_init(&cfg);
    printf("Panel init return %d\n", ret);
    return ret;
}

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
int32_t low_ts_load(int32_t x[], int32_t y[]) {
    int cnt = panel_read_coords(x, y);
    if (cnt > 0) {
        printf("Get coords %d x %d y %d\n", cnt, x[0], y[0]);
    }
    return cnt;
}

/*
 * 配置触摸方向旋转，该方案不需要实现
 * rotate[in]:
 *  low_ts_rotate枚举类型
 * retval:
 *  0: success
 *  -1: fail
 */
int32_t low_ts_set_rotate(int32_t rotate) {
    return 0;
}

/*
 * 关闭触摸
 */
void low_ts_off(void) {
    int ret = panel_power_on(false);
    printf("power off return %d\n", ret);
}

/*
 * 打开触摸
 */
void low_ts_on(void) {
    int ret = panel_power_on(true);
    printf("power on return %d\n", ret);
}

/*
 * 控制忽略检测到的触摸座标,暂时不实现
 * pass:
 *  1:触摸将被忽略掉
 *  0:触摸不会被忽略掉
 * delay:
 *  延时执行的时间,单位为ms
 */
void low_ts_bypass_set(int32_t pass, uint32_t delay) {
    
}
