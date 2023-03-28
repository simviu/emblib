/*
   Author: Sherman Chen
   Create Time: 2022-01-27
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */


#include "emb/embLib.h"
using namespace emb;

//=================== Raspberry Pi ===============
#ifdef RASPBERRY_PI
#include <pigpio.h>

namespace{
    struct LC{
        int duty_range = 20000; // default 255
    }; LC lc_;
}
//----
bool PWM::init()
{
    if(!Hw::inst().chkInit())
    return false;

    //----
    stringstream s;
    s << "PWM::init(), pin=" << cfg_.pin;
    s << ", duty_range:" << lc_.duty_range << "...";
    log_d(s.str());
    int r = gpioSetPWMrange(cfg_.pin, lc_.duty_range);
    if(r==PI_BAD_USER_GPIO)
        log_e("PI_BAD_USER_GPIO"); 
    else if(r==PI_BAD_DUTYRANGE)
        log_e("PI_BAD_DUTYRANGE"); 
    else return true;

    return false;
}
//----
bool PWM::set_duty(float duty)
{
    int r = gpioPWM(cfg_.pin, duty*lc_.duty_range);
    if(r==0) return true;
    else if(r==PI_BAD_USER_GPIO)
        log_e("PWM::set_duty() PI_BAD_USER_GPIO"); 
    else if(r==PI_BAD_DUTYCYCLE)
        log_e("PWM::set_duty() PI_BAD_DUTYCYCLE"); 
    else 
        log_e("PWM::set_duty() unkown err");
    return false;
}
//-------------- Dummy ------------
#else  // RASPBERRY_PI

bool PWM::init(){ return false; }
bool PWM::set_duty(float duty){ return false; }

#endif // RASPBERRY_PI
//================================================
