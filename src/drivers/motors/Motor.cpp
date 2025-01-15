#include "common/assertHandler.hpp"
#include "drivers/motors/Motor.hpp"

Motor::Motor(GeneralPurposeTimer *pwm_timer, uint8_t pwm_channel_index, GpioPin *digital_pin):
    mpPwmChannelPin(pwm_timer->GetChannels()[pwm_channel_index].mpChannelPin),
    mpDigitalPin(digital_pin),
    mpPwmTimer(pwm_timer),
    mPwmChannelIndex(pwm_channel_index)
{
    ASSERT(mpPwmTimer);
    ASSERT(mpPwmChannelPin);
    ASSERT(mpDigitalPin);
}

eGeneralStatus Motor::Halt()
{
    mpPwmTimer->SetPeriodAndDutyCycle(mPwmPeriodMs, 0, mPwmChannelIndex);
    mpDigitalPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
}

eGeneralStatus Motor::Forward(int8_t speed_percent)
{
    ASSERT(speed_percent <=100 && speed_percent >=-100);

    if(speed_percent < 0) // move backwards instead
    {
        Backward(speed_percent);
    }
    else
    {
        mpPwmTimer->SetPeriodAndDutyCycle(mPwmPeriodMs, speed_percent, mPwmChannelIndex);
        mpDigitalPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
    }
}

eGeneralStatus Motor::Backward(int8_t speed_percent)
{
    ASSERT(speed_percent <=100 && speed_percent >=-100);

    if(speed_percent < 0) // move forwards instead
    {
        Forward(speed_percent);
    }
    else
    {
        mpDigitalPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        mpPwmTimer->SetPeriodAndDutyCycle(mPwmPeriodMs, speed_percent, mPwmChannelIndex);
    }
}
