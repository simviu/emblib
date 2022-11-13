
/*
   Author: Sherman Chen
   Create Time: 2022-11-13
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */
 
#pragma once
#include "emb/eutils.h"

namespace emb{
    using namespace eu;
    
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
}

