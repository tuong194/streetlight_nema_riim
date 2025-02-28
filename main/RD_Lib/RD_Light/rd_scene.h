#ifndef RD_SCENE_H
#define RD_SCENE_H

#include "esp_log.h"
#include "time.h"  // rtc
#include "../RD_Sensor/rd_sensorCommon.h"
#include "../RD_Peripheral/rd_flash.h"
#include "../RD_Light/rd_lightCtr.h"

#define GROUP_MAX 				    2
#define SCENE_MAX             	    20
#define SIZE_OF_STRUC_DATA_SCENE	sizeof(Struct_Data_Scene)


typedef struct
{	
	uint8_t Enable_ID;
    uint16_t Scene_ID;
    uint32_t Start_Date;
    uint32_t Stop_Date;
    uint8_t Month;
    uint8_t Date;
    uint8_t Week_Day;
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Relay_Stt;
    uint8_t Dim_per; 
} Struct_Data_Scene;

typedef struct
{
    uint8_t Saved;
    uint8_t Enable_Function;  //   check time Scene active nearest
    uint8_t Amount_Scene;  // number of scene saved
    uint8_t Futute;
    Struct_Data_Scene Scene_Data[SCENE_MAX];
} Struct_Scene_Data; 

typedef struct
{
	uint8_t Uploaded;
	uint32_t Date;   				// Start 01/01/2000
	uint8_t Month;
	uint8_t Date_in_Month;
	uint8_t Week_Day;
	uint8_t Hour;
	uint8_t Minute; 
} Struct_Time_Scene;

extern Struct_Scene_Data          	Scene_Data_Common;			// Data luu Scene
extern Struct_Time_Scene			Time_Scene_Common;	        // thoi gian thuc de cheack scene

void RD_Init_FlashSceneData(void);
void RD_Add_Scene(uint8_t Enable_Stt, uint16_t Scene_ID, uint32_t Start_date_in, uint32_t Stop_date_in , uint8_t Month_in, uint8_t Date_in,  uint8_t Week_Day_in, uint8_t Hour_in, uint8_t Minute_in, uint8_t Relay_Stt_in,uint8_t Dim_per_in);
void RD_SceneScan(void);

#endif /* RD_SCENE_H */
