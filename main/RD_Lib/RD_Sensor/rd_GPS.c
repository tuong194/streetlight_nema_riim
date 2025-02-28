#include "rd_GPS.h"

GPS_Para  GPS_Value; 
uint8_t Rx_UART_GPS[GPS_RX_GET_MAX];
static const char *GPS_TAG = "GPS: ";
volatile uint8_t Time_RTC_Set_Flag = 0; // flag check gps, if gps signal is valid, set Time_RTC_Set_Flag = 1

static void L76LB_Transfer_Data(GPS_Para *GPS_Data)
{
 
	//uint8_t Jump_date 	= 0; // jump date for GMT+7
	uint8_t Count_Point_Data = 0;
	uint8_t New_Point_Data		= 0;
	ESP_LOGI(GPS_TAG, "Read %d bytes: '%s'", CHAR_GET_MAX, GPS_Data->str_Para_GPS);
	for(int i=0; i< CHAR_GET_MAX; i++)
	{
		if(GPS_Data->str_Para_GPS[i] == ',' || GPS_Data->str_Para_GPS[i] == 0x0C)
		{
			Count_Point_Data++;
			New_Point_Data = true;
		}
		
		if(New_Point_Data == true)
		{
			New_Point_Data = false;
			
			switch (Count_Point_Data)
			{
				/*----------------------------------Get Time in day---------------------------*/
			case 1:	 
				GPS_Data->Second  	= (uint8_t) ((GPS_Data->str_Para_GPS[i+5]-0x30)*10 + (GPS_Data->str_Para_GPS[i+6]-0x30)); 
				GPS_Data->Minute  	= (uint8_t) ((GPS_Data->str_Para_GPS[i+3]-0x30)*10 + (GPS_Data->str_Para_GPS[i+4]-0x30)); 
				GPS_Data->Hour  	= (uint8_t) ((((GPS_Data->str_Para_GPS[i+1]-0x30)*10 + (GPS_Data->str_Para_GPS[i+2]-0x30)) +7) % 24); // GMT +7- 24h Mode
				//Jump_date 		 	= (uint8_t) ((((GPS_Data->str_Para_GPS[i+1]-0x30)*10 + (GPS_Data->str_Para_GPS[i+2]-0x30)) +7) / 24);//GMT+7-- 24h Mode 
				break;
			case 2:
				/*----------------------------------Get GPS signal Stt-------------------------*/
				if(GPS_Data->str_Para_GPS[i+1] == 'A')
				{
					GPS_Data->Signal_Stt = true;
					//GPS_Signal = RD_OK;
				}
				else
				{
					GPS_Data->Signal_Stt = false;
				}
				break;
			case 3:
					/*----------------------------------Get North GPS signal Stt-------------------------*/
				if(GPS_Data->Signal_Stt == true)
				{
					uint8_t count_num =0;
					
					for(int j=1; j<=7; j++)
					{
						if(GPS_Data->str_Para_GPS[i+j] == '.')
						{
							count_num =j;
							break;
						}
					}
					
					
					uint8_t North_Val_Dec=0;
					uint32_t North_Val_Float =0;
					switch (count_num)
					{
						case 4:
							North_Val_Dec = (GPS_Data->str_Para_GPS[i+1]-0x30);
							North_Val_Float = (GPS_Data->str_Para_GPS[i+2]-0x30)*100000   + (GPS_Data->str_Para_GPS[i+3]-0x30)*10000  \
																+ (GPS_Data->str_Para_GPS[i+5]-0x30)*1000  	+ (GPS_Data->str_Para_GPS[i+6]-0x30)*100 \
																+ (GPS_Data->str_Para_GPS[i+7]-0x30)*10 		+ (GPS_Data->str_Para_GPS[i+8]-0x30);
							
							North_Val_Float = (North_Val_Float*10)/6;
							GPS_Data->North_GPS[0]  = North_Val_Dec;
							GPS_Data->North_GPS[1]  = (uint8_t) (North_Val_Float >> 16) & 0xff;
							GPS_Data->North_GPS[2]  = (uint8_t) (North_Val_Float >> 8)  & 0xff;
							GPS_Data->North_GPS[3]  = (uint8_t) (North_Val_Float) 			 & 0xff;
							break;
						
						case 5:
							North_Val_Dec = (GPS_Data->str_Para_GPS[i+1]-0x30)*10 + (GPS_Data->str_Para_GPS[i+2]-0x30);
							North_Val_Float = (GPS_Data->str_Para_GPS[i+3]-0x30)*100000   + (GPS_Data->str_Para_GPS[i+4]-0x30)*10000  \
																+ (GPS_Data->str_Para_GPS[i+6]-0x30)*1000  	+ (GPS_Data->str_Para_GPS[i+7]-0x30)*100 \
																+ (GPS_Data->str_Para_GPS[i+8]-0x30)*10 		+ (GPS_Data->str_Para_GPS[i+9]-0x30);
						
							North_Val_Float = (North_Val_Float*10)/6;
							GPS_Data->North_GPS[0]  = North_Val_Dec;
							GPS_Data->North_GPS[1]  = (uint8_t) (North_Val_Float >> 16) & 0xff;
							GPS_Data->North_GPS[2]  = (uint8_t) (North_Val_Float >> 8)  & 0xff;
							GPS_Data->North_GPS[3]  = (uint8_t) (North_Val_Float) 			 & 0xff;
							break;
						
						case 6:
							North_Val_Dec = (GPS_Data->str_Para_GPS[i+1]-0x30)*100 + (GPS_Data->str_Para_GPS[i+2]-0x30)*10 + (GPS_Data->str_Para_GPS[i+3]-0x30);
							North_Val_Float = (GPS_Data->str_Para_GPS[i+4]-0x30)*100000   + (GPS_Data->str_Para_GPS[i+5]-0x30)*10000  \
																+ (GPS_Data->str_Para_GPS[i+7]-0x30)*1000  	+ (GPS_Data->str_Para_GPS[i+8]-0x30)*100 \
																+ (GPS_Data->str_Para_GPS[i+9]-0x30)*10 		+ (GPS_Data->str_Para_GPS[i+10]-0x30);
						
							North_Val_Float = (North_Val_Float*10)/6;
							GPS_Data->North_GPS[0]  = North_Val_Dec;
							GPS_Data->North_GPS[1]  = (uint8_t) (North_Val_Float >> 16) & 0xff;
							GPS_Data->North_GPS[2]  = (uint8_t) (North_Val_Float >> 8)  & 0xff;
							GPS_Data->North_GPS[3]  = (uint8_t) (North_Val_Float) 			 & 0xff;
							break;
						
						default:
							break;
					}
					
					ESP_LOGI(GPS_TAG, "Update Location- N: %u.%lu ",North_Val_Dec, North_Val_Float);
				}
				break;
				// get GPS Location Degree = 0->180 ==> 1/2 map in the world
			case 5:
					/*----------------------------------Get East GPS signal Stt-------------------------*/
				if(GPS_Data->Signal_Stt == true)
				{
					uint8_t count_num =0;
					
					for(int j=1; j<=7; j++)
					{
						if(GPS_Data->str_Para_GPS[i+j] == '.')
						{
							count_num =j;
							break;
						}
					}
					
					
					uint8_t East_Val_Dec=0;
					uint32_t East_Val_Float =0;
					switch (count_num)
					{
						case 4:
							East_Val_Dec = (GPS_Data->str_Para_GPS[i+1]-0x30);
							East_Val_Float = (GPS_Data->str_Para_GPS[i+2]-0x30)*100000   + (GPS_Data->str_Para_GPS[i+3]-0x30)*10000  \
																+ (GPS_Data->str_Para_GPS[i+5]-0x30)*1000  	+ (GPS_Data->str_Para_GPS[i+6]-0x30)*100 \
																+ (GPS_Data->str_Para_GPS[i+7]-0x30)*10 		+ (GPS_Data->str_Para_GPS[i+8]-0x30);
							
							East_Val_Float = (East_Val_Float*10)/6;
							GPS_Data->East_GPS[0]  = East_Val_Dec;
							GPS_Data->East_GPS[1]  = (uint8_t) (East_Val_Float >> 16) & 0xff;
							GPS_Data->East_GPS[2]  = (uint8_t) (East_Val_Float >> 8)  & 0xff;
							GPS_Data->East_GPS[3]  = (uint8_t) (East_Val_Float) 			 & 0xff;
							break;
						
						case 5:
							East_Val_Dec = (GPS_Data->str_Para_GPS[i+1]-0x30)*10 + (GPS_Data->str_Para_GPS[i+2]-0x30);
							East_Val_Float = (GPS_Data->str_Para_GPS[i+3]-0x30)*100000   + (GPS_Data->str_Para_GPS[i+4]-0x30)*10000  \
																+ (GPS_Data->str_Para_GPS[i+6]-0x30)*1000  	+ (GPS_Data->str_Para_GPS[i+7]-0x30)*100 \
																+ (GPS_Data->str_Para_GPS[i+8]-0x30)*10 		+ (GPS_Data->str_Para_GPS[i+9]-0x30);
						
							East_Val_Float = (East_Val_Float*10)/6;
							GPS_Data->East_GPS[0]  = East_Val_Dec;
							GPS_Data->East_GPS[1]  = (uint8_t) (East_Val_Float >> 16) & 0xff;
							GPS_Data->East_GPS[2]  = (uint8_t) (East_Val_Float >> 8)  & 0xff;
							GPS_Data->East_GPS[3]  = (uint8_t) (East_Val_Float) 			 & 0xff;
							break;
						
						case 6:
							East_Val_Dec = (GPS_Data->str_Para_GPS[i+1]-0x30)*100 + (GPS_Data->str_Para_GPS[i+2]-0x30)*10 + (GPS_Data->str_Para_GPS[i+3]-0x30);
							East_Val_Float = (GPS_Data->str_Para_GPS[i+4]-0x30)*100000   + (GPS_Data->str_Para_GPS[i+5]-0x30)*10000  \
																+ (GPS_Data->str_Para_GPS[i+7]-0x30)*1000  	+ (GPS_Data->str_Para_GPS[i+8]-0x30)*100 \
																+ (GPS_Data->str_Para_GPS[i+9]-0x30)*10 		+ (GPS_Data->str_Para_GPS[i+10]-0x30);
						
							East_Val_Float = (East_Val_Float*10)/6;
							GPS_Data->East_GPS[0]  = East_Val_Dec;
							GPS_Data->East_GPS[1]  = (uint8_t) (East_Val_Float >> 16) & 0xff;
							GPS_Data->East_GPS[2]  = (uint8_t) (East_Val_Float >> 8)  & 0xff;
							GPS_Data->East_GPS[3]  = (uint8_t) (East_Val_Float) 			 & 0xff;
							break;
						
						default:
							break;
					}
					
					ESP_LOGI(GPS_TAG, "Update Location- E: %u.%lu ",East_Val_Dec, East_Val_Float);
					
				}
				break;
				/*-------------------------------------Get Date-Month-year-----------------------*/
			case 9:
				if(GPS_Data->str_Para_GPS[i+1] < 0x30 || GPS_Data->str_Para_GPS[i+1] > 0x39) // GPS format V1.03. don't have real time
				{
					GPS_Data->Date   =  00;
					GPS_Data->Month  =  00;
					GPS_Data->Year   =  80;
				}
				else
				{
					GPS_Data->Date   =  (uint8_t) ((GPS_Data->str_Para_GPS[i+1]-0x30)*10 + (GPS_Data->str_Para_GPS[i+2]-0x30)); 
					GPS_Data->Month  =  (uint8_t) ((GPS_Data->str_Para_GPS[i+3]-0x30)*10 + (GPS_Data->str_Para_GPS[i+4]-0x30)); 
					GPS_Data->Year   =  (uint8_t) ((GPS_Data->str_Para_GPS[i+5]-0x30)*10 + (GPS_Data->str_Para_GPS[i+6]-0x30)); 
					//RD_Next_Date(GPS_Data, Jump_date);
				}
				break;
			default:
				break;
				
			}
	  } 
	}
} 


static void GPS_Clear_Str_Data(GPS_Para *GPS_Data)
{
	for(int i=0; i<= CHAR_GET_MAX-1; i++)
	{
		GPS_Data->str_Para_GPS[i] = 0x00;
	}
	
	for(int i=0; i<8; i++)
	{
		GPS_Data->Type_Para_GPS[i] = 0x00;
	}
}



/*
	Received 1 byte data to GPS Data struct
*/
uint8_t GPS_Receive_UART(GPS_Para *GPS_Data, char  Data_In)
{
	uint8_t Transfer_Stt = false;
	static uint8_t Count_Rx;
	static uint8_t Receive_Data ; 
	if(Data_In != 0xff)
	{
		if(Data_In == 0x0A)															// '\n' 
		{
			GPS_Clear_Str_Data(GPS_Data);								// clear Data when end data
			Count_Rx=0;
			Receive_Data=0;																// turn Off Recive Data to
		}
		else
		{
			#if 1
			if(Count_Rx <= 5)
			{
				GPS_Data->Type_Para_GPS[Count_Rx] = Data_In;

				if(strcmp(GPS_Data->Type_Para_GPS, "$GNRMC") == 0)
				{
					Receive_Data=1;														// turn On Recive Data 
				}
				else
				{
					Receive_Data=0;														// turn Off Recive Data 
				}
			}

			if((Receive_Data == 1) && (Count_Rx > 5))
			{
				GPS_Data->str_Para_GPS[Count_Rx-6] = Data_In;
				if(Data_In == 0x2A)   	// '*'
				{		
					L76LB_Transfer_Data(GPS_Data);
					
					#if 1 // clear and restart GPS_Data
					GPS_Clear_Str_Data(GPS_Data);								// clear Data when end data
					Count_Rx=0;
					Receive_Data=0;												// turn Off Recive Data to
					Transfer_Stt = true;
					return Transfer_Stt;
					#endif
				}

			}
			Count_Rx ++;
			if(Count_Rx >=CHAR_GET_MAX) Count_Rx = CHAR_GET_MAX-1;
			#endif
		} 
	}
	return Transfer_Stt;
}


/*
	check GPS signal and converse if have new data 
*/

//extern uint8_t Test_GPS_Flag;
void RD_GPS_Check(void)
{
	uint8_t Retry_GPS =0;
    GPS_Value.New_Data_Ready = true;
	if( GPS_Value.New_Data_Ready == true)
	{
		//Test_GPS_Flag =1; 
		GPS_Value.New_Data_Ready = false;
		for(int i=0; i<= (GPS_RX_GET_MAX-CHAR_GET_MAX); i++)
		{
            uint8_t rx_byte_buff =0x00;
			#if(1) // RD_ESP32_C3
            	uart_read_bytes(GPS_UART_ID, &rx_byte_buff, 1, 100);
			#endif
			if( GPS_Receive_UART( &GPS_Value, rx_byte_buff) == true)
			{
				ESP_LOGI(GPS_TAG, "Update GPS Done- update to RTC date:%d/%d/%d \t Hour:%d:%d:%d \n", GPS_Value.Date, GPS_Value.Month, GPS_Value.Year, GPS_Value.Hour, GPS_Value.Minute, GPS_Value.Second );

				if(GPS_Value.Signal_Stt == true )
				{
					Sensor_Of_Light_Common.GPS_East[0] = GPS_Value.East_GPS[0];
					Sensor_Of_Light_Common.GPS_East[1] = GPS_Value.East_GPS[1];
					Sensor_Of_Light_Common.GPS_East[2] = GPS_Value.East_GPS[2];
					Sensor_Of_Light_Common.GPS_East[3] = GPS_Value.East_GPS[3];

					Sensor_Of_Light_Common.GPS_North[0] = GPS_Value.North_GPS[0];
					Sensor_Of_Light_Common.GPS_North[1] = GPS_Value.North_GPS[1];
					Sensor_Of_Light_Common.GPS_North[2] = GPS_Value.North_GPS[2];
					Sensor_Of_Light_Common.GPS_North[3] = GPS_Value.North_GPS[3];

				// ESP_LOGI(GPS_TAG, "GPS location E: %d:%x:%x:%x", Sensor_Of_Light_Common.GPS_East[0], Sensor_Of_Light_Common.GPS_East[1],Sensor_Of_Light_Common.GPS_East[2],Sensor_Of_Light_Common.GPS_East[3]);
				// ESP_LOGI(GPS_TAG, "GPS location N: %d:%x:%x:%x", Sensor_Of_Light_Common.GPS_North[0], Sensor_Of_Light_Common.GPS_North[1],Sensor_Of_Light_Common.GPS_North[2],Sensor_Of_Light_Common.GPS_North[3]);
					Time_RTC_Set_Flag = 1; // V1.10a. scanScene when GPS = A
				}
				// if(GPS_Value.Year != 80) Time_RTC_Set_Flag = 1;   // GPS Get real Time V1.10a. scanScene when GPS = A

				i = 1000;
			}
			else
			{
				Retry_GPS = true;
			}
			
		}
		
		if(Retry_GPS == true)
		{
			// GPS_Read_Flag = RD_ENABLE;
			// RD_Receive_GPS_Signal();
			// RD_UART_DEBUG_SendString("Retry GPS");
		}
	}
}