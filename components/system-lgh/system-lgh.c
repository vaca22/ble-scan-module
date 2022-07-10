
#include <esp_random.h>
#include "system-lgh.h"
int64_t lc_low_system_timestamp_get(void){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL + (tv.tv_usec / 1000LL));
}



uint32_t lc_low_system_random_number(void){
    return esp_random();
}