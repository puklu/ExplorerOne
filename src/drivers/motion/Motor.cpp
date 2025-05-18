#include <array>
#include "common/assertHandler.hpp"
#include "common/Trace.hpp"
#include "drivers/motion/Motor.hpp"
#include "drivers/interfaces/IDigitalPin.hpp"
#include "drivers/interfaces/IPwm.hpp"
#include "drivers/interfaces/ITimerChannel.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "drivers/stm32f3discovery/timers/GeneralPurposeTimer.hpp"


Motor::Motor(std::shared_ptr<IPwm> pwm_timer, uint8_t pwm_channel_index, std::shared_ptr<IDigitalPin> digital_pin):
    mpPwmChannelPin(pwm_timer->GetChannels()[pwm_channel_index]->GetChannelPin()),
    mpDigitalPin(digital_pin),
    mPwmChannelIndex(pwm_channel_index),
    mpPwmTimer(std::move(pwm_timer))
{
    ASSERT(mpPwmTimer);
    ASSERT(mpPwmChannelPin);
    ASSERT(mpDigitalPin);
    mpPwmTimer->SetPeriod(mPwmPeriodMs);
}

eGeneralStatus Motor::Halt()
{
    mpPwmTimer->SetDutyCycle(0, mPwmChannelIndex);
    mpDigitalPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);

    // mpPwmTimer->Stop();

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus Motor::Forward(int8_t speed_percent)
{
    ASSERT(speed_percent <=100);

    ASSERT(mpPwmTimer->GetIsTimerRunning());

    mpPwmTimer->SetDutyCycle(speed_percent, mPwmChannelIndex);
    mpDigitalPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus Motor::Backward(int8_t speed_percent)
{
    ASSERT(speed_percent <=100);

    ASSERT(mpPwmTimer->GetIsTimerRunning());

    mpDigitalPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
    mpPwmTimer->SetDutyCycle(speed_percent, mPwmChannelIndex);

    return eGeneralStatus::SUCCESS;    
}
