
#ifndef __low_data_log_h__
#define __low_data_log_h__

/*
 * 用来导出大量数据，目前主要是给导录音使用
 * 可以使用uart, spi ...
 * low_data_log在移植时是非必须的
 */


#include "plat_typedef.h"

/*
 * 初始化数据导出模块
 * retval:
 *  0: ok
 * -1: failed
 */
int32_t low_data_log_init(void);

/*
 * 逆初始化数据导出模块，释放占用的资源
 * retval:
 *  0: ok
 * -1: failed
 */
int32_t low_data_log_term(void);

/*
 * 导出数据，要实现成非阻塞的 
 * buf:
 *  导出的数据
 * size:
 *  导出的数据量，单位为字节
 * retval:
 *  0: ok
 * -1: failed
 */
int32_t low_data_log(uint8_t *buf, int32_t size);

#endif


