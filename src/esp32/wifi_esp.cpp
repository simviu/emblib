#include "emb/esp32/embLib_esp.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

using namespace emb;
//----
class WifiEsp : public Wifi
{
public:
    virtual bool connect()override;
protected:
    bool hasInit_ = false;
    bool init();

};
//----
bool WifiEsp::init()
{
    
    return true;
}
//----
bool WifiEsp::connect()
{
    bool ok = true;
    if(!hasInit_)
        ok = init();
    if(!ok) return false;
    return true;
}

//----
Wifi& Wifi::inst()
{
    static Wifi i_; 
    return i_; 
}
//----
bool Wifi::connect()
{  
    return true;  
}
