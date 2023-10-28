#include "emb/esp32/cutils_esp.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace ut
{

    extern void log_i(const string& s)
    {
        //char* p = reinterpret_cast<char*>(s.c_str());
        //ESP_LOGI("[info]:", "this is test");
        string s1 = s + "\n";
        esp_log_write(ESP_LOG_INFO, "info", s1.c_str());
    }
    extern void log_d(const string& s)
    {   esp_log_write(ESP_LOG_DEBUG, "debug", (s+"\n").c_str()); }

    extern void log_w(const string& s)
    {   esp_log_write(ESP_LOG_WARN, "warn", (s+"\n").c_str()); }

    extern void log_e(const string& s)
    {   esp_log_write(ESP_LOG_ERROR, "error", (s+"\n").c_str()); }

    //----
    extern void sys_delay(float t)
    {
        uint32_t n = t*1000;
        vTaskDelay(pdMS_TO_TICKS(n));
    }
}
