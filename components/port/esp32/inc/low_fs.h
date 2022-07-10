
#ifndef __low_fs_h__
#define __low_fs_h__

#include <stdint.h>
#include "ff.h"

/*
 * 平台如果有能力提供文件系统的话(比如linux)，把文件系统接口在这里做封闭
 * rtos平台暂时未支持
 */

/*
 * 由平台决定是否需要这个函数
 */
void low_fs_init(void);

/*
 * A
 * 检查一个路径是否存在,可以是文件夹也可以是文件
 * retval:
 *  0:不存在
 *  1:存在
 */
int32_t low_fs_path_exist(char *path);

/*
 * A
 * 删除一个路径,可以是文件夹也可以是文件
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_fs_path_delete(char *path);

/*
 * A
 * 创建一个目录,如果目录已经存了就什么也不做
 * retval:
 *  0:失败
 *  1:成功
 */
int32_t low_fs_folder_create(char *path);

#define FS_O_READ    FA_READ
#define FS_O_WRITE   FA_WRITE
#define FS_O_RDWR    FA_READ | FA_WRITE
#define FS_O_CREAT   FA_OPEN_ALWAYS  //如果文件不存在，会创建这个文件
#define FS_O_APPEND  FA_OPEN_APPEND  //如果是以写方式打开，会把offset seek到结尾
#define FS_O_TRUNC   FA_CREATE_ALWAYS //如果文件存在，并且以写的方式打开，则原来的文件数据会被清除

/*
 * A
 * 打开一个文件
 * path: 文件路径
 * flag:
 *  FS_FLAG_XXX | FS_FLAG_XXX
 * retval:
 *  -1:failed
 *  >=0: 文件的id
 */
int32_t low_fs_open(char *path, int32_t flag);

/*
 * A
 * 关闭一个之前打开的文件
 * id:打开的文件id
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_fs_close(int32_t fd);

/*
 * A
 * 读文件
 * fd:
 * buf:
 * size:
 * retval:
 *  -1:faied
 *  >=0:实际读到的数据量，如果小于size说明文件读完了
 */
int64_t low_fs_read(int32_t fd, void *buf, uint64_t size);

/*
 * A
 * 写文件
 * fd:
 * buf:
 * size:
 * retval:
 *  -1:failed
 *  size:实际写入的数据量
 */
int64_t low_fs_write(int32_t fd, void *buf, uint64_t size);

#ifndef  SEEK_SET
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2
#endif
#define FS_SEEK_SET SEEK_SET
#define FS_SEEK_CUR SEEK_CUR
#define FS_SEEK_END SEEK_END

/*
 * A
 * retval:
 *  seek操作之后，文件的offset位置
 */
uint64_t low_fs_seek(int32_t fd, int64_t offset, int32_t flag);

/*
 * B
 * 检查一个路径是否是目录
 * retval:
 *  0:是
 *  1:不是
 */
int32_t low_fs_path_isdir(char *path);

/*
 * B
 * 检查一个路径是否是文件
 * retval:
 *  0:是
 *  1:不是
 */
int32_t low_fs_path_isfile(char *path);

/*
 * A
 * 获取文件大小
 * retval:文件大小
 */
int64_t low_fs_file_size(char *path);

/*
 * B
 * 获取目录大小,实际大小,而不是占用磁盘大小
 * retval:文件大小
 */
int64_t low_fs_dir_size(char *path);

/*
 * B
 * 复制文件
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_fs_file_copy(char *src, char *dst);

/*
 * B
 * 移动文件
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_fs_file_move(char *src, char *dst);

/*
 * B
 * 获取文件最后修改的文件
 */
int32_t low_fs_file_time_get(char *path);

/*
 * A
 * 保存数据到文件,会清空文件原来的数据
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_fs_file_data_set(char *path, void *data, int32_t size);

/*
 * A
 * 以追加的方式, 保存数据到文件,
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_fs_file_data_append(char *path, void *data, int32_t size);

/*
 * A
 * 获取文件中的数据,需要释放指针
 * retval:
 *  0>:ok,获取到的文件数据大小
 * -1:failed
 */
int32_t low_fs_file_data_get(char *path, void *data, int32_t *size);

/*
 * A
 * 释放读取的数据指针
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_fs_file_data_get_free(void *data);

/*
 * B
 * 打开一个文件夹
 * retval:
 *  !NULL:ok
 *  NULL:failed
 */
void *low_fs_dir_open(char *path);

/*
 * B
 * 关闭一个文件夹
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_fs_dir_close(void *dir);

/*
 * B
 * 获取文件夹下的一个子名称
 * child_name:
 *  子目录或文件名称,至少需要128字节
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_fs_dir_read(void *dir, char *child_name);

/*
 * A
 * 把文件的修改同步到磁盘中
 * fd:
 *  要同步的文件id
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_fs_fsync(int32_t fd);

#define FS_ALLOCATING       FALLOC_FL_KEEP_SIZE 
#define FS_DEALLOCATING     FALLOC_FL_PUNCH_HOLE
//#define FS_COLLAPSING       FALLOC_FL_COLLAPSE_RANGE //暂还不支持

/*
 * C
 * 该函数暂可以不实现
 * fd:
 *  调用low_fs_open()打开的文件
 * mod:
 *
 * offset:
 *  要删掉或者分配空间的起始地址
 * len:
 *  要删除或者分配空间的大小
 */
int32_t low_fs_fallocate(int32_t fd, int32_t mode, int64_t offset, int64_t len);

/*
 * B
 * 把文件裁剪到指定的大小
 * size:
 *  文件被截断到这个大小
 */
int32_t low_fs_ftruncate(int32_t fd, int64_t size);

#endif

