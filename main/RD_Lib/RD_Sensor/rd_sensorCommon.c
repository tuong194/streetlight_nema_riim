#include "rd_sensorCommon.h"

static const char *SENSOR_TAG = "SENSOR: ";
volatile sensor_of_light_t Sensor_Of_Light_Common;

struct tm RTC_Val;
time_t time_now;
int64_t RTC_offset = 0;

uint8_t rd_exceed_us(int64_t ref, int64_t span_ms)
{
    return ((esp_timer_get_time() - ref) >= span_ms);
}

static void rd_set_GMT_7(void)
{
    //RD_NOTE: set GMT +7 time
    setenv("TZ", "CST-7", 1);
    tzset();
}

inline time_t updateRTCTime(void) // RD_NOTE: trả về số giây trôi qua kể từ 1/1/1970
{
    return time(NULL);
}
/*RD_NOTE: struct tm
esp hỗ trợ bộ đếm rtc(time_t) bắt đầu từ 1/1/1970, nhưng struct tm: tm_year lưu số năm từ 1900, nếu tm_year = 125 thì năm thực tế là 2025
dựa vào đây để tính được khoảng thời gian lệch từ 1/1/1970 so với thời điểm hiện tại
*/
static time_t rd_get_RTC_Offset(uint8_t Seconds, uint8_t Minutes, uint8_t Hours, uint8_t Date, uint8_t Month, uint8_t year) // update
{
    struct tm settimestart;
    time_t time_offset;

    settimestart.tm_year = (year + 2000) - 1900;
    settimestart.tm_mon = Month - 1;
    settimestart.tm_mday = Date;
    settimestart.tm_hour = Hours;
    settimestart.tm_min = Minutes;
    settimestart.tm_sec = Seconds;

    time_offset = mktime(&settimestart) - time(NULL);

    return time_offset;
}

static void rd_RTC_Val_Upload(void)
{
    if (1 == Time_RTC_Set_Flag) // RD_NODE nếu lấy được tín hiệu từ vệ tinh thì update RTC_Val
    {
        time_now = RTC_offset + updateRTCTime();
        localtime_r(&time_now, &RTC_Val);
    }
}

static void set_RTC_from_GPS(void)
{
    RTC_offset = rd_get_RTC_Offset(GPS_Value.Second, GPS_Value.Minute, GPS_Value.Hour, GPS_Value.Date, GPS_Value.Month, GPS_Value.Year);
    time_now = RTC_offset + updateRTCTime();

    localtime_r(&time_now, &RTC_Val); // Chuyển đổi thời gian

    printf("time now: %02d-%02d-%d %02d:%02d:%02d\n",
           RTC_Val.tm_mday, RTC_Val.tm_mon + 1, RTC_Val.tm_year + 1900,
           RTC_Val.tm_hour, RTC_Val.tm_min, RTC_Val.tm_sec);
}

static void rd_check_gps_process(void)
{

    uart_flush(GPS_UART_ID); // clear buffer

    size_t Leng_Ring = 0;
    uint16_t Timeout = 0;
    do
    {
        uart_get_buffered_data_len(GPS_UART_ID, (size_t *)&Leng_Ring); // RD_NOTE: this func need init uart_driver_install(), dùng để lấy data trong buffer
        Timeout++;
        vTaskDelay(10 / portTICK_PERIOD_MS);
        if (Timeout > 500)
        {
            ESP_LOGI(SENSOR_TAG, "waiting for GPS timeout, leng: %d", Leng_Ring);
            break;
        }
    } while (Leng_Ring <= 1000); // wait for received 1000 byte data

    if (Timeout < 500)
    {
        RD_GPS_Check(); // RD_NOTE read UART gps in this func
    }
}

static void rd_GPS_task(void)
{
    rd_check_gps_process();
    if (1 == Time_RTC_Set_Flag)
    {
        set_RTC_from_GPS();
    }

    static uint8_t Send_Location_Flag =0;
	if((GPS_Value.Signal_Stt == true ) && (Send_Location_Flag == 0))
	{
		Send_Location_Flag =1;
		ESP_LOGI(SENSOR_TAG,"Send Location\n");
		RD_Rsp_OPCODE_GPS();
	}
}

/*------------------------------------Light------------------------------------------*/

static uint16_t RD_Get_EnviLight(void)
{
    uint16_t Light_Buff = 0;
    const float Alpha_Light = 1.0;
    Light_Buff = light_val;
    Light_Buff = Light_Buff*Alpha_Light;
    return Light_Buff;
}

static void rd_Light_task(void)
{
    static uint16_t Lightness_Buff;
    Lightness_Buff = RD_Get_EnviLight();
    Sensor_Of_Light_Common.EnviLightness[0] = (uint8_t)((Lightness_Buff >> 8) & 0xff);
    Sensor_Of_Light_Common.EnviLightness[1] = (uint8_t)((Lightness_Buff) & 0xff);

    ESP_LOGI(SENSOR_TAG, "The Light updated: %d lux", Lightness_Buff);
}

/*-----------------------------------------------------------------------------------*/

/*-------------------------------------Temp------------------------------------------*/
static float RD_NTC_Convert(uint16_t ADC_Value)
{
	// t = a + b*ADC_Value
	float a = 87.26;
	float b = -0.01927;
	#if 0
	if(ADC_Value >= 3724)  // >25
	{
		a = 426.5;
		b =  -0.1078;
	}
	else if((ADC_Value >= 3291) && (ADC_Value < 3724))  // 25-50
	{
		a = 240;
		b = -0.0577;  
	}
	else if((ADC_Value >= 2670) && (ADC_Value < 3291))  // 50-75
	{
		a = 182.5;
		b = -0.0403;
	}
	else if((ADC_Value <2670))  // 100->110
	{
		a = 173;
		b = -0.0368;
	}
	#endif
	float Temp = a + ADC_Value*b;
	Temp = (Temp < 5) 	? 5:Temp;
	Temp = (Temp > 100) ? 100:Temp;
	return Temp;
}

static float RD_Get_NTC_Temperature(void)
{
	int16_t Temp_u16_Buff = 0;
	float 	 Temp_fl_Buff  =0;

	Temp_u16_Buff = temp_val;
	if(Temp_u16_Buff > 3900) Temp_u16_Buff = 3900;
	Temp_fl_Buff = RD_NTC_Convert(Temp_u16_Buff );
	
	return Temp_fl_Buff;
}

static void rd_Temp_Task(void)
{
    static float 	 TempFloat=0;
	TempFloat =  RD_Get_NTC_Temperature();
	Sensor_Of_Light_Common.Temp = (uint8_t) (TempFloat/1 + TEMP_OFFSET_ZERO);

    ESP_LOGI(SENSOR_TAG, "Update Temp Done: %0.2f *C \n", TempFloat );
}
/*-----------------------------------------------------------------------------------*/

/*-------------------------------------Power -----------------------------------------*/
static void rd_Power_Task(void)
{
    static uint16_t U_rms_int, I_rms_int, P_rms_int;
    static uint8_t Cos_int;
    static uint32_t Power_Consum_Ws;
    static uint16_t Power_Consum_Buff;
    read_UIP();

    U_rms_int = (uint16_t)data_bl0942.U_hd*10;
	Sensor_Of_Light_Common.Voltage[0]   = (uint8_t)  ((U_rms_int >> 8)& 0xff); 
	Sensor_Of_Light_Common.Voltage[1]   = (uint8_t)  (U_rms_int 	& 0xff);

    I_rms_int = (uint16_t) ((data_bl0942.I_hd*100)/1);
    Sensor_Of_Light_Common.Ampe[0]      = (uint8_t)  ((I_rms_int >> 8)& 0xff); 
	Sensor_Of_Light_Common.Ampe[1]      = (uint8_t)  (I_rms_int 	& 0xff);

    P_rms_int = (uint16_t)data_bl0942.P_hd/1;
    Sensor_Of_Light_Common.Power[0]		= (uint8_t)  ((P_rms_int >> 8)& 0xff);
	Sensor_Of_Light_Common.Power[1]		= (uint8_t)  (P_rms_int 	& 0xff);

    Cos_int  = (uint8_t)data_bl0942.Cos_Phi*100/1;
    Sensor_Of_Light_Common.Cos_UI = Cos_int	;

    Power_Consum_Ws += P_rms_int * (CYCLE_CHECK_POW_TASK_US/1000000);

	Power_Consum_Buff = Power_Consum_Ws/3600; //kW/h
	Sensor_Of_Light_Common.Power_Consum[0]  = (uint8_t)  ((Power_Consum_Buff >> 8)& 0xff); 
	Sensor_Of_Light_Common.Power_Consum[1]  = (uint8_t)  (Power_Consum_Buff 	& 0xff);
}
/*-----------------------------------------------------------------------------------*/

/*-------------------------------------CHECK_ERROR------------------------------------------*/
void RD_Check_Error(void){
	static int64_t  Time_Check_Old =0;
	static uint8_t   Count_Vol_High =0;
	static uint8_t   Count_Vol_Low =0;

	if(esp_timer_get_time() < Time_Check_Old)          Time_Check_Old =0;

	if((esp_timer_get_time() >= TIME_SCAN_ERROR_US) && (esp_timer_get_time() - Time_Check_Old >= TIME_SCAN_ERROR_US))
	{
        Time_Check_Old = esp_timer_get_time();
		union                 //    union contain SttScene - Cos -U -I
		{
			uint8_t SttS_Err;      //khai bao thi se tang them 4 byte. gia tr? == struct duoi
			struct 
			{
				uint8_t U_H         :1;  // bit0
				uint8_t U_L 		:1;  // bit 1
				uint8_t I_H 		:1;   // bit 2
				uint8_t I_L         :1;   // bit 3 
				uint8_t T_H         :1;   // bit 4 
				uint8_t T_L         :1;   // bit 5  
			} SttS_Err_t;
		} SttS_Err;
		
		SttS_Err.SttS_Err = Sensor_Of_Light_Common.Error[0];
		
		uint16_t Vol_High = ((Warning_Config_Val_Common.Voltage_TooHigh[0] << 8) | (Warning_Config_Val_Common.Voltage_TooHigh[1]));
		uint16_t Vol_Pre  = ((Sensor_Of_Light_Common.Voltage[0] << 8)            | (Sensor_Of_Light_Common.Voltage[1]));
		uint16_t Vol_Low = ((Warning_Config_Val_Common.Voltage_TooLow[0] << 8) 	 | (Warning_Config_Val_Common.Voltage_TooLow[1])); 

 
		#if 0  		// check 1 
		if(SttS_Err.SttS_Err_t.U_H == 0) SttS_Err.SttS_Err_t.U_H = (Vol_Pre > Vol_High) 	? 1:0;			// get error Hight Voltage
		else 							 SttS_Err.SttS_Err_t.U_H = (Vol_Pre < (Vol_High-5)) ? 0:1;			// getout error Hight Voltage

		if(SttS_Err.SttS_Err_t.U_L == 0) SttS_Err.SttS_Err_t.U_L = (Vol_Pre < Vol_Low)  	? 1:0;			// get error Low Voltage
		else							 SttS_Err.SttS_Err_t.U_L = (Vol_Pre > (Vol_Low+5))  ? 0:1;			// getout error Low Voltage
		#endif
		
		/*------------------------------ Check Voltage too Hight------------------------*/
		if(SttS_Err.SttS_Err_t.U_H == 0)
		{
			if(Vol_Pre > Vol_High)   			{Count_Vol_High++; ESP_LOGI(SENSOR_TAG," Voltage too Hight  \n");}
			
			if(Count_Vol_High >= 5)	
			{ 	
				Count_Vol_High =5;
				SttS_Err.SttS_Err_t.U_H = 1;																	// get error Low Voltage
				ESP_LOGI(SENSOR_TAG," Voltage too High get Error and turn off \n");			
			}											
			//else					SttS_Err.SttS_Err_t.U_H = 0;											// getout error Low Voltage
		}
		else 
		{
			if(Vol_Pre < (Vol_High-5))
			{
				if(Count_Vol_High >=1)	Count_Vol_High--;
				else 					SttS_Err.SttS_Err_t.U_H =0;
			}				
		}
		
		/*------------------------------ Check Voltage too Low------------------------*/
		if(SttS_Err.SttS_Err_t.U_L == 0)
		{
			if(Vol_Pre < Vol_Low)   			{Count_Vol_Low++; ESP_LOGI(SENSOR_TAG," Voltage too Low  \n");}
			
			if(Count_Vol_Low >= 5)	
			{ 	
				Count_Vol_Low =5;
				SttS_Err.SttS_Err_t.U_L = 1;																	// get error Low Voltage
				ESP_LOGI(SENSOR_TAG," Voltage too Low get Error and turn off \n");			
			}											
			//else					SttS_Err.SttS_Err_t.U_L = 0;											// getout error Low Voltage
		}
		else 
		{
			if(Vol_Pre > (Vol_Low+5))
			{
				if(Count_Vol_Low >=1)	Count_Vol_Low--;
				else 					SttS_Err.SttS_Err_t.U_L =0;
			}				
		}
	
		
		uint16_t Ampe_High = ((Warning_Config_Val_Common.Ampe_TooHigh[0] << 8) 	| (Warning_Config_Val_Common.Ampe_TooHigh[1]));
		uint16_t Ampe_Pre  = ((Sensor_Of_Light_Common.Ampe[0] << 8)            	| (Sensor_Of_Light_Common.Ampe[1])); 
		SttS_Err.SttS_Err_t.I_H = (Ampe_Pre > Ampe_High) ? 1:0;
		
		uint16_t Temp_High = Warning_Config_Val_Common.Temperature_TooHigh;
		uint16_t Temp_Pre  = Sensor_Of_Light_Common.Temp  ; 
		SttS_Err.SttS_Err_t.T_H = (Temp_Pre > Temp_High) ? 1:0;
		
		Sensor_Of_Light_Common.Error[0] = SttS_Err.SttS_Err;
	}
}


/*U,I,Temp,Light,GPS*/
static void rd_sensor_task(void *arg)
{
    static int64_t last_time_gps = 0;
    static int64_t last_time_light_and_temp = 0;
    static int64_t last_time_pow = 0;

    // RD_NOTE: set GMT +7 time
    rd_set_GMT_7();
    rd_adc_init();
    rd_bl0942_init();

    while (1)
    {
        if ((rd_exceed_us(last_time_gps, CYCLE_CHECK_GPS_NEWTASK_US) && GPS_Value.Signal_Stt == false)\
            || (rd_exceed_us(last_time_gps, CYCLE_CHECK_GPS_OLDTASK_US) && GPS_Value.Signal_Stt == true))
        {
            last_time_gps = esp_timer_get_time();
            rd_GPS_task();
        }
        if(rd_exceed_us(last_time_light_and_temp, CYCLE_CHECK_LIGHT_TASK_US))
        {
            last_time_light_and_temp = esp_timer_get_time();
            rd_Light_task();
            rd_Temp_Task();
        }
        if (rd_exceed_us(last_time_pow, CYCLE_CHECK_POW_TASK_US))
        {
            last_time_pow = esp_timer_get_time();
            rd_Power_Task();
        }
        
        rd_RTC_Val_Upload();
        RD_Check_Error();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void rd_sensor_init(void)
{
    xTaskCreate(rd_sensor_task, "Sensor Task:", 2024 * 2, NULL, configMAX_PRIORITIES - 4, NULL);
}








void test_tm()
{

    /* Lấy thời gian hiện tại 1/1/1970
    RTC_offset = rd_get_RTC_Offset(0, 0, 0, 1, 1, 70); // offset  = 0;
    ~ raw_time = RTC_offset + time(NULL) = time(NULL);
    */
    time_t raw_time = updateRTCTime(); // Cach 2: time(&raw_time);
    struct tm time_info;               // Bộ nhớ do người dùng cấp phát
    // localtime_r(&time_now, &RTC_Val);

    RTC_offset = rd_get_RTC_Offset(30, 47, 10, 28, 2, 24); // 10:47:30 28/2/2024
    time_now = RTC_offset + updateRTCTime();

    localtime_r(&time_now, &time_info); // Chuyển đổi thời gian

    printf("time now: %02d-%02d-%d %02d:%02d:%02d\n",
           time_info.tm_mday, time_info.tm_mon + 1, time_info.tm_year + 1900,
           time_info.tm_hour, time_info.tm_min, time_info.tm_sec);
}