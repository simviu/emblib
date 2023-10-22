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
#ifdef RASPBERRY_PI // by "pi/pwm_pi.cpp"
#else // dummy

bool PWM::init(){ return false; }
bool PWM::set_duty(float duty){ return false; }

#endif // RASPBERRY_PI
