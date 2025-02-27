#include "rd_lightCtr.h"

volatile light_stt_t light_stt_val = {
    .Dim_target         = DIM_POWER_UP_DF,
    .Dim_current        = 0,
    .Relay_target       = RELAY_POWER_UP_DF,
    .Relay_current      = RD_OFF,
    .Scene_ID_Active    = 0x0000,
    .Cancel_Scene       = 0x00,
    .Error_All          = 0x00,
    .Safety             = true,
    .Time_To_Safe       = 0
};

void light_relay_ctr(uint8_t ctr_val)
{
    //gpio_set_level(RELAY_PIN, ctr_val);
    uint32_t relay_duty_set =0;
    if(ctr_val == 0)    relay_duty_set = 500; // 50%
    else                relay_duty_set = 0;
    rd_set_duty(RELAY_CHANNEL, relay_duty_set);
}

void light_dim_ctr(uint32_t dim_val)
{
    if(dim_val >= 1000) dim_val = 1024;
    rd_set_duty(DIMER_CHANNEL, dim_val);
}
