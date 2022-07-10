
#ifndef __low_sdio_h__
#define __low_sdio_h__

#include "stdint.h"

#define SD_NOT_PRESET       1
#define SD_PRESENT          0

//和fatfs中的diskio.h中的命令要保持一致
#define SD_DISK_CMD_CTRL_SYNC           0
#define SD_DISK_CMD_GET_SECTOR_COUNT    1
#define SD_DISK_CMD_GET_SECTOR_SIZE     2
#define SD_DISK_CMD_GET_BLOCK_SIZE      3
#define SD_DISK_CMD_CTRL_TRIM           4

/*
 * 特定平台需要的,直接返回0即可
 * retval:
 *   0
 */
int low_sd_status(void);

/*
 * 初始化Sd卡,调用该函数之后就可以使用read/write功能
 * retval:
 *  0:success
 * -1:没有卡或者任何其他错误
 */
int low_sd_init(void);

/*
 * 读卡
 * buff: 数据读到这里
 * sector: 读数据的sector偏移
 * count: 要读的sector数
 * retval:
 *  0:success
 * -1:failed
 */
int low_sd_read_sectors(unsigned char* buff, unsigned long sector, unsigned int count);

/*
 * 写卡
 * buff:
 * sector:
 * count:
 *  0:success
 * -1:failed
 */
int low_sd_write_sectors(void* buff, unsigned long sector, unsigned int count);

/*
 * 根据平台特定来确定是否要实现,可直接返回0
 */
int low_sd_ioctl(unsigned char cmd, void* buff);

/*
 * 系统休眠的时候会调用该函数,在这个函数里完成初始化卡时申请的资源
 */
int low_sd_disk_term(void);

/*
 * 初始化sd卡的电源控制引脚
 */
void low_sd_power_pin_init(void);

#define LOW_SDCARD_POWER_ON     1
#define LOW_SDCARD_POWER_OFF    0
/*
 * 控制sd卡的电源开关,由硬件环境决定是否要实现这个函数
 * on:
 * 1:sd卡供电
 * 0:sd卡断电
 */
void low_sd_power_ctrl(int32_t on);

/*
 * sd卡检测引脚初始化
 */
void low_sd_detect_pin_init(void);

#define LOW_SDCARD_INSERTED     1
#define LOW_SDCARD_REMOVED      0
/*
 * 检测当前有没有卡
 * retval:
 *  0:无卡
 *  1:有卡
 */
int32_t low_sd_is_inserted(void);

#endif


