/**
 * @file setting.c
 * setting file
 */

/*********************
 *      INCLUDES
 *********************/
#include "setting.h"

/*********************
 *      DEFINES
 *********************/
#define TAG                    "setting"
#define SETTTING_NVS_NAMESPACE "setting"
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static nvs_handle_t setting_nvs_handle;
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
esp_err_t setting_nvs_init() {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    return ret;
}
esp_err_t setting_nvs_save_str_value(const char *key, const char *value) {
    esp_err_t ret =
        nvs_open(SETTTING_NVS_NAMESPACE, NVS_READWRITE, &setting_nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(ret));
    } else {
        ret = nvs_set_str(setting_nvs_handle, key, value);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Error (%s) set nvs key: %s value: %s!\n", esp_err_to_name(ret), key, value);
        }
        ret = nvs_commit(setting_nvs_handle);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Error (%s) commit nvs!\n", esp_err_to_name(ret));
        }
        nvs_close(setting_nvs_handle);
    }

    return ret;
}
esp_err_t setting_nvs_read_str_value(const char *key, char *out_value,
                                     size_t *length) {
    esp_err_t ret =
        nvs_open(SETTTING_NVS_NAMESPACE, NVS_READONLY, &setting_nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(ret));
    } else {
        ret = nvs_get_str(setting_nvs_handle, key, out_value, length);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Error (%s) get nvs key: %s!\n", esp_err_to_name(ret), key);
        }
        nvs_close(setting_nvs_handle);
    }

    return ret;
}
esp_err_t setting_nvs_save_int_value(const char *key, int16_t value) {
    esp_err_t ret =
        nvs_open(SETTTING_NVS_NAMESPACE, NVS_READWRITE, &setting_nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(ret));
    } else {
        ret = nvs_set_i16(setting_nvs_handle, key, value);
        ret = nvs_commit(setting_nvs_handle);
        nvs_close(setting_nvs_handle);
    }

    return ret;
}
esp_err_t setting_nvs_read_int_value(const char *key, int16_t *out_value) {
    esp_err_t ret =
        nvs_open(SETTTING_NVS_NAMESPACE, NVS_READWRITE, &setting_nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(ret));
    } else {
        ret = nvs_get_i16(setting_nvs_handle, key, out_value);
        nvs_close(setting_nvs_handle);
    }

    return ret;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
