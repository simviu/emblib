#include "emb/embLib.h"
#include <CppLinuxSerial/SerialPort.hpp>

using namespace emb;
namespace{
    using CLSPort=mn::CppLinuxSerial::SerialPort;
    class SerialImp : public Serial{
    public:

        SerialImp(const string& sPort, int baud);
        virtual bool read(string& s) override;
        virtual bool write(const string& s) override;
    protected:
        Sp<CLSerial::SerialPort> p_port_ = nullptr;
    };
}

//----
SerialImp::SerialImp(const string& sPort, int baud)
{
    p_port_ = mkSp<CLSPort>("/dev/ttyUSB0", 
        BaudRate::B_57600, 
        NumDataBits::EIGHT, 
        Parity::NONE, NumStopBits::ONE);
}

//----
Sp<Serial> Serial::create(const string& sPort, int baud)
{
    return mkSp<SerialImp>(sPort, baud);
}

bool SerialImp::read(string& s)
{
     return false; 
}

bool write(const string& s)
{
     return false; 
}
    