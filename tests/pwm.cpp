// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/io/GpioPin.hpp"
#include "drivers/leds/leds.hpp"
#include "drivers/timers/BasicTimer.hpp"
#include "drivers/timers/GeneralPurposeTimer.hpp"
#include "drivers/timers/GeneralPurposeTimerConfig.hpp"
#include "drivers/timers/ITimer.hpp"
#include "drivers/usart/UsartPin.hpp"
#include "mcuInit.hpp"

int main()
{
    SystemInit();

    ASSERT(isSystemInitialized);

    GpioPinInitStruct pinInit = {};
    pinInit.pin_name          = IO::ePin::IO_UNUSED_B0;
    pinInit.mode              = IO::eMode::IO_MODE_ALT_FUNCTION;
    pinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *tim2_ch2_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, pinInit);

    auto pin = static_cast<GpioPin *>(tim2_ch2_pin);

    uint8_t channel_index = 2;

    GeneralPurposeTimerConfig gptimer_config;
    gptimer_config.mChannels[channel_index].mpChannelPin = pin;
    gptimer_config.mChannels[channel_index].mAlternateFunction =
        IO::eAlternateFunction::IO_AF2;
    gptimer_config.mChannels[channel_index].mSelection =
        Timer::eCaptureCompareSelection::OUTPUT;
    gptimer_config.mChannels[channel_index]
        .mOutputCompareConfig.mOutputCompareMode =
        Timer::eOutputCompareMode::PWM_MODE_1;
    gptimer_config.mChannels[channel_index].mCaptureCompareEnable =
        Timer::eCaptureCompare::ENABLE;
    gptimer_config.mChannels[channel_index].mCaptureCompareCallbackFunction =
        InterruptLed;
    gptimer_config.mChannels[channel_index]
        .mOutputCompareConfig.mPwmDutyCyclePercent = 50;
    gptimer_config.mChannels[channel_index].mOutputCompareConfig.mPwmPeriodMs =
        1000;
    gptimer_config.mChannels[channel_index]
        .mOutputCompareConfig.mOutputComparePreloadEnable =
        Timer::eOutputComparePreloadEnable::ENABLE;

    GeneralPurposeTimer gp_timer(gptimer_config);

    gp_timer.Start();

    int8_t i = 1;
    while (1)
    {
        gp_timer.SetPeriodAndDutyCycle(1, i, channel_index);
        if (i == 100)
        {
            i = 1;
        }
        i++;
    }
}
