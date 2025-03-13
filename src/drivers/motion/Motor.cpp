#include <array>
#include "common/assertHandler.hpp"
#include "common/Trace.hpp"
#include "drivers/motion/Motor.hpp"
#include "drivers/interfaces/IDigitalOutputPin.hpp"
#include "drivers/interfaces/ITimer.hpp"
#include "drivers/interfaces/ITimerChannel.hpp"
#include "drivers/interfaces/PinBase.hpp"


Motor::Motor(std::shared_ptr<ITimer> pwm_timer, uint8_t pwm_channel_index, std::shared_ptr<IDigitalOutputPin> digital_pin):
    mpPwmChannelPin(pwm_timer->GetChannels()[pwm_channel_index]->GetChannelPin()),
    mpDigitalPin(digital_pin),
    mPwmChannelIndex(pwm_channel_index),
    mpPwmTimer(std::move(pwm_timer))
{
    ASSERT(mpPwmTimer);
    ASSERT(mpPwmChannelPin);
    ASSERT(mpDigitalPin);
}

eGeneralStatus Motor::Halt()
{
    mpPwmTimer->SetPeriodAndDutyCycle(mPwmPeriodMs, 0, mPwmChannelIndex);
    mpDigitalPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);

    // mpPwmTimer->Stop();

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus Motor::Forward(int8_t speed_percent)
{
    ASSERT(speed_percent <=100);

    ASSERT(mpPwmTimer->GetIsTimerRunning());

    mpPwmTimer->SetPeriodAndDutyCycle(mPwmPeriodMs, speed_percent, mPwmChannelIndex);
    mpDigitalPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus Motor::Backward(int8_t speed_percent)
{
    ASSERT(speed_percent <=100);

    ASSERT(mpPwmTimer->GetIsTimerRunning());

    mpDigitalPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
    mpPwmTimer->SetPeriodAndDutyCycle(mPwmPeriodMs, speed_percent, mPwmChannelIndex);

    return eGeneralStatus::SUCCESS;    
}
