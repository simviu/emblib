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
    bool get_ch(int chIdx, adc_channel_t& ch)
    {
        switch(chIdx)
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
    int adc_unit_idx_ = 0;
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
    adc_oneshot_unit_handle_t handle;

    if(adc_unit_idx_>1)
    {
        log_e("Only 2 ADC units available(ESP32C3)");
        return false;
    }


    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = (adc_unit_idx_==0)?ADC_UNIT_1 : ADC_UNIT_2,
    };
    adc_unit_idx_++;


    //----
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &handle));
    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(handle, ch, &config));
    adc_handles_[chIdx] = handle;
    return true; 
};
//----
int  ADC::read()
{ 
    int chIdx = cfg_.chIdx; // ESP32C3 is 0-4
    adc_channel_t ch;
    if(!get_ch(chIdx, ch))
        return -1;
    int d=-1;
    auto& handle = adc_handles_[chIdx];
    ESP_ERROR_CHECK(adc_oneshot_read(handle, ch, &d));
    return d; 
};
