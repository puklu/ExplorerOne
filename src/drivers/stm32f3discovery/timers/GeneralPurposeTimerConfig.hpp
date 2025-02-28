/**
 * @file GeneralPurposeTimerConfig.hpp
 * 
 * @brief Header file containing configurations for general-purpose timers.
 * 
 * This file defines two essential classes used to configure general-purpose timers:
 * - `ChannelConfig`: Represents the configuration for individual timer channels, including
 *   pin settings, capture/compare settings, and PWM configurations.
 * - `GeneralPurposeTimerConfig`: Serves as the overall configuration structure for a general-purpose
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

#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "drivers/stm32f3discovery/io/GpioPin.hpp"

typedef void (*InterruptCallback)(void);


/**
 * @class ChannelConfig
 * @brief Represents the configuration settings for an individual channel of a general-purpose timer.
 * 
 * This class serves as a container for the configuration settings needed to set up an individual channel 
 * of a general-purpose timer. It stores the necessary parameters that define the behavior and connection 
 * of the timer channel, including pin selection, alternate function, capture/compare mode, interrupt 
 * settings, and other configuration options.
 * 
 * The `ChannelConfig` class does **not** perform any actual operations; it simply holds and organizes 
 * the configuration data to be passed to the timer during setup.
 * 
 * @note The actual initialization of GPIO pins and timer settings should be performed separately, typically 
 * in a higher-level function that uses this configuration.
 */
class ChannelConfig
{
public:

    ChannelConfig() = default;

    /**
     * @brief Initializes the channel configuration with the specified capture/compare selection mode.
     * 
     * @param selection The capture/compare selection mode to be used for the timer channel.
     */
    void Init(Timer::eCaptureCompareSelection selection);
    
    /**
     * @brief Pointer to the GPIO pin associated with this timer channel.
     */
    GpioPin *mpChannelPin = nullptr;

    /**
     * @brief Alternate function for the timer channel pin. This defines the function the pin will serve when used with the timer.
     */
    IO::eAlternateFunction mAlternateFunction = IO::eAlternateFunction::NONE;

    /**
     * @brief Capture/compare selection mode for this timer channel.
     */
    Timer::eCaptureCompareSelection mSelection = Timer::eCaptureCompareSelection::NOT_SELECTED;

    /**
     * @brief Enable or disable capture/compare for this channel.
     */
    Timer::eCaptureCompare mCaptureCompareEnable = Timer::eCaptureCompare::DISABLE;

    /**
     * @brief Enable or disable capture/compare interrupt for this channel.
     */
    Timer::eCaptureCompareInterrupt mCaptureCompareInterruptEnable = Timer::eCaptureCompareInterrupt::DISABLE;

    /**
     * @brief Value to be used for capture/compare operations. Default is set to 2000.
     */
    uint32_t mCaptureCompareValue = 2000;

    /**
     * @brief Callback function to be invoked when a capture/compare event occurs.
     */
    InterruptCallback mCaptureCompareCallbackFunction = nullptr;

    /**
     * @brief Pointer to the CCMR (Capture/Compare Mode Register) specific to this timer instance.
     */
    volatile uint32_t *mCcmrRegister = nullptr; // to store CCMR register since it can be different based on which TIM it is

    /**
     * @brief Pointer to the CCR (Capture/Compare Register) specific to this timer instance.
     */
    volatile uint32_t *mCcrRegister = nullptr;  // to store CCR register since it can be different based on which TIM it is

    /**
     * @union Configuration union that holds either input capture or output compare settings.
     * 
     * This union allows for a unified representation of input capture and output compare configurations.
     */
    union
    {
        /**
         * @brief Input capture configuration parameters.
         */
        struct
        {
            Timer::eInputCaptureFilter mInputCaptureFilter;
            Timer::eInputCapturePrescaler mInputCapturePrescaler;
        } mInputCaptureConfig ;
        
        /**
         * @brief Output compare configuration parameters.
         */
        struct
        {
            Timer::eOutputCompareClearEnable mOutputCompareEnable;
            Timer::eOutputComparePreloadEnable mOutputComparePreloadEnable;
            Timer::eOutputCompareMode mOutputCompareMode;
            uint8_t mPwmDutyCyclePercent;
            uint32_t mPwmPeriodMs;
        } mOutputCompareConfig; 
    };

};


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
    ChannelConfig mChannels[GENERAL_PURPOSE_TIMER_NUM_CHANNELS]; // array of channel configurations
    
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
