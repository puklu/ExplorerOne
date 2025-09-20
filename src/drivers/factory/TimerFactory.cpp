#include "drivers/stm32f3discovery/timers/BasicTimer.hpp"
#include "drivers/stm32f3discovery/leds/leds.hpp"
#include "TimerFactory.hpp"

// TODO: Fix/implement me! Not tested or even used yet.
ITimer* TimerFactory::CreateBasicTimer()
{
    BasicTimerConfig timer_config;
    timer_config.mPrescalerValue = 7999;
    timer_config.mAutoReloadRegisterValue = 1000;
    timer_config.mCb = nullptr;
    timer_config.mOnePulseMode = Timer::eOnePulseMode::DISABLE_ONE_PULSE_MODE;
    timer_config.mAutoReloadPreload = Timer::eAutoReloadPreload::ARR_BUFFERED;
    timer_config.mUpdateRequestSource = Timer::eUpdateRequestSource::ONLY_OVERFLOW_UNDERFLOW;
    timer_config.mEnableUpdateEvent = Timer::eUpdateEvent::ENABLE_EVENT_GENERATION;
    timer_config.mMasterModeSelection = Timer::eMasterModeSelection::SEND_RESET;
    timer_config.mUpdateDmaRequest = Timer::eUpdateDmaRequest::DISABLE;
    timer_config.mUpdateInterrupt = Timer::eUpdateInterrupt::ENABLE;
    // timer_config.mTimerClockPeriodMs = Milliseconds{1000}; 

    // auto timer = std::make_unique<BasicTimer>(timer_config);
    auto timer =  new BasicTimer(timer_config);

    return timer;
}