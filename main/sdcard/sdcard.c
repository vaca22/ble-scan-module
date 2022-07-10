/**
 * @file sdcard.c
 * sdcard file
 */

/*********************
 *      INCLUDES
 *********************/
#include "sdcard.h"

/*********************
 *      DEFINES
 *********************/
#define TAG         "file"
#define MOUNT_POINT "/sdcard"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static sdmmc_card_t *sdcard;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
esp_err_t sd_card_init() {
    esp_err_t ret;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files              = 5,
        .allocation_unit_size   = 16 * 1024};

    const char mount_point[] = MOUNT_POINT;

    sdmmc_host_t host               = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    slot_config.width = 1;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    ESP_LOGI(TAG, "SDIO初始化成功");

    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config,
                                  &mount_config, &sdcard);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "初始化失败");

        return;
    }
    ESP_LOGI(TAG, "文件系统挂在成功");

    sdmmc_card_print_info(stdout, sdcard);
}

esp_err_t sd_card_test() {
    const char *file_hello = MOUNT_POINT "/hello.txt";

    ESP_LOGI(TAG, "打开文件 %s", file_hello);
    FILE *file = fopen(file_hello, "w");
    if (file == NULL) {
        ESP_LOGE(TAG, "创建文件失败");
        return;
    }
    fprintf(file, "Helloword!\n");
    fclose(file);

    file = fopen(file_hello, "r");
    if (file == NULL) {
        ESP_LOGE(TAG, "打开失败");
        return;
    }

    char line[64];
    fgets(line, sizeof(line), file);
    fclose(file);

    char *pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "读出内容: '%s'", line);
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
