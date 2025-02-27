#include "rd_flash.h"

void rd_read_flash(void *buffer, uint32_t address, uint32_t length){
    esp_flash_read(NULL, buffer, address, length);
}

void rd_flash_write(void *buffer, uint32_t address, uint32_t length){
    esp_flash_erase_region(NULL, address, 4096);
    esp_flash_write(NULL, buffer, address, length);
}