#include "rd_lightCommon.h"
#include "esp_log.h"

static const  char *LIGHT_COM_TAG = "LIGHT_RIIM";

data_config_flash_t		        Data_Config_Flash;
warning_config_t                Warning_Config_Val_Common;
start_config_t			        Start_Config_Val_Common;
network_info_t		            Network_Inf_Common;

extern const uint8_t Net_Key_Default[16];
extern struct tm RTC_Val;
extern uint8_t rd_exceed_us(int64_t ref, int64_t span_ms);

uint8_t Restart_flag_en = 0;  // restart chip khi co ban tin
uint64_t clock_time_restart_us =0;

static void RD_upload_flashDataConfig(void)
{
    Data_Config_Flash.Saved = FLASH_SAVED_CODE;
    Data_Config_Flash.Last_Light_Stt.Dim_target 	            = light_stt_val.Dim_target;
    Data_Config_Flash.Last_Light_Stt.Relay_target	            = light_stt_val.Relay_target;

    Data_Config_Flash.Start_Config.Relay_Stt	                = Start_Config_Val_Common.Relay_Stt;
    Data_Config_Flash.Start_Config.Dim		               		= Start_Config_Val_Common.Dim;


    Data_Config_Flash.Warning_Config_Val.Voltage_TooHigh[0]  	= Warning_Config_Val_Common.Voltage_TooHigh[0];
    Data_Config_Flash.Warning_Config_Val.Voltage_TooHigh[1]  	= Warning_Config_Val_Common.Voltage_TooHigh[1];
    Data_Config_Flash.Warning_Config_Val.Voltage_TooLow[0]   	= Warning_Config_Val_Common.Voltage_TooLow[0];
    Data_Config_Flash.Warning_Config_Val.Voltage_TooLow[1]   	= Warning_Config_Val_Common.Voltage_TooLow[1];

    Data_Config_Flash.Warning_Config_Val.Ampe_TooHigh[0]     	= Warning_Config_Val_Common.Ampe_TooHigh[0];
    Data_Config_Flash.Warning_Config_Val.Ampe_TooHigh[1]     	= Warning_Config_Val_Common.Ampe_TooHigh[1];
    Data_Config_Flash.Warning_Config_Val.Ampe_TooLow[0]         = Warning_Config_Val_Common.Ampe_TooLow[0];
    Data_Config_Flash.Warning_Config_Val.Ampe_TooLow[1]         = Warning_Config_Val_Common.Ampe_TooLow[1];


    Data_Config_Flash.Warning_Config_Val.Temperature_TooHigh    = Warning_Config_Val_Common.Temperature_TooHigh;
    Data_Config_Flash.Warning_Config_Val.Temperature_TooLow     = Warning_Config_Val_Common.Temperature_TooLow;    

    Data_Config_Flash.Warning_Config_Val.Light_TooHigh[0]       = Warning_Config_Val_Common.Light_TooHigh[0];
    Data_Config_Flash.Warning_Config_Val.Light_TooHigh[1]       = Warning_Config_Val_Common.Light_TooHigh[1];
    Data_Config_Flash.Warning_Config_Val.Light_TooLow[0]        = Warning_Config_Val_Common.Light_TooLow[0];
    Data_Config_Flash.Warning_Config_Val.Light_TooLow[1]        = Warning_Config_Val_Common.Light_TooLow[1];

    Data_Config_Flash.Warning_Config_Val.Power_Default[0]       = Warning_Config_Val_Common.Power_Default[0];
    Data_Config_Flash.Warning_Config_Val.Power_Default[1]       = Warning_Config_Val_Common.Power_Default[1];

    Data_Config_Flash.Warning_Config_Val.Type_Active            = Warning_Config_Val_Common.Type_Active;
    Data_Config_Flash.Warning_Config_Val.Time_Recall_Active     = Warning_Config_Val_Common.Time_Recall_Active;
    Data_Config_Flash.Warning_Config_Val.Time_Recall[0]         = Warning_Config_Val_Common.Time_Recall[0];
    Data_Config_Flash.Warning_Config_Val.Time_Recall[1]         = Warning_Config_Val_Common.Time_Recall[1];
	Data_Config_Flash.Warning_Config_Val.Type_Active.Type_Active_All			= Warning_Config_Val_Common.Type_Active.Type_Active_All;
	
    Data_Config_Flash.Network_Inf_Config.Connect_Stt            = 0x00; //Network_Inf_Common.Connect_Stt;
    Data_Config_Flash.Network_Inf_Config.Power_Tx[0]            = Network_Inf_Common.Power_Tx[0];
    Data_Config_Flash.Network_Inf_Config.Power_Tx[1]            = Network_Inf_Common.Power_Tx[1];
    Data_Config_Flash.Network_Inf_Config.Channel_RIIM           = Network_Inf_Common.Channel_RIIM;
    Data_Config_Flash.Network_Inf_Config.Cycle_Rsp[0]           = Network_Inf_Common.Cycle_Rsp[0];
    Data_Config_Flash.Network_Inf_Config.Cycle_Rsp[1]           = Network_Inf_Common.Cycle_Rsp[1];
    Data_Config_Flash.Network_Inf_Config.Time_Stack             = Network_Inf_Common.Time_Stack;
    Data_Config_Flash.Network_Inf_Config.Num_Stack_Rsp[0]       = Network_Inf_Common.Num_Stack_Rsp[0];
    Data_Config_Flash.Network_Inf_Config.Num_Stack_Rsp[1]       = Network_Inf_Common.Num_Stack_Rsp[1];

    Data_Config_Flash.Network_Inf_Config.Group_ID[0]            = Network_Inf_Common.Group_ID[0];
    Data_Config_Flash.Network_Inf_Config.Group_ID[1]            = Network_Inf_Common.Group_ID[1];

    Data_Config_Flash.Network_Inf_Config.NumInGroup             = Network_Inf_Common.NumInGroup;
    for(int i=0; i<16; i++)
    {
       Data_Config_Flash.Network_Inf_Config.Netkey[i] = Network_Inf_Common.Netkey[i];
    }
}


void RD_Save_DataConfig(void)
{
    RD_upload_flashDataConfig();
    rd_flash_write(FLASH_ADD_CONFIG, (uint8_t *)&Data_Config_Flash, sizeof(Data_Config_Flash));
}
/*
Set value as default value:
*/ 
void RD_Save_DataConfig_Defaut(void)
{

	Start_Config_Val_Common.Relay_Stt = RELAY_STT_DF;
	Start_Config_Val_Common.Dim   = DIM_DF;


	Warning_Config_Val_Common.Voltage_TooHigh[0] 	= (VOLTAGE_TOOHIGH_DF>>8) 	&0xFF;
	Warning_Config_Val_Common.Voltage_TooHigh[1] 	= (VOLTAGE_TOOHIGH_DF)  	&0xFF;
	Warning_Config_Val_Common.Voltage_TooLow[0]  	= (VOLTAGE_TOOLOW_DF >>8) 	&0xFF; 
	Warning_Config_Val_Common.Voltage_TooLow[1] 	= (VOLTAGE_TOOLOW_DF) 		&0xFF;

	Warning_Config_Val_Common.Ampe_TooHigh[0]     	= (AMPE_TOOHIGH_DF>>8) 		&0xFF;
	Warning_Config_Val_Common.Ampe_TooHigh[1]       = (AMPE_TOOHIGH_DF)  		&0xFF;
	Warning_Config_Val_Common.Ampe_TooLow[0]        = (AMPE_TOOLOW_DF>>8) 		&0xFF;
	Warning_Config_Val_Common.Ampe_TooLow[1]        = (AMPE_TOOLOW_DF)  		&0xFF; 

	Warning_Config_Val_Common.Temperature_TooHigh   = TEMP_TOOHIGH_DF;
	Warning_Config_Val_Common.Temperature_TooLow    = TEMP_TOOLOW_DF;    

	Warning_Config_Val_Common.Light_TooHigh[0]      = (LIGHT_TOOHIGH_DF>>8) 	&0xFF;
	Warning_Config_Val_Common.Light_TooHigh[1]      = (LIGHT_TOOHIGH_DF)  		&0xFF;
	Warning_Config_Val_Common.Light_TooLow[0]       = (LIGHT_TOOLOW_DF>>8) 		&0xFF;
	Warning_Config_Val_Common.Light_TooLow[1]       = (LIGHT_TOOLOW_DF)  		&0xFF;

	Warning_Config_Val_Common.Power_Default[0]      = (uint8_t) ((POWER_DF>>8)           & 0xff);
	Warning_Config_Val_Common.Power_Default[1]      = (uint8_t) ((POWER_DF)              & 0xff);

    Warning_Config_Val_Common.Type_Active.Type_Active_All           = (uint8_t) (TYPE_WAR_ACTIVE_DF      & 0xff);
	Warning_Config_Val_Common.Time_Recall_Active    = (uint8_t) (TIME_RECALL_ACTIVE_DF   & 0xff);
	Warning_Config_Val_Common.Time_Recall[0]        = (uint8_t) ((TIME_RECALL_WAR_DF>>8) & 0xff);
	Warning_Config_Val_Common.Time_Recall[1]        = (uint8_t) ((TIME_RECALL_WAR_DF)    & 0xff);

	Network_Inf_Common.Connect_Stt					= 0;
	Network_Inf_Common.Power_Tx[0]                	= (POWER_TX_DF>>8) 			&0xFF;
	Network_Inf_Common.Power_Tx[1]                	= (POWER_TX_DF)  			&0xFF;
    Network_Inf_Common.Channel_RIIM                 = CHANNEL_DF;

	Network_Inf_Common.Group_ID[0]                	= (GROUP_ID_DF>>8) 			&0xFF;
	Network_Inf_Common.Group_ID[1]                	= (GROUP_ID_DF)  			&0xFF;
    Network_Inf_Common.NumInGroup                   = NUM_IN_GROUP_DF;
    for(int i=0; i<16; i++)
    {
       Network_Inf_Common.Netkey[i] = Net_Key_Default[i];
    }

    RD_Save_DataConfig();
}

//load data from flash
static void RD_download_flashDataConfig(void)
{
        Start_Config_Val_Common.Dim                     = Data_Config_Flash.Start_Config.Dim;
        Start_Config_Val_Common.Relay_Stt               = Data_Config_Flash.Start_Config.Relay_Stt; 

        light_stt_val.Dim_current                       = 0;
        light_stt_val.Relay_current                     = 0;
        light_stt_val.Dim_target                        = Start_Config_Val_Common.Dim;
        light_stt_val.Relay_target                      = Start_Config_Val_Common.Relay_Stt;
 
        Warning_Config_Val_Common.Voltage_TooHigh[0]    = Data_Config_Flash.Warning_Config_Val.Voltage_TooHigh[0];
        Warning_Config_Val_Common.Voltage_TooHigh[1]    = Data_Config_Flash.Warning_Config_Val.Voltage_TooHigh[1];
        Warning_Config_Val_Common.Voltage_TooLow[0]     = Data_Config_Flash.Warning_Config_Val.Voltage_TooLow[0];
        Warning_Config_Val_Common.Voltage_TooLow[1]     = Data_Config_Flash.Warning_Config_Val.Voltage_TooLow[1];

        Warning_Config_Val_Common.Ampe_TooHigh[0]       = Data_Config_Flash.Warning_Config_Val.Ampe_TooHigh[0];
        Warning_Config_Val_Common.Ampe_TooHigh[1]       = Data_Config_Flash.Warning_Config_Val.Ampe_TooHigh[1];
        Warning_Config_Val_Common.Ampe_TooLow[0]        = Data_Config_Flash.Warning_Config_Val.Ampe_TooLow[0];
        Warning_Config_Val_Common.Ampe_TooLow[1]        = Data_Config_Flash.Warning_Config_Val.Ampe_TooLow[1];
 

        Warning_Config_Val_Common.Temperature_TooHigh   = Data_Config_Flash.Warning_Config_Val.Temperature_TooHigh;
        Warning_Config_Val_Common.Temperature_TooLow    = Data_Config_Flash.Warning_Config_Val.Temperature_TooLow;    

        Warning_Config_Val_Common.Light_TooHigh[0]      = Data_Config_Flash.Warning_Config_Val.Light_TooHigh[0];
        Warning_Config_Val_Common.Light_TooHigh[1]      = Data_Config_Flash.Warning_Config_Val.Light_TooHigh[1];
        Warning_Config_Val_Common.Light_TooLow[0]       = Data_Config_Flash.Warning_Config_Val.Light_TooLow[0];
        Warning_Config_Val_Common.Light_TooLow[1]       = Data_Config_Flash.Warning_Config_Val.Light_TooLow[1];
			
        Warning_Config_Val_Common.Power_Default[0]      = Data_Config_Flash.Warning_Config_Val.Power_Default[0];
        Warning_Config_Val_Common.Power_Default[1]      = Data_Config_Flash.Warning_Config_Val.Power_Default[1];	

        Warning_Config_Val_Common.Type_Active           = Data_Config_Flash.Warning_Config_Val.Type_Active;
        Warning_Config_Val_Common.Time_Recall_Active    = Data_Config_Flash.Warning_Config_Val.Time_Recall_Active;
        Warning_Config_Val_Common.Time_Recall[0]        = Data_Config_Flash.Warning_Config_Val.Time_Recall[0];
        Warning_Config_Val_Common.Time_Recall[1]        = Data_Config_Flash.Warning_Config_Val.Time_Recall[1];

		Network_Inf_Common.Connect_Stt                  = 0x00; //Data_Config_Flash.Network_Inf_Config.Connect_Stt; // RD_Todo
        Network_Inf_Common.Cycle_Rsp[0]                 = Data_Config_Flash.Network_Inf_Config.Cycle_Rsp[0];
        Network_Inf_Common.Cycle_Rsp[1]                 = Data_Config_Flash.Network_Inf_Config.Cycle_Rsp[1];
        Network_Inf_Common.Time_Stack                   = Data_Config_Flash.Network_Inf_Config.Time_Stack;
        Network_Inf_Common.Num_Stack_Rsp[0]             = Data_Config_Flash.Network_Inf_Config.Num_Stack_Rsp[0];
        Network_Inf_Common.Num_Stack_Rsp[1]             = Data_Config_Flash.Network_Inf_Config.Num_Stack_Rsp[1]; 
        
        Network_Inf_Common.Group_ID[0]                  = Data_Config_Flash.Network_Inf_Config.Group_ID[0];
        Network_Inf_Common.Group_ID[1]                  = Data_Config_Flash.Network_Inf_Config.Group_ID[1];
        Network_Inf_Common.NumInGroup                   = Data_Config_Flash.Network_Inf_Config.NumInGroup;

		if( Data_Config_Flash.Network_Inf_Config.Netkey[1] == 0x00)
			{
				for(int i=0; i<16; i++)
					{
						Network_Inf_Common.Netkey[i] 	= Net_Key_Default[i];
					}
					Network_Inf_Common.Power_Tx[0]  = POWER_TX_DF_H;
					Network_Inf_Common.Power_Tx[1]  = POWER_TX_DF_L;
                    Network_Inf_Common.Channel_RIIM = CHANNEL_DF;
			}
		else
			{
	 
				Network_Inf_Common.Power_Tx[0] = Data_Config_Flash.Network_Inf_Config.Power_Tx[0];
				Network_Inf_Common.Power_Tx[1] = Data_Config_Flash.Network_Inf_Config.Power_Tx[1];
                Network_Inf_Common.Channel_RIIM = Data_Config_Flash.Network_Inf_Config.Channel_RIIM;
				for(int i=0; i<16; i++)
				{
					Network_Inf_Common.Netkey[i] = Data_Config_Flash.Network_Inf_Config.Netkey[i] ;
				}
			}
}

void RD_Init_FlashDataConfig(void)
{   
    ESP_LOGI(LIGHT_COM_TAG, "ESP32C3 StreetLight Version %02d.%02d\n",(FIRM_VERSION >> 8)& 0xFF , FIRM_VERSION	& 0xFF );
    rd_read_flash(&Data_Config_Flash, FLASH_ADD_CONFIG, sizeof(Data_Config_Flash));
    /*----------------- the first read Flash-- Write data default----------------*/
	if(Data_Config_Flash.Saved != FLASH_SAVED_CODE)							 
	{   
        ESP_LOGI(LIGHT_COM_TAG, "First read flash data config add: 0x %x\n",FLASH_ADD_CONFIG);
        RD_Save_DataConfig_Defaut();
    }
    else
    {
        RD_download_flashDataConfig();

        ESP_LOGI(LIGHT_COM_TAG, "Read data config.\n");
        ESP_LOGI(LIGHT_COM_TAG, "Network key: ");

        char buff[64];
        for(int i=0; i<16; i++)
        {
           // ESP_LOGI(LIGHT_COM_TAG, "-0x%x ",Network_Inf_Common.Netkey[i]);
            sprintf(&buff[i * 4], "-0x%x", Network_Inf_Common.Netkey[i]);
        }

        ESP_LOGI(LIGHT_COM_TAG, "%s", buff);
        
        ESP_LOGI(LIGHT_COM_TAG, "Dim current: %d - Dim target: %d", light_stt_val.Dim_current, light_stt_val.Dim_target);
        ESP_LOGI(LIGHT_COM_TAG, "Network TxPow 0x%x-%x: Channel: %d ",Network_Inf_Common.Power_Tx[0], Network_Inf_Common.Power_Tx[1], Network_Inf_Common.Channel_RIIM);
        ESP_LOGI(LIGHT_COM_TAG, "Group ID:  0x%x%x Num InGroup: %d",Network_Inf_Common.Group_ID[0], Network_Inf_Common.Group_ID[1], Network_Inf_Common.NumInGroup);
       ESP_LOGI(LIGHT_COM_TAG, "Time Rsp TSPH:  0x%02x%02x TimeTask: %u , NumStakRsp %u" , Network_Inf_Common.Cycle_Rsp[0], Network_Inf_Common.Cycle_Rsp[1]  , Network_Inf_Common.Time_Stack, Network_Inf_Common.Num_Stack_Rsp[0]);
   
    }
}

void RD_Init_Flash(void)
{
    RD_Init_FlashSceneData(); // Data of Scene
    RD_Init_FlashDataConfig(); // Data configuration
}


static void RD_ctrByError(void)
{
    /* Detected error by Voltage, Control Relay off*/
	if((Sensor_Of_Light_Common.Error[0] & 0x01 ) || ((Sensor_Of_Light_Common.Error[0] >> 1) & 0x01)) // voltage too high and too low
	{
        if(light_stt_val.Safety == true){
            ESP_LOGI(LIGHT_COM_TAG, "Error voltage ctrl off to save: ");
            light_set_safety(false);
        }
	}
	/* No have error by Voltage, Control Relay = Light_Stt_Common.Relay*/
	else
	{
        light_set_safety(true);
    }
}


// loop check time and send TSPH
static void RD_Scan_Send_Mess_cycle(void)
{
	static int64_t time_Buff=0;
	static int64_t time_Buff_Last=0;
	int64_t Time_buff_RTC=0;
	
	if(Time_RTC_Set_Flag == true) // co ban tin gps
	{
		uint8_t Second_buff = RTC_Val.tm_sec;
		uint8_t Minute_buff	= RTC_Val.tm_min;
		uint8_t Hour_buff   = RTC_Val.tm_hour;
		
		
		if(Hour_buff >=12)
		{
			Time_buff_RTC = Second_buff + Minute_buff*60 + (Hour_buff - 12)*60*60;
		}
		else
		{
			Time_buff_RTC = Second_buff + Minute_buff*60 + (Hour_buff + 12)*60*60;
		}
	}
	time_Buff  = Time_buff_RTC;
	
	uint16_t Cycle_Rsp_Buff = (uint16_t) ((Network_Inf_Common.Cycle_Rsp[0] << 8) | (Network_Inf_Common.Cycle_Rsp[1]));
	//ESP_LOGI(LIGHT_COM_TAG, "Time_RTC_Set_Flag: %d, Time_Buff: %d\n", Time_RTC_Set_Flag, time_Buff);
    //ESP_LOGI(LIGHT_COM_TAG, "time_Buff: %d,\n",time_Buff);
	if((time_Buff != time_Buff_Last) && (Cycle_Rsp_Buff > 0))
	{
		time_Buff_Last = time_Buff;

		uint16_t Num_Stack_Rsp_Buff = (uint16_t) ((Network_Inf_Common.Num_Stack_Rsp[0] << 8) | (Network_Inf_Common.Num_Stack_Rsp[1]));
		uint8_t  Time_Stack_Buff	= (uint8_t)  Network_Inf_Common.Time_Stack;
		
		uint16_t Time_send_Buff = Time_Stack_Buff*Num_Stack_Rsp_Buff;
        // ESP_LOGI(LIGHT_COM_TAG, "time_Buff: %d,\n",time_Buff);
        // ESP_LOGI(LIGHT_COM_TAG, "Cycle_Rsp_Buff: %d\n",Cycle_Rsp_Buff);
        // ESP_LOGI(LIGHT_COM_TAG, " NetStt: %d,Year: %d \n", Network_Inf_Common.Connect_Stt, GPS_Value.Year );
		if( ((time_Buff%Cycle_Rsp_Buff) == Time_send_Buff) && (Network_Inf_Common.Connect_Stt == 1) )
		{
			if(GPS_Value.Year != 80)
			{
                ESP_LOGI(LIGHT_COM_TAG, "TSPH send\n");
				RD_Rsp_OPCODE_TSPH();
			}
		} 
	}
}


static void RD_LightCommonUpdate(void *arg)
{
    static int64_t last_time_tick_scene = 0;
    static int64_t last_time_tick_ctrByError = 0;
    static int64_t last_time_tick_lightCtr = 0;
    static int64_t last_time_tick_TSPH = 0;
    static int64_t last_time_tick_test = 0;

    Scene_Data_Common.Enable_Function = true;

    //RD_Test_Init(); 
    while(true)
    {
        if(esp_timer_get_time() < last_time_tick_scene)         last_time_tick_scene =0;
        if(esp_timer_get_time() < last_time_tick_ctrByError)    last_time_tick_ctrByError =0;
        if(esp_timer_get_time() < last_time_tick_lightCtr)      last_time_tick_lightCtr =0;
        if(esp_timer_get_time() < last_time_tick_TSPH)          last_time_tick_TSPH =0;
        if(esp_timer_get_time() < last_time_tick_test)          last_time_tick_test =0;
        if(esp_timer_get_time() < clock_time_restart_us)          clock_time_restart_us =0;

        if(rd_exceed_us(last_time_tick_scene, CYCLE_CHECK_SCENE_TASK_US))
        {
            last_time_tick_scene = esp_timer_get_time();
            RD_SceneScan();
        }
        
        if(rd_exceed_us(last_time_tick_ctrByError, CYCLE_CHECK_CTR_BY_ERR_TASK_US))
        {
            last_time_tick_ctrByError = esp_timer_get_time();
            /*------------------turn off light if voltage too hight or too low---------------*/
            #if VOLTAGE_PROTECT
                RD_ctrByError();
            #endif
        }

        
        if(rd_exceed_us(last_time_tick_lightCtr, CYCLE_CHECK_CTRL_TASK_US))
        {
            last_time_tick_lightCtr = esp_timer_get_time();
            light_ctr_update();
        }

        if(rd_exceed_us(last_time_tick_TSPH, CYCLE_CHECK_TSPH_TASK_US))
        {
            last_time_tick_TSPH = esp_timer_get_time();
            RD_Scan_Send_Mess_cycle();

            if((1 == Restart_flag_en) && rd_exceed_us(clock_time_restart_us, TIME_RESTART_AFTER_MESS_US))
            {
                printf("restart\n");
                gpio_set_level(RST_RIIM_PIN, 0);
                vTaskDelay(10 / portTICK_PERIOD_MS);
                esp_restart();
            }
        }

        if(esp_timer_get_time() - last_time_tick_test >= 500000)
        {
            last_time_tick_test = esp_timer_get_time();
            //RD_Testting_Loop();
        }

        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}

void RD_LightCommon_callTask(void)
{
    xTaskCreate(RD_LightCommonUpdate, "RD_LightCommonUpdate task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
}

void RD_AddGroup(uint16_t GroupID, uint8_t NumInGroup)
{
    Network_Inf_Common.Group_ID[0] = (uint8_t) (GroupID>>8 & 0xFF);
    Network_Inf_Common.Group_ID[1] = (uint8_t) (GroupID    & 0xFF);
    Network_Inf_Common.NumInGroup  = NumInGroup;
    RD_Save_DataConfig();
}
uint8_t RD_CheckGroup(uint16_t GroupID, uint8_t NumInGroup)
{
    uint16_t Check_G_ID_Buff = (uint16_t)  ((Network_Inf_Common.Group_ID[0] << 8) | (Network_Inf_Common.Group_ID[1]));
    if( (GroupID == Check_G_ID_Buff) && ((NumInGroup == 0x00) || (Network_Inf_Common.NumInGroup  == NumInGroup) ))
    {
        return true;
    }
    else
    {
        return false;
    }
}






