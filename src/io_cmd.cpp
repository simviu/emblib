/*
   Author: Sherman Chen
   Create Time: 2022-01-27
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */

#include "emb/embLib.h"

using namespace emb;

namespace{
    
}

//----
void IO_Cmd::init_cmds()
{
    init_cmds_PWM();
}
//----
void IO_Cmd::init_cmds_PWM()
{

    auto p = mkSp<Cmd>("(PWM commands)");
    p->add("init", "pin=<PIN>", 
    [&](CStrs& args)->bool{
        KeyVals kvs(args);
        int pin = 0; 
        if(!kvs.get("pin", pin)) return false;
        pwms_[pin] = mkSp<PWM>(pin);
        return true;
    });
    //-----
    p->add("set", "pin=<PIN> duty=<0-1.0> ", 
    [&](CStrs& args)->bool{
        KeyVals kvs(args);
        int pin = 0; double duty=0;
        if(!kvs.get("pin", pin)) return false;
        if(!kvs.get("duty", duty)) return false;
        if(pwms_.find(pin)==pwms_.end())
        {
            stringstream s;  s << "PWM not init on pin:" << pin;
            log_e(s.str()); return false;
        }
        return pwms_[pin]->set_duty(duty);
    });
    add("pwm", p);
}

//----
void IO_Cmd::init_cmds_Servo()
{

    auto p = mkSp<Cmd>("(Servo commands)");
    p->add("init", "pin=<PIN>", 
    [&](CStrs& args)->bool{
        KeyVals kvs(args);
        int pin = 0; 
        if(!kvs.get("pin", pin)) return false;
        pwms_[pin] = mkSp<PWM>(pin);
        return true;
    });
    //-----
    p->add("set", "pin=<PIN> [degree=<D> | us=<uSEC>] ", 
    [&](CStrs& args)->bool{
        KeyVals kvs(args);
        int pin = 0; 
        if(!kvs.get("pin", pin)) return false;
        if(servos_.find(pin)==servos_.end())
        {
            stringstream s;  s << "Servo not init on pin:" << pin;
            log_e(s.str()); return false;
        }
        //----
        auto p = servos_[pin];

        double d=0; int u = 0;
        if(s2d(kvs["degree"], d))
            return p->set_degree(d);
        else if(s2d(kvs["us"], u))
            return p->set_us(u);
        log_e("fail to get or parse 'degree' or 'us' para");
        return false;
    });
    add("pwm", p);
}
