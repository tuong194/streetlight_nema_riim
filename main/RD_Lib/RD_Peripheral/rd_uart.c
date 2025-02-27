#include "rd_uart.h"

static void rd_uart_init_riim(void)
{
    const uart_config_t uart_config = {
        .baud_rate = RIIM_UART_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(RIIM_UART_ID, RIIM_UART_TXRX_BUF_SIZE * 2, RIIM_UART_TXRX_BUF_SIZE * 2, 0, NULL, 0); // use uart_get_buffered_data_len()
    uart_param_config(RIIM_UART_ID, &uart_config);
    uart_set_pin(RIIM_UART_ID, RIIM_TX_PIN, RIIM_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void rd_riim_uart_ota_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200, // ota baud
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(RIIM_UART_ID, RIIM_UART_TXRX_BUF_SIZE * 2, RIIM_UART_TXRX_BUF_SIZE * 2, 0, NULL, 0);
    uart_param_config(RIIM_UART_ID, &uart_config);
    uart_set_pin(RIIM_UART_ID, RIIM_TX_PIN, RIIM_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

static void rd_uart_init_gps(void)
{
    const uart_config_t uart_gps_config = {
        .baud_rate = GPS_UART_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(GPS_UART_ID, GPS_UART_RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(GPS_UART_ID, &uart_gps_config);
    uart_set_pin(GPS_UART_ID, GPS_TX_PIN, GPS_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void rd_uart_init(void)
{
    rd_uart_init_riim();
    rd_uart_init_gps();
}

int sendData(const char* logName, const char* data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}
#if 0
static void tx_task(void *arg)
{
    static const char *TX_TASK_TAG = "UART1 RIIM";
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    while (1) {
        sendData(TX_TASK_TAG, "Hello world");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RIIM_UART_TXRX_BUF_SIZE+1);
    while (1) {
        const int rxBytes = uart_read_bytes(RIIM_UART_ID, data, RIIM_UART_TXRX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
       // uart_
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}
#endif