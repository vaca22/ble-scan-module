
#ifndef __low_ble_h__
#define __low_ble_h__

#include "stdint.h"

#define BLE_SVC_TYPE_CONN           0	//配网服务
#define BLE_SVC_TYPE_RC             1   //电机或者遥控
#define BLE_SVC_TYPE_MAX            2   //ble支持的服务总数,目前只有配网和遥控两种

#define BLE_SVC_CONN_SSID_LEN    	(32 + 1)  //配网时ssid的最大长度
#define BLE_SVC_CONN_KEY_LEN     	(32 + 1)  //配网时key的最大长度
#define BLE_SVC_CONN_PID_LEN     	8         //配网时设备pid的长度

//连网控制命令,由app发过来
#define BLE_SVC_CONN_CTRL_NONE      0   //什么也不做，初始值
#define BLE_SVC_CONN_CTRL_CONNECT   1   //开始连网
#define BLE_SVC_CONN_CTRL_RESET     2   //清除之前设置的ssid和key
#define BLE_SVC_CONN_CTRL_STOP      3   //关闭ble
//连网状态
#define BLE_SVC_CONN_STAT_IDLE      0   //不在连网
#define BLE_SVC_CONN_STAT_ING       1   //正在连网
#define BLE_SVC_CONN_STAT_SUCC      2   //连网成功
#define BLE_SVC_CONN_STAT_FAIL      3   //连网失败

#define BLE_SVC_RC_PID_LEN          8   //遥控器时pid的长度
#define BLE_SVC_RC_KEY_VAL_LEN      20  //遥控器时遥控按键信息的最大长度
#define BLE_SVC_RC_STAT_LEN         10  //遥控器时返回给遥控器状态信息的最大长度

//ble作为service时各characteristic的uuid,不同平台必须使用这些uuid
enum
{
    BLE_UUID_NET_CFG_SVC = 0x2B00,  //网络配置服务的uuid
    BLE_UUID_NET_CFG_VAL_PID,       //存放设备id的ATT的uuid
    BLE_UUID_NET_CFG_VAL_SSID,      //存放ssid
    BLE_UUID_NET_CFG_VAL_KEY,       //存放key
    BLE_UUID_NET_CFG_VAL_CTRL,      //连网控制
    BLE_UUID_NET_CFG_VAL_STAT,      //连网状态

    BLE_UUID_RC_SVC = 0x2C00,       //遥控服务的uuid
    BLE_UUID_RC_PID,                //存放设备的id的ATT的uuid
    BLE_UUID_RC_KEY,                //存放设备收到的遥控数据
    BLE_UUID_RC_STAT,               //存放设备端的状态，或者要返回给遥控器的信息
};

//attr table中各个特性的索引, 由平台决定是否需要这些索引
enum
{
    BLE_ATTR_DB_IDX_SVC_CONN,       //联网
    BLE_ATTR_DB_IDX_CHAR_PID,       //设备pid特性申明
    BLE_ATTR_DB_IDX_CHAR_PID_VAL,   //设备pid特性值
    BLE_ATTR_DB_IDX_CHAR_SSID,      //ssid特性申明
    BLE_ATTR_DB_IDX_CHAR_SSID_VAL,  //ssid特性值
    BLE_ATTR_DB_IDX_CHAR_KEY,       //key的特性申明
    BLE_ATTR_DB_IDX_CHAR_KEY_VAL,   //key特性值
    BLE_ATTR_DB_IDX_CHAR_CTRL,      //ctrl declaration
    BLE_ATTR_DB_IDX_CHAR_CTRL_VAL,  //ctrl val
    BLE_ATTR_DB_IDX_CHAR_STAT,      //stat declaration
    BLE_ATTR_DB_IDX_CHAR_STAT_VAL,  //stat val

    BLE_ATTR_DB_IDX_SVC_RC,         //蓝牙遥控
    BLE_ATTR_DB_IDX_CHAR_RC_PID,    //设备pid特性申明
    BLE_ATTR_DB_IDX_CHAR_RC_PID_VAL,//设备pid特性值
    BLE_ATTR_DB_IDX_CHAR_RC_KEY,    //遥控器按键信息特性申明
    BLE_ATTR_DB_IDX_CHAR_RC_KEY_VAL,//遥控器按键信息值
    BLE_ATTR_DB_IDX_CHAR_RC_STAT,   //设备遥控状态特性申明
    BLE_ATTR_DB_IDX_CHAR_RC_STAT_VAL,//设备遥控状态值

    BLE_ATTR_DB_IDX_NUM,
};

//ble service启动时的配置参数结构体,由平决定是否需要这个定义
typedef union
{
	struct low_ble_service_conn_param
	{
    	uint8_t pid[BLE_SVC_CONN_PID_LEN]; //设备pid
	}conn;
    struct low_ble_service_param_rc
    {
        uint8_t pid[BLE_SVC_RC_PID_LEN]; //设备pid
    }rc;
}low_ble_service_param_t;

//ble服务结构体,由平台决定是否需要这个定义
typedef union
{
	struct low_ble_service_conn_cfg             //连网服务
	{
		int32_t stat;							//连网的状态,由应用层来设置
		int32_t ctrl;							//连网控制,由client来设置
		uint8_t ssid[BLE_SVC_CONN_SSID_LEN];	//client设置的ssid
		uint8_t key[BLE_SVC_CONN_KEY_LEN];		//client设置的key
    	uint8_t pid[BLE_SVC_CONN_PID_LEN];		//设备pid
	}conn;
    struct low_ble_service_rc                   //遥控器服务
    {
        int32_t connected;                      //ble遥控是否处于连接状态
        int32_t key_chg;                        //key发生变化的标志，读一次之后会被清除掉
        uint8_t pid[BLE_SVC_RC_PID_LEN];
        uint8_t stat[BLE_SVC_RC_STAT_LEN];
        uint8_t key[BLE_SVC_RC_KEY_VAL_LEN];
    }rc;
	uint8_t reserver[4096];						//额外的空间，为了凑够4k,Esp32平台特有的
}low_ble_service_cfg_t;

//ble作为service时的结构体,由平台决定是否需要这个定义
typedef struct low_ble_service_struct
{
	int32_t					type;
	low_ble_service_cfg_t	cfg;
}low_ble_service_t;

//ble管理结构体,由平台决定是否需要这个定义
typedef struct low_ble_struct
{
    int32_t				init_flag;
    low_ble_service_t	*service[BLE_SVC_TYPE_MAX];
    int32_t flag_gatts_gap_handler;
}low_ble_t;

/*
 * 设置ble的名字,在ble service启动之前必须先调用这个函数把ble的名字设置好
 * TY:淘云
 * 01:硬件平台
 * 1051:pid
 * ABCD:mac的后四位
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_ble_set_name(char* first_name, char* hw, int32_t pid, char* mac);

/*
 * 初始化ble模块，只初始化硬件模块，不启动任务服务
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_ble_init(void);

/*
 * 关闭ble模块，应用层不再使用ble的时候会调用这个函数
 */
void low_ble_term(void);

/*
 * 启动中ble配网服务
 * pid: 设备的pid, 会把pid放到 BLE_UUID_NET_CFG_VAL_PID 中
 *      传入的值应该是16进制的 1051AABBCCDDEEFF, 可以使用wifi_connect_get_pid_mac_hex()来获取
 * retval:
 *  0:配网服务启动成功
 * -1:failed
 */
int32_t low_ble_service_conn_start(uint8_t *pid);

/*
 * 中止配网服务
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_ble_service_conn_stop(void);

/*
 * 连网状态设置，设备连网任务会根据当前连网的状态来调用这个函数，设置不同的状态
 * app通过读BLE_UUID_NET_CFG_VAL_STAT来拿到设备的连网情况
 * param:
 *  在STAT_FAIL的情况下表示出错的错误码
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_ble_service_conn_stat_set(int32_t stat, void* param);

/*
 * 连网控制命令，app通过写BLE_UUID_NET_CFG_VAL_CTRL,来控制设备连网启动等操作
 * 设备连网任务，会不断查询这个控制命令，并根据命令做出对应的操作
 * retval:
 *  app写在BLE_UUID_NET_CFG_VAL_CTRL中的控制命令
 */
int32_t low_ble_service_conn_ctrl_get(void);

/*
 * 获取app设置的ssid, app通过写BLE_UUID_NET_CFG_VAL_SSID来设置要连的网络名称
 * 设备连网任务,在收到app发来的连网命令之后，会调用这个接口来获取ssid
 * retval:
 *  app写在BLE_UUID_NET_CFG_VAL_SSID中的ssid
 */
uint8_t *low_ble_service_conn_ssid_get(void);

/* 获取app设置的key，app通过写BLE_UUID_NET_CFG_VAL_KEY来设置密码
 * 设备连网任务，在收到app发来的连网命令之后，人调用这个接口来获取key
 * retval:
 *  app写在BLE_UUID_NET_CFG_VAL_KEY中的key
 */
uint8_t *low_ble_service_conn_key_get(void);

/*
 * remote control遥控控制相关的服务
 * pid: 设备的pid, 会把pid放到 BLE_UUID_RC_PID 中
 *      传入的值应该是16进制的 1051AABBCCDDEEFF, 可以使用wifi_connect_get_pid_mac_hex()来获取
 * retval:
 *  0:遥控服务启动成功
 * -1:failed
 */
int32_t low_ble_service_rc_start(uint8_t *pid);

/*
 * 中止遥控服务
 */
int32_t low_ble_service_rc_stop(void);

/*
 * 获取app的遥控状态, app通过写BLE_UUID_RC_KEY把遥控按键状态发送给设备
 * 设备端遥控任务会不断调用这个接口来获取遥控操作
 * buf:
 *  存放BLE_UUID_RC_KEY中的按键状态，长度由BLE_SVC_RC_KEY_VAL_LEN决定
 * len:
 *  存放实际读到的数据长度
 * retval:
 *  0:从上次函数调用到本次调用遥控值没有发生过变化
 *  1:遥控值发生过变化
 * -1:failed
 */
int32_t low_ble_service_rc_key_get(uint8_t* buf, int32_t *len);

/*
 * 设置遥控器的状态,用于告诉遥控器，设备的一些状态信息，目前实现一个空函数就行了
 * stat:
 *  要设置的状态值
 * type:
 *  要设置的状态类型
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_ble_service_rc_stat_set(uint8_t* stat, int32_t type);

/*
 * 检查当前ble遥控是否处于连接状态
 * retval:
 *  1:遥控是连着的
 *  0:遥控是断开的
 */
int32_t low_ble_service_rc_connect_chk(void);


#endif

