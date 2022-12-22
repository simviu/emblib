#include "emb/embLib.h"

# ifdef __TOBE_DEPRECATED__
#include <CppLinuxSerial/SerialPort.hpp>

using namespace emb;
namespace{
    class SerialImp : public Serial{
    public:

        SerialImp(const string& sPort, int baud);
        virtual bool read(string& s) override;
        virtual bool write(const string& s) override;
    protected:
        Sp<mn::CppLinuxSerial::SerialPort> p_port_ = nullptr;
    };
}

//----
SerialImp::SerialImp(const string& sPort, int baud)
{
    using namespace mn::CppLinuxSerial;
    p_port_ = mkSp<SerialPort>("/dev/ttyUSB0", 
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

#endif // # ifdef __TOBE_DEPRECATED__

