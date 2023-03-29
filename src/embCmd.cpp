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
void EmbCmd::init_cmds()
{
    init_cmds_PWM();
    init_cmds_Servo();
    init_cmds_SPI();
    init_cmds_Motor();
}
//----
void EmbCmd::init_cmds_gpio()
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
    add("io", p);    
}
//----
void EmbCmd::init_cmds_PWM()
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
void EmbCmd::init_cmds_Servo()
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
    add("servo", p);
}



//----
void EmbCmd::init_cmds_SPI()
{

    auto p = mkSp<Cmd>("(SPI commands)");
    p->add("init", "ch=<CH> spd=<SPD> [flags=<FLAGS>]", 
    [&](CStrs& args)->bool{
        KeyVals kvs(args);
        int ch = 0, spd=0, flags=0;
        if(!kvs.get("ch",  ch )) return false;
        if(!kvs.get("spd", spd)) return false;
        //---
        auto p = mkSp<SPI>();
        if(s2d(kvs["flags"], flags))
            p->cfg_.flags = flags;

        spis_[ch] = p;
        return p->init(ch, spd);
    });
    //-----
    p->add("write", "ch=[CH] d=<BYTE>", 
    [&](CStrs& args)->bool{
        KeyVals kvs(args);
        int ch=0;
        if(!kvs.get("ch",  ch )) return false;
        if(spis_.find(ch)==spis_.end())
        {
            stringstream s;  s << "SPI not init on ch:" << ch;
            log_e(s.str()); return false;
        }
        auto p = spis_[ch];
        uint32_t d = 0;
        uint8_t d1 = d; 
        Buf buf;
        if(s2hex(kvs["d"], d)) 
        {
            buf.p = &d1;
            buf.n = 1;
            return p->write(buf); 
        }
        log_e("Fail to get data");
        return false;
    });
    //-----
    p->add("read", "ch=[CH] n=<N>", 
    [&](CStrs& args)->bool{
        KeyVals kvs(args);
        int ch=0; int n=0;
        bool ok = true;
        ok &= kvs.get("ch",  ch) &&
              kvs.get("n",   n );
        if(spis_.find(ch)==spis_.end())
        {
            stringstream s;  s << "SPI not init on ch:" << ch;
            log_e(s.str()); return false;
        }
        auto p = spis_[ch];
        Buf buf(n);
        stringstream s;
        if(!p->read(buf))
        {
            s << "fail to read spi ch:" << ch;
            return false; 
        } 
        //---
        log_i(s.str());
        return true;
    });

    //-----
    add("spi", p);
}

//------
void EmbCmd::init_cmds_Motor()
{
    auto p = mkSp<Cmd>("(Motor commands)");
    p->add("init", "ch=[CH] pins=<PIN1,PIN2> ", 
    [&](CStrs& args)->bool{
        KeyVals kvs(args);
        int ch;
        if(!kvs.get("ch", ch)) return false;
        //---
        string spins; 
        if(!kvs.get("pins", spins))return false;
        vector<int> pins;
        if(!s2data(spins, pins)) return false;
        if(pins.size()<2)return false;
        //---
        auto p = mkSp<MotorD>();
        p->cfg_.pin1 = pins[0];
        p->cfg_.pin2 = pins[1];
        motors_[ch] = p;
        return p->init();
    });    
    //---
    p->add("set", "ch=[CH] spd=[0..1] [-back]", 
    [&](CStrs& args)->bool{
        KeyVals kvs(args);
        int ch;
        if(!kvs.get("ch", ch)) return false;
        //----
        stringstream s;  
        if(motors_.find(ch)==motors_.end())
        {
            s << "No motor on ch:" << ch;
            log_e(s.str()); return false;
        }
        //----
        auto pm = motors_[ch];
        double spd=0; bool fwd = true;
        if(!kvs.get("spd", spd))return false;
        if(kvs.has("-back")) fwd = false;
        return pm->set(spd, fwd);
	
    });
    //-----
    add("motor", p);
}

