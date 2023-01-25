
/*
   Author: Sherman Chen
   Create Time: 2022-11-13
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */
 
#pragma once

#include "ut/cutil.h"

namespace emb{
    using namespace ut;
    //-----
    class GPIO{
    public:
    };
    //-----
    class PWM{
    public:
        struct Cfg{
            float freq = 50; // 50Hz
        };
        // occupy 0-1.0
        void set_occupy(float ocp);
    };
    //----
    // Servo always center 1.5ms (1500uS)
    // Most min/max typical 1ms - 2ms, some 0.5ms-2.5ms
    // Frequency, most 50Hz (20ms), 
    // Many good at 40-200Hz(5ms)
    class Servo : public PWM{
    public:
        // degree -90 -- +90, most case
        void set_degree(float dgr); 
        // set micro, 1000 ot 2000, center at 1500
        void set_uS(int uS);
    };
    //---- Motor
    class Motor{
    public:
        // throttle 0-1.0
        void set_throttle(float spd, bool fwd=true);
    };
    //---- Serial
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
}

