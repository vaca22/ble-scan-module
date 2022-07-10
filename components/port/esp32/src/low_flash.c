
#include "low_flash.h"

/*
 * 初始化flash
 * retval:
 * 0:success
 *-1:failed
 */
int32_t low_flash_init(void)
{
    return 0;
}

/*
 * 不再使用flash了，把他关掉
 * retval:
 * 0:success
 *-1:failed
 */
int32_t low_flash_term(void)
{
    return 0;
}

/*
 * 往flash读数据
 * buf[in]: 要写入的数据
 * addr[in]: 要写入的数据地址
 * len[in]:要写入的数据量
 * retval:
 *  0:success
 * -1:failed
 */
int32_t low_flash_write(void* buf, uint32_t addr, uint32_t len)
{
    return 0;
}

/*
 * 从flash读数据
 * buf[out]:存放读到的数据
 * addr[in]:读数据的地址
 * len[in]:要读的数据量
 * retval:
 *  0:success
 * -1:failed
 */
int32_t low_flash_read(void* buf, uint32_t addr, uint32_t len)
{
    return 0;
}

/*
 * 擦除flash中的一块,要检查地址以及长度是否符合擦除的对齐要求
 * addr[in]:擦除的起始地址
 * len[in]:擦除数据的大小
 * retval:
 * 0:success
 *-1:failed
 */
int32_t low_flash_erase(uint32_t addr, uint32_t len)
{
    return 0;
}

/*
 * 查询flash的sector大小，sector大小是可以擦除的最少字节数
 * retval:
 *  sector大小
 */
uint32_t low_flash_sector_size(void)
{
    return 0;
}


