#pragma once

#include "rd_lightCtr.h"
#include "rd_scene.h"
#include "../RD_RIIM/rd_riim.h"

#define FIRM_VERSION 0x010A  // V1.10

#define FLASH_SAVED_CODE			0x03/*  */

#define VOLTAGE_PROTECT 					1
#define CYCLE_CHECK_SCENE_TASK_US  			500000			// 0.5sec
#define CYCLE_CHECK_TSPH_TASK_US  			500000			// 0.5sec
#define CYCLE_CHECK_CTR_BY_ERR_TASK_US  	5000000			// 5sec
#define CYCLE_CHECK_CTRL_TASK_US			800			   // 0.8 msec

#define TIME_RESTART_AFTER_MESS_US		    10000000   //10s 
#define RELAY_STT_DF      	    	1
#define DIM_DF	                	100
#define VOLTAGE_TOOHIGH_DF      	2700           //270v
#define VOLTAGE_TOOLOW_DF       	1500
#define AMPE_TOOHIGH_DF  			150        	  // 1.5A
#define AMPE_TOOLOW_DF   			1
#define TEMP_TOOHIGH_DF 			130          	// 80*c
#define TEMP_TOOLOW_DF 				2        
#define LIGHT_TOOHIGH_DF 			20000      		// chua dung
#define LIGHT_TOOLOW_DF 			1
#define TYPE_WAR_ACTIVE_DF      	0
#define TIME_RECALL_ACTIVE_DF   	0
#define TIME_RECALL_WAR_DF      	120        
#define POWER_TX_DF 				(0x0013) 
#define GROUP_ID_DF 				(0x0000)   
#define NUM_IN_GROUP_DF				(0x00)
#define CHANNEL_DF					(0x01)
#define POWER_DF              		(0x0064)



typedef struct
{
	uint8_t Voltage_TooHigh[2];
	uint8_t	Voltage_TooLow[2];
	uint8_t	Ampe_TooHigh[2];
	uint8_t	Ampe_TooLow[2]; 
	uint8_t Temperature_TooHigh;
	uint8_t Temperature_TooLow;
	uint8_t Light_TooHigh[2];
	uint8_t Light_TooLow[2];
	uint8_t Power_Default[2];
    //uint8_t Type_Active;
	union                               // bit7-Relay_____bit6->0-Dim
	{
		uint8_t Type_Active_All; //;    khai bao thi se tang them 1 byte. gia trị == struct duoi
		struct 
		{
			uint8_t Temp_H   :1;  //bit0
			uint8_t Temp_L   :1;
			uint8_t Light_H  :1;
			uint8_t Light_L  :1;
			uint8_t Vol_H    :1;
			uint8_t Vol_L    :1;
			uint8_t Ampe_H   :1; 
			uint8_t Ampe_L   :1;  //bit 7
			
		} Type_Active_One; 
	} Type_Active;
    uint8_t Time_Recall_Active;
    uint8_t Time_Recall[2];

}	warning_config_t;

typedef struct
{
	uint8_t Relay_Stt;
	uint8_t Dim;
} start_config_t;

typedef struct
{
    uint8_t Netkey[16];
    uint8_t Connect_Stt;
    uint8_t Power_Tx[2];
	uint8_t Channel_RIIM;
    uint8_t	Cycle_Rsp[2];
    uint8_t Time_Stack;
    uint8_t Num_Stack_Rsp[2];
	uint8_t Group_ID[2];
	uint8_t NumInGroup;
}	network_info_t;

typedef struct
{
    uint8_t Saved;              // check Fist Flash 
    start_config_t          Start_Config;
    network_info_t          Network_Inf_Config;
    light_stt_t             Last_Light_Stt;
    warning_config_t      Warning_Config_Val;
} data_config_flash_t;  // data config save to flash

extern warning_config_t 		Warning_Config_Val_Common;
extern start_config_t			Start_Config_Val_Common;
extern network_info_t			Network_Inf_Common;

extern uint8_t Restart_flag_en;

void RD_LightCommon_callTask(void);
