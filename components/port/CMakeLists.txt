
set(COMPONENT_SRCS
    "esp32/src/low_flash.c"
    "esp32/src/low_fs.c"
    "esp32/src/low_lcd.c"
    "esp32/src/low_play.c"
    "esp32/src/low_rec.c"
    "esp32/src/low_sdcard.c"
    "esp32/src/low_system.c"
    "esp32/src/low_ts.c"
    # "esp32/src/low_ble.c"
    "esp32/src/low_wifi.c"
    "esp32/src/plat_os.c"
    # "esp32/src/low_wifi_http.c"
    "esp32/plat/oled1306.c"
    "esp32/plat/eink_1680.c"
    "esp32/plat/driver/audio_hal.c"
    "esp32/plat/driver/board_def.c"
    "esp32/plat/driver/i2c_bus.c"
    "esp32/plat/driver/low_drv.c"
    "esp32/plat/driver/panel.c"
    "esp32/plat/driver/es8311/es8311.c"
    "esp32/plat/driver/chs6x/chsc6x_drv.c"
    )

set(COMPONENT_ADD_INCLUDEDIRS
        ./esp32/inc
        ./esp32/plat
        ./esp32/plat/include 
        ./esp32/plat/driver
        ./esp32/plat/driver/es8311
        ./esp32/plat/driver/chs6x
        #./esp32/plat/driver/e0266
    )

# set(COMPONENT_REQUIRES utils spi_flash nvs_flash esp_adc_cal sdmmc ff)

set(COMPONENT_REQUIRES spi_flash nvs_flash esp_adc_cal sdmmc ff collections_c bt)

register_component()

