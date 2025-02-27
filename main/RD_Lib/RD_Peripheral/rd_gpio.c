#include "rd_gpio.h"

void gpio_set_pin_input(gpio_num_t GPIO_NUM, gpio_int_type_t INTR_TYPE)
{
    gpio_config_t gpio_config_pin = {
        .pin_bit_mask = 1ULL << GPIO_NUM,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = INTR_TYPE};
    gpio_config(&gpio_config_pin);
    // gpio_evt_queue = xQueueCreate(30, sizeof(uint32_t));
}

void gpio_set_pin_output(gpio_num_t GPIO_NUM)
{
    gpio_config_t gpio_config_pin = {
        .pin_bit_mask = 1ULL << GPIO_NUM,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&gpio_config_pin);
}

void ledc_pwm_init_timer(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY, // Set output frequency at ... kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
}

void ledc_pwm_init_channel(gpio_num_t GPIO_NUM, ledc_channel_t LEDC_CHANNEL_RD)
{
    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL_RD,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = GPIO_NUM,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void rd_set_duty(ledc_channel_t LEDC_CHANNEL_RD, uint32_t duty)
{
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_RD, duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_RD);
}

