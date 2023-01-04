
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
    class PWM{
    public:
        // occupy 0-1.0
        void set(float ocp);
    };
    //----
    class Servo{
    public:

    };
    //---- Motor
    class Motor{

    };
    //---- Serial
    class Serial{
    public:
        // default 8N1
        Sp<Serial> create();
        virtual bool open(const string& sPort, int baud);
        virtual bool read(char* buf, int n){ return false; };
        virtual bool write(char* buf, int n){ return false; };
    };
}

