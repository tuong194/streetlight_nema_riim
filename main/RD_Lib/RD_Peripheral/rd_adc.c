#include "rd_adc.h"

static const char *ADC_TAG = "ADC: ";

// static uint16_t adc1_chan_mask = ADC1_BIT_MASK;
static uint8_t adc_dma_result[SIZE_OF_ADC_BUFF] = {0};

int16_t temp_val = 0;
int16_t light_val = 0;

static adc_continuous_handle_t handle = NULL;
static TaskHandle_t s_task_handle;

static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    BaseType_t mustYield = pdFALSE;
    // Notify that ADC continuous driver has done enough number of conversions
    vTaskNotifyGiveFromISR(s_task_handle, &mustYield);
    return (mustYield == pdTRUE);
}

static void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle)
{
    adc_continuous_handle_t handle = NULL;

    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = SIZE_OF_ADC_BUFF, // max size buff
        .conv_frame_size = SIZE_OF_ADC_BUFF,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle));

    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = SAMPLE_FREQ_KHZ * 1000, // 20kHz
        .conv_mode = ADC_CONV_MODE,
        .format = ADC_OUTPUT_TYPE,
    };

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
    dig_cfg.pattern_num = channel_num;
    for (int i = 0; i < channel_num; i++)
    {
        adc_pattern[i].atten = ADC_ATTEN_DB_11;
        adc_pattern[i].channel = channel[i] & 0x7;
        adc_pattern[i].unit = ADC_UNIT;
        adc_pattern[i].bit_width = ADC_WIDTH_BIT_12;

        ESP_LOGI(ADC_TAG, "adc_pattern[%d].atten is :%" PRIx8, i, adc_pattern[i].atten);
        ESP_LOGI(ADC_TAG, "adc_pattern[%d].channel is :%" PRIx8, i, adc_pattern[i].channel);
        ESP_LOGI(ADC_TAG, "adc_pattern[%d].unit is :%" PRIx8, i, adc_pattern[i].unit);
    }
    dig_cfg.adc_pattern = adc_pattern;
    ESP_ERROR_CHECK(adc_continuous_config(handle, &dig_cfg));

    *out_handle = handle;
}

void rd_adc_init(void)
{
    s_task_handle = xTaskGetCurrentTaskHandle();
    static adc_channel_t adc_channel_array[NUM_OF_ADC1_CHANNEL] = {ADC1_CHANNEL_L, ADC1_CHANNEL_T};
    continuous_adc_init(adc_channel_array, NUM_OF_ADC1_CHANNEL, &handle);
    adc_continuous_evt_cbs_t cbs = {
        .on_conv_done = s_conv_done_cb,
    };
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(handle, &cbs, NULL));
    ESP_ERROR_CHECK(adc_continuous_start(handle));
}

void read_temp_light(void)
{
    static uint32_t ret_num = 0;
    int16_t adc1_temp_raw[5] = {0}, adc1_light_raw[5] = {0};     
    esp_err_t ret;                                                            // số byte thực tế trong buff
    //ulTaskNotifyTake(pdTRUE, portMAX_DELAY);                                                     // khi buff_adc đầy mới xử lý đoạn sau ko thì sẽ bị chờ ở đây
    ret = adc_continuous_read(handle, adc_dma_result, SIZE_OF_ADC_BUFF, &ret_num, 0); // ret_num: so byte thuc te trong buff
    
    if(ret == ESP_OK)
    {
        for (int i = 0; i < ret_num; i += ADC_RESULT_BYTE_SIZE)
        {
            adc_digi_output_data_t *p = (adc_digi_output_data_t *)&adc_dma_result[i];
            uint32_t chan_num = ADC_GET_CHANNEL(p);
            uint32_t data = ADC_GET_DATA(p);
            if (chan_num < SOC_ADC_CHANNEL_NUM(ADC_UNIT))
            {
                //ESP_LOGI(ADC_TAG, "Channel: %" PRIu32 ", Value: %" PRIx32, chan_num, data);
                if (chan_num == ADC1_CHANNEL_L)
                {
                    adc1_temp_raw[i / ADC_FRAME_SIZE] = data;
                    //ESP_LOGI(ADC_TAG, "temp[%d] = %d", i / ADC_FRAME_SIZE, adc1_temp_raw[i / ADC_FRAME_SIZE]);
                }
                else if (chan_num == ADC1_CHANNEL_T)
                {
                    adc1_light_raw[i / ADC_FRAME_SIZE] = data;
                    //ESP_LOGI(ADC_TAG, "light[%d] = %d", i / ADC_FRAME_SIZE, adc1_light_raw[i / ADC_FRAME_SIZE]);
                }
            }
            else
            {
                ESP_LOGW(ADC_TAG, "Invalid data [_%" PRIu32 "_%" PRIx32 "]", chan_num, data);
            }
        }
        temp_val = (adc1_temp_raw[0] + adc1_temp_raw[1] + adc1_temp_raw[2] + adc1_temp_raw[3] + adc1_temp_raw[4]) / 5;
        light_val = (adc1_light_raw[0] + adc1_light_raw[1] + adc1_light_raw[2] + adc1_light_raw[3] + adc1_light_raw[4]) / 5;
        ESP_LOGI(ADC_TAG, "temp_val_adc = %d, light_val_adc = %d", temp_val, light_val);

        vTaskDelay(1);
        
    }
    else if (ret == ESP_ERR_TIMEOUT){

    }
}


