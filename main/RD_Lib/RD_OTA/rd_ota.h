#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_mac.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/uart.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "../../build/config/sdkconfig.h"
#include <sys/param.h>
#include <esp_http_server.h>
#include <esp_ota_ops.h>

#include "../RD_Light/rd_lightCtr.h"
#include "../RD_Peripheral/rd_uart.h"
// #include "../RD_Peripheral/rd_gpio.hpp"
//#include "../RD_RIIM/rd_riim.hpp"
#define HIDE_SSID_EN   0

#define EXAMPLE_ESP_WIFI_SSID      "RD_ESP32"
#define EXAMPLE_ESP_WIFI_PASS      "RDSL@2804"
#define EXAMPLE_ESP_WIFI_CHANNEL   1
#define EXAMPLE_MAX_STA_CONN       4

#define XMODEM_FRANME_SIZE         128 // 128 byte/ 1 pack send data
#define XMODEM_RSP_SIZE_DF         100
#define XMODEM_PACKET_STR_SIZE     132 // 1soh,
#define XMODEM_SOH_DF               0x01

#define RIIM_OTA_CONFIRM_TIMEOUT_US   1000000 // 1s
#define RIIM_OTA_PACKET_TIMEOUT_US    120000000 // 120s

#define X_STX  0x02
#define X_ACK  0x06
#define X_NAK  0x15

#define RIIM_type "RC188x"
#define XMODEM_DONE_RSP "OKOKRC188x"

//extern uint8_t OTAWifiInitByMac_Flag;
 
extern void RD_OTA_server_setup(char * ssid_setup, char * pass_setup);
extern void wifi_init_softap(char * ssid_setup, char * pass_setup);

#ifdef __cplusplus
}
#endif