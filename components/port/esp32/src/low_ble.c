
#include "stdio.h"
#include "string.h"

#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"

#include "stdio.h"

#include "plat_os.h"
#include "low_ble.h"
#include "esp_gatt_common_api.h"
#define BLE_MODE                    ESP_BT_MODE_BLE//ESP_BT_MODE_BTDM
#define BLE_APPID_SVC_NET_CFG       0
#define BLE_APPID_SVC_RC            1
#define BLE_NAME_LEN                16
#define BLE_MANU_INFO_LEN			4


#define BLE_PROFILE_NUM_SVC         2 //service profile的个数
#define BLE_PROFILE_IDX_NET_CFG     0 //网络配置在所有profile中的序号
#define BLE_PROFILE_IDX_RC          1

char ble_name[BLE_NAME_LEN] = {0};


uint8_t manu[BLE_MANU_INFO_LEN] = {0};
uint8_t service_data[] = {0x03, 0x04, 0x05, 0x06};

static uint8_t low_ble_service_uuid[] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x00,
};

uint16_t handle_table[BLE_ATTR_DB_IDX_NUM];


static esp_ble_adv_data_t ble_adv_config = {
    .set_scan_rsp       = false,
    .include_name       = true,
    .include_txpower    = true,
    .min_interval       = 0x20,
    .max_interval       = 0x40,
    .appearance         = 0x00,

    .manufacturer_len   = 1,
    .p_manufacturer_data = manu,

    .service_data_len   = 1,
    .p_service_data     = service_data,

    .service_uuid_len   = sizeof(low_ble_service_uuid),
    .p_service_uuid     = low_ble_service_uuid,
    .flag               = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static esp_ble_adv_params_t ble_adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    //.peer_addr            =
    //.peer_addr_type       =
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

struct gatts_profile_inst {
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
};




static int32_t low_ble_service_start(int32_t type, low_ble_service_param_t *param);
static void low_ble_service_stop(int32_t type);
static void ble_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void ble_gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
static int32_t ble_gatts_attr_db_val_set(int32_t type, uint8_t* val, int32_t len);
static void low_ble_service_rc_connect_set(int32_t connect);
static void low_ble_service_rc_key_chg_reset(void);
static int32_t ble_server_uuid_set(uint16_t uuid);
static int32_t ble_server_manu_info_set(uint8_t type);


low_ble_t ble = {0};

int32_t low_ble_set_name(char* first_name, char* hw, int32_t pid, char* mac)
{
    if(strlen(ble_name) > 0)
    {
        printf("[ble]set_name already seted:%s\n", ble_name);
        return 0;
    }
    snprintf(ble_name, BLE_NAME_LEN, "%s_%s%d%s", first_name, hw, pid, mac);
	printf("[ble]ble set name :%s\n", ble_name);
    return 0;
}

int32_t low_ble_init(void)
{
    int32_t i;
    esp_err_t rev;
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if(ble.init_flag)
    {
        return 0;
    }
    rev = esp_bt_controller_init(&bt_cfg);
    if(rev)
    {
        printf("[ble]init controller init failed:0x%x\n", rev);
        return -1;
    }
    rev = esp_bt_controller_enable(BLE_MODE);
    if(rev)
    {
        printf("[ble]init controller enable failed:0x%x\n", rev);
        return -1;
    }
    rev = esp_bluedroid_init();
    if(rev)
    {
        printf("[ble]init bluetooth init failed:0x%x\n", rev);
        return -1;
    }
    rev = esp_bluedroid_enable();
    if(rev)
    {
        printf("[ble]init bluetooth enable failed:0x%x\n", rev);
        return -1;
    }
    ble.init_flag = 1;
    ble.flag_gatts_gap_handler = 0;
    for(i = 0; i < BLE_SVC_TYPE_MAX; i++)
    {
        ble.service[i] = NULL;
    }
	    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(500);
    if (local_mtu_ret){
        printf("[ble]local_mtu_ret:%d\n",local_mtu_ret);
    }
    return 0;
}

void low_ble_term(void)
{
    if(ble.init_flag == 0)
    {
        return;
    }
    low_ble_service_stop(BLE_SVC_TYPE_CONN);
    low_ble_service_stop(BLE_SVC_TYPE_RC);

    esp_bluedroid_disable();
    esp_bluedroid_deinit();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    ble.init_flag = 0;
}

low_ble_service_t* low_ble_service_obj_get(int32_t type)
{
    if(type >= BLE_SVC_TYPE_MAX)
    {
        return NULL;
    }
    return ble.service[type];
}

int32_t low_ble_service_obj_set(int32_t type, low_ble_service_t* service)
{
    if(type >= BLE_SVC_TYPE_MAX)
    {
        return -1;
    }
    ble.service[type] = service;
    return 0;
}

int32_t low_ble_adv_data_config(int32_t type)
{

	esp_ble_gap_stop_advertising();
	switch(type)
	{
		case BLE_SVC_TYPE_CONN:
            ble_server_uuid_set(BLE_UUID_NET_CFG_SVC);
            break;
		case BLE_SVC_TYPE_RC:
			ble_server_uuid_set(BLE_UUID_RC_SVC);
			break;
        default:
			printf("[low_ble]adv data config err type:%d\n", type);
			return -1;
	}
	ble_server_manu_info_set(type);
	
    return  esp_ble_gap_config_adv_data(&ble_adv_config);
}

/*
 * 启动一个ble服务，目前只有配网一种服务
 * cfg:
 *  与服务相关的配置
 * retval:
 *  0:ok
 * -1:failed
 */
int32_t low_ble_service_start(int32_t type, low_ble_service_param_t *param)
{
    low_ble_service_t *service = low_ble_service_obj_get(type);
    if(service != NULL)
    {
        printf("[ble]service_start already started\n");				
		low_ble_adv_data_config(type);	/*重新配置广播数据内容*/
        return 0;
    }
    if(ble.flag_gatts_gap_handler == 0) //要先注册回调函数,后面才可以去注册app
    {
        ble.flag_gatts_gap_handler = 1;
        esp_ble_gatts_register_callback(ble_gatts_event_handler);
        esp_ble_gap_register_callback(ble_gap_event_handler);
    }

    service = os_mem_alloc(sizeof(low_ble_service_t));
	service->type = type;
	switch (type)
	{
		case BLE_SVC_TYPE_CONN:
            low_ble_service_obj_set(type, service);
            printf("[ble]service_start for connect\n");
		    service->cfg.conn.stat = BLE_SVC_CONN_STAT_IDLE;
			service->cfg.conn.ctrl = BLE_SVC_CONN_CTRL_NONE;
			memset(service->cfg.conn.ssid, 0, BLE_SVC_CONN_SSID_LEN);
			memset(service->cfg.conn.key, 0, BLE_SVC_CONN_KEY_LEN);
			memcpy(service->cfg.conn.pid, param->conn.pid, BLE_SVC_CONN_PID_LEN);
			ble_server_uuid_set(BLE_UUID_NET_CFG_SVC);
            esp_ble_gatts_app_register(BLE_APPID_SVC_NET_CFG);
			break;
        case BLE_SVC_TYPE_RC:
            low_ble_service_obj_set(type, service);
            printf("[ble]service_start for rc\n");
            memset(service->cfg.rc.key, 0, BLE_SVC_RC_KEY_VAL_LEN);
            memset(service->cfg.rc.stat, 0, BLE_SVC_RC_STAT_LEN);
            memcpy(service->cfg.rc.pid, param->rc.pid, BLE_SVC_RC_PID_LEN);
            low_ble_service_rc_connect_set(0);
            low_ble_service_rc_key_chg_reset();
			ble_server_uuid_set(BLE_UUID_RC_SVC);
            esp_ble_gatts_app_register(BLE_APPID_SVC_RC);
            break;
		default:
			break;
	}

    //esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    return 0;
}

/*
 * 停止ble服务
 * type:
 *  要停止的服务类型，目前还只有BLE_SVC_TYPE_CONN这一种
 * retval:
 *  0: ok
 * -1: failed
 */
void low_ble_service_stop(int32_t type)
{
	
    low_ble_service_t *service;
	
    service = low_ble_service_obj_get(type);
    if(service == NULL) //没有这个service或者service已经stop了
    {
        printf("[ble]service_stop:%d already stoped\n", type);
        return;
    }
    switch(type)
    {
        case BLE_SVC_TYPE_CONN:
            esp_ble_gatts_app_unregister(BLE_APPID_SVC_NET_CFG);
            break;
		case BLE_SVC_TYPE_RC:
			esp_ble_gatts_app_unregister(BLE_APPID_SVC_RC);
			break;
        default:
            break;
    }

    os_mem_free(service);
    low_ble_service_obj_set(type, NULL);

}

int32_t low_ble_service_conn_start(uint8_t *pid)
{
	int32_t type;
	low_ble_service_param_t param;
	type = BLE_SVC_TYPE_CONN;
	memcpy(param.conn.pid, pid, BLE_SVC_CONN_PID_LEN);
    return low_ble_service_start(type, &param);
}

int32_t low_ble_service_conn_stop(void)
{
    low_ble_service_stop(BLE_SVC_TYPE_CONN);
    return 0;
}


int32_t low_ble_service_conn_stat_set(int32_t stat, void* param)
{
    uint8_t stat_buf[2];
    int32_t val;
    low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_CONN);
    stat_buf[0] = 0;
    stat_buf[1] = 0;
    stat_buf[0] = stat;
    if(stat == BLE_SVC_CONN_STAT_FAIL)
    {
        val = (int32_t)param;
        stat_buf[1] = val;
    }
	service->cfg.conn.stat = stat;
    ble_gatts_attr_db_val_set(BLE_ATTR_DB_IDX_CHAR_STAT_VAL, stat_buf, 2);
    return 0;
}

int32_t low_ble_service_conn_ctrl_set(uint8_t ctrl)
{
    low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_CONN);
	if(service == NULL)
	{
		printf("[low_ble]conn_ctrl_set get service null\n");	
		return -1;
	}
	service->cfg.conn.ctrl = ctrl;
	printf("[low_ble]ctrl set:%d\n", service->cfg.conn.ctrl);
	return 0;
}

int32_t low_ble_service_conn_ctrl_get(void)
{
    low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_CONN);
	if(service == NULL)
	{
		printf("[low_ble]conn_ctrl_get get service null\n");	
		return -1;
	}
    return service->cfg.conn.ctrl;
}

int32_t low_ble_service_conn_ssid_set(uint8_t *ssid, int32_t offset)
{
    low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_CONN);
	if(service == NULL)
	{
		printf("[low_ble]ssid set get service null\n");
		return -1;
	}
    snprintf((char*)service->cfg.conn.ssid + offset, BLE_SVC_CONN_SSID_LEN - offset, (char*)ssid);
	printf("[low_ble]ssid set:%s\n", (char*)service->cfg.conn.ssid);
	return 0;
}

uint8_t *low_ble_service_conn_ssid_get(void)
{
    low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_CONN);
	if(service == NULL)
	{
		printf("[low_ble]conn_ssid_get get service null\n");
		return NULL;
	}
	return service->cfg.conn.ssid;
}

int32_t low_ble_service_conn_key_set(uint8_t *key, int32_t offset)
{
    low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_CONN);
	if(service == NULL)
	{
		printf("[low_ble]conn_key_set get service null\n");
		return -1;
	}
    snprintf((char*)service->cfg.conn.key + offset, BLE_SVC_CONN_KEY_LEN - offset, (char*)key);
	printf("[low_ble]key set:%s\n", (char*)service->cfg.conn.key);
	return 0;
}

uint8_t *low_ble_service_conn_key_get(void)
{
    low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_CONN);
	if(service == NULL)
	{
		printf("[low_ble]conn_key_get get service null\n");
		return NULL;
	}
	return service->cfg.conn.key;
}

uint8_t *low_ble_service_conn_pid_get(void)
{
    low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_CONN);
	if(service == NULL)
	{
		printf("[low_ble]conn_pid_get get service null\n");
		return NULL;
	}
	return service->cfg.conn.pid;
}

int32_t low_ble_service_rc_start(uint8_t *pid)
{
    int32_t type;
    low_ble_service_param_t param;
    type = BLE_SVC_TYPE_RC;
    memcpy(param.rc.pid, pid, BLE_SVC_RC_PID_LEN);
    return low_ble_service_start(type, &param);
}

int32_t low_ble_service_rc_stop(void)
{
    low_ble_service_stop(BLE_SVC_TYPE_RC);
    return 0;
}

uint8_t *low_ble_service_rc_pid_get(void)
{
	low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_RC);
	if(service == NULL)
	{
		printf("[ble]rc_pid_get: already stoped\n");
		return NULL;
	}		
	return service->cfg.rc.pid;
}

int32_t low_ble_service_rc_key_set(uint8_t* key, int32_t len)
{
	low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_RC);
	if(service == NULL)
	{
		printf("[ble]rc_key_set: already stoped\n");
		return -1;
	}
	service->cfg.rc.key_chg = 1;
	memcpy(service->cfg.rc.key, key, len);
	return 0;
}

void low_ble_service_rc_key_chg_reset(void)
{
    low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_RC);
	if(service == NULL)
	{
		printf("[low_ble]rc_key_chg_reset get service null\n");
		return;
	}
    service->cfg.rc.key_chg = 0;
}

int32_t low_ble_service_rc_key_get(uint8_t* buf, int32_t *len)
{
	int32_t chg;
	low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_RC);
	if(service == NULL)
	{
		printf("[ble]c_key_get: already stoped\n");
		return -1;
	}
	memcpy(buf, service->cfg.rc.key, BLE_SVC_RC_KEY_VAL_LEN);
	*len = BLE_SVC_RC_KEY_VAL_LEN;
	chg = service->cfg.rc.key_chg;
	service->cfg.rc.key_chg = 0;
	return chg;
}

int32_t low_ble_service_rc_stat_set(uint8_t* stat, int32_t type)
{
	uint8_t *stat_buf;
	low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_RC);
	if(service == NULL)
	{
		printf("[ble]rc_stat_set: already stoped\n");
		return -1;
	}
	stat_buf = service->cfg.rc.stat;
	stat_buf[0]++;
	return ble_gatts_attr_db_val_set(BLE_ATTR_DB_IDX_CHAR_RC_STAT_VAL, stat_buf, BLE_SVC_RC_STAT_LEN);
}

void low_ble_service_rc_connect_set(int32_t connect)
{
	low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_RC);
	if(service == NULL)
	{
		printf("[ble]rc_connect_set: already stoped\n");
		return;
	}
	service->cfg.rc.connected = connect;
	printf("[ble]--------------------->rc_connect:%d\n", connect);
}

int32_t low_ble_service_rc_connect_chk(void)
{
    low_ble_service_t* service = low_ble_service_obj_get(BLE_SVC_TYPE_RC);
	if(service == NULL) //没有这个service或者service已经stop了
    {
        printf("[ble]service_rc_connect_chk:already stoped\n");
        return -1;
    }
    if(service->cfg.rc.connected)
    {
        return 1;
    }
    return 0;
}

static int32_t ble_server_uuid_set(uint16_t uuid)
{
	low_ble_service_uuid[12] = uuid & 0X00FF;
	low_ble_service_uuid[13] = (uuid & 0XFF00) >> 8;
	printf("[ble]server uuid set:0X%X\n", uuid);
	return 0;
}
static int32_t ble_server_manu_info_set(uint8_t type)
{
	snprintf((char *)manu, BLE_MANU_INFO_LEN, "%d", type + 1);
	printf("[ble] manu info set:%s\n", manu);
	return 0;
}

static void ble_gatts_profile_event_handler_cb(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

static struct gatts_profile_inst ble_profile_tab_svc[BLE_PROFILE_NUM_SVC] = 
{
    [BLE_PROFILE_IDX_NET_CFG] =
    {
        .gatts_cb = ble_gatts_profile_event_handler_cb,
        .gatts_if = ESP_GATT_IF_NONE,
    },

    [BLE_PROFILE_IDX_RC] =
    {
        .gatts_cb = ble_gatts_profile_event_handler_cb,
        .gatts_if = ESP_GATT_IF_NONE,
    }
};

#define CHAR_DECLARATION_SIZE   1
static const uint16_t uuid_primary_service      = ESP_GATT_UUID_PRI_SERVICE;
static const uint16_t uuid_char_declaration     = ESP_GATT_UUID_CHAR_DECLARE;
//static const uint16_t uuid_char_description     = ESP_GATT_UUID_CHAR_DESCRIPTION;
static const uint16_t uuid_ble_net_service      = BLE_UUID_NET_CFG_SVC;
static const uint16_t uuid_ble_net_val_ssid     = BLE_UUID_NET_CFG_VAL_SSID;
static const uint16_t uuid_ble_net_val_key      = BLE_UUID_NET_CFG_VAL_KEY;
static const uint16_t uuid_ble_net_val_pid      = BLE_UUID_NET_CFG_VAL_PID;
static const uint16_t uuid_ble_net_val_ctrl     = BLE_UUID_NET_CFG_VAL_CTRL;
static const uint16_t uuid_ble_net_val_stat     = BLE_UUID_NET_CFG_VAL_STAT;
static const uint16_t uuid_ble_rc_service       = BLE_UUID_RC_SVC;
static const uint16_t uuid_ble_rc_pid           = BLE_UUID_RC_PID;
static const uint16_t uuid_ble_rc_key           = BLE_UUID_RC_KEY;
static const uint16_t uuid_ble_rc_stat          = BLE_UUID_RC_STAT;

//static uint8_t char_prop_notify = ESP_GATT_CHAR_PROP_BIT_NOTIFY;
static uint8_t char_prop_rd     = ESP_GATT_CHAR_PROP_BIT_READ;
static uint8_t char_prop_wr     = ESP_GATT_CHAR_PROP_BIT_WRITE;
static uint8_t char_prop_rw     = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE;

//uint16_t handle_table[BLE_ATTR_DB_IDX_NUM];

static const esp_gatts_attr_db_t ble_gatts_db[] = 
{
    [BLE_ATTR_DB_IDX_SVC_CONN] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_primary_service,
         ESP_GATT_PERM_READ,
         sizeof(uint16_t), sizeof(uuid_ble_net_service), (uint8_t*)&uuid_ble_net_service}
    },
//pid characteristic 特性申明
    [BLE_ATTR_DB_IDX_CHAR_PID] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_char_declaration,
         ESP_GATT_PERM_READ,
         CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t*)&char_prop_rd}
    },
    [BLE_ATTR_DB_IDX_CHAR_PID_VAL] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_ble_net_val_pid,
         ESP_GATT_PERM_READ,
         BLE_SVC_CONN_PID_LEN, 0, NULL}
    },

//ssid characteristic 特性申明
    [BLE_ATTR_DB_IDX_CHAR_SSID] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_char_declaration,
         ESP_GATT_PERM_READ,
         CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t*)&char_prop_rw}
    },
//ssid characteristic value
    [BLE_ATTR_DB_IDX_CHAR_SSID_VAL] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_ble_net_val_ssid,
         ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
         BLE_SVC_CONN_SSID_LEN, 0, NULL}
    },

//key characteristic
    [BLE_ATTR_DB_IDX_CHAR_KEY] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_char_declaration,
         ESP_GATT_PERM_READ,
         CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t*)&char_prop_rw}
    },
//key charactristic value
    [BLE_ATTR_DB_IDX_CHAR_KEY_VAL] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_ble_net_val_key,
         ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
         BLE_SVC_CONN_KEY_LEN, 0, NULL}
    },

//ctrl characteristic
    [BLE_ATTR_DB_IDX_CHAR_CTRL] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_char_declaration,
         ESP_GATT_PERM_READ,
         CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t*)&char_prop_wr}
    },
//ctrl charactristic value
    [BLE_ATTR_DB_IDX_CHAR_CTRL_VAL] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_ble_net_val_ctrl,
         ESP_GATT_PERM_WRITE,
         1, 0, NULL}
    },

//stat characteristic
    [BLE_ATTR_DB_IDX_CHAR_STAT] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_char_declaration,
         ESP_GATT_PERM_READ,
         CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t*)&char_prop_rd}
    },
//stat charactristic value
    [BLE_ATTR_DB_IDX_CHAR_STAT_VAL] =
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_ble_net_val_stat,
         ESP_GATT_PERM_READ,
         2, 0, NULL}
    },

    [BLE_ATTR_DB_IDX_SVC_RC] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_primary_service,
         ESP_GATT_PERM_READ,
         sizeof(uint16_t), sizeof(uuid_ble_rc_service), (uint8_t*)&uuid_ble_rc_service}
    },
//pid characteristic
    [BLE_ATTR_DB_IDX_CHAR_RC_PID] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_char_declaration,
         ESP_GATT_PERM_READ,
         CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t*)&char_prop_rd}
    },
//pid charactristic value
    [BLE_ATTR_DB_IDX_CHAR_RC_PID_VAL] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_ble_rc_pid,
         ESP_GATT_PERM_READ,
         BLE_SVC_RC_PID_LEN, 0, NULL}
    },

//rc_key characteristic
    [BLE_ATTR_DB_IDX_CHAR_RC_KEY] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_char_declaration,
         ESP_GATT_PERM_READ,
         CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t*)&char_prop_rw}
    },
//rc_key charactristic value
    [BLE_ATTR_DB_IDX_CHAR_RC_KEY_VAL] = 
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_ble_rc_key,
         ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
         BLE_SVC_RC_KEY_VAL_LEN, 0, NULL}
    },
    [BLE_ATTR_DB_IDX_CHAR_RC_STAT] = 
//rc_stat characteristic
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_char_declaration,
         ESP_GATT_PERM_READ,
         CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t*)&char_prop_rd}
    },
//rc_stat charactristic value
    [BLE_ATTR_DB_IDX_CHAR_RC_STAT_VAL] =
    {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t*)&uuid_ble_rc_stat,
         ESP_GATT_PERM_READ,
         BLE_SVC_RC_STAT_LEN, 0, NULL}
    }
};

static void ble_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    int i;
    uint16_t appid;
    struct gatts_reg_evt_param* reg = &param->reg;
    appid = reg->app_id;
    printf("[ble]gatts_event_handler [appid:%d gatts_if:%d eve:%d]\n", appid, gatts_if, event);
    if(event == ESP_GATTS_REG_EVT)
    {
        if(param->reg.status == ESP_GATT_OK) 
        {
            printf("[ble]gatts_event_handler REG ok\n");
            ble_profile_tab_svc[appid].gatts_if = gatts_if;
        }
        else
        {
            printf("[ble]gatts_event_handler reg app failed:%d\n", param->reg.status);
            return;
        }
    }
    for(i = 0; i < BLE_PROFILE_NUM_SVC; i++)
    {
        if(gatts_if == ESP_GATT_IF_NONE || gatts_if == ble_profile_tab_svc[i].gatts_if)
        {
            if(ble_profile_tab_svc[i].gatts_cb)
            {
                ble_profile_tab_svc[i].gatts_cb(event, gatts_if, param);
            }
        }
    }
    return;
}

static void ble_gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch(event)
    {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            printf("[ble]gap_event BLE_ADV_DATA_SET_COMPLETE_EVT\n");
            esp_ble_gap_start_advertising(&ble_adv_params);
            break;
        case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
            printf("[ble]gap_event BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT\n");
            break;
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            printf("[ble]gap_event BLE_ADV_START_COMPLETE_EVT\n");
            if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
            {
                printf("[ble]adverstising start failed\n");
            }
            break;
        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
            printf("[ble]gap_event BLE_UPDATE_CONN_PARAMS_EVT\n");
            {
                //struct ble_update_conn_params_evt_param *update;
                //update = &param->update_conn_params;
                //printf("int min:%x max:%x latency:%x conn:%x timeout:%x\n", update->min_int, update->max_int, update->latency, update->conn_int);
            }
            break;
        default:
            printf("[ble]------------------->gap_event defualt:%d\n", event);
            break;

    }
    return;
}

static int32_t ble_gatts_attr_db_val_set(int32_t type, uint8_t* val, int32_t len)
{
    int32_t rev;
    uint16_t handle;
    switch(type)
    {
        case BLE_ATTR_DB_IDX_CHAR_PID_VAL:
            handle = handle_table[type];
            break;
        case BLE_ATTR_DB_IDX_CHAR_STAT_VAL:
            handle = handle_table[type];
            break;
        case BLE_ATTR_DB_IDX_CHAR_RC_PID_VAL:
            handle = handle_table[type];
            break;
        case BLE_ATTR_DB_IDX_CHAR_RC_STAT_VAL:
            handle = handle_table[type];
            break;
        default:
            printf("[ble]attr_db_val_set err type:%d not support now\n", type);
            return -1;
    }
    rev = esp_ble_gatts_set_attr_value(handle, len, val);
    //printf("[ble]attr_db_val set:%d h:%d rev:%d\n", type, handle, rev);
    return rev;
}

int32_t ble_gatts_profile_eve_deel_read(esp_gatt_if_t gatts_if, struct gatts_read_evt_param* read)
{
    return 0;
}

int32_t ble_gatts_profile_eve_deel_write(esp_gatt_if_t gatts_if, struct gatts_write_evt_param* write)
{
    uint32_t handle;
    handle = write->handle;
    if(handle == handle_table[BLE_ATTR_DB_IDX_CHAR_SSID_VAL])
    {
		low_ble_service_conn_ssid_set(write->value, write->offset);
    }
    else if(handle == handle_table[BLE_ATTR_DB_IDX_CHAR_KEY_VAL])
    {
		low_ble_service_conn_key_set(write->value, write->offset);
    }
    else if(handle == handle_table[BLE_ATTR_DB_IDX_CHAR_CTRL_VAL])
    {
		low_ble_service_conn_ctrl_set(*(uint8_t*)write->value);
    }
    else if(handle == handle_table[BLE_ATTR_DB_IDX_CHAR_RC_KEY_VAL])
    {
        low_ble_service_rc_key_set(write->value, write->len);
    }
    return 0;
}

/*
 * 现在所有profile共用一个event_handler
 */
static void ble_gatts_profile_event_handler_cb(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    switch(event)
    {
        case ESP_GATTS_REG_EVT:
            {
                uint16_t appid;
                struct gatts_reg_evt_param* reg;
                reg = &param->reg;
                appid = reg->app_id;

                printf("[ble]profile_event_handler_cb REG appid:%d gatts_if:%d status:%d\n", appid, gatts_if, reg->status);		

				ble_server_manu_info_set(appid);
                esp_ble_gap_set_device_name(ble_name);
                esp_ble_gap_config_adv_data(&ble_adv_config);
                if(appid == BLE_APPID_SVC_NET_CFG)
                {
                    esp_ble_gatts_create_attr_tab(&ble_gatts_db[BLE_ATTR_DB_IDX_SVC_CONN], gatts_if, BLE_ATTR_DB_IDX_SVC_RC, appid);
                }
                else if(appid == BLE_APPID_SVC_RC)
                {
                    esp_ble_gatts_create_attr_tab(&ble_gatts_db[BLE_ATTR_DB_IDX_SVC_RC], gatts_if, BLE_ATTR_DB_IDX_NUM - BLE_ATTR_DB_IDX_SVC_RC, appid);
                }
                break;
            }
        case ESP_GATTS_READ_EVT:
            printf("[ble]profile_event READ\n");
            ble_gatts_profile_eve_deel_read(gatts_if, &param->read);
            break;
        case ESP_GATTS_WRITE_EVT:
            //printf("[ble]profile_event WRITE\n");
            ble_gatts_profile_eve_deel_write(gatts_if, &param->write);
            break;
        case ESP_GATTS_CREAT_ATTR_TAB_EVT:
            printf("[ble]profile_event CREAT_ATTR_TAB\n");
            {
                int32_t i, uuid, rev, handle;
                struct gatts_add_attr_tab_evt_param *add;
                add = &param->add_attr_tab;
                uuid = add->svc_uuid.uuid.uuid16;
                printf("[ble]svc_uuid:0x%x the number of handle:%d\n", uuid, add->num_handle);
                if(add->status != ESP_GATT_OK)
                {
                    printf("create attribute table failed, err:0x%x\n", add->status);
                    break;
                }
                for(i = 0; i < add->num_handle; i++)
                {
                    printf("creat ok handle:%d\n", add->handles[i]);
                }
                if(uuid == BLE_UUID_NET_CFG_SVC)
                {
                    memcpy(&handle_table[BLE_ATTR_DB_IDX_SVC_CONN], add->handles, add->num_handle * sizeof(uint16_t));
                    ble_gatts_attr_db_val_set(BLE_ATTR_DB_IDX_CHAR_PID_VAL, low_ble_service_conn_pid_get(), BLE_SVC_CONN_PID_LEN);
    				low_ble_service_conn_stat_set(BLE_SVC_CONN_STAT_IDLE, NULL);
                    handle = handle_table[BLE_ATTR_DB_IDX_SVC_CONN];
                    rev = esp_ble_gatts_start_service(handle);
                    printf("start service handle:%d rev:%d\n", handle, rev);
                }
                else if(uuid == BLE_UUID_RC_SVC)
                {
                    uint8_t init_stat[10];
                    memset(init_stat, 0xFF, 10);
                    memcpy(&handle_table[BLE_ATTR_DB_IDX_SVC_RC], add->handles, add->num_handle * sizeof(uint16_t));
                    ble_gatts_attr_db_val_set(BLE_ATTR_DB_IDX_CHAR_RC_PID_VAL, low_ble_service_rc_pid_get(), BLE_SVC_RC_PID_LEN);
                    ble_gatts_attr_db_val_set(BLE_ATTR_DB_IDX_CHAR_RC_STAT_VAL, init_stat, 10);
                    handle = handle_table[BLE_ATTR_DB_IDX_SVC_RC];
                    rev = esp_ble_gatts_start_service(handle);
                    printf("start service handle:%d rev:%d\n", handle, rev);
                }
            }
            break;
        case ESP_GATTS_CONNECT_EVT:
            printf("[ble]profile_event CONNECT\n");
            if((gatts_if == ble_profile_tab_svc[BLE_PROFILE_IDX_RC].gatts_if) && (manu[0] != '1'))
            {
                low_ble_service_rc_connect_set(1);
            }
	     esp_ble_gap_stop_advertising();
            break;
        case ESP_GATTS_DISCONNECT_EVT:
            printf("[ble]profile_event DISCONNECT\n");
            if((gatts_if == ble_profile_tab_svc[BLE_PROFILE_IDX_RC].gatts_if) && (manu[0] != '1'))
            {
                low_ble_service_rc_connect_set(0);
            }
            esp_ble_gap_start_advertising(&ble_adv_params);
            break;
        case ESP_GATTS_MTU_EVT:
            printf("[ble]profile_event MTU:%d\n", param->mtu.mtu);
            break;
        case ESP_GATTS_START_EVT:
        case ESP_GATTS_SET_ATTR_VAL_EVT:
            break;
        default:
            printf("[ble]profile_event default:%d\n", event);
            break;
    }
}

#if 0
#include "robot_cfg.h"

#define LOW_BLE_TEST_CONN   0
#define LOW_BLE_TEST_RC     1

void low_ble_test(void)
{
extern int32_t wifi_connect_get_pid_mac_hex(uint8_t *buf);
extern char* low_wifi_get_mac_int_str(void);
extern int32_t low_wifi_init(void);
	uint8_t pid[8], key[BLE_SVC_CONN_KEY_LEN];
    int32_t ctrl, ctrl_pre, key_rev, key_len;
    low_wifi_init();
    wifi_connect_get_pid_mac_hex(pid);
    low_ble_init();
    low_ble_set_name("TY", HW_NAME, PRODUCT_ID, low_wifi_get_mac_int_str() + 8);
#if LOW_BLE_TEST_CONN == 1
	low_ble_service_conn_start(pid);
#endif
#if LOW_BLE_TEST_RC == 1
    low_ble_service_rc_start(pid);
    ctrl = ctrl_pre = low_ble_service_conn_ctrl_get();
#endif
    while(1)
    {
        os_thread_sleep(100);
#if LOW_BLE_TEST_CONN == 1
        ctrl = low_ble_service_conn_ctrl_get();
        if(ctrl != ctrl_pre)
        {
            ctrl_pre = ctrl;
            switch(ctrl)
            {
                case BLE_SVC_CONN_CTRL_NONE:
                    printf("[ble]test idle\n");
                    break;
                case BLE_SVC_CONN_CTRL_CONNECT:
                    printf("[ble]test connect\n");
                    printf("ssid:%s key:%s\n", low_ble_service_conn_ssid_get(), low_ble_service_conn_key_get());
                    break;
                case BLE_SVC_CONN_CTRL_RESET:
                    printf("[ble]test connect reset\n");
                    break;
                default:
                    printf("[ble]test err conn ctrl:%d\n", ctrl);
                    break;
            }
        }
#endif
#if LOW_BLE_TEST_RC == 1
        key_len = BLE_SVC_RC_KEY_VAL_LEN;
        key_rev = low_ble_service_rc_key_get(key, &key_len);
        if(key_rev)
        {
            int32_t i;
            printf("[ble]test key changed:\n");
            for(i = 0; i < key_len; i++)
            {
                printf("0x%x, ", key[i]);
            }
            printf("\n");
            low_ble_service_rc_stat_set(NULL, 0);
        }
#endif
    }
}

#endif