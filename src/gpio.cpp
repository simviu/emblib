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

//----
bool GPIO::init()
{
    // mode : r:0, w:1
    int p = cfg_.pin;
    int m = cfg_.mode;
    {
        stringstream s;
        s << "GPIO init, pin" << p;
        s << ", mode(0/1 for r/w):" << m;
        log_i(s.str());
    }
    //---
    int r=0;
    if(m==0) 
        r = gpioSetMode(PI_INPUT);
    else if(m==1) 
        r = gpioSetMode(PI_OUTPUT);
    else {
        log_e("not support mode, expect mode 0/1 for r/w");
        return false;
    }
    //----
    if(r==0)
    {
        log_i("GPIO init OK");
        return true;
    }
    //---- err
    stringstream s;
    s << "fail to init gpio, ";
    if(r==PI_BAD_GPIO)
        s << "PI_BAD_GPIO";
    else if (r==PI_BAD_MODE)
        s << "PI_BAD_MODE";
    else 
        s << "unknown err code:" << r;
    log_e(s.str());
    return false;

}
//-----
bool GPIO::read(bool& d)
{
    int r = gpioRead(cfg_.pin);
    if(r >= 0)
    {
        d = r;
        return true;
    }
    //----
    stringstream s;
    s << "gpio read fail, err code:" << r;
    log_e(s.str());
    return false;
}
//-----
bool GPIO::write(bool d)
{
    int r = gpioWrite(cfg_.pin, d);
    if(r==0) return false;
    stringstream s;
    s << "gpio wr fail, err code" << r;
    log_e(s.str());
}

#endif //#ifdef RASPBERRY_PI
