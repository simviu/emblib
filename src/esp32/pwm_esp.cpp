#include "driver/ledc.h"
#include "esp_err.h"
#include "emb/esp32/embLib_esp.h"

using namespace emb;

#define LEDC_MODE  LEDC_LOW_SPEED_MODE

namespace {
    ledc_channel_t get_ch_enum(int ch)
    {
        return 
            (ch==0)? LEDC_CHANNEL_0 :
            (ch==1)? LEDC_CHANNEL_1 :
            (ch==2)? LEDC_CHANNEL_2 :
            (ch==3)? LEDC_CHANNEL_3 :
            (ch==4)? LEDC_CHANNEL_4 :
            LEDC_CHANNEL_5 ;
            
    }
}

//-----
bool PWM::init()
{
    log_i("    Set PWM pin...");
    //----- allocate timer channel
    int tmr=0; 
    int ch = 0;
    auto& sys = Esp32Sys::inst();
    if(!sys.get_timer(tmr, ch)) {
        log_e("    Failed");
        return false;
    }
    cfg_.ch_id = ch;
    //-----
     // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = (uint32_t)cfg_.freq,  
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));


    //-----
    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = cfg_.pin,
        .speed_mode     = LEDC_MODE,
        .channel        = get_ch_enum(ch), 
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    //------------
    stringstream s;
    s << "      done, pin:" << cfg_.pin;
    s << ", freq:"  << cfg_.freq << ", ch_id:" << cfg_.ch_id; 
    log_i(s.str());

    return true;
}

//----
bool PWM::set_duty(float duty)
{
    // e.g.:
    // // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
    int dn = 8191 * duty;
    auto& ch = cfg_.ch_id;

    ledc_channel_t ch_e = get_ch_enum(ch);
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, ch_e, dn));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, ch_e));

    //stringstream s;
    //s << " PWM::set_duty(), duty=" << duty << ", dn=" << dn;    
    //log_i(s.str());
    return true;
}

