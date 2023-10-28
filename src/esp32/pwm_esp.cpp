#include "driver/ledc.h"
#include "esp_err.h"
#include "emb/esp32/embLib_esp.h"

using namespace emb;

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_FREQUENCY          (50) // Frequency in Hertz. Set frequency at 5 kHz
//#define PWM_FREQ 50 // Hz

bool PWM::init()
{
    log_i("Set PWM pin...");
     // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = (uint32_t)cfg_.freq,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = cfg_.pin,
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    //------------
    stringstream s;
    s << " done, pin:" << cfg_.pin << ", freq:" << cfg_.freq;
    log_i(s.str());

    return true;
}

//----
bool PWM::set_duty(float duty)
{
    // e.g.:
    // // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
    int dn = 8191 * duty;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, dn));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));

    stringstream s;
    //s << " PWM::set_duty(), duty=" << duty << ", dn=" << dn;    
    //log_i(s.str());
    return true;
}

