#pragma once

#include <stdio.h>
#include <string.h>
#include "driver/adc.h"
#include "esp_adc/adc_continuous.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"

#define GET_UNIT(x)                 ((x>>3) & 0x1)
#define ADC_GET_CHANNEL(p_data)     ((p_data)->type2.channel)
#define ADC_GET_DATA(p_data)        ((p_data)->type2.data)

#define NUM_OF_ADC1_CHANNEL         2
#define ADC1_CHANNEL_L              ADC1_CHANNEL_1       // GPIO1
#define ADC1_CHANNEL_T              ADC1_CHANNEL_0       // GPIO0

#define ADC_UNIT                    ADC_UNIT_1    // ADC1  (esp32 c3 chỉ có ADC1)
#define ADC_CONV_MODE               ADC_CONV_SINGLE_UNIT_1
#define ADC_OUTPUT_TYPE             ADC_DIGI_OUTPUT_FORMAT_TYPE2

#define SAMPLE_FREQ_KHZ             20   // read 50Hz electric. 200 sample/1cycle/2channel.   50*200*2 = 20KHz
#define ADC_RESULT_BYTE_SIZE        SOC_ADC_DIGI_RESULT_BYTES // 4byte
#define ADC_FRAME_SIZE              (ADC_RESULT_BYTE_SIZE * NUM_OF_ADC1_CHANNEL)
#define SIZE_OF_ADC_BUFF            (5*ADC_FRAME_SIZE) // 5 sample


#define ADC1_BIT_MASK       (BIT(0) | BIT(1))    

#define SETMAX(Val_SET, Num_Max) 	Val_SET = (Val_SET > Num_Max) ? Num_Max : Val_SET   
#define SETMIN(Val_SET, Num_Min) 	Val_SET = (Val_SET < Num_Min) ? Num_Min : Val_SET

extern int16_t temp_val;
extern int16_t light_val;

void rd_adc_init(void);
void read_temp_light(void);