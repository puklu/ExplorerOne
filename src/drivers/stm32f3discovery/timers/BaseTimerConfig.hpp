#pragma once

#include <memory>
#include <vector>
#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "TimerChannel.hpp"

using InterruptCallback = void(*)(void);
class BaseTimerConfig
{
public:

    BaseTimerConfig() = default;

    uint16_t mPrescalerValue = 7999;
    uint16_t mAutoReloadRegisterValue = 1000;
    InterruptCallback mCb = nullptr;
    Timer::eOnePulseMode mOnePulseMode = Timer::eOnePulseMode::DISABLE_ONE_PULSE_MODE;
    Timer::eAutoReloadPreload mAutoReloadPreload = Timer::eAutoReloadPreload::ARR_BUFFERED;
    Timer::eUpdateRequestSource mUpdateRequestSource = Timer::eUpdateRequestSource::ONLY_OVERFLOW_UNDERFLOW;
    Timer::eUpdateEvent mEnableUpdateEvent = Timer::eUpdateEvent::ENABLE_EVENT_GENERATION;
    Timer::eMasterModeSelection mMasterModeSelection = Timer::eMasterModeSelection::SEND_RESET;
    Timer::eUpdateDmaRequest mUpdateDmaRequest = Timer::eUpdateDmaRequest::DISABLE;
    Timer::eUpdateInterrupt mUpdateInterrupt = Timer::eUpdateInterrupt::ENABLE;
    Milliseconds mTimerClockPeriodMs = Milliseconds{0};
};
