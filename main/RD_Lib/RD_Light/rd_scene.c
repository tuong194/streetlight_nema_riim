#include "rd_scene.h"

static const  char *SCENE_TAG = "SCENE_TAG";

Struct_Scene_Data           Scene_Data_Common;			// Data luu Scene
Struct_Time_Scene			Time_Scene_Common;
volatile uint8_t s_save_flash;

static void RD_Scene_SaveDataFlash_Default(void)
{
	Struct_Scene_Data Data_Scene_DF = {};
    rd_flash_write(&Data_Scene_DF, FLASH_ADD_SCENE, sizeof(Data_Scene_DF));
}

void RD_Init_FlashSceneData(void)
{
    ESP_LOGI(SCENE_TAG,"Read flash");
    rd_read_flash(&Scene_Data_Common, FLASH_ADD_SCENE, sizeof(Scene_Data_Common));

	if(Scene_Data_Common.Saved == 0xff)
	{
		ESP_LOGI(SCENE_TAG,"The first time scene data init. Save Data default");

		RD_Scene_SaveDataFlash_Default();
        ESP_ERROR_CHECK(esp_flash_write(NULL, &Scene_Data_Common, FLASH_ADD_SCENE, sizeof(Scene_Data_Common)));
	}
	else
	{
		ESP_LOGI(SCENE_TAG,"device has %u scene saved \n", Scene_Data_Common.Amount_Scene);
	}

}
static void RD_Scene_SaveDataFlash(void)
{
    rd_flash_write(&Scene_Data_Common.Saved, FLASH_ADD_SCENE, sizeof(Scene_Data_Common));
}


void RD_Add_Scene(uint8_t Enable_Stt, uint16_t Scene_ID, uint32_t Start_date_in, uint32_t Stop_date_in , uint8_t Month_in, uint8_t Date_in,  uint8_t Week_Day_in, uint8_t Hour_in, uint8_t Minute_in, uint8_t Relay_Stt_in,uint8_t Dim_per_in)
{
	uint8_t saved=0;
	uint8_t count_Scene=0;
	uint8_t Edit_Location=0;
	for(int i=0; i<SCENE_MAX ; i++)
	{
		uint16_t Check_SCENE_ID_Buff =0x0000;

		Check_SCENE_ID_Buff = Scene_Data_Common.Scene_Data[i].Scene_ID;
		
		if(Check_SCENE_ID_Buff == Scene_ID)
		{
			ESP_LOGI(SCENE_TAG,"This Scene_ID Saved before.  \n");
			saved=1;
			Edit_Location = i ;
		}

		if(Check_SCENE_ID_Buff != 0x0000)
		{
			count_Scene++;
			if(count_Scene>= Scene_Data_Common.Amount_Scene)
			{
				break;
			}
		}
	}

	if( saved == 0)
	{
		if(Scene_Data_Common.Amount_Scene >= SCENE_MAX)
		{
			ESP_LOGI(SCENE_TAG,"Full Memory to save Scene \n");
		}
		else
		{
			/*-------------------------Update data to Struct----------------*/
 
			Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Enable_ID		= Enable_Stt;
			
			Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Scene_ID 	    = Scene_ID;
			
			Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Start_Date	    = Start_date_in;
			
			Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Stop_Date  	= Stop_date_in;

			Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Month   	   	=  Month_in;
			Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Date   		=  Date_in;
			Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Week_Day  		=  Week_Day_in;

			Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Hour  			=  Hour_in;
			Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Minute  		=  Minute_in; 
			
			Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Relay_Stt  	=  Relay_Stt_in;

			Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Dim_per  		=  Dim_per_in;

			Scene_Data_Common.Amount_Scene++;
			
			/*-------------------------Save to Flash-----------------------*/
		   RD_Scene_SaveDataFlash();
	   }
	} 	// uint8_t saved=0;

	else  // edit Scene
	{
				/*-------------------------Update data to Struct----------------*/
 
			Scene_Data_Common.Scene_Data[Edit_Location].Enable_ID		= Enable_Stt;
		
			Scene_Data_Common.Scene_Data[Edit_Location].Scene_ID        = Scene_ID;
			
			Scene_Data_Common.Scene_Data[Edit_Location].Start_Date      = Start_date_in;
			
			Scene_Data_Common.Scene_Data[Edit_Location].Stop_Date       = Stop_date_in;

			Scene_Data_Common.Scene_Data[Edit_Location].Month   	   	=  Month_in;
			Scene_Data_Common.Scene_Data[Edit_Location].Date   			=  Date_in;
			Scene_Data_Common.Scene_Data[Edit_Location].Week_Day  		=  Week_Day_in;

			Scene_Data_Common.Scene_Data[Edit_Location].Hour  			=  Hour_in;
			Scene_Data_Common.Scene_Data[Edit_Location].Minute  		=  Minute_in;
			 

			Scene_Data_Common.Scene_Data[Edit_Location].Relay_Stt  		=  Relay_Stt_in;

			Scene_Data_Common.Scene_Data[Edit_Location].Dim_per  		=  Dim_per_in;

			//Scene_Data_Common.Amount_Scene++;
			/*-------------------------Save to Flash-----------------------*/
		   
		   RD_Scene_SaveDataFlash();
	}
 
}


void RD_Enable_Scene_Function(uint8_t Stt_active)
{
    Scene_Data_Common.Enable_Function =  Stt_active;
    /*-------------------------Save to Flash-----------------------*/

    RD_Scene_SaveDataFlash();
}

void RD_Enable_Scene_ID(uint8_t Stt_active, uint16_t Scene_ID_in)
{
	uint16_t Check_G_ID_Buff =0x0000;
	
	s_save_flash = 0;
	for(int i=0; i<Scene_Data_Common.Amount_Scene ; i++) 
	{
		

		Check_G_ID_Buff = Scene_Data_Common.Scene_Data[i].Scene_ID;

		if(Scene_ID_in == Check_G_ID_Buff )																			// Null 
 
		{
			/*-------------------------Update data to Struct----------------*/
			s_save_flash  = 0x02;
			Scene_Data_Common.Scene_Data[i].Enable_ID 	= Stt_active;
			//
			break; // getout of loop 
		}
	}
	
	if(s_save_flash == 0x02)
	{
		/*-------------------------Save to Flash-----------------------*/ 
		RD_Scene_SaveDataFlash();
	}
	else
	{
		ESP_LOGI(SCENE_TAG,"bug enable Scene \n"); 
	} 
}

/*
Enable Scene Function and recheck all Scene saved. call Scene  has most recent Time
*/

void RD_Enable_Scene(uint8_t Stt_active, uint16_t Scene_ID_in)
{
	const uint16_t Scene_Function = 0xFFFF;
	if(Scene_ID_in == Scene_Function)
	{
		RD_Enable_Scene_Function(Stt_active);
	}
	else
	{
		RD_Enable_Scene_ID(Stt_active, Scene_ID_in);
	}
}
void RD_Delete_Scene_All(void)
{
	for(int i = 0; i<=Scene_Data_Common.Amount_Scene ; i++)
	{
			Scene_Data_Common.Scene_Data[i].Enable_ID 			= 0x00;
			
			Scene_Data_Common.Scene_Data[i].Scene_ID	 		= 0x0000;
			
			Scene_Data_Common.Scene_Data[i].Start_Date		    = 0x00;
			
			Scene_Data_Common.Scene_Data[i].Stop_Date     		= 0x00;

			Scene_Data_Common.Scene_Data[i].Month   	   		= 0x00;
			Scene_Data_Common.Scene_Data[i].Date   				= 0x00;
			Scene_Data_Common.Scene_Data[i].Week_Day  			= 0x00;

			Scene_Data_Common.Scene_Data[i].Hour  				= 0x00;
			Scene_Data_Common.Scene_Data[i].Minute  			= 0x00; 

			Scene_Data_Common.Scene_Data[i].Relay_Stt  			= 0x00;
			Scene_Data_Common.Scene_Data[i].Dim_per  		 	= 0x00;
	}
	
			Scene_Data_Common.Amount_Scene = 0; 
	/*-------------------------Save to Flash-----------------------*/ 
	RD_Scene_SaveDataFlash();


}
/*
 * Delete Scene 
*/
void RD_Delete_Scene(uint16_t Scene_ID_in)
{

	if(Scene_ID_in == 0xffff)  // delete all Scene 
	{
		RD_Delete_Scene_All();
	}
	else						// delete 1 scene
	{
		for(int i=0; i<Scene_Data_Common.Amount_Scene ; i++)
		{
			uint16_t Check_G_ID_Buff =0x00;

			Check_G_ID_Buff =  Scene_Data_Common.Scene_Data[i].Scene_ID;

			if(Check_G_ID_Buff == Scene_ID_in)																			// Null 
			{
				/*-------------------------Update data to Struct----------------*/
				Scene_Data_Common.Scene_Data[i].Enable_ID		= 0x00;    
				
				Scene_Data_Common.Scene_Data[i].Scene_ID	 	= 0x00;
				
				Scene_Data_Common.Scene_Data[i].Start_Date 	    = 0x00;
				
				Scene_Data_Common.Scene_Data[i].Stop_Date     	= 0x00;
				Scene_Data_Common.Scene_Data[i].Month   	  	= 0x00;
				Scene_Data_Common.Scene_Data[i].Date   			= 0x00;
				Scene_Data_Common.Scene_Data[i].Week_Day  		= 0x00;

				Scene_Data_Common.Scene_Data[i].Hour  			= 0x00;
				Scene_Data_Common.Scene_Data[i].Minute  		= 0x00; 

				Scene_Data_Common.Scene_Data[i].Relay_Stt  		= 0x00;
				Scene_Data_Common.Scene_Data[i].Dim_per  		= 0x00;

				Scene_Data_Common.Amount_Scene--;


				//   back data end Scene to position Scene delete before
				Scene_Data_Common.Scene_Data[i].Enable_ID		= Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Enable_ID;
				
				Scene_Data_Common.Scene_Data[i].Scene_ID	 	= Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Scene_ID;
				
				Scene_Data_Common.Scene_Data[i].Start_Date 	    = Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Start_Date;
				
				Scene_Data_Common.Scene_Data[i].Stop_Date  	    = Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Stop_Date;

				Scene_Data_Common.Scene_Data[i].Month   	  	= Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Month;
				Scene_Data_Common.Scene_Data[i].Date   			= Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Date;
				Scene_Data_Common.Scene_Data[i].Week_Day  		= Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Week_Day;

				Scene_Data_Common.Scene_Data[i].Hour  			= Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Hour;
				Scene_Data_Common.Scene_Data[i].Minute  		= Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Minute;
	
	
				Scene_Data_Common.Scene_Data[i].Relay_Stt  		=  Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Relay_Stt;
				Scene_Data_Common.Scene_Data[i].Dim_per  		=  Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Dim_per;


				// dell data end Scene Backuped
				Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Enable_ID 			= 0x00;
				
				Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Scene_ID	 	    = 0x00;
				
				Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Start_Date		    = 0x00;
				
				Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Stop_Date  		= 0x00;

				Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Month   	   		= 0x00;
				Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Date   			= 0x00;
				Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Week_Day  			= 0x00;

				Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Hour  				= 0x00;
				Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Minute  			= 0x00; 

				Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Relay_Stt  		= 0x00;
				Scene_Data_Common.Scene_Data[Scene_Data_Common.Amount_Scene].Dim_per  		 	= 0x00;

				/*-------------------------Save to Flash-----------------------*/ 
	        RD_Scene_SaveDataFlash();
			break; // getout of functiuon
			}

			if(i == (Scene_Data_Common.Amount_Scene-1)  )
			{
				ESP_LOGI(SCENE_TAG,"Delete  Error no have Scene ID \n");  
			} 
		}
	}
}
// check time input with real time and return active Scene_Stt
// 0-not Active
// 1 - Active Scene
uint8_t RD_Check_Time_Scene(uint32_t Start_date_in, uint32_t Stop_date_in, uint8_t Month_in, uint8_t Date_Month_in, uint8_t Week_Day_in, uint8_t Hour_in, uint8_t Minute_in )
{
	uint8_t Scnene_Active 	= 0; 
	uint8_t Date_Flag 		= 0;
	uint8_t Month_Flag		= 0;
	uint8_t Date_Month_Flag = 0;
	uint8_t Week_Day_Flag 	= 0;
	uint8_t Hour_Flag		= 0;
	uint8_t Minute_Flag		= 0;

	uint32_t Date_Buff          = Time_Scene_Common.Date;
	uint8_t  Month_Buff			= Time_Scene_Common.Month;
	uint8_t  Date_Month_Buff	= Time_Scene_Common.Date_in_Month;
	uint8_t  Week_Day_Buff		= Time_Scene_Common.Week_Day;

	//T?
	if(Week_Day_Buff == 0)// Sunday format time.h
	{
		Week_Day_Buff = 6;	// format RD_outDoor 
	}
	else
	{
		Week_Day_Buff = Week_Day_Buff -1; //
	}
	uint8_t  Hour_Buff  		= Time_Scene_Common.Hour;
	uint8_t  Minute_Buff        = Time_Scene_Common.Minute;

	if ((Start_date_in <= Date_Buff) && (Stop_date_in >= Date_Buff))			// check time Start and stop Scene
	{
		Date_Flag =1;
	}

	if ((Month_in == 0xff) || (Month_in == Month_Buff))							// check Month
	{
		Month_Flag =1;
	}

	if ((Date_Month_in == 0xff) || (Date_Month_in == Date_Month_Buff))			// check date in month
	{
		Date_Month_Flag =1;
	}

	if((Week_Day_in == 0xff) || ( (Week_Day_in >> Week_Day_Buff ) & 0x01) )	// check day in week	Note. config by bit
	{
		Week_Day_Flag =1;
	}

	if((Hour_in == 0xff) || (Hour_in == Hour_Buff))
	{
		Hour_Flag =1;
	}

	if((Minute_in == 0xff) || (Minute_in == Minute_Buff)) 		
	{
		Minute_Flag =1;
	}

	if(     (Date_Flag==1) && (Month_Flag==1) && (Date_Month_Flag == 1)  \
		&& (Week_Day_Flag==1) && (Hour_Flag==1) && (Minute_Flag==1))
	{
		Scnene_Active = true;		// Scene active
	}

	return Scnene_Active;
}


//  check tat ca Scene va bat Scene gan nhat. ham nay khong nam torng vong lap
void RD_Check_Scene_PowOn(void)
{
	uint8_t  Active_Buff =0;
	uint8_t Dim_Buff =0;
	uint16_t Scene_ID_Active_Buff =0;
	uint8_t Relay_Buff=0;
	uint16_t Minute_Active_Last_Buff = 0;			// check time Scene active nearest
	if((Scene_Data_Common.Enable_Function == true) && (light_stt_val.Cancel_Scene == false))
	{
		for(int i=0; i <Scene_Data_Common.Amount_Scene; i++)
		{ 
			
			//Check_S_ID_Buff = (uint16_t) ((Scene_Data_Common.Scene_Data[i].Scene_ID[0] << 8) | (Scene_Data_Common.Scene_Data[i].Scene_ID[1]));
			uint32_t Start_Date_Buff=0;
			Start_Date_Buff = Scene_Data_Common.Scene_Data[i].Start_Date;

			uint32_t Stop_Date_Buff=0;
			Stop_Date_Buff = Scene_Data_Common.Scene_Data[i].Stop_Date;

		 	uint8_t Month_Buff 		= Scene_Data_Common.Scene_Data[i].Month; 
		 	uint8_t Date_Buff  		= Scene_Data_Common.Scene_Data[i].Date;
		 	uint8_t Week_Day_Buff 	= Scene_Data_Common.Scene_Data[i].Week_Day;
		 	uint8_t Hour_Buff 		= Scene_Data_Common.Scene_Data[i].Hour;
		 	uint8_t Minute_Buff     = Scene_Data_Common.Scene_Data[i].Minute;
		 	
		 	uint16_t Minute_Actived_Buff 	= Hour_Buff*60 + Minute_Buff; // check time active nearest
		 	uint16_t Minute_Now_Buff		= Time_Scene_Common.Hour * 60  + Time_Scene_Common.Minute;

			uint8_t Scene_Active_Buff =  RD_Check_Time_Scene(Start_Date_Buff, Stop_Date_Buff, Month_Buff, Date_Buff, Week_Day_Buff, 0xff, 0xff);
			
			uint8_t ID_Ennable_Buff		= Scene_Data_Common.Scene_Data[i].Enable_ID;
			if((Scene_Active_Buff == 1) && (ID_Ennable_Buff == true) && (Minute_Active_Last_Buff <= Minute_Actived_Buff) && (Minute_Actived_Buff <= Minute_Now_Buff))
			{
				Active_Buff =1;
				Scene_ID_Active_Buff = Scene_Data_Common.Scene_Data[i].Scene_ID;
				Dim_Buff   = Scene_Data_Common.Scene_Data[i].Dim_per;
				Relay_Buff = Scene_Data_Common.Scene_Data[i].Relay_Stt;
				Minute_Active_Last_Buff = Minute_Actived_Buff;
			}
		}
  }

 
// active Scene has time nearest
  if (Active_Buff == 1)
  { 
  	light_stt_val.Dim_target     = Dim_Buff;
  	light_stt_val.Relay_target   = Relay_Buff; 
	light_stt_val.Scene_ID_Active = Scene_ID_Active_Buff;

	ESP_LOGI(SCENE_TAG, "Start Power. Active Scene: 0x%x, Dim: %d , Relay: %d \n", Scene_ID_Active_Buff, Dim_Buff, Relay_Buff );
		
  }

  else
  {
 	
    light_stt_val.Scene_ID_Active  = 0x0000;
    ESP_LOGI(SCENE_TAG, "No Scene to call. set Dim defaul, Dim: %d , Relay: %d \n", light_stt_val.Dim_target , light_stt_val.Relay_target );

  }
}


// check time and call Scene 
void RD_Check_Scene_Loop(void)
{
	uint8_t  Active_Buff =0;
	uint8_t  Dim_Buff =0;
	uint16_t Scene_ID_Active_Buff =0;
	uint8_t  Relay_Buff=0;
	static uint16_t Last_Time_Check = 0;

	uint16_t Minute_Now_Buff	=0; 
	Minute_Now_Buff = (uint8_t)		(Time_Scene_Common.Minute);
	if((Scene_Data_Common.Enable_Function == true) && (Minute_Now_Buff != Last_Time_Check)  )
	{
		for(int i=0; i <Scene_Data_Common.Amount_Scene; i++)
		{ 
			//Check_S_ID_Buff = (uint16_t) (Scene_Data_Common.Scene_Data[i].Scene_ID[0] << 8) | (Scene_Data_Common.Scene_Data[i].Scene_ID[1]);

			uint32_t Start_Date_Buff = Scene_Data_Common.Scene_Data[i].Start_Date;
			uint32_t Stop_Date_Buff  = Scene_Data_Common.Scene_Data[i].Stop_Date;
			uint8_t Month_Buff 		 = Scene_Data_Common.Scene_Data[i].Month; 
		 	uint8_t Date_Buff  		 = Scene_Data_Common.Scene_Data[i].Date;
		 	uint8_t Week_Day_Buff 	 = Scene_Data_Common.Scene_Data[i].Week_Day;
		 	uint8_t Hour_Buff 		 = Scene_Data_Common.Scene_Data[i].Hour;
		 	uint8_t Minute_Buff      = Scene_Data_Common.Scene_Data[i].Minute;
            uint8_t ID_Ennable_Buff	 = Scene_Data_Common.Scene_Data[i].Enable_ID;
 		  
		 	uint8_t Scene_Active_Buff = RD_Check_Time_Scene(Start_Date_Buff, Stop_Date_Buff, Month_Buff, Date_Buff, Week_Day_Buff, Hour_Buff, Minute_Buff);
						
			if((Scene_Active_Buff == 1) && (ID_Ennable_Buff == true))
			{
				Active_Buff =1;
				Scene_ID_Active_Buff = Scene_Data_Common.Scene_Data[i].Scene_ID;
				Dim_Buff   = Scene_Data_Common.Scene_Data[i].Dim_per;
				Relay_Buff = Scene_Data_Common.Scene_Data[i].Relay_Stt; 
				break;
			}
		}
  }

  if (Active_Buff == 1)
  {  
  	light_stt_val.Dim_target   = Dim_Buff;
  	light_stt_val.Relay_target = Relay_Buff;
  	light_stt_val.Scene_ID_Active  = Scene_ID_Active_Buff;

    ESP_LOGI(SCENE_TAG,"  Active Scene: 0x%x, Dim: %d , Relay: %d \n", Scene_ID_Active_Buff, Dim_Buff, Relay_Buff );
  }

  Last_Time_Check = Minute_Now_Buff;
 
}

static uint32_t  RD_countDay2000(struct tm timeCheck)
{
	const int64_t timeZero_s 	= 946659600; 							// 01 Jan 2000  ()
	int64_t  timeCheck_s 		= mktime(&timeCheck);  					// time (seconds) since 01 Jan 1970

    uint32_t timeDelta_d 		= (timeCheck_s - timeZero_s)/(86400);    // 86400 (seconds)/day

	return timeDelta_d;
}
void RD_Scene_UploadTime(struct tm timeNow)
{
    Time_Scene_Common.Uploaded  	= 1; 	 
    Time_Scene_Common.Date      	= RD_countDay2000(timeNow); 		// Update date from 1/1/2000

    Time_Scene_Common.Month  		= timeNow.tm_mon;					// 0->11
    Time_Scene_Common.Date_in_Month = timeNow.tm_mday; 
    Time_Scene_Common.Week_Day 		= timeNow.tm_wday;					// cn=0   t2->t7 = 1->6

    Time_Scene_Common.Hour 			= timeNow.tm_hour;					// 24h mode
    Time_Scene_Common.Minute 		= timeNow.tm_min;	
}
// loop upload time and call Check Scene
void RD_SceneScan(void)
{	
	static int Last_time_minute = -1;
	static uint8_t Scan_Power_On = 1;
	if( (Last_time_minute != RTC_Val.tm_min) && (Time_RTC_Set_Flag == true) )
	{
		Last_time_minute = RTC_Val.tm_min;
		RD_Scene_UploadTime(RTC_Val);
	}
	if((Scan_Power_On == 1) && (Time_Scene_Common.Uploaded ==1) && (light_stt_val.Safety == true) && (Time_RTC_Set_Flag == true))
	{
		Scan_Power_On=0;
		RD_Check_Scene_PowOn();
	}
	if((Time_Scene_Common.Uploaded ==1) && (light_stt_val.Cancel_Scene == false) && (light_stt_val.Safety == true))
	{
		RD_Check_Scene_Loop();
	}
}



