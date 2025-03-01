#include "rd_riim.h"

const char *RIIM_TAG = "RIIM TAG";
uint8_t Message_Rsp_Last[LENGMESS_RIIMDF] = {0};
#if 1
const uint8_t Net_Key_Default[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
#else
const uint8_t Net_Key_Default[16] = {0x01, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}; // Dev
#warning "WARNING: This is a custom key  test"
#endif

TS_GWIF_IncomingData	*vrts_GWIF_IncomeMessage;
Riim_Data_t Riim_Data_Val_Str = {
    .Mac = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .Version = {0x00, 0x00}};

uint8_t Message_Rsp_Last[LENGMESS_RIIMDF] = {0};
uint8_t vrsc_GWIF_TempBuffer[64] = {0};
uint8_t check;
static uint16_t vrui_GWIF_LengthMeassge;
unsigned int vrui_GWIF_Opcode;

static _Bool vrb_GWIF_RestartMessage = true;
static _Bool vrb_GWIF_UpdateLate = false;
static _Bool vrb_GWIF_CheckNow = false;
static _Bool messageUpdate = false;

TaskHandle_t xTask_RD_Riim_Process_Handle;

void UART_PutArray(uint8_t cmdLength, uint8_t *message)
{
  // const uint8_t messageLength = 32;
  uint8_t mess_buff[LENGMESS_RIIMDF] = {};
  for (int n = 0; n < cmdLength; n++)
  {
    mess_buff[n] = message[n];
  }
  uart_write_bytes(RIIM_UART_ID, mess_buff, LENGMESS_RIIMDF);
}

char Checksum_Cal(TS_GWIF_IncomingData *data, unsigned int length)
{
  char result = (data->Length + data->Opcode[0] + data->Opcode[1]);

  for (int i = 0; i < length - 3; i++)
  {
    result = (result + data->Message[i]);
  }

  result = result & 0xff;
  return result;
}

static void RD_CheckData(void)
{
  // uint16_t Leng_Ring =0; //= Ring_Bufer_RIIM_Rec.Leng;
  size_t Leng_Ring = 0;
  uart_get_buffered_data_len(RIIM_UART_ID, (size_t *)&Leng_Ring);
  if (Leng_Ring >= 1)
  {
    ESP_LOGI(RIIM_TAG, "recive %d byte\n", Leng_Ring);
    if (vrb_GWIF_UpdateLate == false)
    {
      if (vrb_GWIF_RestartMessage == true)
      {
        if (Leng_Ring >= MESSAGE_HEADLENGTH)
        {
          uart_read_bytes(RIIM_UART_ID, &vrsc_GWIF_TempBuffer[0], MESSAGE_HEADLENGTH, 100);
          ESP_LOGI(RIIM_TAG, "read %d byte Mess Riim\n", MESSAGE_HEADLENGTH);
          vrui_GWIF_LengthMeassge = (vrts_GWIF_IncomeMessage->Length) & 0xff;
          vrui_GWIF_Opcode = ((vrts_GWIF_IncomeMessage->Opcode[0] << 8) | vrts_GWIF_IncomeMessage->Opcode[1]) & 0xffff;
          vrb_GWIF_RestartMessage = false;
          messageUpdate = true;
        }
      }
      else
      { // note
        // READ_DataFromRing(&Ring_Bufer_RIIM_Rec, &vrsc_GWIF_TempBuffer[MESSAGE_HEADLENGTH - 1]);
        uart_read_bytes(RIIM_UART_ID, &vrsc_GWIF_TempBuffer[MESSAGE_HEADLENGTH - 1], 1, 100);
        vrui_GWIF_LengthMeassge = (vrts_GWIF_IncomeMessage->Length) & 0xff;
        vrui_GWIF_Opcode = ((vrts_GWIF_IncomeMessage->Opcode[0] << 8) | vrts_GWIF_IncomeMessage->Opcode[1]) & 0xffff;
        messageUpdate = true;
      }
      if (messageUpdate == true)
      {
        messageUpdate = false;
        if ((vrts_GWIF_IncomeMessage->Hearder[0] == 0xaa) && (vrts_GWIF_IncomeMessage->Hearder[1] == 0xf5))
        {
          // uartCsend_DEBUG("ok:\n ");
          ESP_LOGI(RIIM_TAG, "co ban tin\n");
          if ((vrui_GWIF_Opcode == OPCODE_TSPH_MR) ||
              (vrui_GWIF_Opcode == OPCODE_SET_SCENE) ||
              (vrui_GWIF_Opcode == OPCODE_DELETE_SCENE) ||
              (vrui_GWIF_Opcode == OPCODE_EN_DIS_SCENE) ||
              (vrui_GWIF_Opcode == OPCODE_SET_WARING) ||
              (vrui_GWIF_Opcode == OPCODE_SET_DIM_DF) ||
              (vrui_GWIF_Opcode == OPCODE_SET_POWER_TRAN_MR) ||
              (vrui_GWIF_Opcode == OPCODE_KICK_OUT_MR) ||
              (vrui_GWIF_Opcode == OPCODE_DIM_ALL) ||
              (vrui_GWIF_Opcode == OPCODE_DIM_ONE_MR) ||
              (vrui_GWIF_Opcode == OPCODE_ERRO_MESSANGER) ||
              (vrui_GWIF_Opcode == OPCODE_SET_NETKEY_MR) ||
              (vrui_GWIF_Opcode == OPCODE_ASK_NETWORK_INF) ||
              (vrui_GWIF_Opcode == OPCODE_ROUTER_INFO) ||
              (vrui_GWIF_Opcode == OPCODE_REBOOT_MR) ||
              (vrui_GWIF_Opcode == OPCODE_SET_SCENE) ||
              (vrui_GWIF_Opcode == OPCODE_DELETE_SCENE) ||
              (vrui_GWIF_Opcode == OPCODE_EN_DIS_SCENE) ||
              (vrui_GWIF_Opcode == OPCODE_ADD_GROUP_MR) ||
              (vrui_GWIF_Opcode == OPCODE_DELETE_GROUP_MR) ||
              (vrui_GWIF_Opcode == OPCODE_DELETE_GROUP_G) ||
              (vrui_GWIF_Opcode == OPCODE_DIM_GROUP) ||
              (vrui_GWIF_Opcode == OPCODE_SET_DF_POWER) ||
              (vrui_GWIF_Opcode == OPCODE_SET_TIME_RSP) ||
              (vrui_GWIF_Opcode == OPCODE_EN_DIS_SCENE_G) ||
              (vrui_GWIF_Opcode == OPCODE_EN_DIS_SCENE_ALL) ||
              (vrui_GWIF_Opcode == OPCODE_SET_WARING_G) ||
              (vrui_GWIF_Opcode == OPCODE_SET_WARING_All) ||
              (vrui_GWIF_Opcode == OPCODE_SET_DF_POWER_G) ||
              (vrui_GWIF_Opcode == OPCODE_SET_DF_POWER_ALL) ||
              (vrui_GWIF_Opcode == OPCODE_SET_DIM_DF_G) ||
              (vrui_GWIF_Opcode == OPCODE_SET_DIM_DF_ALL) ||
              (vrui_GWIF_Opcode == OPCODE_DELETE_SCENE_G) ||
              (vrui_GWIF_Opcode == OPCODE_DELETE_SCENE_ALL) ||
              (vrui_GWIF_Opcode == OPCODE_PING_DISCONECT) ||
              (vrui_GWIF_Opcode == OPCODE_GPS) ||
              (vrui_GWIF_Opcode == OPCODE_TEST_MODE))
          {
            if (Leng_Ring >= vrui_GWIF_LengthMeassge - MESSAGE_OFFSET)
            {
              // for(int Count = 0; Count < vrui_GWIF_LengthMeassge - MESSAGE_OFFSET; Count ++){
              // 	READ_DataFromRing(&Ring_Bufer_RIIM_Rec, &vrsc_GWIF_TempBuffer[MESSAGE_HEADLENGTH + Count]);
              // }
              uart_read_bytes(RIIM_UART_ID, &vrsc_GWIF_TempBuffer[MESSAGE_HEADLENGTH], vrui_GWIF_LengthMeassge - MESSAGE_OFFSET, 100);
              vrb_GWIF_UpdateLate = false;
              vrb_GWIF_CheckNow = true;
              vrb_GWIF_RestartMessage = true;
            }
            else
            {
              vrb_GWIF_UpdateLate = true;
              vrb_GWIF_RestartMessage = false;
              vrb_GWIF_CheckNow = false;
            }
          }
          else
          {
            vrsc_GWIF_TempBuffer[0] = vrsc_GWIF_TempBuffer[1];
            vrsc_GWIF_TempBuffer[1] = vrsc_GWIF_TempBuffer[2];
            vrsc_GWIF_TempBuffer[2] = vrsc_GWIF_TempBuffer[3];
            vrsc_GWIF_TempBuffer[3] = vrsc_GWIF_TempBuffer[4];
            vrb_GWIF_RestartMessage = false;
            vrb_GWIF_UpdateLate = false;
          }
        }
        else
        {
          vrsc_GWIF_TempBuffer[0] = vrsc_GWIF_TempBuffer[1];
          vrsc_GWIF_TempBuffer[1] = vrsc_GWIF_TempBuffer[2];
          vrsc_GWIF_TempBuffer[2] = vrsc_GWIF_TempBuffer[3];
          vrsc_GWIF_TempBuffer[3] = vrsc_GWIF_TempBuffer[4];
          vrb_GWIF_RestartMessage = false;
          vrb_GWIF_UpdateLate = false;
        }
      }
    }
    else
    {
      if (Leng_Ring >= (vrui_GWIF_LengthMeassge - MESSAGE_OFFSET))
      {
        ESP_LOGI(RIIM_TAG, "Process after\n"); // uart1
        // for(int vrui_Count = 0; vrui_Count < (vrui_GWIF_LengthMeassge - MESSAGE_OFFSET); vrui_Count++){
        // 	READ_DataFromRing(&Ring_Bufer_RIIM_Rec, &vrsc_GWIF_TempBuffer[MESSAGE_HEADLENGTH + vrui_Count]);
        // }
        uart_read_bytes(RIIM_UART_ID, &vrsc_GWIF_TempBuffer[MESSAGE_HEADLENGTH], vrui_GWIF_LengthMeassge - MESSAGE_OFFSET, 100);
        // Ban tin da trung khop cau truc, bat co xu ly ban tin
        vrb_GWIF_UpdateLate = false;
        vrb_GWIF_CheckNow = true;
        vrb_GWIF_RestartMessage = true;
      }
    }
  }
}

static void RD_MessModel(void){
	if(vrb_GWIF_CheckNow)
	{
		vrb_GWIF_CheckNow = false;
		ESP_LOGI(RIIM_TAG,"New:");//uart1

		ESP_LOGI(RIIM_TAG,"Header:0x%2x- 0x%2x",vrts_GWIF_IncomeMessage->Hearder[0],vrts_GWIF_IncomeMessage->Hearder[1]);
		 
		ESP_LOGI(RIIM_TAG,"lenght:0x%2x",vrts_GWIF_IncomeMessage->Length);
		 
		ESP_LOGI(RIIM_TAG,"Opcode:0x%2x",vrui_GWIF_Opcode);
		  
		ESP_LOGI(RIIM_TAG, "\tMessage:");//uart1
		for (int vrui_Count = 0; vrui_Count < vrui_GWIF_LengthMeassge - 2; vrui_Count++){
			ESP_LOGI(RIIM_TAG,"%2x-",vrts_GWIF_IncomeMessage->Message[vrui_Count]);
			
		}
		
		ESP_LOGI(RIIM_TAG, "\n");//uart

		ESP_LOGI(RIIM_TAG,"\tsum_send:0x%2x\n",vrts_GWIF_IncomeMessage->Message[vrui_GWIF_LengthMeassge-3]);
		
		ESP_LOGI(RIIM_TAG,"\tsum_check:0x%2x\n",Checksum_Cal(vrts_GWIF_IncomeMessage,vrui_GWIF_LengthMeassge));
		 
		
		if(vrts_GWIF_IncomeMessage->Message[vrui_GWIF_LengthMeassge-3] !=  Checksum_Cal(vrts_GWIF_IncomeMessage,vrui_GWIF_LengthMeassge))
		{
			ESP_LOGI(RIIM_TAG,"Sai check Sum- Khong xu ly\n");			
		}
		else
		{
				uint16_t OP_CODE_BUFF = ((vrts_GWIF_IncomeMessage->Opcode[0]<<8) | vrts_GWIF_IncomeMessage->Opcode[1])& 0xffff;
			/*------------------------------- Xu ly tung Opcode ---------------------------------------------------------------*/		
				switch(OP_CODE_BUFF)
				{
					case OPCODE_ROUTER_INFO:
 						RD_Model_OPCODE_ROUTER_INFO();
						break;
					case OPCODE_TSPH_MR:
						RD_Model_OPCODE_TSPH();
						break; 
					case OPCODE_SET_WARING:
					RD_Model_OPCODE_SET_WARING();
						break;
					 case OPCODE_SET_DIM_DF:
					 	RD_Model_OPCODE_SET_DIM_DF();
					 	break;
					case OPCODE_SET_POWER_TRAN_MR:
						RD_Model_OPCODE_SET_POWER_TRAN_MR();
						break;
					case OPCODE_SET_NETKEY_MR:
						RD_Model_OPCODE_SET_NETKEY();
						break;
					case OPCODE_KICK_OUT_MR:
						RD_Model_OPCODE_KICK_OUT_MR();
						break;
					case OPCODE_ERRO_MESSANGER:
						RD_Model_OPCODE_ERRO_MESSANGER();
						break; 
					case OPCODE_DIM_ALL:
						RD_Model_OPCODE_DIM_ALL( );
						break; 
					case OPCODE_DIM_ONE_MR:
						RD_Model_OPCODE_DIM_ONE_MR();
						break;
					case OPCODE_ASK_NETWORK_INF:
						RD_Model_OPCODE_ASK_NETWORK_INF();
						break;
					case OPCODE_REBOOT_MR:
						RD_Model_OPCODE_REBOOT_MR();
						break;
					case OPCODE_SET_SCENE:
						RD_Model_OPCODE_SET_SCENE();
						break;
					case OPCODE_DELETE_SCENE:
						RD_Model_OPCODE_DELETE_SCENE();
						break;
					case OPCODE_EN_DIS_SCENE:
						RD_Model_OPCODE_EN_DIS_SCENE();
						break;
					case OPCODE_ADD_GROUP_MR:
						RD_Model_OPCODE_ADD_GROUP_MR();
						break;
					case OPCODE_DELETE_GROUP_MR:
						RD_Model_OPCODE_DELETE_GROUP_MR();
						break;
					case OPCODE_DELETE_GROUP_G:
						RD_Model_OPCODE_DELETE_GROUP_G();
						break;
					case OPCODE_SET_DF_POWER:
						RD_Model_OPCODE_SET_DF_POWER();
						break;
					case OPCODE_SET_TIME_RSP:
						RD_Model_OPCODE_SET_TIME_RSP();
						break;
					case OPCODE_DIM_GROUP:
						RD_Model_OPCODE_DIM_GROUP();
						break;
					case OPCODE_EN_DIS_SCENE_G:
						RD_Model_OPCODE_EN_DIS_SCENE_G();
						break;
					case OPCODE_EN_DIS_SCENE_ALL:
						RD_Model_OPCODE_EN_DIS_SCENE_ALL();
						break;
					case OPCODE_SET_WARING_G:
						RD_Model_OPCODE_SET_WARING_G();
						break;
					case OPCODE_SET_WARING_All:
						RD_Model_OPCODE_SET_WARING_All();
						break;
					case OPCODE_SET_DF_POWER_G:
						RD_Model_OPCODE_SET_DF_POWER_G();
						break;
					case OPCODE_SET_DF_POWER_ALL:
						RD_Model_OPCODE_SET_DF_POWER_ALL();
						break;
					case OPCODE_SET_DIM_DF_G:
						RD_Model_OPCODE_SET_DIM_DF_G();
						break;
					case OPCODE_SET_DIM_DF_ALL:
						RD_Model_OPCODE_SET_DIM_DF_ALL();
						break;
					case OPCODE_DELETE_SCENE_G:
						RD_Model_OPCODE_DELETE_SCENE_G();
						break;
					case OPCODE_DELETE_SCENE_ALL:
						RD_Model_OPCODE_DELETE_SCENE_ALL();
						break;
					case OPCODE_PING_DISCONECT:
						RD_Model_OPCODE_PING_DISCONECT();
						break;
					case OPCODE_GPS:
						RD_Model_OPCODE_GPS();
						break;
					case OPCODE_TEST_MODE:
						RD_Model_OPCODE_TEST_MODE();
						break;

				}
	  }
	}
		
}
static void RD_Riim_Process(void *arg)
{
	while (1)
	{
		RD_CheckData();
		RD_MessModel();
		vTaskDelay(200 / portTICK_PERIOD_MS);
		/* code */
	}
	
}
void rd_riim_init(void){
	vrts_GWIF_IncomeMessage = (TS_GWIF_IncomingData *) (&vrsc_GWIF_TempBuffer[0]);
	xTaskCreate(RD_Riim_Process, "Riim Task:", 2048*2, NULL, configMAX_PRIORITIES-4, &xTask_RD_Riim_Process_Handle);

}

