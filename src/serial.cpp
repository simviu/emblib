#include "emb/embLib.h"

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

using namespace emb;

// Ref : 
//     https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/#overview


namespace{
    //----
    class SerialImp : public Serial{
    public:
        SerialImp(){};
        virtual bool open(const string& sPort, int baud)override;
        virtual bool read(Buf& b) override;
        virtual bool readln(string& sln)override;
        virtual bool write(const Buf& b) override;

    protected:
        int fd_ = -1;

        
    };
    //----- term_cfg
    void term_cfg(struct termios& tty, int baud)
    {
        // 8N1
        tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
        tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
        tty.c_cflag |= CS8; // 8 bits per byte (most common)

        // No flow control
        tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)

        // required
        tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
        // Non-canonical
        tty.c_lflag &= ~ICANON;

        // diable echo
        tty.c_lflag &= ~ECHO; // Disable echo
        tty.c_lflag &= ~ECHOE; // Disable erasure
        tty.c_lflag &= ~ECHONL; // Disable new-line echo        

        // No interrupt
        tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

        // Input : No special char, No software flow ctrl
        tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl

        // Output control
        tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
        tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

        //---- not in Linux
//        tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT IN LINUX)
//        tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX)    }

        // No blocking
        tty.c_cc[VTIME] = 0;
        tty.c_cc[VMIN] = 0;

        // Specifying a custom baud rate when using GNU C
        cfsetispeed(&tty, baud);
        cfsetospeed(&tty, baud);

    }
    
    
}

//----
bool SerialImp::open(const string& sPort, int baud)
{
    //---- 1) Open Port
    fd_ = ::open(sPort.c_str(), O_RDWR | O_NONBLOCK);
    if (fd_ < 0) 
    {
        log_e("  Fail to open serial port: '"+sPort+"'"+
          ", errno:"+to_string(errno));
        log_e("  Check port exists, or user permission set by 'sudo adduser $USER dialout'.");
        return false;
    }  
    //----- 2) Conifg
    int err_num=0;
    struct termios tty;
    if(tcgetattr(fd_, &tty) != 0)
    {
        log_e("  Fail to read cfg of serial port: '"+sPort+"'"+
          ", errno:"+to_string(errno));
        return false;
    }
    term_cfg(tty, baud);


    //---- done and set it
    // Save tty settings, also checking for error
    if (tcsetattr(fd_, TCSANOW, &tty) != 0) 
    {
        log_e("  Fail to config serial port: '"+sPort+"'"+
          ", errno:"+to_string(errno));
        return false;
    }
    //----
    log_i("Serial opened: '"+sPort+"', baud:"+ to_string(baud));
    return true;
    
}

//---- static factory
Sp<Serial> Serial::create()
{
    return mkSp<SerialImp>();
}
//------

bool SerialImp::read(Buf& b)
{
    if(fd_ < 0) return false;
    int n = ::read(fd_, b.p, b.n);
    return (b.n==n); 
}
//------
bool SerialImp::readln(string& sln)
{
    CFile f(fd_);
    return f.readln(sln);
}

//------
bool SerialImp::write(const Buf& b)
{
    if(fd_ < 0) return false;
    size_t n = ::write(fd_, b.p, b.n);
    return (b.n==n); 
}



