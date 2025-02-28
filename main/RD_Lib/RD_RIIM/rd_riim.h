#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "stdint.h"
#include "esp_log.h"
#include "rd_model.h"
#include "../RD_Peripheral/rd_uart.h"

#define LENGMESS_RIIMDF         32
#define TEMPBUFF_LENGTH			64

#define POWER_TX_DF_H			0x00
#define POWER_TX_DF_L	        0x13
#define CHANNEL_DF				0x01


#define TEMPBUFF_LENGTH			64
#define LENGMESS_RIIMDF         32

#define MESSAGE_HEADLENGTH	    (5)
#define MESSAGE_MAXLENGTH		(59)
#define MESSAGE_OFFSET          (2)

#define HEADER_MESS_RSP					0x55AA

 
#define OPCODE_ROUTER_INFO      	0x0002
#define OPCODE_TSPH_MR				0x0003  
#define	OPCODE_SET_POWER_TRAN_MR	0x0007   
#define OPCODE_SET_NETKEY_MR		0x0009 
#define	OPCODE_KICK_OUT_MR			0x000A 
#define	OPCODE_PING_MR				0x000B
#define OPCODE_ADD_GROUP_MR			0x000C
#define	OPCODE_DELETE_GROUP_MR		0x000D
#define	OPCODE_DELETE_GROUP_G		0x000E
#define OPCODE_REBOOT_MR			0x000F
#define OPCODE_SET_DF_POWER			0x0010
#define OPCODE_SET_WARING 			0x0011
#define OPCODE_SET_SCENE			0x0012
#define	OPCODE_DELETE_SCENE			0x0013
#define OPCODE_EN_DIS_SCENE			0x0014
#define	OPCODE_DIM_ALL				0x0015 
#define OPCODE_DIM_ONE_MR			0x0016
#define OPCODE_DIM_GROUP			0x0017
#define OPCODE_SET_DIM_DF			0x0018
#define	OPCODE_SET_TIME_RSP			0x0019
#define	OPCODE_ASK_NETWORK_INF		0x1000
#define OPCODE_EN_DIS_SCENE_G		0x001A
#define OPCODE_EN_DIS_SCENE_ALL 	0x001B
#define OPCODE_SET_WARING_G			0x001D
#define OPCODE_SET_WARING_All		0x001C
#define OPCODE_SET_DF_POWER_G		0x001E
#define OPCODE_SET_DF_POWER_ALL		0x001F
#define OPCODE_SET_DIM_DF_G 		0x0020 
#define OPCODE_SET_DIM_DF_ALL		0x0021
#define OPCODE_DELETE_SCENE_G 		0x0022
#define OPCODE_DELETE_SCENE_ALL		0x0023
#define OPCODE_PING_DISCONECT 		0x1001
#define OPCODE_GET_MAC				0x1002

#define OPCODE_TEST_MODE			0xF000

#define	OPCODE_SET_POWER_TRAN		0x0009 
#define OPCODE_ASK_INFORMATION		0x000a
#define	OPCODE_ERRO_MESSANGER		0x000B
#define	OPCODE_GPS					0x0026 
#define OPCODE_RIIM_STT 			0x0010


#define LENG_OPCODE_LAMP_STT_RSP			5
#define LENG_OPCODE_TSPH_RSP				14      //17	
#define	LENG_OPCODE_GPS_RSP					11		// 2+4+4+1
#define LENG_OPCODE_SET_HCL_RSP 			4
#define LENG_OPCODE_DELETE_ID_HCL_RSP 		4
#define LENG_OPCODE_ENABLE_HCL_RSP 			4
#define LENG_OPCODE_SET_NETKEY_HCL_RSP 		3 
#define LENG_OPCODE_SET_POWER_TRAN_MR_RSP 	5
#define LENG_OPCODE_KICK_OUT_MR_RSP 		3
typedef struct IncomingData{
	uint8_t 	Hearder[2];
	uint8_t		Length;
	uint8_t		Opcode[2];
	uint8_t		Message[MESSAGE_MAXLENGTH];
} TS_GWIF_IncomingData;


typedef struct Riim_Data_t{
	uint8_t 	Mac[8];
	uint8_t		Version[2];
} Riim_Data_t;


