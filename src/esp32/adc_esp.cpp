#include "emb/esp32/embLib_esp.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"


using namespace emb;

//---- implemented for ESP32C3,
// ESP32 has more channels and 1 more unit.
namespace {
    //----
    bool get_ch(int pin, adc_channel_t& ch)
    {
        switch(pin)
        {
            case 0 : ch= ADC_CHANNEL_0; break;
            case 1 : ch= ADC_CHANNEL_1; break;
            case 2 : ch= ADC_CHANNEL_2; break;
            case 3 : ch= ADC_CHANNEL_3; break;
            case 4 : ch= ADC_CHANNEL_4; break;
            default: return false; break;
        }
        return true;
    }
    //---- There are 2 A/D unit, use only one.
    struct ADC_unitd // ADC unit data
    {
        bool had_init = false; 
        adc_oneshot_unit_handle_t handle;
        //----
        bool chkInit(){
            if(had_init) return true;
            adc_oneshot_unit_init_cfg_t cfg = {
                .unit_id = ADC_UNIT_1 ,
            };
            ESP_ERROR_CHECK(adc_oneshot_new_unit(&cfg, &handle));
            had_init = true;
            log_i("  ADC unit 1 init ok");
            return true;
        };
    }; ADC_unitd adc_ud1;
}

//-----
bool ADC::init()
{
    int pin = cfg_.pin; // ESP32C3 is 0-4
    log_i("  ADC alloc on pin:"+to_string(pin));

    adc_channel_t ch;
    if(!get_ch(pin, ch))
    {
        log_e("  Failed to allocate ADC pin:"+to_string(pin));
        return false;
    }
    //---- always unit1, so far.
    if(!adc_ud1.chkInit())
        return false;

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_6,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_ud1.handle, ch, &config));
    log_i("  ADC channel allocat ok on pin:"+to_string(pin));
    return true; 
};
//----
int  ADC::read()
{ 
    adc_channel_t ch;
    if(!get_ch(cfg_.pin, ch))
        return -1;
    int d=-1;
    ESP_ERROR_CHECK(adc_oneshot_read(adc_ud1.handle, ch, &d));
    return d; 
};
