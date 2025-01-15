#pragma once

#include "drivers/io/GpioPin.hpp"
#include "drivers/timers/GeneralPurposeTimer.hpp"

class Motor
{
public:
    Motor(GeneralPurposeTimer *pwm_timer, uint8_t pwm_channel_index, GpioPin *digital_pin);
    eGeneralStatus Halt();
    eGeneralStatus Forward(int8_t speed_percent);
    eGeneralStatus Backward(int8_t speed_percent);

private:
    GpioPin *mpPwmChannelPin = nullptr;
    GpioPin *mpDigitalPin = nullptr;
    uint8_t  mPwmChannelIndex = 0;
    uint8_t  mPwmPeriodMs = 2;      
    GeneralPurposeTimer *mpPwmTimer = nullptr;
};
