/**
 * @file GeneralPurposeTimerConfig.hpp
 * 
 * @brief Header file containing configurations for general-purpose timers.
 *  This file defines class `GeneralPurposeTimerConfig`which serves as the overall configuration structure for a general-purpose
 *   timer. It holds arrays of `ChannelConfig` instances and various timer settings, such as 
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
#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "ChannelConfig.hpp"

typedef void (*InterruptCallback)(void);


/**
 * @class GeneralPurposeTimerConfig
 * @brief Represents the configuration settings for a general-purpose timer.
 * 
 * This class serves as a configuration container for general-purpose timer setups. It holds 
 * various parameters needed to configure the timer, including channel settings, clock division, 
 * auto-reload settings, direction, DMA requests, interrupt triggers, and other timer configurations.
 * 
 * The `GeneralPurposeTimerConfig` class is intended to store all the configuration options necessary 
 * for setting up and initializing a timer. It does **not** perform any operations; it simply stores 
 * and organizes configuration data that will be used by the timer during setup.
 * 
 * @note Actual timer setup and usage should be handled by other classes that utilize this configuration.
 */
class GeneralPurposeTimerConfig
{
public:

    GeneralPurposeTimerConfig();

    /**
     * @brief Array of `ChannelConfig` objects representing each channel of the timer.
     */
    std::array<std::shared_ptr<ChannelConfig>, GENERAL_PURPOSE_TIMER_NUM_CHANNELS> mChannels;
    
    /**
     * @brief Clock division for the timer filter.
     */
    Timer::eClockDivisionForFilter mClkDivisionForFilter = Timer::eClockDivisionForFilter::ONE;

    /**
     * @brief Auto-reload preload enable.
     */
    Timer::eAutoReloadPreload mEnableAutoReloadPreload = Timer::eAutoReloadPreload::ARR_BUFFERED;

    /**
     * @brief Direction of the timer (up counter or down counter).
     */
    Timer::eDirection mDirection = Timer::eDirection::UPCOUNTER;

    /**
     * @brief Update request source for the timer.
     */
    Timer::eUpdateRequestSource mUpdateRequestSource = Timer::eUpdateRequestSource::ANY_EVENT;

    /**
     * @brief Enable or disable event generation for timer updates.
     */
    Timer::eUpdateEvent mEnableUpdateEvent = Timer::eUpdateEvent::ENABLE_EVENT_GENERATION;

    /**
     * @brief Enable or disable DMA request on trigger.
     */
    Timer::eTriggerDmaRequest mTriggerDmaRequest = Timer::eTriggerDmaRequest::DISABLE;

    /**
     * @brief Enable or disable DMA request on update.
     */
    Timer::eUpdateDmaRequest mUpdateDmaRequest = Timer::eUpdateDmaRequest::DISABLE;

    /**
     * @brief Enable or disable trigger interrupt.
     */
    Timer::eTriggerInterrupt mTriggerInterrupt = Timer::eTriggerInterrupt::DISABLE;

    /**
     * @brief Enable or disable update interrupt.
     */
    Timer::eUpdateInterrupt mEnableUpdateInterrupt = Timer::eUpdateInterrupt::ENABLE;

    /**
     * @brief Prescaler value for the timer. Determines the clock division before counting.
     */
    uint16_t mPrescalerValue = 7999;

    /**
     * @brief Auto-reload register value for the timer, used to set the period.
     */
    uint32_t mAutoReloadRegisterValue = 7000;

    /**
     * @brief Optional callback function to be called on timer interrupt.
     */
    InterruptCallback mCb = nullptr;

    /**
     * @brief One pulse mode configuration for the timer.
     */
    Timer::eOnePulseMode mOnePulseMode = Timer::eOnePulseMode::DISABLE_ONE_PULSE_MODE;

    /**
     * @brief Enable or disable the timer counter.
     */
    Timer::eCounterEnable mEnableCounter = Timer::eCounterEnable::ENABLE;

    /**
     * @brief Master mode selection for timer synchronization.
     */
    Timer::eMasterModeSelection mMasterModeSelection = Timer::eMasterModeSelection::SEND_RESET;
};
