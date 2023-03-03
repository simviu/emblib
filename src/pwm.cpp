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
// #if RASPBERRY_PI
#include <pigpio.h>

//----
bool PWM::init()
{
    return true;
}
//----
void PWM::set_duty(float duty)
{

}
// #endif // RASPBERRY_PI
//================================================