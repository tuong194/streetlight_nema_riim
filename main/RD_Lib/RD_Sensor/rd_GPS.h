#ifndef EF46E927_CAD9_4DE9_8FF1_866481AA73CE
#define EF46E927_CAD9_4DE9_8FF1_866481AA73CE

#include "../RD_Peripheral/rd_uart.h"
#include "rd_sensorCommon.h"

#define  GPS_RX_GET_MAX 	1024
#define  CHAR_GET_MAX  		100

typedef struct 
{
	uint8_t  New_Data_Ready;
	char str_Para_GPS[CHAR_GET_MAX];
	char Type_Para_GPS[8];

	uint8_t North_GPS[4];							// 20.998811.  byte 0->4 14/04/3d/9b
	uint8_t East_GPS[4];

    uint8_t  Hour;
    uint8_t  Minute;
    uint8_t  Second;

    uint8_t  Year;
    uint8_t  Month;
    uint8_t  Date;

    uint8_t  Signal_Stt;
} GPS_Para;

extern volatile uint8_t Time_RTC_Set_Flag;
extern GPS_Para  GPS_Value;

void RD_GPS_Check(void);

#endif /* EF46E927_CAD9_4DE9_8FF1_866481AA73CE */
