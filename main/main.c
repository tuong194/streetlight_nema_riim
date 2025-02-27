
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_timer.h"

#include "RD_Lib/rd_lib.hpp"

extern void RD_OTA_server_setup(char * ssid_setup, char * pass_setup);

static const char *MAIN_TAG = "RD_MAIN";

void app_main(void)
{
    ESP_LOGI(MAIN_TAG, "--------------------------StreetLight RangDong ESP32-C3" );
    
    RD_OTA_server_setup(EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);


}


