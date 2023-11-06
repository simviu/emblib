/*
   Author: Sherman Chen
   Create Time: 2023-08-23
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
#include <assert.h>
#include <atomic>

namespace ut
{
    
    using namespace std;

    using CStr = const string;
    using Strs = vector<string>;
    using CStrs = const vector<string>;

    /*
    extern void log_i(const string& s);
    extern void log_d(const string& s);
    extern void log_w(const string& s);
    extern void log_e(const string& s);
    namespace sys{
        extern void sleep(double t);
    }
    */

    //-----------
    // log
    //-----------
    namespace utlog{
        extern bool openFile(CStr& sFile);
        extern void dbg(CStr& s);
        extern void inf(CStr& s);
        extern void err(CStr& s);
        extern void errf(CStr& s);
        extern void str(CStr& s);
        using FuncCbk=std::function<void(CStr& s)>;
        extern void setCallbk(FuncCbk cbk);
    }
    // shortcuts
    const auto log_s = utlog::str;
    const auto log_i = utlog::inf;
    const auto log_d = utlog::dbg;
    const auto log_e = utlog::err;
    const auto log_ef = utlog::errf;

    //------------
    // string utils
    //------------
    using CStr = const string;
    using Strs = vector<string>;
    using CStrs = const vector<string>;

    //-----   TODO: to be deprecated, replace by KeyVals
    //--- parse key/value table, e.g.:     
    // file=a.txt n=10 ...
    using StrTbl = map<string, string>;
    using CStrTbl = const StrTbl;   
    inline string lookup(CStrTbl& m, CStr& sk)
    {  
        auto it=m.find(sk); 
        if(it==m.end()) 
            return ""; 
        return it->second; 
    }
    extern bool parseKV(CStrs& ss, StrTbl& kv);
    inline bool has(CStrTbl& m, CStr& sk)
    { auto it=m.find(sk); return it!=m.end(); }

    //----
    extern string remove(const string& s, const char c);
    extern vector<string> tokens(const string& s, char c_deli=' ');
    //----
    template<typename T>
        bool s2d(const string& s, T& d)
        { stringstream t(s); t>>d; return !t.fail(); }
    // TODO: template this by s2d()
    extern bool s2data(const string& s, vector<double>& ds, char c_deli=',');
    extern bool s2data(const string& s, vector<int>& ds,    char c_deli=',');
    // hex string must with prefix of "0x"
    extern bool s2hex(const string& s, uint32_t& d);
    //---- err exception
    class ErrExcept : std::exception{
    public:
        using exception::exception;
        ErrExcept(const string& s):s_(s){}
        virtual string str()const{ return s_; }
    protected:
        string s_;
    };
    //-------
    // KeyVals
    //-------
    class KeyVals{
    public:
        KeyVals(){}
        KeyVals(const string& s, char c_sep=' ')
            { parse(s, c_sep); }
        KeyVals(CStrs& ss){ parse(ss); };

        //---
        bool parse(CStrs& ss){ return parseKV(ss, items);  };
        bool parse(const string& s, char c_sep=' ');

        //----
        bool has(const string& sKey)const;
        string operator[] (const string& skey) { return get(skey); }
        string operator[] (const string& skey) const{ return get(skey); }
        string get(const string& skey)const;

        bool get(const string& skey, double& d)const;
        bool get(const string& skey, int& d)const;
        bool get(const string& skey, bool& d)const;
        bool get(const string& skey, string& s)const;

        string query(const string& skey)const
        { return (has(skey))?get(skey) :""; }
        //----
        map<string, string> items;
    };
    //-----------------------------
    //	Aliase for std::shared_ptr
    //-----------------------------
    //---- aliase shared_ptr<T> to Sp<T>
    template<class T> using Sp = std::shared_ptr<T>;
    
    //---- aliase make_shared<T> to makeSp<T>
    template<class T, class ..._Args>
    inline static std::shared_ptr<T> mkSp(_Args&& ...__args)
    { return std::make_shared<T>(__args...); };

    //---- namespace fn
    /*
    namespace fn
    {
        extern string nopath(const string& s);
    }
    */
   // file path decode
    
    //-----------
    // container utils
    //-----------
    template<class T>
    inline Sp<T> lookup(map<string, Sp<T>>& m, CStr& s)
    { auto it = m.find(s); 
        return(it==m.end())?nullptr:it->second; }
    

    //-----------
    // math
    //-----------
    inline double toRad(double d){ return d*M_PI/180.0;  };
    inline double toDgr(double d){ return d*180.0/M_PI;  };
    template<typename T>
    T dgrIn180(const T& d)
    {   return d - ((int)(d/360.0+(d>0?0.5:-0.5)))*360.0; }
    inline bool isValid(double d)
    { return !(std::isnan(d)||std::isinf(d)); }
    //-----
    class PolyCurve{
    public:
        PolyCurve(){}
        PolyCurve(const vector<double>& k):k_(k){};
        float calc(double t)const;    
        void setk(const vector<double>& k){ k_ = k; }
    protected:
        vector<double> k_;
    };        
    //-----------
    // log
    //-----------
    /*
    namespace utlog{
        extern bool openFile(CStr& sFile);
        extern void dbg(CStr& s);
        extern void inf(CStr& s);
        extern void err(CStr& s);
        extern void errf(CStr& s);
        extern void str(CStr& s);
        using FuncCbk=std::function<void(CStr& s)>;
        extern void setCallbk(FuncCbk cbk);
    }
    // shortcuts
    const auto log_s = utlog::str;
    const auto log_i = utlog::inf;
    const auto log_d = utlog::dbg;
    const auto log_e = utlog::err;
    const auto log_ef = utlog::errf;
    */
    //----
    namespace sys{
        struct FPath{
            FPath(const string& sf);
            string path, base, ext;
        };
        extern string pwd();
        inline void sleepMS(int ms){
            this_thread::sleep_for(chrono::milliseconds(ms) );
        }
        inline void sleep(double s){
            size_t ms = s * 1000;
            this_thread::sleep_for(chrono::milliseconds(ms) );
        }
        //--- time functions
        using Time = std::chrono::high_resolution_clock::time_point;
        inline Time now()
        { return std::chrono::high_resolution_clock::now(); }
        inline double elapse(const Time t1, const Time t2)
        { std::chrono::duration<double> e=t2-t1; return e.count(); }
        //----
        class FPS{
        public:
            struct Cfg{ int N_avg=10; };
            Cfg cfg_;
            // check positive as valid
            void tick();
            double fps()const{ return fps_; }
        protected:
            double fps_=-1;
            Time t_ = now();
            list<double> dts;
        };
        //----
        extern bool exists(const string& s);
        extern bool mkdir(const string& s);
    };
    //----------------

    //---- Binary Buf
    struct Buf{
        uint8_t* p = nullptr; 
        size_t n = 0;

        Buf(){}
        Buf(uint8_t* p, size_t n): p(p), n(n){}
        Buf(size_t n) { resize(n); }
        void resize(size_t nn)
        { 
            if(p!=nullptr) delete p;
            p = new uint8_t[nn];
            n = nn;
            bDel = true;
        }

        ~Buf()
        { if(bDel && p!=nullptr) 
            delete p; }

        uint8_t operator[](int i) // TODO: exception
        {  if(i>=n) return 0xee; return p[i]; }

        //--- to C vector buf
        void to(vector<uint8_t>& d)const
        { for(int i=0;i<n;i++)d.push_back(*(p+i)); }
    protected:
        bool bDel = false;
    };

}
