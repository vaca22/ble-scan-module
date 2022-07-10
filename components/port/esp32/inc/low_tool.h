
#ifndef __low_tool_h__
#define __low_tool_h__

#include "stdint.h"

/*
 * 调用系统工具
 * 目前linux平台需要
 * rtos暂不实现
 */

/*
 * 执行系统命令
 */
int32_t low_tool_sys_cmd(char *cmd);

typedef struct zip_info_struct
{
    int32_t files;
    int64_t size;
} zip_info_t;

/*
 * 设置zip工具的路径
 */
int32_t low_tool_zip_path_set(char *path);

/*
 * 获取一个zip文件信息
 * zip:
 *  zip文件的绝对路径
 * info:
 *  信息写到这里面来
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_tool_zip_info_get(char *zip, zip_info_t *info);

/*
 * 解压zip文件过程的回调函数，每解一个文件都会调用一次
 * index:
 *  当前解压的文件序号
 * name:
 *  当前解压的文件的文件名
 * user:
 *  unzip_do()时传递的用户参数
 */
typedef void(*unzip_cb)(int32_t index, char *name, void *user);

/*
 * zip:
 *  压缩包的路径，必须是绝对路径
 * dst:
 *  解压到这个目录，必须是绝对路径
 * key:
 *  如果有密码的话，用来传递密码
 * cb:
 *  可以为空，每解压一个文件会调用一次这个回调
 */
int32_t low_tool_unzip_do(char *zip, char *dst, char *key, unzip_cb cb, void *user);

#endif

