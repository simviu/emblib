/*
   Author: Sherman Chen
   Create Time: 2023-03-28
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */


#include "emb/embLib.h"
using namespace emb;

//---- platform init
static bool hw_chk_init();

// singleton
Hw& Hw::inst()
{
    static Hw inst_;
    return inst_;
}

bool Hw::chkInit()
{ return hw_chk_init(); }
//=================== Raspberry Pi ===============
#ifdef RASPBERRY_PI

//---
bool hw_chk_init()
{
    static bool bInit = false;
    if(bInit) return true;
    //----
    log_d("pigpio init");
    int r = gpioInitialise();
    if(r>=0) {
        bInit = true;
        return true;
    }
    //----
    stringstream s;
    s >> "pigpio init failed, code:" <<r;
    log_e(s.str());
    
    return false;    
}


#include <pigpio.h>

//--------------- dummy
#else // 
bool hw_chk_init(){ return false; }


#endif // RASPBERRY_PI
