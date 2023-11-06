#include "emb/cutil_mcu.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace ut
{
namespace utlog
{

    namespace{
        FuncCbk f_cbk_ = nullptr;
    }
    //-----
    extern void setCallbk(FuncCbk cbk)
    {
        f_cbk_ = cbk;
    }

    //---- base log
    void logs(CStr& s, bool isErr = false)
    {
        if(isErr)
            cerr << s << flush;
        else
            cout << s << flush;
        

        //----
        if(f_cbk_!=nullptr)
            f_cbk_(s);
    }

    //-----
    extern void str(CStr& s)
    { logs(s); }
    extern void dbg(CStr& s)
    { logs("[dbg]:"+s +"\n"); }
    extern void inf(CStr& s)
    { logs("[info]:"+s +"\n"); }
    extern void err(CStr& s)
    { logs("[err]:"+s +"\n", true); }
    extern void errf(CStr& s)
    { logs("[err]: Failed to open file:"+s +"\n", true); }
    //-----
    /*
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
    */
    
}

}
