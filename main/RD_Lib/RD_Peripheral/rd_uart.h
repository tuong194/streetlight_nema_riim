//#pragma once
#ifndef RD_UART_H
#define RD_UART_H

#include <stdint.h>
#include <stdbool.h>
#include "string.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "sdkconfig.h"

#if 0
    #define RIIM_UART_ID UART_NUM_0
    #define RIIM_TX_PIN UART_PIN_NO_CHANGE // GPIO22 ESP32-c3
    #define RIIM_RX_PIN UART_PIN_NO_CHANGE // GPIO21 ESP32-c3

    #define GPS_UART_ID UART_NUM_1
    #define GPS_TX_PIN GPIO_NUM_5
    #define GPS_RX_PIN GPIO_NUM_4
#else
    #define RIIM_UART_ID        UART_NUM_1
    #define RIIM_TX_PIN         GPIO_NUM_5 // UART_PIN_NO_CHANGE // GPIO22 ESP32-c3
    #define RIIM_RX_PIN         GPIO_NUM_4 // UART_PIN_NO_CHANGE // GPIO21 ESP32-c3

    #define GPS_UART_ID         UART_NUM_0
    #define GPS_TX_PIN          UART_PIN_NO_CHANGE // GPIO22 ESP32-c3
    #define GPS_RX_PIN          UART_PIN_NO_CHANGE // GPIO21 ESP32-c3
#endif

#define RIIM_UART_BAUD 9600
#define RIIM_UART_TXRX_BUF_SIZE 256

#define GPS_UART_BAUD 9600
#define GPS_UART_RX_BUF_SIZE 1024

void rd_uart_init(void);

#endif