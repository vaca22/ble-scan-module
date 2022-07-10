
#ifndef __low_port_h__
#define __low_port_h__

#include "stdio.h"
#include "port_cfg.h"

#ifndef NULL
#define NULL	0
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef bool 
#define bool int
#endif

/*
 * 在此文件中定义平台相关的杂项
 */

#define port_log(...)               do{printf(__VA_ARGS__);}while(0)
#define port_log_imm(...)           do{printf(__VA_ARGS__);fflush(stdout);}while(0)

//#define CHECK_PARAM(exp) ((exp) ? (void)0 : port_log("%s, %d\n", __FILE__, __LINE__))
#define CHECK_PARAM(exp) (void)0
	
#endif

