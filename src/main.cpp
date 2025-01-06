// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "common/Trace.hpp"
#include "drivers/io/GpioPin.hpp"
#include "drivers/leds/leds.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/timers/BasicTimer.hpp"
#include "drivers/timers/ITimer.hpp"
#include "drivers/timers/GeneralPurposeTimer.hpp"
#include "drivers/timers/GeneralPurposeTimerConfig.hpp"
#include "drivers/usart/UsartPin.hpp"
#include "mcuInit.hpp"


int main()
{

    SystemInit();
    
    ASSERT(isSystemInitialized);

    // TimerInitStruct timer_init_struct = {};
    // // timer_init_struct.prescaler_value = 7999;
    // // timer_init_struct.auto_reload_register_value = 2000;
    // timer_init_struct.cb = InterruptLed;

    // // ITimer *basictimer = new BasicTimer(timer_init_struct);
    // BasicTimer basictimer(timer_init_struct);
    // // basictimer.SetPeriodAndCount(1, 3000);
    // basictimer.Start();
    // // basictimer.EnableInterrupt();

    GpioPinInitStruct pinInit = {};
    pinInit.pin_name          = IO::ePin::IO_FRONT_MOTOR_RIGHT_B;
    pinInit.mode              = IO::eMode::IO_MODE_ALT_FUNCTION;
    pinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *tim2_ch2_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, pinInit);
    
    auto pin = static_cast<GpioPin *>(tim2_ch2_pin);

    // BlinkLed(pin);

    GeneralPurposeTimerConfig gptimer_config;
    gptimer_config.mChannels[1].mpChannelPin = pin;
    gptimer_config.mChannels[1].mAlternateFunction = IO::eAlternateFunction::IO_AF1;
    gptimer_config.mChannels[1].mSelection = Timer::eCaptureCompareSelection::OUTPUT;
    gptimer_config.mChannels[1].mOutputCompareConfig.mOutputCompareMode = Timer::eOutputCompareMode::PWM_MODE_1;
    gptimer_config.mChannels[1].mCaptureCompareEnable = Timer::eCaptureCompare::ENABLE;
    gptimer_config.mChannels[1].mCaptureCompareCallbackFunction = InterruptLed;
    gptimer_config.mChannels[1].mOutputCompareConfig.mPwmDutyCyclePercent = 100;
    gptimer_config.mChannels[1].mOutputCompareConfig.mPwmPeriodMs = 1;
    gptimer_config.mChannels[1].mOutputCompareConfig.mOutputComparePreloadEnable = Timer::eOutputComparePreloadEnable::ENABLE;

    GeneralPurposeTimer gp_timer(gptimer_config);

    gp_timer.Start();

    while (1)   
    {
    }

}   
