/*
   Author: Sherman Chen
   Create Time: 2023-03-13
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */


#include "emb/embLib.h"

using namespace emb;

bool ServoPWM::init()
{
    bool ok = PWM::init();
    set_degree(cfg_.dgr_init);
    return ok;
}

// For most servo, 1000us to 2000us, 
//  center at 1500us, range is  +-500us
bool ServoPWM::set_degree(float dgr)
{ 
   
    float us = cfg_.us_min + dgr * cfg_.us_per_dgr;
    return set_us(us);   
}

// T = 1.0 / f
// h = us * 0.000001
// d = h / T = us * 0.000001 * f
bool ServoPWM::set_us(int us)
{
    float f = PWM::cfg_.freq;
    float d = us * f * 0.000001;
    PWM::set_duty(d);
    return true;
}