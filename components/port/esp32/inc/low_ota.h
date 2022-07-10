#ifndef __low_ota_h__
#define __low_ota_h__

#include "stdint.h"
#include "low_port.h"

/*
 * 系统使用双备份来完成升级
 * 对于linux来说需要low层提供uboot, kernel, rootfs这三个的双备份操作接口
 * rtos暂未实现
 */

#define LOW_OTA_ID0     0
#define LOW_OTA_ID1     1

/*
 * 写入uboot
 * path:
 *  已经下载的uboot文件
 * retval:
 *  0:ok
 */
int32_t low_ota_uboot_flash(char *path, int32_t id, int32_t file_crc);

/*
 * 获取当前运行的kernel的id
 * retval:
 *  LOW_OTA_ID0/ID1
 */
int32_t low_ota_kernel_cur(void);

/*
 * 写kernel
 * path:
 *  已经下载的kernel文件
 * id:
 *  写到这个id的kernel位置中
 * retval:
 *  0:ok
 */
int32_t low_ota_kernel_flash(char *path, int32_t id, int32_t file_crc);

/*
 * 触发使用id这一份kernel
 * id:
 *  想要生效的kernel的id
 * retval:
 *  0:ok
 */
int32_t low_ota_kernel_trig(int32_t id);

int32_t low_ota_rootfs_cur(void);

int32_t low_ota_rootfs_flash(char *path, int32_t id, int32_t file_crc);

int32_t low_ota_rootfs_trig(int32_t id);

#endif

