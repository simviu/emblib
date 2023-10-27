#pragma once
// EPS32 servo implemented by mcpwm from examples.

#include "driver/mcpwm_prelude.h"
#include "emb/esp32/cutils_esp.h"
#include "emb/embLib.h"

using namespace emb;


// Please consult the datasheet of your servo before changing the following parameters
#define SERVO_MIN_PULSEWIDTH_US 500  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US 2500  // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE        -90   // Minimum angle
#define SERVO_MAX_DEGREE        90    // Maximum angle

#define SERVO_PULSE_GPIO             16        // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD        20000    // 20000 ticks, 20ms

/*
inline uint32_t esp32_ang2us(int angle)
{
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}
*/
//--------------------
/*
class Servo{
public:
    virtual bool init(int pin) = 0;
    virtual bool set(float ang) = 0; // -90 to 90

    Servo(){}
    struct Cfg{
        int pin = 2;
    }; Cfg cfg_;
protected:

};
*/
//---------------------
// ESP32 mcpwm data
struct MCPWM_dt{
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_oper_handle_t oper = NULL;
    mcpwm_cmpr_handle_t comparator = NULL;
    mcpwm_gen_handle_t generator = NULL;

};

//---------------------
class ServoEsp : public ServoPWM{
public:
    using ServoPWM::ServoPWM;

    virtual bool init() override;
    virtual bool set_degree(float ang) override; // -90 to 90
  //virtual bool set_us(int us)override;

protected:
    MCPWM_dt mc_;
};
