
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
    //---- Hw platform
    class Hw{
    public:
        static Hw& inst(); // singleton
        bool chkInit();
    };

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
            vector<int> pins;
        }; Cfg cfg_;
        // spd [-1.0 ... -1.0]
        virtual bool set(float spd)=0;
        virtual bool init()=0;
    };
    //---- MotorD 
    // ( DRV8833, 2 pins switch PWM)
    class MotorD : public Motor{
    public:
        MotorD(){}

        struct Cfg{
            bool b_slow_decay=false;
        }; Cfg cfg_; 
        // spd -10 to 1.0
        virtual bool set(float spd)override;
        virtual bool init()override;
    protected:
        PWM pwm1_;
        PWM pwm2_;
    };
    //---- MotorT
    // ( TB6612, 2 pins for mode, 1 pin PWM)
    class MotorT : public Motor{
    public:
        MotorT(){}

        struct Cfg{
        }; Cfg cfg_; 
        // spd -1.0 to 1.0
        virtual bool set(float spd)override;
        virtual bool init()override;
    protected:
        PWM pwm_;
        GPIO io1_,io2_;
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
    class EmbCmd : public Cmd{
    public:
        EmbCmd(){ init_cmds(); }
    protected:
        void init_cmds();
        void init_cmds_gpio();
        void init_cmds_PWM();
        void init_cmds_Servo();
        void init_cmds_SPI();
        void init_cmds_Motor();

        map<int, Sp<PWM>> pwms_;
        map<int, Sp<ServoPWM>> servos_;
        map<int, Sp<SPI>> spis_;
        map<int, Sp<Motor>> motors_;
    };

}

