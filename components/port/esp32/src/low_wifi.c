
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_ping.h"
#include "ping/ping.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "plat_os.h"

#include "low_ble.h"
#include "low_wifi.h"

enum ap_stat
{
    AP_STAT_STOP = 0,       //ap没有开
    AP_STAT_READY,          //热点已经建立
    AP_STAT_APP_CONNECTED,  //有设备连到热点上
    AP_STAT_APP_DISCONNECT, //设备断开连接
    AP_STAT_APP_REVERSED1,  //保留
    AP_STAT_CONNECTING,     //收到配置的网络，处于尝试连网状态
    AP_STAT_SUCCESS,        //连网成功
    AP_STAT_FAILED,         //连网失败
};

enum sta_stat
{
    STAT_IDLE,
    STAT_CONNECTING,    
    STAT_CONNECTED,
    STAT_DISCONNECTED
};

#define MAC_STR_SIZE				20 

#define WIFI_AP_STAT_STOP			0
#define WIFI_AP_STAT_START			1

#define WIFI_AP_CTRL_STOP			0
#define WIFI_AP_CTRL_START			1

#define WIFI_AP_SCAN_RESULT_NONE 	0			//未搜索到指定的热点
#define	WIFI_AP_SCAN_RESULT_HAVE  	1			//搜索到了指定的热点

#define WIFI_AP_SCAN_TIMEOUT		(20 * 1000)	//搜索热点的最大超时时间

static int32_t low_wifi_initflag = 0;
static enum ap_stat   ap_stat   = AP_STAT_STOP;
static enum sta_stat  sta_stat  = STAT_IDLE;
static TimerHandle_t  timer_handle_ip   = NULL;
static TimerHandle_t  timer_handle_connect  = NULL;
static int32_t low_wifi_err_code = 0;
static uint32_t low_wifi_ip;
#define CONNECT_RETRY_CNT   2
static int32_t connect_retry = 0;
static int32_t wifi_ap_scan_result = WIFI_AP_SCAN_RESULT_NONE;		//是否查询到指定的ssid，

static EventGroupHandle_t wifi_event_group;
#define CONNECTED_BIT   BIT0
#define WIFI_CONNECT_TIMOUT 10000   //连上网断的超时时间
#define GET_IP_TIMEOUT      15000    //连到网络后获取IP的超时时间，这么长时间内没有获取到ip的话就认为这个网络连接失败

static int32_t low_wifi_mac_str_to_int(char *mac_str, uint8_t mac[]);
#define low_wifi_err_code_set(v)    low_wifi_err_code = v

void low_wifi_timout_cb_connect(TimerHandle_t timer)
{
    printf("[low_wifi]connect timout!\n");
	low_wifi_err_code_set(WIFI_REASON_CONNECT_TIMEOUT);
	connect_retry = CONNECT_RETRY_CNT;
    esp_wifi_disconnect();
	sta_stat = STAT_DISCONNECTED;
}

int32_t low_wifi_timer_start_connect(void)
{
    int32_t timer_id;
    timer_id = 3;
    timer_handle_connect = xTimerCreate("con_tim", WIFI_CONNECT_TIMOUT, pdFALSE, (void*)timer_id, low_wifi_timout_cb_connect);
    if(timer_handle_connect != NULL)
    {
        xTimerStart(timer_handle_connect, 10);
        return 0;
    }
    printf("[low_wifi]timer_start_connect failed\n");
    return -1;
}

void low_wifi_timer_stop_connect(void)
{
    if(timer_handle_connect == NULL)
    {
        return;
    }
    xTimerStop(timer_handle_connect, 10);
    timer_handle_connect = NULL;
}

void low_wifi_timout_cb_get_ip(TimerHandle_t timer)
{
    printf("[low_wifi]getip timout!\n");
	low_wifi_err_code_set(WIFI_REASON_GET_IP_TIMEOUT);
    connect_retry = CONNECT_RETRY_CNT;
    esp_wifi_disconnect();
}

int32_t low_wifi_timer_start_get_ip(void)
{
    int32_t timer_id;
    timer_id = 2;
    timer_handle_ip = xTimerCreate("ip_tim", GET_IP_TIMEOUT, pdFALSE, (void*)timer_id, low_wifi_timout_cb_get_ip);
    if(timer_handle_ip != NULL)
    {
        xTimerStart(timer_handle_ip, 10);
        return 0;
    }
    printf("[low_wifi]timer_start_get_ip failed\n");
    return -1;
}

void low_wifi_timer_stop_get_ip(void)
{
    if(timer_handle_ip == NULL)
    {
        return;
    }
    xTimerStop(timer_handle_ip, 10);
    timer_handle_ip = NULL;
}
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    uint32_t tmp;
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        low_wifi_timer_stop_connect();
        low_wifi_timer_start_get_ip();
        printf("[low_wifi]event_handle station connected\n");
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        low_wifi_timer_stop_get_ip();
        low_wifi_ip = event->event_info.got_ip.ip_info.ip.addr;
        printf("[low_wifi]event_handle station got IP\n");
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        sta_stat = STAT_CONNECTED;
        connect_retry = CONNECT_RETRY_CNT; //网络已经连上了，把重连计数去掉,不重连
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        low_wifi_timer_stop_get_ip();
        low_wifi_timer_stop_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        tmp = event->event_info.disconnected.reason;
        printf("[low_wifi]event_handle station disconnect reason:%d\n", tmp);
		if ((low_wifi_err_code_get() != WIFI_REASON_CONNECT_TIMEOUT) && (low_wifi_err_code_get() != WIFI_REASON_GET_IP_TIMEOUT))
		{
			low_wifi_err_code_set(tmp);
		}
        if(++connect_retry >= CONNECT_RETRY_CNT)
        {
            printf("[low_wifi]event_handle station disconnect\n");
            sta_stat = STAT_DISCONNECTED;
        }
        else
        {
            if(tmp == WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT) //密码错误
            {
                connect_retry = CONNECT_RETRY_CNT;
                sta_stat = STAT_DISCONNECTED;
            }
            else
            {
                printf("[low_wifi]event_handle station reconnect:%d\n", connect_retry);
                esp_wifi_connect();
				low_wifi_timer_start_connect();
            }
        }
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        printf("[low_wifi]event_handle ap connected\n");
        ap_stat = AP_STAT_APP_CONNECTED;
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        tmp = event->event_info.disconnected.reason;
        printf("[low_wifi]event_handle ap disconnected reason:%d\n", tmp);
		if (ap_stat == AP_STAT_APP_CONNECTED)	// ap连网过程中忽略热点断开事件
		{
        	ap_stat = AP_STAT_APP_DISCONNECT;
		}
        break;
    default:
        break;
    }
    return ESP_OK;
}

int32_t low_wifi_init(void)
{
    int32_t rev = 100;
    if(low_wifi_initflag)
    {
        return 0;
    }
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_FLASH) );

    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    rev = esp_wifi_set_protocol(ESP_IF_WIFI_STA, (WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N));
    ESP_ERROR_CHECK( esp_wifi_start() );
    low_wifi_initflag = 1;
    printf("[low_wifi]init set_protocol rev:%d\n", rev);
	//esp_wifi_set_ps(WIFI_PS_MODEM);		//wifi不工作降低功耗
    return 0;
}

int32_t low_wifi_term(void)
{
    low_wifi_disconnect(); //不管当前的状态都断一次网
    esp_wifi_stop();
    return 0;
}

int32_t low_wifi_get_mac_addr(uint8_t mac[])
{
    return esp_wifi_get_mac(ESP_IF_WIFI_STA, mac);
}

void low_wifi_bssid_show(uint8_t* bssid)
{
    int32_t i;
    for(i = 0; i < 5; i++)
    {
        printf("%x:", bssid[i]);
    }
    printf("%x\n", bssid[i]);
}

int32_t low_wifi_scan(wifi_scan_cb cb, void* userdata)
{
    int32_t i;
    uint16_t len, ap_num = 0;
    scan_result_t result;
    wifi_ap_record_t *ap_records;

	/* IDF给出disconnect事件，其内部仍然处于连接中，这时候调用scan会得到 
	    invalid scan when connecting错误，所以这里调用disconnect，确保wifi已经断开
	*/
	if (ap_stat == AP_STAT_APP_DISCONNECT)
	{
		esp_wifi_disconnect();
	}
	
    printf("[low_wifi]esp_wifi_scan_start\n");
    esp_wifi_scan_start(NULL, true);
    esp_wifi_scan_stop();
    esp_wifi_scan_get_ap_num(&ap_num);
    if(ap_num == 0)
    {
        printf("[low_wifi]scan find none ssid\n");
        return 0;
    }
    len = ap_num * sizeof(wifi_ap_record_t);
    printf("[low_wifi]scan ap num:%d need ram:%d\n", ap_num, len);
    ap_records = os_mem_alloc(len);
    if(ap_records == NULL)
    {
        printf("[low_wifi]scan alloc ap_record buf NULL\n");
        while(1);
    }

    esp_wifi_scan_get_ap_records(&ap_num, ap_records);
    for(i = 0; i < ap_num; i++) {
        memcpy(&result.ssid[0], ap_records[i].ssid, LOW_WIFI_SSID_LEN);
        sprintf(result.bssid, "%s", low_wifi_str_of_mac(ap_records[i].bssid));
        result.rssi = ap_records[i].rssi;
        result.authmode = ap_records[i].authmode;
        result.pairwise_cipher = ap_records[i].pairwise_cipher;
        result.group_cipher = ap_records[i].group_cipher;
        printf("[low_wifi]scan ssid:%s, rssi:%d\n", ap_records[i].ssid, ap_records[i].rssi);
        printf("[low_wifi]scan bssid:");
        if(cb)
            cb(&result, userdata);
    }
    os_mem_free(ap_records);
    return 0;
}

void low_wifi_cfg_reset(low_wifi_cfg_t *cfg)
{
    cfg->probe = NULL;
}

typedef struct ssid_bssid_stren_struct
{
    char ssid[LOW_WIFI_SSID_LEN];
    char bssid[LOW_WIFI_BSSID_LEN];
    int8_t rssi; 
}ssid_bssid_stren_t;

/*
 * 用于查找一个ssid信号最强的信号的bssid时的回调
 */
int32_t ssid_highest_bssid_cb(scan_result_t* result, void* parm)
{
    ssid_bssid_stren_t *highest = parm;
    if(strcmp((char*)result->ssid, highest->ssid) == 0)
    {
        if(result->rssi > highest->rssi)
        {
            memcpy(highest->bssid, result->bssid, 6);
            highest->rssi = result->rssi;
            printf("[low_wifi]hiest_bssid_cb higher:%d %s\n",
                    highest->rssi, result->bssid);
        }
        else
        {
            printf("[low_wifi]lower %d %s\n", result->rssi, result->bssid);
        }
    }
    return 0;
}

/*
 * 获取环境中这个ssid强度最强的一个网络的bssid
 * retval:
 *  0:找到了对应的bssid
 * -1:没有找到bssid,这应该是一个隐藏网络
 */
#define RSSI_LOWEST -120
int32_t low_wifi_ssid_highest_bssid(char* ssid, uint8_t* bssid)
{
    ssid_bssid_stren_t stren;
    memset(&stren, 0, sizeof(ssid_bssid_stren_t));
    sprintf(stren.ssid, "%s", ssid);
    stren.rssi = RSSI_LOWEST;
    low_wifi_scan(ssid_highest_bssid_cb, &stren); 
    if(stren.rssi != RSSI_LOWEST)
    {
        memcpy(bssid, stren.bssid, 6); 
        printf("[low_wifi]ssid_highest_bssid ssid:%s rssi:%d ", ssid, stren.rssi);
        low_wifi_bssid_show(bssid);
        return 0;
    }
    return -1;
}

int32_t low_wifi_connect(low_wifi_cfg_t* cfg)
{
    int32_t rev = 0;
    if(cfg == NULL){
        return -1;
    }    
    wifi_config_t wifi_config ;
    bzero(&wifi_config, sizeof(wifi_config));
	low_wifi_err_code_set(WIFI_REASON_NONE);
    memcpy(wifi_config.sta.ssid, cfg->ssid, strlen(cfg->ssid));
    memcpy(wifi_config.sta.password, cfg->key, strlen(cfg->key));
    if(strlen(cfg->bssid))
    { //如果设置了bssid这里使用bssid
        low_wifi_mac_str_to_int(cfg->bssid, wifi_config.sta.bssid); 
        wifi_config.sta.bssid_set = 1;
    }
#if 0 //现在的版本应用层会传来信号强度最强的
    rev = low_wifi_ssid_highest_bssid(cfg->ssid, wifi_config.sta.bssid);
    if(rev == 0)
    {
        wifi_config.sta.bssid_set = 1;
    }
#endif

    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    connect_retry = 0;
	sta_stat = STAT_CONNECTING;
    rev = esp_wifi_connect();
    printf("[low_wifi]connect esp_wifi_connect rev:%d\n", rev);
    low_wifi_timer_start_connect();
    while(sta_stat == STAT_CONNECTING)
    {
        ets_printf("."); 
        vTaskDelay(100);
    }
	if(sta_stat == STAT_DISCONNECTED)
	{
    	return -1;
	}
	tcpip_adapter_dns_info_t dns_server;
	tcpip_adapter_get_dns_info(TCPIP_ADAPTER_IF_STA, ESP_NETIF_DNS_MAIN, &dns_server);
	printf("[low_wifi]dns1 = %s\n", inet_ntoa(dns_server.ip.u_addr.ip4.addr));
	tcpip_adapter_get_dns_info(TCPIP_ADAPTER_IF_STA, ESP_NETIF_DNS_BACKUP, &dns_server);
	printf("[low_wifi]dns2 = %s\n", inet_ntoa(dns_server.ip.u_addr.ip4.addr));
	return 0;
}

int32_t low_wifi_disconnect(void)
{
    connect_retry = CONNECT_RETRY_CNT; //不允许重连
    printf("[low_wifi]disconnect sta_stat:%d\n", sta_stat);
    switch(sta_stat)
    {
        case STAT_IDLE:
            break;
        case STAT_CONNECTING:
            printf("[low_wifi]wifi_disconnect when is connecting\n");
            while(sta_stat == STAT_CONNECTING) //正在连网，等连网结束
            {
                ets_printf(".");
                os_thread_sleep(50);
            }
            if(sta_stat == STAT_DISCONNECTED) //如果连网失败，这里什么也不用做
            {
                break;
            }
            //连网成功了，和STAT_CONNECTED同样处理
        case STAT_CONNECTED:
			connect_retry = CONNECT_RETRY_CNT;
            esp_wifi_disconnect();
            sta_stat = STAT_DISCONNECTED; //网络断开后把状态设下
            break;
        case STAT_DISCONNECTED: //当前网络是断开的，什么也不用做
            break;
        default:
            printf("[low_wifi]wifi_disconnect err stat:%d\n", sta_stat);
            return 0;
    }
    return 0;
}

int32_t low_wifi_stat_chk_connect(void)
{
    return (sta_stat == STAT_CONNECTED ? 0 : -1);
}

#define WIFI_QUALITY_THREAD  -60

int32_t low_wifi_stat_chk_quality(void)
{
    int32_t rev;
    wifi_ap_record_t info;
    rev = esp_wifi_sta_get_ap_info(&info);
    if(rev)
    {
        printf("[low_wifi]stat_chk_quality get ap info failed\n");
        return -1;
    }
    printf("[low_wifi]stat_chk_quality rssi:%d\n", info.rssi);
    if(info.rssi >= WIFI_QUALITY_THREAD)
    {
        return 0;
    }
    return -1;
}

int32_t low_wifi_cur_rssi_get(void)
{
    int32_t rev;
    wifi_ap_record_t info;
    rev = esp_wifi_sta_get_ap_info(&info);
    if(rev)
    {
        printf("[low_wifi]cur_rssi get failed\n");
        return -1;
    }
    return info.rssi;
}

int32_t low_wifi_cur_ssid_get(char* ssid_name)
{
    int32_t rev;
    wifi_ap_record_t info;
    rev = esp_wifi_sta_get_ap_info(&info);
    if(ssid_name == NULL){
        return -1;
    }
    if(rev)
    {
        printf("[low_wifi]cur_ssid_get failed\n");
        return -1;
    }
    sprintf(ssid_name, "%s", info.ssid);
    return 0;
}

int32_t low_wifi_cur_bssid_get(uint8_t mac[])
{
    int32_t rev;
    wifi_ap_record_t info;
    rev = esp_wifi_sta_get_ap_info(&info);
    if(rev)
    {
        printf("[low_wifi]cur_bssid_get failed\n");
        return -1;
    }
    memcpy(mac, info.bssid, 6);
    return 0;
}

int32_t low_wifi_err_code_get(void)
{
    return low_wifi_err_code;
}

int32_t low_wifi_get_ip(char ip[])
{
    if(sta_stat != STAT_CONNECTED)
    {
        return -1;
    }
    sprintf(ip, "%s", inet_ntoa(low_wifi_ip));
    return 0;
}

int32_t low_wifi_mac_str_to_int(char *mac_str, uint8_t mac[])
{
    int32_t rev, i, m[6];
    rev = sscanf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x",
            &m[0], &m[1], &m[2], &m[3], &m[4], &m[5]);
    if(rev == 6)
    {
        for(i = 0; i < 6; i++)
        {
            mac[i] = m[i];
        }
        return 0;
    }
    return -1;
}

char *low_wifi_str_of_mac(uint8_t mac[])
{
    static char mac_str[20];
    sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1],mac[2], mac[3], mac[4], mac[5]);
    return mac_str;
}

char *low_wifi_str_of_authmode(int32_t authmode)
{
    switch (authmode) {
    case WIFI_AUTH_OPEN:
        return "OPEN";
    case WIFI_AUTH_WEP:
        return "WEP";
    case WIFI_AUTH_WPA_PSK:
        return "WPA_PSK";
    case WIFI_AUTH_WPA2_PSK:
        return "WPA2_PSK";
    case WIFI_AUTH_WPA_WPA2_PSK:
        return "WPA_WPA2_PSK";
    case WIFI_AUTH_WPA2_ENTERPRISE:
        return  "WPA2_ENTERPRISE";
    case WIFI_AUTH_WPA3_PSK:
        return "WPA3_PSK";
    case WIFI_AUTH_WPA2_WPA3_PSK:
        return "WPA2_WPA3_PSK";
    default:
        return "UNKNOWN";
    }
}

char *low_wifi_str_of_cipher(int32_t cipher)
{
    switch(cipher)
    {
    case WIFI_CIPHER_TYPE_NONE:
        return "NONE";
    case WIFI_CIPHER_TYPE_WEP40:
        return "WEP40";
    case WIFI_CIPHER_TYPE_WEP104:
        return "WEP104";
    case WIFI_CIPHER_TYPE_TKIP:
        return "TKIP";
    case WIFI_CIPHER_TYPE_CCMP:
        return "CCMP";
    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        return "TKIP_CCMP";
    default:
        return "UNKNOWN";
    }
}





