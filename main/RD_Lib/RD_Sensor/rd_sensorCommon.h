#ifndef D2FBBE63_C376_43A9_B68E_6A4004F16373
#define D2FBBE63_C376_43A9_B68E_6A4004F16373

#include <stdint.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "time.h"  // rtc

#include "rd_GPS.h"
#include "../RD_RIIM/rd_model.h"
#include "../RD_Peripheral/rd_adc.h"
#include "../RD_Light/rd_lightCommon.h"
#include "BL0942.h"

#define CYCLE_CHECK_RTC_TASK_US    (500000)     // 500 ms check
#define CYCLE_CHECK_GPS_NEWTASK_US (20000000)    // 20 seconds when gps signal is invalid
#define CYCLE_CHECK_GPS_OLDTASK_US (300000000)  // 300 seconds when gps signal is valid
#define CYCLE_CHECK_TEMP_TASK_US   (30000000)   // 30s
#define CYCLE_CHECK_LIGHT_TASK_US  (30000000)   // 30s
#define CYCLE_CHECK_POW_TASK_US    (30000000)   // 30s
#define TIME_SCAN_ERROR_US         (CYCLE_CHECK_POW_TASK_US)

#define TEMP_OFFSET_ZERO		50					// set dải đo -50*C-->200*C

typedef struct
{
    uint8_t Voltage[2];		    // uint Vol
    uint8_t Ampe[2];			// Unit 0.01 A
    uint8_t Cos_UI;				// unit 0.01
    uint8_t Power[2];			// W
	uint8_t Power_Consum[2];	// W
    uint8_t Temp;				// *c
    uint8_t EnviLightness[2];
    uint8_t GPS_North[4];
    uint8_t GPS_East[4];
    uint8_t Error[2];
} sensor_of_light_t;

extern volatile sensor_of_light_t Sensor_Of_Light_Common;
extern struct tm RTC_Val;

void rd_sensor_init(void);

#endif /* D2FBBE63_C376_43A9_B68E_6A4004F16373 */
