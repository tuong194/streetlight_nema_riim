#include "rd_flash.h"

static const  char *FLASH_TAG = "FLASH_TAG";

void rd_read_flash(void *buffer, uint32_t address, uint32_t length){
    ESP_LOGI(FLASH_TAG,"Read flash at 0x%03x  \n", (unsigned int)address);
    ESP_ERROR_CHECK(esp_flash_read(NULL, buffer, address, length));
}

void rd_flash_write(void *buffer, uint32_t address, uint32_t length){
    ESP_LOGI(FLASH_TAG,"Write flash at 0x%03x  \n", (unsigned int)address);
    ESP_ERROR_CHECK(esp_flash_erase_region(NULL, address, 4096));
    ESP_ERROR_CHECK(esp_flash_write(NULL, buffer, address, length));
}