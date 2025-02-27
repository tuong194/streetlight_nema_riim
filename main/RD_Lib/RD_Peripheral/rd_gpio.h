#pragma once

#include <stdio.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define ESP_INTR_FLAG_DEFAULT   0

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES           LEDC_TIMER_10_BIT // Set duty resolution to 13 bits
#define LEDC_FREQUENCY          (2000) // Frequency in Hertz. Set frequency at 4 kHz
#define PWM_MODE                LEDC_LOW_SPEED_MODE


#define RELAY_PIN       GPIO_NUM_8
#define DIMER_PIN       GPIO_NUM_6 //GPIO_NUM_7
#define HB_LED_PIN      GPIO_NUM_2 //GPIO_NUM_6  //RD_NOTE leb báo chân 6 nha, dimer_pin chân 7
#define RST_RIIM_PIN    GPIO_NUM_10     

#define RELAY_CHANNEL   LEDC_CHANNEL_1
#define DIMER_CHANNEL   LEDC_CHANNEL_0

void rd_set_duty(ledc_channel_t LEDC_CHANNEL_RD, uint32_t duty);