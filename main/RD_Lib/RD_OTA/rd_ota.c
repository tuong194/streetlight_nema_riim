#include "rd_ota.h"
#include "stdio.h"
#include "esp_timer.h"


// #ifdef __cplusplus
// extern "C"
// {
// #endif
//void RD_Save_DataConfig(void);

// #ifdef __cplusplus
// }
// #endif
#if(HIDE_SSID_EN)
     #warning "WARNING: Wifi hide"
#endif
//#include "../RD_Light/rd_lightCommon.hpp"




/*
 * Serve OTA update portal (index.html)
 */
extern const uint8_t index_intro_html_start[] asm("_binary_index_intro_html_start");
extern const uint8_t index_intro_html_end[] asm("_binary_index_intro_html_end");

extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");

extern const uint8_t index_setkey_html_start[] asm("_binary_index_setkey_html_start");
extern const uint8_t index_setkey_html_end[] asm("_binary_index_setkey_html_end");

static const char *TAG = "OTA:";

uint8_t Esp32_ssid_ap[32] = EXAMPLE_ESP_WIFI_SSID;
uint8_t Esp32_pass_ap[32] = EXAMPLE_ESP_WIFI_PASS;

/*--------------------------OTA ESP-------------------------*/
uint8_t RD_INFOR[5]  = {0x49,0x4e,0x46,0x4f,0x0d};
uint8_t RD_IMAG1[5]  = {0x49,0x4d,0x41,0x47,0x0d}; 


uint8_t X_EOF[1] = {0x04};

//uint8_t OTAWifiInitByMac_Flag =0; // flag to sign Wifi init ssid and pass form Mac off RIIM. if have no mac. wifi Init ssid and pass as default EXAMPLE_ESP_WIFI_SSID

typedef enum Xmodem_state_t{
	Xmodem_get_infor,
	Xmodem_get_infor_wait,
	Xmodem_imag,
	Xmodem_imag_wait,
	Xmodem_send,
	Xmodem_send_wait,
	Xmodem_end,
	Xmodem_end_wait
}Xmodem_state_t;

typedef struct 
{
	uint8_t SOH; 						//Start of header
	uint8_t PacketNumber; 	// packet of number
	uint8_t C_PacketNumber; // Complement of packet number
	uint8_t Packet[128];		// data
	uint8_t Checksum;				// total checksum 
}Xmodem_data_trans_t;

extern TaskHandle_t xTask_RD_Riim_Process_Handle;

static int Xmodem_read(void *buff, uint32_t buff_len);
static void Xmodem_send_mess(uint8_t *data_in, size_t data_leng);
static void xmodem_next_state(Xmodem_state_t *state_in);
static uint8_t xmodem_checkSum(uint8_t *data);

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG,"station" MACSTR "join, AID=%d",MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

// init wifi 
void wifi_init_softap(char * ssid_setup, char * pass_setup)
{
    ESP_ERROR_CHECK(esp_netif_init());                      //init TCP/IP
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,    // handler even
                                                        NULL,
                                                        NULL));

	//wifi_config_t wifi_config = {0};
    wifi_config_t wifi_config = {
        .ap = {
           // .ssid = *ssid_setup,                                     // hightLight
            //.password = EXAMPLE_ESP_WIFI_PASS,                              // hightLight
           // .ssid_len = strlen((char *) EXAMPLE_ESP_WIFI_SSID),                          
            .ssid_len = strlen(ssid_setup),                          
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,                        // hightLight
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,              
            .max_connection = EXAMPLE_MAX_STA_CONN,
			.ssid_hidden    = HIDE_SSID_EN													
        },
    };

	for(int i=0; i< strlen(ssid_setup); i++)
	{
		wifi_config.ap.ssid[i] = *(ssid_setup + i);
	}
	for(int i=0; i< strlen(pass_setup); i++)
	{
		wifi_config.ap.password[i] = *(pass_setup +i);
	}

	// if((Riim_Data_Val_Str.Version[0]==0x00) && (Riim_Data_Val_Str.Version[1]==0x00) )
	// {
	// 	for(int i=0; i< strlen((char *) Riim_Data_Val_Str.Mac); i++)
	// 	{
	// 		wifi_config.ap.ssid[i] = Riim_Data_Val_Str.Mac[i];
	// 	}
	// }

    if (strlen(ssid_setup) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;                               // hightLight
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             ssid_setup, pass_setup, EXAMPLE_ESP_WIFI_CHANNEL);
}


/**
 * @brief send start xmodem
 * 
 * @param state_in 
 */
static void RD_Xmodem_get_infor(Xmodem_state_t * state_in){
	Xmodem_send_mess(RD_INFOR, 5);
	xmodem_next_state(state_in);
}

/**
 * @brief check rsp of start require
 * 
 * @param state_in 
 */
static void RD_Xmodem_get_infor_wait(Xmodem_state_t * state_in){
	int data_rsp_leng=0;
	uint8_t data_RIIM_rsp[XMODEM_RSP_SIZE_DF] = {0};
	data_rsp_leng = Xmodem_read(data_RIIM_rsp, XMODEM_RSP_SIZE_DF-1);

	static uint8_t timeout_infor=0;
	if ((data_rsp_leng >0) && (strncmp((char*)&data_RIIM_rsp[0],RIIM_type,6) == 0)){
			xmodem_next_state(state_in);
			ESP_LOGI(TAG, "Xmodem get infor done");
	}
	else{
			timeout_infor++;
			if(timeout_infor >=2) {  // timeout and check again
				*state_in = Xmodem_get_infor;
				timeout_infor =0;
			}
	}
}

static void RD_Xmodem_imag(Xmodem_state_t * state_in){
	//uart_flush(RIIM_UART_ID); //T_NOTE
	Xmodem_send_mess(RD_IMAG1, 5);
	xmodem_next_state(state_in);
}

static void RD_Xmodem_imag_wait(Xmodem_state_t * state_in){
	int data_rsp_leng=0;
	uint8_t data_RIIM_rsp[2] = {0};
	data_rsp_leng = Xmodem_read(data_RIIM_rsp, 1);
	if(data_rsp_leng > 0){
		if(data_RIIM_rsp[0] == 0x15){// ACK imag of RIIM 
			xmodem_next_state(state_in);
			ESP_LOGI(TAG, "Xmodem imag done: rsp " );
		}
		else{
			ESP_LOGI(TAG, "Xmodem imag error: rsp 0x%x",data_RIIM_rsp[0] );
		}
	}
}

/**
 * @brief send data update RIIM on Xmodem		
 * 
 * @param state_in Xmodem stt	
 * @param req pointer of http req
 * @param remaining_data num of data remaining 
 */
static void RD_Xmodem_send(Xmodem_state_t * state_in, httpd_req_t *req, size_t remaining_data, Xmodem_data_trans_t *RD_xmodem_data_send){
	//static Xmodem_data_trans_t RD_xmodem_data_send = {0};
	char http_get_data_buff[XMODEM_FRANME_SIZE]= {0}; 			
	int http_data_leng =0;

	RD_xmodem_data_send->SOH							= XMODEM_SOH_DF;
	RD_xmodem_data_send->PacketNumber 		= RD_xmodem_data_send->PacketNumber + 1;
	RD_xmodem_data_send->C_PacketNumber 	= ~RD_xmodem_data_send->PacketNumber;
	http_data_leng = httpd_req_recv(req, http_get_data_buff, MIN(remaining_data, XMODEM_FRANME_SIZE));

	/*--------------------check http data leng-------------------*/
	if(http_data_leng < XMODEM_FRANME_SIZE || remaining_data <= XMODEM_FRANME_SIZE){  
		if(http_data_leng < 0) ESP_ERROR_CHECK(http_data_leng);
		else{ 			// last packet 
			if(remaining_data >0)
			{
				for(int i=0; i< XMODEM_FRANME_SIZE; i++){
					if(i < remaining_data) 	RD_xmodem_data_send->Packet[i] = http_get_data_buff[i];
					else 						RD_xmodem_data_send->Packet[i] = 0xFF;
				}
			}
			*state_in = Xmodem_end;
			ESP_LOGI(TAG, "RIIM OTA send last packet  : %d", RD_xmodem_data_send->PacketNumber);
		}
	}
	else{
		for(int i=0; i< XMODEM_FRANME_SIZE; i++){
			if(i < XMODEM_FRANME_SIZE) 	RD_xmodem_data_send->Packet[i] = http_get_data_buff[i];
		}
		xmodem_next_state(state_in);
	}

	if(remaining_data >0)
	{
		RD_xmodem_data_send->Checksum = xmodem_checkSum(&RD_xmodem_data_send->Packet[0]);
		Xmodem_send_mess(&RD_xmodem_data_send->SOH,sizeof(*RD_xmodem_data_send));
		ESP_LOGI(TAG, "RIIM OTA send packet  : %d", RD_xmodem_data_send->PacketNumber);
	}


}

/**
 * @brief check send data update RIIM on Xmodem	
 * 
 * @param state_in 
 * @param remaining_data 
 */
static void RD_Xmodem_send_wait(Xmodem_state_t * state_in, size_t *remaining_data){
	int data_rsp_leng=0;
	uint8_t data_RIIM_rsp[2] = {0};
	data_rsp_leng = Xmodem_read(data_RIIM_rsp, 1);

	if(data_rsp_leng > 0){
		//ESP_LOGI(TAG, "data_RIIM_rsp[0] %x", data_RIIM_rsp[0]);
		if(data_RIIM_rsp[0] == X_ACK){
			if(*remaining_data <= 128)   remaining_data =0;
			else  							*remaining_data  = *remaining_data - XMODEM_FRANME_SIZE;
			*state_in = Xmodem_send;
		}
		else if(data_RIIM_rsp[0] == X_NAK){
			Xmodem_send_mess(X_EOF, 1);
			*remaining_data =0;
			ESP_LOGI(TAG, "send packet error NACK OTA RIIM falil");
		}
	}
}

static void RD_Xmodem_end(Xmodem_state_t * state_in, size_t *remaining_data){
	int data_rsp_leng=0;
	uint8_t data_RIIM_rsp[2] = {0};
	data_rsp_leng = Xmodem_read(data_RIIM_rsp, 1);

	if(data_rsp_leng > 0){
		if(data_RIIM_rsp[0] == X_ACK){
			xmodem_next_state(state_in);
			Xmodem_send_mess(X_EOF, 1);
			
		}
		else if(data_RIIM_rsp[0] == X_NAK){
			Xmodem_send_mess(X_EOF, 1);
			*remaining_data =0;
			ESP_LOGI(TAG, "send end error NACK OTA RIIM falil");
		}
	}
}

static void RD_Xmodem_end_wait(Xmodem_state_t * state_in, size_t *remaining_data){
	int data_rsp_leng=0;
	uint8_t data_RIIM_rsp[2] = {0};
	data_rsp_leng = Xmodem_read(data_RIIM_rsp, 1);

	if(data_rsp_leng > 0){
	if(data_RIIM_rsp[0] == X_ACK){
		xmodem_next_state(state_in);
		ESP_LOGI(TAG, "OTA RIIM send packet done ");
		*remaining_data =0;
	}
	else if(data_RIIM_rsp[0] == X_NAK){
		ESP_LOGI(TAG, "send end error NACK OTA RIIM falil");
		*remaining_data =0;
	}
	}
}



/*--------------------------------------------HTTP ------------------------------------------------------- */
esp_err_t index_get_handler(httpd_req_t *req)
{
	char html_buffer[2048];
	char key_str[64] = {0};
	for(int i=0; i<16; i++)
	{
		//sprintf(&key_str[i * 2], "%02x", Network_Inf_Common.Netkey[i]); T_NOTE
	}
	snprintf(html_buffer, sizeof(html_buffer), (const char *)  index_intro_html_start,(0x010A >> 8) & 0xFF, 0x010A & 0xFF, key_str);
	httpd_resp_send(req, html_buffer, HTTPD_RESP_USE_STRLEN);

	return ESP_OK;

}

esp_err_t index_ota_get_handler(httpd_req_t *req)
{
	httpd_resp_send(req, (const char *) index_html_start, index_html_end - index_html_start);
	return ESP_OK;
}

esp_err_t index_setkey_get_handler(httpd_req_t *req)
{

	httpd_resp_send(req, (const char *) index_setkey_html_start, index_setkey_html_end - index_setkey_html_start);
	return ESP_OK;
}

/*
 * Handle ESP32 OTA file upload
 */
esp_err_t update_post_handler(httpd_req_t *req)
{
	char buf[1000];
	ESP_LOGI(TAG, "ESP OTA Start");
	esp_ota_handle_t ota_handle;
	int remaining = req->content_len;

	const esp_partition_t *ota_partition = esp_ota_get_next_update_partition(NULL);
	ESP_ERROR_CHECK(esp_ota_begin(ota_partition, OTA_SIZE_UNKNOWN, &ota_handle));

	while (remaining > 0) {
		int recv_len = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)));

		// Timeout Error: Just retry
		if (recv_len == HTTPD_SOCK_ERR_TIMEOUT) {
			continue;

		// Serious Error: Abort OTA
		} else if (recv_len <= 0) {
			httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Protocol Error");
			return ESP_FAIL;
		}

		// Successful Upload: Flash firmware chunk
		if (esp_ota_write(ota_handle, (const void *)buf, recv_len) != ESP_OK) {
			httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Flash Error");
			return ESP_FAIL;
		}

		remaining -= recv_len;
	}

	// Validate and switch to new OTA image and reboot
	if (esp_ota_end(ota_handle) != ESP_OK || esp_ota_set_boot_partition(ota_partition) != ESP_OK) {
			httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Validation / Activation Error");
			return ESP_FAIL;
	}

	httpd_resp_sendstr(req, "Firmware update complete, rebooting now!\n");

	vTaskDelay(5000 / portTICK_PERIOD_MS);
	esp_restart();

	return ESP_OK;
}

esp_err_t update_post_handler_riim(httpd_req_t *req){
	size_t remaining_OTA_data = req->content_len;
	Xmodem_data_trans_t RD_xmodem_data_send = {0};
	

	vTaskSuspend(xTask_RD_Riim_Process_Handle);
	
	ESP_LOGI(TAG, "RIIM OTA Start, data_leng: %lu", (uint32_t)remaining_OTA_data);
	//gpio_set_level(RST_RIIM_PIN, 0); //T_NOTE
	vTaskDelay(10 / portTICK_PERIOD_MS);
	//rd_riim_uart_ota_init();
	//gpio_set_level(RST_RIIM_PIN, 1);
	Xmodem_state_t  xmodem_status = Xmodem_get_infor;

	int64_t RIIM_OTA_StartTickUs = esp_timer_get_time();
	while(remaining_OTA_data >0){
		if(esp_timer_get_time() - RIIM_OTA_StartTickUs >= RIIM_OTA_PACKET_TIMEOUT_US) break; // stop OTA
		switch (xmodem_status)
		{
		case Xmodem_get_infor:
				RD_Xmodem_get_infor(&xmodem_status);
				ESP_LOGI(TAG, "RD_Xmodem_get_infor");
			break;
		case Xmodem_get_infor_wait:
				RD_Xmodem_get_infor_wait(&xmodem_status);
				ESP_LOGI(TAG, "RD_Xmodem_get_infor_wait");
			break;
		case Xmodem_imag:
				RD_Xmodem_imag(&xmodem_status);
				ESP_LOGI(TAG, "RD_Xmodem_imag");
			break;
		case Xmodem_imag_wait:
				RD_Xmodem_imag_wait(&xmodem_status);
				ESP_LOGI(TAG, "RD_Xmodem_imag_wait");
			break;
		case Xmodem_send:
				RD_Xmodem_send(&xmodem_status, req, remaining_OTA_data, &RD_xmodem_data_send);
				ESP_LOGI(TAG, "RD_Xmodem_send");
			break;
		case Xmodem_send_wait:
				RD_Xmodem_send_wait(&xmodem_status, &remaining_OTA_data);
				ESP_LOGI(TAG, "RD_Xmodem_send_wait");
			break;
		case Xmodem_end:
			RD_Xmodem_end(&xmodem_status, &remaining_OTA_data);
				ESP_LOGI(TAG, "RD_Xmodem_end");

			break;
		case Xmodem_end_wait:
			RD_Xmodem_end_wait(&xmodem_status, &remaining_OTA_data);
				ESP_LOGI(TAG, "RD_Xmodem_end_wait");

			break;
		default:
			break;
		}
	}
	//uart_flush(RIIM_UART_ID); T_NOTE
	vTaskDelay(2000 / portTICK_PERIOD_MS);

	int data_rsp_leng=0;
	uint8_t data_RIIM_rsp[10] = {0};
	data_rsp_leng = Xmodem_read(data_RIIM_rsp, 10);
	for(int i = 0; i < 10; i++){
		ESP_LOGI(TAG, "RIIM OTA confirm: 0x%x", data_RIIM_rsp[i]);
	}
	if(data_rsp_leng > 0){
		if(strncmp((char*) &data_RIIM_rsp[0], XMODEM_DONE_RSP, 10) == 0){
			httpd_resp_sendstr(req, "Firmware update complete riim\n");


		}
		else{
			httpd_resp_sendstr(req, "Firmware update err riim, try again\n");
		}
	}
	else{
		httpd_resp_sendstr(req, "Firmware update err riim111, try again\n");
	}
	//gpio_set_level(RST_RIIM_PIN, 0); //T_NOTE
	vTaskDelay(10 / portTICK_PERIOD_MS);
	//gpio_set_level(RST_RIIM_PIN, 1);
	//rd_riim_uart_use_init();
	//uart_flush(RIIM_UART_ID);
	vTaskResume(xTask_RD_Riim_Process_Handle);
	return ESP_OK;
}

static uint8_t hex_string_to_bytes(const char *hex_string, unsigned char *byte_array, size_t hex_string_length) {
    // Ensure the input is a string of exactly 32 hex characters
    if (hex_string_length != 32) {
        ESP_LOGI(TAG, "Input hex string must be exactly 32 characters long.\n");
        return 0;
    }

    // Process each pair of hex characters
    for (int i = 0; i < hex_string_length; i += 2) {
        // Extract the current pair of hex characters
        char hex_pair[3];
        hex_pair[0] = hex_string[i];
        hex_pair[1] = hex_string[i + 1];
        hex_pair[2] = '\0';

		char *num_err;
        // Convert hex pair to integer
        byte_array[i / 2] = (unsigned char) strtol(hex_pair, &num_err, 16);
		if (*num_err != '\0') {

        	ESP_LOGI(TAG, "key_err");
			return 0;

		} 

    }

	return 1;
}
/*
 * HTTP Server
 */

esp_err_t submit_key_post_handler(httpd_req_t *req) {
     char input_string[100] = {0}; // Chuỗi nhận từ client
    char buf[100];
	uint8_t net_key_buff[16];
    int ret, remaining = req->content_len;
	uint8_t Set_key_stt = 0;
  	ESP_LOGI(TAG, "sub _ key");
    while (remaining > 0) {
        if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf) -1 )))  <= 0) {
              ESP_LOGI(TAG, "HTTPD err ID %d", ret);
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                 ESP_LOGI(TAG, "HTTPD_SOCK_ERR_TIMEOUT");
                continue; // Retry receiving if timeout
            }
            return ESP_FAIL;
        }
        remaining -= ret;
        buf[ret] = '\0'; 
               /* Log data received */
        // Parse the received data (assuming form-urlencoded format)
        char *input = strstr(buf, "inputText=");
        if (input) {
            input += strlen("inputText=");
            strcpy(input_string, input);
            ESP_LOGI(TAG, "Received input: %s, leg: %d", input_string, strlen(input_string));

			Set_key_stt = hex_string_to_bytes(input_string, net_key_buff, strlen(input_string));
        }
    }
	if( 0 == Set_key_stt)
	{
		ESP_LOGI(TAG,"Net key submitted error format");
  		httpd_resp_send(req, "Net key submitted error format", HTTPD_RESP_USE_STRLEN);	
	} else{
		ESP_LOGI(TAG,"Net key submitted success- device will reset");
   		httpd_resp_send(req, "Net key submitted success- device will reset", HTTPD_RESP_USE_STRLEN);
		for(int i=0; i<16; i++)
		{
			//Network_Inf_Common.Netkey[i] 	= net_key_buff[i]; //T_NOTE
		}
			//Flash save
			//RD_Save_DataConfig(); // T_NOTE

			vTaskDelay(1000 / portTICK_PERIOD_MS);
			esp_restart();
		}
    return ESP_OK;
}


esp_err_t set_percentage_handler(httpd_req_t *req) {
    char query[32];
    char value_str[8];
    int value;

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) { //get query string từ request URL ex: /setPercentage?value=75 thì query = value=75
        if (httpd_query_key_value(query, "value", value_str, sizeof(value_str)) == ESP_OK) { // ex: value_str = 75
            value = atoi(value_str);
            ESP_LOGI(TAG, "Received value: %d\n", value);
            const char* response = "Value received";
            httpd_resp_send(req, response, strlen(response));
			light_set_ctr(value); 
            return ESP_OK;
        }
    }
    httpd_resp_send_404(req);
    return ESP_FAIL;
}
httpd_uri_t index_get = {
	.uri	  = "/",
	.method   = HTTP_GET,
	.handler  = index_get_handler,
	.user_ctx = NULL
};

httpd_uri_t set_percentage_uri = {
	.uri       = "/setPercentage",
	.method    = HTTP_GET,
	.handler   = set_percentage_handler,
	.user_ctx  = NULL
};

httpd_uri_t update_post = { // btn ota esp
	.uri	  = "/update",
	.method   = HTTP_POST,
	.handler  = update_post_handler,
	.user_ctx = NULL
};

httpd_uri_t update_post_riim = { // btn ota riim 
	.uri	  = "/update_riim",
	.method   = HTTP_POST,
	.handler  = update_post_handler_riim,
	.user_ctx = NULL
};

httpd_uri_t index_ota_get = { // goto index.html (page ota)
	.uri	  = "/ota",
	.method   = HTTP_GET,
	.handler  = index_ota_get_handler,
	.user_ctx = NULL
};

httpd_uri_t index_setkey_get = { // goto setkey.html (page set key)
	.uri	  = "/setKey",
	.method   = HTTP_GET,
	.handler  = index_setkey_get_handler,
	.user_ctx = NULL
};

httpd_uri_t submit_key_post = { // btn subnet key
	.uri	  = "/submit_netkey",
	.method   = HTTP_POST,
	.handler  = submit_key_post_handler,
	.user_ctx = NULL
};

static esp_err_t http_server_init(void)
{
	static httpd_handle_t http_server = NULL;

	httpd_config_t config = HTTPD_DEFAULT_CONFIG();

	if (httpd_start(&http_server, &config) == ESP_OK) {
		httpd_register_uri_handler(http_server, &index_get);
		httpd_register_uri_handler(http_server, &index_ota_get);
		httpd_register_uri_handler(http_server, &index_setkey_get);
		httpd_register_uri_handler(http_server, &update_post);
		httpd_register_uri_handler(http_server, &update_post_riim);
		httpd_register_uri_handler(http_server, &submit_key_post);
		httpd_register_uri_handler(http_server, &set_percentage_uri);
	}

	return http_server == NULL ? ESP_FAIL : ESP_OK;
}

void RD_OTA_server_setup(char * ssid_setup, char * pass_setup) {
        //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
       ret = nvs_flash_init();
   }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap(ssid_setup, pass_setup);

    ESP_ERROR_CHECK(http_server_init());
}

/**
 * @brief calculate anf return Xmodem check sum from data send 	
 * 
 * @param data : pointer to data send 	
 * @return uint8_t : checksum off data in
 * 
 */
static uint8_t xmodem_checkSum(uint8_t *data){
	uint8_t check_sum_buff =0;

	for(int i=0; i< XMODEM_FRANME_SIZE; i++){
		check_sum_buff = check_sum_buff + data[i];
	}

	return (check_sum_buff & 0xff);
}
/**
 * @brief set next phase for Xmodem state
 * 
 * @param state_in add of Xmodem state valriable
 */
static void xmodem_next_state(Xmodem_state_t *state_in){
	switch (*state_in)
	{
	case Xmodem_get_infor:
		*state_in = Xmodem_get_infor_wait;
		break;
	case Xmodem_get_infor_wait:
		*state_in = Xmodem_imag;
		break;
	case Xmodem_imag:
		*state_in = Xmodem_imag_wait;
		break;	
	case Xmodem_imag_wait:
		*state_in = Xmodem_send;
		break;
	case Xmodem_send:
		*state_in = Xmodem_send_wait;
		break;
	case Xmodem_send_wait:
		*state_in = Xmodem_end;
		break;
	case Xmodem_end:
		*state_in = Xmodem_end_wait;
		break;
	case Xmodem_end_wait:
		*state_in = Xmodem_get_infor;
		break;
	default:
		*state_in = Xmodem_get_infor;
		break;
	}
}

static void Xmodem_send_mess(uint8_t *data_in, size_t data_leng){
  uart_write_bytes(RIIM_UART_ID, (const char *) data_in, data_leng);
}
static int Xmodem_read(void *buff, uint32_t buff_len){
	memset(buff, 0x00, buff_len);
	return uart_read_bytes(RIIM_UART_ID, buff, buff_len, 20 / portTICK_PERIOD_MS); // T_EDIT
}
// #ifdef __cplusplus
// }
// #endif