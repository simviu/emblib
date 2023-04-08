
#include "emb/embLib.h"

using namespace emb;
//----
bool DualMotor::chkInit()
{
    if( motors_[0]==nullptr || 
        motors_[1]==nullptr)
    {
        log_e("DualMotor::init(), motors_ empty");
        return false;
    }
    return true;
}

//----
void DualMotor::init_motors(int type)
{
    for(int i=0;i<2;i++)
        if(type==0)
            motors_[i]=mkSp<MotorD>();
        else if(type==1)
            motors_[i]=mkSp<MotorT>();
        else 
            log_e("  DualMotor::init_motors() , unkown type:"+to_string(type));

}
//---
bool DualMotor::init()
{
    bool ok = true;
    
    if(!chkInit())return false;
    ok &= motors_[0]->init();
    ok &= motors_[1]->init();
    return ok; 
}
//---
bool DualMotor::set(float spdL, float spdR)
{
    if(!chkInit())return false;
    bool ok = true;
    ok &= motors_[0]->set(spdL);
    ok &= motors_[1]->set(spdR);
    return ok;
}
