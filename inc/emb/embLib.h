
/*
   Author: Sherman Chen
   Create Time: 2022-11-13
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */
 
#pragma once

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <cmath>

#include <vector>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

namespace emb{
    
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
        Sp<Serial> create(const string& sPort, int baud);
        virtual bool read(string& s){ return false; };
        virtual bool write(const string& s){ return false; };
    };
}

