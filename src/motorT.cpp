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
bool MotorT::init()
{
    auto& pins = Motor::cfg_.pins;
    if(pins.size()<3)
    { log_e("MotorD expect 3 pins in cfg"); return false; }
    //----    
    io1_.cfg_.pin = pins[0];
    io2_.cfg_.pin = pins[1];
    pwm_.cfg_.pin = pins[2];
    bool ok = true;
    ok &= pwm_.init();
    ok &= io1_.init();
    ok &= io2_.init();
    return ok;
}
//----
bool MotorT::set(float spd)
{
    bool ok = true;
    bool fwd = (spd>0);

    ok &= io1_.write(fwd);
    ok &= io2_.write(!fwd);
    ok &= pwm_.set_duty(fabs(spd));
    return ok;
}
