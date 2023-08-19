/*
   Author: Sherman Chen
   Create Time: 2023-03-13
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */


#include "emb/embLib.h"

using namespace emb;


// For most servo, 1000us to 2000us, 
//  center at 1500us, range is  +-500us
bool ServoPWM::set_degree(float dgr)
{ 
    float r = cfg_.dgr_range;
    float d = Rng(-r, r).cut(dgr);
    float us = 1500 + 500*d/r;
    return true;   
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