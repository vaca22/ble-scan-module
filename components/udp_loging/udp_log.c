//
// Created by vaca on 7/10/22.
//
#include "esp_log.h"
#include "udp_log.h"
#include <string.h>
#include <lwip/sockets.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"


int udp_log_fd;
static struct sockaddr_in serveraddr;
static uint8_t buf[500];


int udp_logging_vprintf( const char *str, va_list l ) {
    int len;
    char task_name[16];
    char *cur_task = pcTaskGetTaskName(xTaskGetCurrentTaskHandle());
    strncpy(task_name, cur_task, 16);
    task_name[15] = 0;
    if (strncmp(task_name, "tiT", 16) != 0)
    {
        len = vsprintf((char*)buf, str, l);
        sendto(udp_log_fd, buf, len, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) ;
    }
    return vprintf( str, l );
}

int udp_logging_init(const char *ipaddr, unsigned long port) {
    struct timeval send_timeout = {1,0};
    udp_log_fd = 0;
    ESP_LOGI("UDP_LOGGING", "initializing udp logging...");
    if( (udp_log_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        ESP_LOGE("UDP_LOGGING", "Cannot open socket!");
        return -1;
    }

    uint32_t ip_addr_bytes;
    inet_aton(ipaddr, &ip_addr_bytes);
    ESP_LOGI("UDP_LOGGING", "Logging to 0x%x", ip_addr_bytes);

    memset( &serveraddr, 0, sizeof(serveraddr) );
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( port );
    serveraddr.sin_addr.s_addr = ip_addr_bytes;

    int err = setsockopt(udp_log_fd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&send_timeout, sizeof(send_timeout));
    if (err < 0) {
        ESP_LOGE("UDP_LOGGING", "Failed to set SO_SNDTIMEO. Error %d", err);
    }

    esp_log_set_vprintf(udp_logging_vprintf);

    return 0;
}

void init_udp_logging(){
    udp_logging_init(REMOTE_ADDR,REMOTE_PORT);
}