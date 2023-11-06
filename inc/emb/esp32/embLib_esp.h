#pragma once

#include "emb/cutil_mcu.h"
#include "emb/embLib.h"


namespace emb{

    //----
    class Esp32Sys{
    public:
        static Esp32Sys& inst(); // Singleton

        //---- There are 2 timer, 6-8 channels.
        // So far we only use high accuracy timer.
        bool get_timer(int& timer_id, int& ch_id);
    protected:

    };
}
