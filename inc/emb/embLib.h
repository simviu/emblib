
/*
   Author: Sherman Chen
   Create Time: 2022-11-13
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */
 
#pragma once

#include "ut/cutil.h"

//----- cross platform macro
#ifdef __APPLE__
  #include <TargetConditionals.h>
  #if TARGET_OS_MAC
    #define SYS_MAC
  #endif /* TARGET_OS_MAC */

#elif defined __linux__
    #define SYS_LINUX
#elif defined _WIN32 || defined _WIN64
    #define SYS_WIN
#else
#error "unknown platform"
#endif
//-----

#ifdef __arm__
    #define RASPBERRY_PI  // Possible right
#endif


//-----
namespace emb{
    using namespace ut;
    //---- platform


    //-----
    class GPIO{
    public:
        GPIO(){}
        GPIO(int pin, int mode)
        {   cfg_.pin = pin; 
            cfg_.mode = mode; }
        struct Cfg{
            int pin = 0;
            int mode = 0; // r:0, w:1
        }; Cfg cfg_;
        bool init();
        bool read(bool& d);
        bool write(bool d);
    };
    //-----
    class PWM{
    public:
        PWM(){}
        PWM(int pin){ cfg_.pin = pin; init(); };
        struct Cfg{
            int pin=0;
            float freq = 50; // 50Hz
        }; Cfg cfg_;
        bool init();
        // duty cycle 0-1.0
        bool set_duty(float duty);
    };
    //----
    // Servo always center 1.5ms (1500uS)
    // Most min/max typical 1ms - 2ms, some 0.5ms-2.5ms
    // Frequency, most 50Hz (20ms), 
    // Many good at 40-200Hz(5ms)
    class ServoPWM : public PWM{
    public:
        using PWM::PWM;
        struct Cfg{
            float dgr_range = 90; 
        }; Cfg cfg_;

        bool set_degree(float dgr);
        // set micro, 1000 ot 2000, center at 1500
        bool set_us(int us);
    };
    //---- SPI
    class SPI{
    public:
        struct Cfg{
            int flags=0; 
        }; Cfg cfg_;
        bool init(int ch, int spd);
        bool xfer(Buf& buf);
        bool write(Buf& buf);
        bool read(Buf& buf);
    protected:
        int h_ = -1; // handle
    };
    //---- Motor
    class Motor{
    public:
        Motor(){}

        struct Cfg{
            int mode=0; // 0: DC motor, fwd/bwd
            int pin1=-1;
            int pin2=-1;
            bool b_slow_decay=false;
        }; Cfg cfg_; 
        // spd 0-1.0
        bool set(float spd, bool fwd);
        bool init();
    protected:
        PWM pwm1_;
        PWM pwm2_;
    };
    //---- Serial (UART)
    class Serial{
    public:
        struct Cfg{
            float timeout = 1;
        }; Cfg cfg_;
        
        // default 8N1
        static Sp<Serial> create();
        virtual bool open(const string& sPort, int baud){ return false; };
        virtual bool read(Buf& buf){ return false; };
        virtual bool write(const Buf& buf){ return false; };
        virtual bool readln(string& sln){ return false; };
        virtual bool readFrm(const string& sHeader, 
                     Buf& buf){ return false; };
        bool write(const string& s)
        {
            uint8_t* p = (uint8_t*)s.c_str();
            Buf b(p, s.length());
            return write(b);
        }        
    };
    //-----
    class IO_Cmd : public Cmd{
    public:
        IO_Cmd(){ init_cmds(); }
    protected:
        void init_cmds();
        void init_cmds_PWM();
        void init_cmds_Servo();
        void init_cmds_SPI();

        map<int, Sp<PWM>> pwms_;
        map<int, Sp<ServoPWM>> servos_;
        map<int, Sp<SPI>> spis_;
    };

}

