/*
   Author: Sherman Chen
   Create Time: 2022-01-27
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */


#include "emb/embLib.h"

using namespace emb;

//----
bool Motor::init()
{
    
    pwm1_.cfg_.pin = cfg_.pin1;
    pwm2_.cfg_.pin = cfg_.pin2;
    bool ok = true;
    ok &= pwm1_.init();
    ok &= pwm2_.init();
    return true;
}
//----
bool Motor::set(float spd, bool fwd)
{
    bool ok = true;
    float sd = cfg_.b_slow_decay ? 1.0 : 0.0;
    if(fwd)
    {
        ok &= pwm1_.set_duty(spd);
        ok &= pwm2_.set_duty(sd);
    }
    else{
        ok &= pwm1_.set_duty(sd);
        ok &= pwm2_.set_duty(spd);
    }
    return true;
}
