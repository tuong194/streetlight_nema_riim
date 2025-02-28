#pragma once

#include "../RD_Peripheral/rd_gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"


#define DIM_MIN_BEFORE_RELAY_OFF    10  //10% dim 
#define TSS_DEFAULT					50
<<<<<<< HEAD
#define RELAY_POWER_UP_DF			1
=======
#define RELAY_POWER_UP_DF			1  // RD_ON
>>>>>>> master
#define DIM_POWER_UP_DF				100 //100% power
//#define CYCLE_TOGGLE_RELAY_OFF		1250 //1250us ==>800Hz
#define CYCLE_TOGGLE_RELAY_OFF		700 //700us
typedef enum {
	RD_OFF = 0,
	RD_ON = 1,
} RD_onoff_stt_t;

typedef struct
{
	int8_t Dim_target;
	int8_t Dim_current;
	uint8_t Relay_target;
	uint8_t Relay_current;
	uint16_t Scene_ID_Active;
	uint8_t Cancel_Scene;
	union                               // bit7-Relay_____bit6->0-Dim
	{
		uint8_t Error_All; //;    khai bao thi se tang them 1 byte. gia trị == struct duoi
		struct
		{
			uint8_t Temp_H : 1;  //bit0
			uint8_t Temp_L : 1;
			uint8_t Light_H : 1;
			uint8_t Light_L : 1;
			uint8_t Vol_H : 1;
			uint8_t Vol_L : 1;
			uint8_t Ampe_H : 1;
			uint8_t Ampe_L : 1;  //bit 7

		} Error_One;
	};
	uint8_t Safety;
	uint16_t Time_To_Safe;	  // time count down from time cancel Error (Unit:s)
} light_stt_t;

extern volatile light_stt_t light_stt_val;

void light_set_safety(uint8_t safety_set);
void light_set_ctr(uint8_t DimSet);
void light_ctr_update(void) ;
