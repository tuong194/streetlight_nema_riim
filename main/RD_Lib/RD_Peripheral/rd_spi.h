#pragma once

#include <string.h>
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"


#define HSPI_HOST   SPI2_HOST
#define PIN_NUM_MISO 5
#define PIN_NUM_MOSI 6
#define PIN_NUM_CLK  4
//#define PIN_NUM_CS   7

#define FREQ_SPI        900    // FREQ_SPI*1000
#define SPI_BUF_MAX     4096 

int32_t spi_read_register_signed(uint8_t reg_addr);
uint32_t spi_read_register_unsigned(uint8_t reg_addr);
uint8_t rd_send_setup(uint8_t reg_addr,uint8_t *data_send);