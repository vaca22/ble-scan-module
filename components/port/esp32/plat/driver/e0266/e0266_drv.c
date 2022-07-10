
#include <stdlib.h>
#include "driver/gpio.h"
#include "spi_bus.h"
#include "scr_interface_driver.h"
#include "epaper.h"
#include "board.h"
#include "esp_log.h"

#define TAG "EPAPER"

typedef struct {
    epaper_cfg_t            cfg;
    scr_interface_driver_t* drv_iface;
    spi_bus_handle_t        spi_bus;
    bool                    inited;
} my_epaper_t;

static my_epaper_t my_paper;

#define EPAPER_WRITE_CMD(cmd)      my_paper.drv_iface->write_cmd(my_paper.drv_iface, (cmd))
#define EPAPER_WRITE_DATA(data)    my_paper.drv_iface->write_data(my_paper.drv_iface, (data))

#define E0266_COMMAND_DRIVER_CONTROL  (0x1)
#define E0266_COMMAND_DATA_MODE       (0x11)
#define E0266_COMMAND_SW_RESET        (0x12)
#define E0266_COMMAND_RAM_X_RANGE     (0x44)
#define E0266_COMMAND_RAM_Y_RANGE     (0x45)
#define E0266_COMMAND_BOADER_WAVE     (0x3c)
#define E0266_COMMAND_MASTER_ACTIVE   (0x20)
#define E0266_COMMAND_DISPLAY_UPDATE  (0x21)
#define E0266_COMMAND_DISPLAY_UPDATE2 (0x22)
#define E0266_COMMAND_DEEP_SLEEP      (0x10)
#define E0266_COMMAND_X_ADRESS_COUNTER     (0x4e)
#define E0266_COMMAND_Y_ADRESS_COUNTER     (0x4f)
#define E0266_COMMAND_WRITE_BLACK_WHITE_RAM   (0x24)
#define E0266_COMMAND_WRITE_RED_RAM           (0x26)

#define E0266_HEIGHT                         152
#define E0266_COLUMNS                        296

int my_epaper_reset() {
    if (my_paper.inited == 0) {
        return -1;
    }
    if (my_paper.cfg.reset_pin >= 0) {
        uint8_t reset_pin = my_paper.cfg.reset_pin;
        gpio_set_level(reset_pin, 1);
        msleep(200);   
        gpio_set_level(reset_pin, 0);                //module reset    
        msleep(2);
        gpio_set_level(reset_pin, 1);
        msleep(200);
    }
    return 0;
}

static void wait_busy_quit(void) {
    int timeout = 60;
    uint8_t busy_pin = my_paper.cfg.busy_pin;
	while (timeout-- > 0) {
		msleep(20);
        if (gpio_get_level(busy_pin) == 0) {
            return;
        }
	}   
	ESP_LOGE(TAG, "wait busy timeout");
}

static int init_spi_driver(epaper_cfg_t* cfg) {
    spi_config_t spi_cfg = {
        .miso_io_num = -1,
        .mosi_io_num = cfg->mosi_pin,
        .sclk_io_num = cfg->sclk_pin,
        .max_transfer_sz = cfg->width * cfg->height,
    };
    spi_bus_handle_t spi_bus = spi_bus_create(2, &spi_cfg);
    if (spi_bus == NULL) {
        ESP_LOGE(TAG, "Fail to create SPI");
        return -1;
    }
    my_paper.spi_bus = spi_bus;
    scr_interface_spi_config_t spi_lcd_cfg = {
        .spi_bus = spi_bus,
        .pin_num_cs = cfg->cs_pin,
        .pin_num_dc = cfg->dc_pin,
        .clk_freq = cfg->freq,
        .swap_data = false,
    };
    scr_interface_driver_t *iface_drv;
    int ret = scr_interface_create(SCREEN_IFACE_SPI, &spi_lcd_cfg, &iface_drv);
    if (ret != 0) {
        ESP_LOGE(TAG, "Fail to create scr interface");
        return -1;
    }
    my_paper.drv_iface = iface_drv;
    return 0;
}

void deinit_spi_drv() {
    if (my_paper.drv_iface) {
        scr_interface_delete(my_paper.drv_iface);
        my_paper.drv_iface = NULL;
    }
    if (my_paper.spi_bus) {
        spi_bus_delete(&my_paper.spi_bus);
        my_paper.spi_bus = NULL;
    }
}

int my_epaper_init(epaper_cfg_t* cfg) {
    int ret = 0;
    ret = init_spi_driver(cfg);
    if (ret != 0) {
        ESP_LOGE(TAG, "fail to init spi driver");
        return -1;
    }
    my_epaper_reset();
    EPAPER_WRITE_CMD(E0266_COMMAND_SW_RESET);//soft  reset
	wait_busy_quit();

    EPAPER_WRITE_CMD(E0266_COMMAND_DRIVER_CONTROL);		
    EPAPER_WRITE_DATA(0x27);
    EPAPER_WRITE_DATA(0x01);
    EPAPER_WRITE_DATA(0x00);

    EPAPER_WRITE_CMD(E0266_COMMAND_DATA_MODE);		// data enter mode
    EPAPER_WRITE_DATA(0x01);//y-- x++ 
            
    EPAPER_WRITE_CMD(E0266_COMMAND_RAM_X_RANGE);		// set RAM x address start/end, in page 36
    EPAPER_WRITE_DATA(0x00);		
    EPAPER_WRITE_DATA(0x12);          // RAM x address start at 104/8=13-1=12
            
    EPAPER_WRITE_CMD(E0266_COMMAND_RAM_Y_RANGE);		 
    EPAPER_WRITE_DATA(0x27);		 
    EPAPER_WRITE_DATA(0x01);		
    EPAPER_WRITE_DATA(0x00);		// RAM y address end at 00h;
    EPAPER_WRITE_DATA(0x00);	
            
    EPAPER_WRITE_CMD(E0266_COMMAND_BOADER_WAVE);		// board
    EPAPER_WRITE_DATA(0x05);		//GS1-->GS1
    //TODO need check S0 to S175???        
    EPAPER_WRITE_CMD(E0266_COMMAND_DISPLAY_UPDATE);		//  Display update control
    EPAPER_WRITE_DATA(0x00); //normal	
    EPAPER_WRITE_DATA(0x80);	        //S8-S167 for 1675 type panel   
	wait_busy_quit();
    my_paper.inited = true;
	return 0;
}

int my_epaper_draw(uint8_t* black, uint8_t* red) {
     if (my_paper.inited == 0) {
        return -1;
    }
    int mode = 0;
    while (mode < 2) {
        //prepare position
        EPAPER_WRITE_CMD(E0266_COMMAND_X_ADRESS_COUNTER);		// set RAM x address count to 0;
        EPAPER_WRITE_DATA(0x00);//5bits avail
        EPAPER_WRITE_CMD(E0266_COMMAND_Y_ADRESS_COUNTER);		// set RAM y address count to 296;	2D9
        EPAPER_WRITE_DATA(0x27); //7..0
        EPAPER_WRITE_DATA(0x01);// ....8

        uint8_t ram_sel = mode ? E0266_COMMAND_WRITE_RED_RAM : E0266_COMMAND_WRITE_BLACK_WHITE_RAM;

        EPAPER_WRITE_CMD(ram_sel); //address inc auto
        uint8_t* v   = black;
        uint8_t* end = black + (E0266_COLUMNS * E0266_HEIGHT >> 3);
        if (mode) {
            while (v < end) {
                EPAPER_WRITE_DATA(*(v++));
            }
        }
        else {
            while (v < end) {
                EPAPER_WRITE_DATA(*(v++));
            }
        }
        mode++;
    }
    EPAPER_WRITE_CMD(E0266_COMMAND_DISPLAY_UPDATE2);
    EPAPER_WRITE_DATA(0xF7);		//Load LUT from MCU(0x32), Display update
    EPAPER_WRITE_CMD(E0266_COMMAND_MASTER_ACTIVE);
    wait_busy_quit();
    return 0;
}

int my_epaper_clear() {
     if (my_paper.inited == 0) {
        return -1;
    }
    int row, col;
    EPAPER_WRITE_CMD(E0266_COMMAND_X_ADRESS_COUNTER);		// set RAM x address count to 0;
    EPAPER_WRITE_DATA(0x00);//5bits avail
    EPAPER_WRITE_CMD(E0266_COMMAND_Y_ADRESS_COUNTER);		// set RAM y address count to 296;	2D9
    EPAPER_WRITE_DATA(0x27); //7..0
    EPAPER_WRITE_DATA(0x01);// ....8
    EPAPER_WRITE_CMD(E0266_COMMAND_WRITE_BLACK_WHITE_RAM); //address inc auto

    for(col=0; col<296; col++) {
        for(row=0; row<19; row++) {
            EPAPER_WRITE_DATA(0xff);
        }
    }
    EPAPER_WRITE_CMD(E0266_COMMAND_X_ADRESS_COUNTER);		// set RAM x address count to 0;
    EPAPER_WRITE_DATA(0x00);
    EPAPER_WRITE_CMD(E0266_COMMAND_Y_ADRESS_COUNTER);		// set RAM y address count to 296;	2D9
    EPAPER_WRITE_DATA(0x27);
    EPAPER_WRITE_DATA(0x01);
    EPAPER_WRITE_CMD(E0266_COMMAND_WRITE_RED_RAM);
    for(col=0; col<296; col++) {
        for(row=0; row<19; row++) {
            EPAPER_WRITE_DATA(0xff);
        }
    }
    EPAPER_WRITE_CMD(E0266_COMMAND_DISPLAY_UPDATE2);
    EPAPER_WRITE_DATA(0xF7);		//Load LUT from MCU(0x32), Display update
    EPAPER_WRITE_CMD(E0266_COMMAND_MASTER_ACTIVE);
    wait_busy_quit();
    return 0;
}

int my_epaper_sleep() {
     if (my_paper.inited == 0) {
        return -1;
    }
    EPAPER_WRITE_CMD(E0266_COMMAND_DEEP_SLEEP);
  	EPAPER_WRITE_DATA(0x01);
    gpio_set_level(my_paper.cfg.cs_pin, 1);
    return 0;
}

int my_epaper_wakeup() {
    if (my_paper.inited == 0) {
        return -1;
    }
    my_epaper_reset();
    return 0;
}

int my_epaper_deinit() {
    if (my_paper.inited == 0) {
        return -1;
    }
    my_epaper_sleep();
    deinit_spi_drv();
    my_paper.inited = false;
    return 0;
}

epaper_impl_t my_epaper_impl = {
    .init = my_epaper_init,
    .deinit = my_epaper_deinit,
    .draw  = my_epaper_draw,
    .sleep  = my_epaper_sleep,
    .wakeup  = my_epaper_wakeup,
    .clear  = my_epaper_clear,
};
