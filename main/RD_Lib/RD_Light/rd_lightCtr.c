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
    if(ctr_val == 0)    relay_duty_set = 500; // pwm 50% to off relay
    else                relay_duty_set = 0;
    rd_set_duty(RELAY_CHANNEL, relay_duty_set);
}

void light_dim_ctr(uint32_t dim_val)
{
    if(dim_val >= 1000) dim_val = 1024;
    rd_set_duty(DIMER_CHANNEL, dim_val);
}


void light_set_onoff(uint8_t onoff_set)
{
    light_stt_val.Relay_target  = onoff_set;
}

void light_set_safety(uint8_t safety_set)
{
    light_stt_val.Safety = safety_set;
}

void light_set_dim(uint8_t dim_set)
{
    light_stt_val.Dim_target = dim_set;
}

static void light_relay_ctr_update(void) // control relay
{
    if(light_stt_val.Relay_current != light_stt_val.Relay_target)
    {
        
        if(light_stt_val.Relay_target == RD_ON)  
        {
            light_stt_val.Relay_current = light_stt_val.Relay_target;
            
            //set hardware
            light_relay_ctr(light_stt_val.Relay_current);
        }else
        {
            light_stt_val.Relay_current = light_stt_val.Relay_target;
            
            //set hardware
            light_relay_ctr(light_stt_val.Relay_current);
        }
    }

}

static void light_dim_ctr_update(void) // control dim
{
    static uint16_t transaction_count=0;
    if(light_stt_val.Dim_current != light_stt_val.Dim_target)
    {
        transaction_count++;
        if(transaction_count >= TSS_DEFAULT)
        {
            transaction_count = 0;
            if(light_stt_val.Dim_current < light_stt_val.Dim_target)
            {   
                if(light_stt_val.Relay_current == RD_OFF){
                    // wait to relay on before continuing dim up
                }
                else{
                    light_stt_val.Dim_current++;
                }
            }
             if(light_stt_val.Dim_current > light_stt_val.Dim_target)
            {
                light_stt_val.Dim_current--;
            }

            if(light_stt_val.Dim_current >= 100) 		light_stt_val.Dim_current = 100;
            if(light_stt_val.Dim_current <=0) 		    light_stt_val.Dim_current = 0;  
            // set hardware
            light_dim_ctr(light_stt_val.Dim_current*10);
        }
    }
}

static void light_ctr_updateError(void){

    for( ; light_stt_val.Dim_current == 0; light_stt_val.Dim_current--)
    {
        // set hardware
        light_dim_ctr(light_stt_val.Dim_current*10);
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
    light_stt_val.Relay_current = 0;
    light_relay_ctr(light_stt_val.Relay_current);
}

void light_set_ctr(uint8_t DimSet)
{
	if(DimSet == 0){
		light_stt_val.Relay_target 			= RD_OFF;
	}
	if(DimSet != 0){
		light_stt_val.Relay_target 			= RD_ON;
	}
	
	light_stt_val.Dim_target 			= DimSet;

	light_stt_val.Cancel_Scene 	        = true;
}

void light_ctr_update(void) 
{
    static uint8_t safety_stt_last = true;
    static int64_t last_time_toggle_relay = 0;
    static uint8_t last_level_relay      = 0;
    if(light_stt_val.Safety == true){
       
        light_relay_ctr_update();
        light_dim_ctr_update();
        safety_stt_last = true;
    }
    else{
        if(safety_stt_last == true){
            light_ctr_updateError();
            safety_stt_last = false;
        }

    }
}

