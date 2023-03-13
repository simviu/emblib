/*
   Author: Sherman Chen
   Create Time: 2023-03-13
   Email: schen@simviu.com
   Copyright(c): Simviu Inc.
   Website: https://www.simviu.com
 */


#include "emb/embLib.h"

using namespace emb;

namespace{

    //---- err string of pigpio spi functions
    string err_str(int r)
    {
        return 
            (r==PI_BAD_HANDLE)?"PI_BAD_HANDLE":
            (r==PI_BAD_SPI_COUNT)?"PI_BAD_SPI_COUNT":
            (r==PI_SPI_XFER_FAILED)?"PI_SPI_XFER_FAILED":
            (r==PI_BAD_SPI_CHANNEL)?"PI_BAD_SPI_CHANNEL":
            (r==PI_BAD_SPI_SPEED)?"PI_BAD_SPI_SPEED":
            (r==PI_BAD_FLAGS)?"PI_NO_AUX_SPI":
            (r==PI_SPI_OPEN_FAILED)?"PI_SPI_OPEN_FAILED":
            "Unknown";
    }
}

//----
bool SPI::init(int ch, int spd)
{
    //----
    h_ = spiOpen(0, spd, 0);
    stringstream s;
    if(h_>=0)
    {
        s << "SPI open ok for channel : " << ch;
        s << ", spd=" << spd;
        log_i(s.str());
        return true;
    }
    
    log_e("SPI open failed : " + err_str(h_));
    return false;
}
//----
bool SPI::xfer(Buf& buf)
{
    int r = spiXfer(h_, buf.p, buf.p, buf.n);
    if(r==buf.n)return true;
    
    log_e("spiXfer : " + err_str(h_));
    return false;
}

//----
bool SPI::read(Buf& buf)
{
    int r = spiRead(h_, buf.p, buf.p, buf.n);
    if(r==buf.n)return true;
    
    log_e("spiRead : " + err_str(h_));
    return false;
}
//----
bool SPI::write(Buf& buf)
{
    int r = spiWrite(h_, buf.p, buf.p, buf.n);
    if(r==buf.n)return true;
    
    log_e("spiWrite : " + err_str(h_));
    return false;
}
