#pragma once

#include "../RD_Peripheral/rd_spi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define USR_WRPROT      (0x1D)
#define SOFT_RESET      (0x1C)
#define GAIN_CR         (0x1A)
#define REG_OT_FUNX     (0x18)
#define REG_WATT        (0x06)
#define REG_IRMS        (0x03)
#define REG_VRMS        (0x04)

typedef struct
{
    float U_hd;
    float I_hd;
    float P_hd;
    float Cos_Phi;
    uint8_t flag_stuck;
} data_bl0942_t;

extern data_bl0942_t data_bl0942;

void rd_bl0942_init(void);
void read_UIP(void);