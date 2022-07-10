
#include "sdmmc_cmd.h"
#include "driver/sdmmc_defs.h"
#include "driver/sdmmc_host.h"
#include "plat_os.h"
#include "low_sdcard.h"

#define SD_PIN_DETECT   36

int low_sd_status(void)
{
    return 0;
}

static sdmmc_card_t *s_card = NULL;

void low_sd_card_info_show(sdmmc_card_t *card)
{
    int32_t print_scr, print_csd;
    const char *type;
    print_scr = print_csd = 0;
    printf("Name:%s\n", card->cid.name);
    if(card->is_sdio)
    {
        type = "SDIO";
        print_scr = 1;
        print_csd = 1;
    }
    else if(card->is_mmc)
    {
        type = "MMC";
        print_csd = 1;
    }
    else
    {
        //print_csd = 1;
        type = (card->ocr & SD_OCR_SDHC_CAP) ? "SDHC/SDXC" : "SDSC";
    }
    printf("Type:%s\n", type);
    if(card->max_freq_khz < 1000)
    {
        printf("Speed: %dkHz\n", card->max_freq_khz);
    }
    else
    {
        printf("Speed %dMHz %s\n", card->max_freq_khz / 1000,
                card->is_ddr ? ", DDR" : "");
    }
    printf("Size: %lluMB\n", ((uint64_t) card->csd.capacity) * card->csd.sector_size / (1024 * 1024));
    if (print_csd)
    {
        printf("CSD: ver=%d, sector_size=%d, capacity=%d read_bl_len=%d\n",
                card->csd.csd_ver,
                card->csd.sector_size, card->csd.capacity, card->csd.read_block_len);
    }
    if (print_scr)
    {
        printf("SCR: sd_spec=%d, bus_width=%d\n", card->scr.sd_spec, card->scr.bus_width);
    }
}

int low_sd_init(void)
{
    low_sd_detect_pin_init();
    if (s_card) {
        printf("[low_sd]init already done reinit now!\n");
        sdmmc_host_deinit();
        free(s_card);
        s_card = NULL;
    }
    esp_err_t err = sdmmc_host_init();
    if (err != ESP_OK) {
        printf("[low_sd]init host_init failed:0x%x\n", err);
        return -1;
    }

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
#if 0
    host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;
    host.flags &= ~SDMMC_HOST_FLAG_DDR;
#endif
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 1;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    err = sdmmc_host_init_slot(SDMMC_HOST_SLOT_1, &slot_config);
    if (err != ESP_OK) {
        printf("[low_sd]init slot_init failed:0x%x\n", err);
        return -1;
    }
    s_card = (sdmmc_card_t*) malloc(sizeof(sdmmc_card_t));
    if (!s_card) {
        printf("[low_sd]init alloc_card_t NULL\n");
        sdmmc_host_deinit();
        return -1;
    }

    err = sdmmc_card_init(&host, s_card);
    if (err != ESP_OK) {
        printf("[low_sd]init card_init failed:0x%x\n", err);
        sdmmc_host_deinit();
        free(s_card);
        s_card = NULL;
        return -1;
    }
    printf("[low_sd]init ok\n");
    low_sd_card_info_show(s_card);
    return 0;
}

int32_t addr_is_psram(const void* addr)
{
#define PSRAM_ADDR_START    0x3f800000
#define PSRAM_ADDR_END      (PSRAM_ADDR_START + 0x400000)

    uint32_t val = (uint32_t)addr;
    if(val >= PSRAM_ADDR_START && val <= PSRAM_ADDR_END)
    {
        return 1;
    }
    return 0;
}

static int32_t read_flag = 0;
static int32_t write_flag = 0;

int low_sd_read_sectors(unsigned char* buff, unsigned long sector, unsigned int count)
{
#define READ_RETRY_CNT 3
    int32_t read_retry = 0;
    if (!s_card) {
        return -1;
    }
    if(read_flag || write_flag)
    {
        while(1)
        {
            printf("[low_sd]read when write:%d read:%d this is forbidden\n",
                    write_flag, read_flag);
        }
    }
    if(addr_is_psram(buff))
    {
        //printf("[low_sd]read buf is in psram:0x%x\n", (uint32_t)buff);
    }
sd_disk_read_loop:
    read_flag = 1;
    esp_err_t err = sdmmc_read_sectors(s_card, buff, sector, count);
    read_flag = 0;
    if (err != ESP_OK) {
        int32_t inserted = low_sd_is_inserted();
        printf("[low_sd]sdmmc_read_blocks failed (0x%x)\n", err);
        ++read_retry;
        if(read_retry < READ_RETRY_CNT && inserted == LOW_SDCARD_INSERTED)
        {
            printf("[low_sddisk]read sector:%ld cnt:%d retry:%d instered:%d\n",
                    sector, count, read_retry, inserted);
            os_thread_sleep(1000);
            goto sd_disk_read_loop;
        }
        //fs_sdcard_skip_set();
        return -1;
    }
    return 0;
}

int low_sd_write_sectors(void* buff, unsigned long sector, unsigned int count)
{
#define WRITE_RETRY_CNT 3
    int32_t write_retry;
    if (!s_card) {
        return -1;
    }
    if(write_flag || read_flag)
    {
        while(1)
        {
            printf("[low_sd]write when write:%d read:%d this is forbidden\n",
                    write_flag, read_flag);
        }
    }
    if(addr_is_psram(buff))
    {
        //printf("[low_sd]write buf is in psram:0x%x\n", (uint32_t)buff);
    }
    write_retry = 0;
sd_disk_write_loop:
    write_flag = 1;
    esp_err_t err = sdmmc_write_sectors(s_card, buff, sector, count);
    write_flag = 0;
    if (err != ESP_OK) 
    {
        int32_t inserted = low_sd_is_inserted();
        printf("[low_sd]sdmmc_write_blocks failed (0x%x)", err);
        ++write_retry;
        if(write_retry < WRITE_RETRY_CNT && inserted == LOW_SDCARD_INSERTED)
        {
            printf("[low_sd]write sector:%ld cnt:%d retry:%d inserted:%d\n",
                    sector, count, write_retry, inserted);
            os_thread_sleep(1000);
            goto sd_disk_write_loop;
        }
        //fs_sdcard_skip_set();
        return -1;
    }
    else if(write_retry)
    {
        printf("[low_sd]write ok after retry:%d\n", write_retry);
    }
    return 0;
}

int low_sd_ioctl(unsigned char cmd, void* buff)
{
    if (!s_card) {
        return -1;
    }
    switch(cmd) {
        case SD_DISK_CMD_CTRL_SYNC:
            return 0;
        case SD_DISK_CMD_GET_SECTOR_COUNT:
            printf("[low_sd]-------->get sector cnt:%d\n", s_card->csd.capacity);
            *((uint32_t*) buff) = s_card->csd.capacity;
            return 0;
        case SD_DISK_CMD_GET_SECTOR_SIZE:
            printf("[low_sd]-------->get blocksize:%d\n", s_card->csd.sector_size);
            *((uint32_t*) buff) = s_card->csd.sector_size;
            return 0;
        case SD_DISK_CMD_GET_BLOCK_SIZE:
            printf("[low_sd]-------->get blocksize:%d\n", 64 * 1024);
            *((uint32_t*) buff) = 64 * 1024; /* value is fixed in CSD v2 */
            return 0;
        case SD_DISK_CMD_CTRL_TRIM:
            return 0;
    }
    return -1;
}

int low_sd_disk_term(void)
{
    return 0;
}

void low_sd_power_pin_init(void)
{
    ;
}

void low_sd_power_ctrl(int32_t on)
{
    ;
}

void low_sd_detect_pin_init(void)
{
    gpio_config_t pin_config = 
    {
        .mode           = GPIO_MODE_INPUT,
        .pin_bit_mask   = (1Ull << SD_PIN_DETECT),
        .pull_up_en     = GPIO_PULLUP_DISABLE,
        .pull_down_en   = GPIO_PULLDOWN_DISABLE,
    };
    gpio_config(&pin_config);
}

int32_t low_sd_is_inserted(void)
{
    int32_t rev;
    rev = gpio_get_level(SD_PIN_DETECT);
    if(rev == 1)
    {
        return LOW_SDCARD_REMOVED;
    }
    else
    {
        return LOW_SDCARD_INSERTED;
    }
}

/*************test************/






