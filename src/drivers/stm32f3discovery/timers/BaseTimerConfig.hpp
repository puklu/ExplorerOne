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

    virtual ~BaseTimerConfig() = default;

    /**
     * @brief Prescaler value for the timer. Determines the clock division before counting.
     */
    uint16_t mPrescalerValue = 7999;

    /**
     * @brief Auto-reload register value for the timer, used to set the period.
     */
    uint16_t mAutoReloadRegisterValue = 1000;

    /**
     * @brief Optional callback function to be called on timer interrupt.
     */
    InterruptCallback mCb = nullptr;

    /**
     * @brief One pulse mode configuration for the timer.
     */
    Timer::eOnePulseMode mOnePulseMode = Timer::eOnePulseMode::DISABLE_ONE_PULSE_MODE;

    /**
     * @brief Auto-reload preload enable.
     */
    Timer::eAutoReloadPreload mAutoReloadPreload = Timer::eAutoReloadPreload::ARR_BUFFERED;

    /**
     * @brief Update request source for the timer.
     */
    Timer::eUpdateRequestSource mUpdateRequestSource = Timer::eUpdateRequestSource::ONLY_OVERFLOW_UNDERFLOW;

    /**
     * @brief Enable or disable event generation for timer updates.
     */
    Timer::eUpdateEvent mEnableUpdateEvent = Timer::eUpdateEvent::ENABLE_EVENT_GENERATION;

    /**
     * @brief Master mode selection for timer synchronization.
     */
    Timer::eMasterModeSelection mMasterModeSelection = Timer::eMasterModeSelection::SEND_RESET;

    /**
     * @brief Enable or disable DMA request on update.
     */
    Timer::eUpdateDmaRequest mUpdateDmaRequest = Timer::eUpdateDmaRequest::DISABLE;

    /**
     * @brief Enable or disable update interrupt.
     */
    Timer::eUpdateInterrupt mUpdateInterrupt = Timer::eUpdateInterrupt::ENABLE;


    Milliseconds mTimerClockPeriodMs = 1_ms;
};
