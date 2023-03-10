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
    //--- PWM
    {
        auto p = mkSp<Cmd>("(PWM commands)");
        p->add("init", "pin=<PIN>", 
        [&](CStrs& args)->bool{
            return true;
        });
        add("pwm", p);
    }
}
