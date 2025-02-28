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
    // const float Alpha_Light = 1.0;
    // Light_Buff = adc1_light_raw;
    // Light_Buff = Light_Buff*Alpha_Light;
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
static void rd_Temp_Task(void)
{
}
/*-----------------------------------------------------------------------------------*/

/*-------------------------------------Power -----------------------------------------*/
static void rd_Power_Task(void)
{
}
/*-----------------------------------------------------------------------------------*/

/*-------------------------------------CHECK_ERROR------------------------------------------*/
void RD_Check_Error(void){

}


/*U,I,Temp,Light,GPS*/
static void rd_sensor_task(void *arg)
{
    static int64_t last_time_gps = 0;
    static int64_t last_time_light_and_temp = 0;
    static int64_t last_time_pow = 0;

    // RD_NOTE: set GMT +7 time
    rd_set_GMT_7();

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

    RTC_offset = rd_get_RTC_Offset(30, 47, 10, 28, 2, 24);
    time_now = RTC_offset + updateRTCTime();

    localtime_r(&time_now, &time_info); // Chuyển đổi thời gian

    printf("time now: %02d-%02d-%d %02d:%02d:%02d\n",
           time_info.tm_mday, time_info.tm_mon + 1, time_info.tm_year + 1900,
           time_info.tm_hour, time_info.tm_min, time_info.tm_sec);
}