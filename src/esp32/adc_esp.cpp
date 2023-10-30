#include "emb/esp32/embLib_esp.h"

using namespace emb;

//---- implemented for ESP32C3,
// ESP32 has more channels and 1 more unit.
namespace {
    //----
    bool get_ch(int chIdx, adc_channel_t& ch)
    {
        switch(cfg_.pin)
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
    //----
    adc_oneshot_unit_handle_t adc_handles_[10];
}

//-----
bool ADC::init()
{
    int chIdx = cfg_.chIdx; // ESP32C3 is 0-4
    adc_channel_t ch;
    if(!get_ch(chIdx, ch))
    {
        log_e("Error ADC channel:"+to_string(chIdx));
        return false;
    }
    //---- always unit0, so far.
    adc_oneshot_unit_handle_t& adc1_handle = adc_handles_[chIdx];
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));
    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = EXAMPLE_ADC_ATTEN,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ch, &config));
    return true; 
};
//----
int  ADC::read()
{ 
    int chIdx = cfg_.pin; // ESP32C3 is 0-4
    adc_channel_t ch;
    if(!get_ch(chIdx, ch))
        return -1;
    int d=-1;
    adc_oneshot_unit_handle_t& adc1_handle = adc_handles_[chIdx];
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ch, &d));
    return d; 
};
