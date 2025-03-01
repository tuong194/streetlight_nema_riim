#include "rd_spi.h"

static const char *TAG = "SPI_BL0942";
spi_device_handle_t spi;

void spi_master_init(void) {
    // Cấu hình bus SPI
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,      // Chân MISO
        .mosi_io_num = PIN_NUM_MOSI,      // Chân MOSI
        .sclk_io_num = PIN_NUM_CLK,      // Chân SCLK
        .quadwp_io_num = -1,             // Không sử dụng WP (Write Protect)
        .quadhd_io_num = -1,             // Không sử dụng HD (Hold)
        .max_transfer_sz = SPI_BUF_MAX          // Kích thước truyền tối đa
    };

    // Cấu hình thiết bị SPI
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = FREQ_SPI * 1000,          // Clock 10 MHz
        .mode = 1,                            // Chế độ SPI 0
        .spics_io_num = -1,                  // Chân CS
        .queue_size = 7,                    // Hàng đợi 7 giao dịch
        .pre_cb = NULL,                    // Không cần callback trước khi truyền
    };

    // Khởi tạo bus SPI không DMA (dma_chan = 0)
    spi_bus_initialize(HSPI_HOST, &buscfg, 0);

    // Gắn thiết bị slave vào bus SPI
    spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
}

/*read data signed*/
int32_t spi_read_register_signed(uint8_t reg_addr){
    uint8_t tx_data[6];
    uint8_t rx_data[6];
    // Tạo giao dịch SPI
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 6*8;  
    tx_data[0] = 0x58;             // Truyền và nhận 2 byte
    tx_data[1] = reg_addr;
    tx_data[2] = 0;
    tx_data[3] = 0;
    tx_data[4] = 0;    
    tx_data[5] = 0;   
    t.tx_buffer = tx_data;        // Dữ liệu gửi đi (địa chỉ thanh ghi)
    t.rx_buffer = rx_data;        // Buffer nhận dữ liệu trả về
    t.rxlength = 6*8;              // Số bit nhận
    // Thực hiện giao dịch SPI
    esp_err_t ret = spi_device_transmit(spi, &t);
    if (ret != ESP_OK) {
        printf("SPI transaction failed: %s\n", esp_err_to_name(ret));
        return 0;
    }
    uint32_t CRC_check_temp = (0x58 + reg_addr + rx_data[2] +rx_data[3]+rx_data[4]);
    uint8_t CRC_check = CRC_check_temp & 0xff;
    CRC_check =~ CRC_check;
    if(CRC_check == rx_data[5])
    {
        int32_t data_out = rx_data[2] << 16 | rx_data[3] << 8 | rx_data[4] ;
        if (data_out & 0x800000) {
            // Số âm, ta cần mở rộng bit dấu
            data_out |= 0xFF000000;  // Gán các bit cao hơn 24 thành 1 (bù 2 mở rộng)
        }
        return data_out;
    }
    return 0;
}

uint32_t spi_read_register_unsigned(uint8_t reg_addr) {
    uint8_t tx_data[6];
    uint8_t rx_data[6];
    // Tạo giao dịch SPI
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 6*8;  
    tx_data[0] = 0x58;             // Truyền và nhận 2 byte
    tx_data[1] = reg_addr;
    tx_data[2] = 0;
    tx_data[3] = 0;
    tx_data[4] = 0;    
    tx_data[5] = 0;   
    t.tx_buffer = tx_data;        // Dữ liệu gửi đi (địa chỉ thanh ghi)
    t.rx_buffer = rx_data;        // Buffer nhận dữ liệu trả về
    t.rxlength = 6*8;              // Số bit nhận

    // Thực hiện giao dịch SPI
    esp_err_t ret = spi_device_transmit(spi, &t);
    if (ret != ESP_OK) {
        printf("SPI transaction failed: %s\n", esp_err_to_name(ret));
        return 0;
    }
    uint32_t data_out = rx_data[2] << 16 | rx_data[3] << 8 | rx_data[4] ;
    uint32_t CRC_check_temp = (0x58 + reg_addr + rx_data[2] +rx_data[3]+rx_data[4]);
    uint8_t CRC_check = CRC_check_temp & 0xff;
    CRC_check =~ CRC_check;
    if (CRC_check == rx_data[5])
    {
        /* code */
        return data_out;
    }
    return 0; 
}

uint8_t RD_send_unlock(void){
    uint8_t tx_data[6];
    uint8_t rx_data[6];
    // Tạo giao dịch SPI
    uint32_t CRC_check_temp = 0xa8 + 0x1D + 0x55;
    uint8_t CRC_check = CRC_check_temp & 0xff;
    CRC_check =~ CRC_check; 
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 6*8;  
    tx_data[0] = 0xa8;             // Truyền và nhận 2 byte
    tx_data[1] = 0x1D;
    tx_data[2] = 0x00;
    tx_data[3] = 0x00;
    tx_data[4] = 0x55;   
    tx_data[5] = CRC_check;   
    t.tx_buffer = tx_data;        // Dữ liệu gửi đi (địa chỉ thanh ghi)
    t.rx_buffer = rx_data;        // Buffer nhận dữ liệu trả về
    t.rxlength = 6*8;              // Số bit nhận
    // Thực hiện giao dịch SPI
    esp_err_t ret = spi_device_transmit(spi, &t);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI transaction failed: %s\n", esp_err_to_name(ret));
        return 0;
    }
    return 1;
}

uint8_t rd_send_setup(uint8_t reg_addr,uint8_t *data_send){
    uint8_t tx_data[6];
    uint8_t rx_data[6];
    ESP_LOGI(TAG,"send set up reg_addr: %2x",reg_addr);
    RD_send_unlock();
    // Tạo giao dịch SPI
    uint32_t CRC_check_temp = (0xa8 + reg_addr + data_send[0] + data_send[1] + data_send[2]);
    uint8_t CRC_check = CRC_check_temp & 0xff;
    CRC_check =~ CRC_check; 
   
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 6*8;  
    tx_data[0] = 0xa8;             // Truyền và nhận 2 byte
    tx_data[1] = reg_addr;
    tx_data[2] = data_send[0];
    tx_data[3] = data_send[1];
    tx_data[4] = data_send[2];   
    tx_data[5] = CRC_check;   
    t.tx_buffer = tx_data;        // Dữ liệu gửi đi (địa chỉ thanh ghi)
    t.rx_buffer = rx_data;        // Buffer nhận dữ liệu trả về
    t.rxlength = 6*8;              // Số bit nhận
    // Thực hiện giao dịch SPI
    esp_err_t ret = spi_device_transmit(spi, &t);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG,"SPI transaction failed: %s\n", esp_err_to_name(ret));
        return 0;
    }
    //ESP_LOGW(TAG,"send set up reg_addr: %2x",spi_read_register_unsigned(reg_addr));
    return 1;
}

