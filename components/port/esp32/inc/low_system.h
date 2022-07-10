
#ifndef __low_system_h__
#define __low_system_h__

#include "string.h"
#include "stdint.h"

#define SYS_LED_1	0
#define SYS_LED_2	1
#define SYS_LED_3	2
#define SYS_LED_4	3

/*
 * 完成系统时钟，ram等初始化
*/
int32_t low_system_init(void);

/*
 * 应用层调用pwr_down()之前会调用该函数
 */
void low_system_term(void);

/*
 * 初始化led
 */
void low_system_led_init(void);

/*
 * 点亮一个led
 */
void low_system_led_on(int32_t led);

/*
 * 关闭一个led
 */
void low_system_led_off(int32_t led);

/*
 * 设置led打开时的亮度等级,只有支持pwm控制的led才有效
 * 设置完等级之后在on的情况下生效
 * level:
 *  0-100
 */
void low_system_led_set_level(int32_t led, int32_t level);

void low_system_key_init(void);

/*
 * 采样按键输入值,这个函数直接采集此刻按键状态就行了，不需要做消抖处理
 * retval:
 *  一个bit对应一个按键，bit[x]==1,对应按键按下了，bit[x]==0,对应按键是放开的
 */
uint32_t low_system_key_load(void);

/*
 * 如果平台有能力提供电量信息的话就返回电量信息
 * 如果平台没有能力提供电量信息,就返回此刻的电压值,单位为mv
 * retval:
 *  -1: 获取电量失败
 *  >0: 返回电量百分比
 */
uint32_t low_system_battery_capacity(void);

#define BAT_DISCHARGING     0
#define BAT_CHARGING        1
#define BAT_FULL            2
#define BAT_NOPRESENT       3
/*
 * 返回电池的充电状态
 */
int32_t low_system_battery_status(void);

/*
 * 返回电池的温度
 * 非必须，根据硬件情况看是否实现
 * -1:出错
 */
int32_t low_system_battery_temperature(void);

typedef void(*low_timer_cb)(int32_t ms);

/*
 * 定动定时器，应用层只需要1个定时器,精度要求为1ms
 * dur_ms:
 *  定时周期
 * cb:
 *  定时冋调回调函数
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_system_timer_start(int32_t dur_ms, low_timer_cb cb);

/*
 * 关闭启动的定时器
 */
void low_system_timer_stop(void);

#define SYS_PWRSW_OFF  0
#define SYS_PWRSW_ON   1
/*
 * 如果系统开是拔动开关的情况下实现这个函数
 * retval:
 *  SYS_PWRSW_OFF
 *  SYS_PWRSW_ON
 */
int32_t low_system_pwr_switch_chk(void);

#define SYS_PWRBTN_REL  0
#define SYS_PWRBTN_PUS  1
/*
 * 如果系统开关是按钮的话,实现这个函数
 * retval:
 *  SYS_PWRBTN_REL
 *  SYS_PWRBTN_PUS
 */
int32_t low_system_pwr_button_chk(void);

#define SYS_WKUP_SRC_KEY    0
#define SYS_WKUP_SRC_RTC    1
/*
 * 进入休眠状态,退出的时候系统的ram不会丢失
 * sec:
 *  休眠持续的时间，单位为秒
 * retval:
 *  休眠唤醒的触发源
 *  SYS_WKUP_SRC_KEY/SYS_WKUP_SRC_RTC
 */
int32_t low_system_pwr_sleep(int32_t sec);

/*
 * 系统关机,进入最低功耗状态，恢复的时候可以是重启或者上电
 */
void low_system_pwr_down(void);

/*
 * 系统重启
 */
void low_system_reboot(void);

/*
 * 获取一个随机数
 */
uint32_t low_system_random_number(void);

/*
 * 获取sn号
 * 根据特定产品是否有sn号决定是否要实现
 */
char *low_system_sn_get(void);

/*
 * 获取系统的时间戳
 */
uint32_t low_system_timestamp_get(void);

/*
 * 设置系统时间戳
 */
int32_t low_system_timestamp_set(uint32_t stamp);

#endif

