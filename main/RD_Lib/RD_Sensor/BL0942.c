#include "BL0942.h"

static const char *BL0942_TAG = "BL0942: ";
static uint32_t U_in, I_in;
static int32_t P_in;
data_bl0942_t data_bl0942;

static void RD_setup_BL0942(void)
{
    uint8_t Set_CF_ZX[3] = {0x00, 0x00, 0x23}; // 0010 0011: ZX 10, CF2 00, CF1 11
    uint8_t Set_Gain[3] = {0x00, 0x00, 0x03};
    uint8_t Set_Soft_Reset[3] = {0x5a, 0x5a, 0x5a};
    rd_send_setup(GAIN_CR, Set_Gain);          // 0x1A
    rd_send_setup(SOFT_RESET, Set_Soft_Reset); // 0x1C
    rd_send_setup(REG_OT_FUNX, Set_CF_ZX);     // 0x18
    ESP_LOGI(BL0942_TAG, "set up OK!!");
}

void read_UIP(void)
{
    float temp_cal;
    U_in = spi_read_register_unsigned(REG_VRMS);
    temp_cal = 2375.72118f / (73989.0f * 510.0f); // temp_U //= (1.218*(390000*5 + 510)*0.001)
    data_bl0942.U_hd = U_in * temp_cal;
    ESP_LOGI(BL0942_TAG, "U hd: %.2f V, ", data_bl0942.U_hd);


    I_in = spi_read_register_unsigned(REG_IRMS);
    temp_cal = 1.218 / 305978; // temp_I
    data_bl0942.I_hd = (I_in * temp_cal);
    ESP_LOGI(BL0942_TAG, "I hd: %.4f A, ", data_bl0942.I_hd);
    
    if (data_bl0942.I_hd < 0.0001)
    {
        data_bl0942.P_hd = 0;
        data_bl0942.Cos_Phi = 0;
    }
    else
    {
        P_in = spi_read_register_signed(REG_WATT);
        temp_cal = 0.001604122; //=((1.218*1.218)*(390000*5 + 510))/(3537*0.001*510*1000*1000)  temp_P
        data_bl0942.P_hd = P_in * temp_cal;
        if (data_bl0942.P_hd < 0 || data_bl0942.P_hd > 10000)
            data_bl0942.P_hd = 0;
        ESP_LOGI(BL0942_TAG, "P hieu dung: %.3f W \n", data_bl0942.P_hd);
        data_bl0942.Cos_Phi = (data_bl0942.P_hd) / ((data_bl0942.U_hd) * (data_bl0942.I_hd));
    }
}

static void rd_bl0942_task(void *arg){
    RD_setup_BL0942();
    while (1)
    {
        read_UIP();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void rd_bl0942_init(void)
{
    RD_setup_BL0942();
}