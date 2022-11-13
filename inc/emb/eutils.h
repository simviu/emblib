
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

namespace eu{
    using namespace std;
    //-----------------------------
    //	Aliase for std::shared_ptr
    //-----------------------------
    //---- aliase shared_ptr<T> to Sp<T>
    template<class T> using Sp = std::shared_ptr<T>;
    
    //---- aliase make_shared<T> to makeSp<T>
    template<class T, class ..._Args>
    inline static std::shared_ptr<T> mkSp(_Args&& ...__args)
    { return std::make_shared<T>(__args...); };

     //-----------
    // log
    //-----------
    namespace elog{
        extern bool openFile(CStr& sFile);
        extern void dbg(CStr& s);
        extern void inf(CStr& s);
        extern void err(CStr& s);
        extern void errf(CStr& s);
    }
    // shortcuts
    const auto log_i = elog::inf;
    const auto log_d = elog::dbg;
    const auto log_e = elog::err;
    const auto log_ef = elog::errf;
    //----
}
