#pragma once

#include <stdio.h>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>

namespace ut{
    using namespace std;
    extern void log_i(const string& s);
    extern void log_d(const string& s);
    extern void log_w(const string& s);
    extern void log_e(const string& s);
    namespace sys{
        extern void sleep(double t);

    }
}
