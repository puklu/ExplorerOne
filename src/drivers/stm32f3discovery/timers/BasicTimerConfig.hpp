/**
 * @file GeneralPurposeTimerConfig.hpp
 * 
 * @brief Header file containing configurations for general-purpose timers.
 *  This file defines class `GeneralPurposeTimerConfig`which serves as the overall configuration structure for a general-purpose
 *   timer. It holds arrays of `TimerChannel` instances and various timer settings, such as 
 *   clock division, direction, auto-reload, interrupt handling, DMA requests, and others.
 * 
 * The configurations stored in these classes are used to setup timers.
 * 
 * These classes do **not** perform any actual timer operation; they serve as data containers 
 * to be used in conjunction with timer hardware setup functions.
 * 
 * @note Timer setup and operations are handled by other components or modules that utilize 
 *       these configuration classes.
 */

 #pragma once

 #include <array>
 #include <memory>
 #include <vector>
 #include "common/defines.hpp"
 #include "common/PinDefinitions.hpp"
 #include "TimerChannel.hpp"
 
 using InterruptCallback = void(*)(void);
 
 

 class BasicTimerConfig
 {
 public:
 
    BasicTimerConfig() = default;

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
 