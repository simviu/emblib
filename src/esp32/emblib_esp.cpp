#include "emb/esp32/embLib_esp.h"

using namespace emb;


//---- global esp32 sys
namespace 
{
    struct Def{
        int max_timer_ch_id = 6;
    }; const Def def_;
    //----
    struct TimerDt{
        int ch_idx = 0;
    }; TimerDt timerd_;
} 


//---- singleton
Esp32Sys& Esp32Sys::inst()
{
    static Esp32Sys sys_;
    return sys_;
}
//-----
bool Esp32Sys::get_timer(int& timer_id, int& ch_id)
{
    timer_id = 0;
    ch_id = timerd_.ch_idx;
    if(ch_id > def_.max_timer_ch_id)
    {
        log_e("Failed to get timer, out of timer channel.");
        return false;        
    }
    timerd_.ch_idx ++;
    return true;
}
