//
// Created by vaca on 7/9/22.
//

#ifndef BLE_SCAN_MODULE_WIFI_LGH_H
#define BLE_SCAN_MODULE_WIFI_LGH_H

#include <string.h>

#define EXAMPLE_ESP_WIFI_SSID      "vaca"
#define EXAMPLE_ESP_WIFI_PASS      "22345678"
#define EXAMPLE_ESP_MAXIMUM_RETRY  -1

void wifi_init_sta();


void init_wifi_scan(void);
void lgh_wifi_scan_start();
#endif //BLE_SCAN_MODULE_WIFI_LGH_H
