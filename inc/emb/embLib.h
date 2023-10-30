
/*
   Author: Sherman Chen
   Create Time: 2022-11-13
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */
 
#pragma once

//--------------


//--------------

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
#elif defined ESP_PLATFORM
    #define SYS_MCU
    #include "emb/esp32/cutils_esp.h"
#else
#error "unknown platform"
#endif
//-----

#ifdef SYS_MCU
    #include "emb/cutil_mcu.h"
#else // SYS_MCU
    #include "ut/cutil.h"
#endif // SYS_MCU

//---- TODO: STM32
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
        PWM(int pin){ cfg_.pin = pin; };
        struct Cfg{
            int pin=-1;
            float freq = 50; // 50Hz
            // Info for hardware implementation.
            int ch_id = 0; // timer channel ID 
        }; Cfg cfg_;
        virtual bool init();
        // duty cycle 0-1.0
        virtual bool set_duty(float duty);
    };
    //----
    class UWB{
    public:
        struct Cfg{
        }; Cfg cfg_;
        virtual bool init()=0;
        virtual bool rangeTo(int id)=0;
        
        
    };

    //----
    // Servo always center 1.5ms (1500uS)
    // Most min/max typical 1ms - 2ms, 90 dgr
    // Some 0.5ms-2.5ms
    //    0: 500, 90: 1500, 180: 2500
    // Frequency, most 50Hz (20ms), 
    // Many good at 40-200Hz(5ms)
    class ServoPWM : public PWM{
    public:
        using PWM::PWM;
        struct Cfg{
            float dgr_init = 90;
            float us_min = 500; // min microsec
            float us_per_dgr = 1000/90.0;
            int dgr2us(float dgr)const 
            { return us_min + us_per_dgr * dgr; }
        }; Cfg cfg_;
        virtual bool init()override;
        virtual bool set_degree(float dgr);
        // set pulse uS, 
        virtual bool set_us(int us);
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
    //----
    class DualMotor{
    public:
        // 0: type D(DRV8833), 2pins
        // 1: type T(TB6612), 3pins
        DualMotor(int type) { init_motors(type); }
        struct Cfg{
            Motor::Cfg motor_cfgs[2];
        };
        bool init();
        bool set(float spdL, float spdR);
        Sp<Motor> motors_[2]{nullptr, nullptr};
    private:
        void init_motors(int type);
        bool chkInit();
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
    //----
    // e.g.: SSD1306 / SH1106
    class OLED{
    public:
        struct Cfg{
            int pin_SDA = 5;
            int pin_SCL = 6;
        }; Cfg cfg_;
    };
    //--------------------
#ifndef SYS_MCU
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
#endif // SYS_MCU

}

