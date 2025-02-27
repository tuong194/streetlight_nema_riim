#ifndef RD_FLASH_H
#define RD_FLASH_H

#include "esp_flash.h"
#include "esp_log.h"

#define  FLASH_ADD_CONFIG 0x310000
#define  FLASH_ADD_GROUP  0x311000
#define  FLASH_ADD_SCENE  0x312000

void rd_read_flash(void *buffer, uint32_t address, uint32_t length);
void rd_flash_write(void *buffer, uint32_t address, uint32_t length);

#endif /* RD_FLASH_H */
