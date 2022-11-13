/*
   Author: Sherman Chen
   Create Time: 2022-11-13
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */

#include "emb/eutils.h"
#include <stdio.h>

using namespace eu;

//-------------
// logf
//-------------
namespace elog
{
    namespace{
        ofstream logFile_;
    }
    
    //---- log file
    extern bool openFile(CStr& sFile)
    {
        logFile_.open(sFile);
        if(!logFile_.is_open())
        {
            err("Fail to open file:"+sFile);
            return false;
        }
        //----
        inf("log file open : "+sFile);
        return true;

    }

    //---- base log
    void logs(CStr& s, bool isErr = false)
    {
        if(isErr)
            cerr << s << endl << flush;
        else
            cout << s << endl << flush;
        //----
        if(logFile_)
        {
            logFile_ << (s + "\n");
            logFile_.flush();
        }
    }

    //-----
    extern void dbg(CStr& s)
    { logs("[dbg]:"+s); }
    extern void inf(CStr& s)
    { logs("[info]:"+s); }
    extern void err(CStr& s)
    { logs("[err]:"+s, true); }
    extern void errf(CStr& s)
    { logs("[err]: Failed to open file:"+s, true); }
}