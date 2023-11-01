/*
   Author: Sherman Chen
   Create Time: 2023-10-27
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */


#include "emb/embLib.h"
using namespace emb;

//------- dummy implementation ----

bool PWM::init()
{
    return false;
}
//---- 
bool PWM::set_duty(float duty)
{
    return false;
}
//----------
bool OLED::init(){ return true; }
bool OLED::write(int vpos, const string& s, int font_sz)
    { return false; }
void OLED::clear(){ }
void OLED::clear(int ln){ }
//-----
bool ADC::init(){ return false; };
int  ADC::read(){ return -1; };
//----
Wifi& Wifi::inst(){ static Wifi i_; return i_; }
bool Wifi::connect(){  return true;  }
