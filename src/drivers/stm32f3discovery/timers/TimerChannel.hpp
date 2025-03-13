/**
* @file TimerChannel.hpp
* This file defines a class `TimerChannel` that represents the configuration for individual timer channels, including
*   pin settings, capture/compare settings, and PWM configurations.
*/

 #pragma once

#include <memory>
#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "drivers/interfaces/ITimerChannel.hpp"
#include "drivers/stm32f3discovery/io/GpioPin.hpp"

typedef void (*InterruptCallback)(void);


/**
 * @class TimerChannel
 * @brief Represents the configurations for an individual channel of a general-purpose timer.
 * 
 * This class serves as a container for the configuration settings needed to set up an individual channel 
 * of a general-purpose timer. It stores the necessary parameters that define the behavior and connection 
 * of the timer channel, including pin selection, alternate function, capture/compare mode, interrupt 
 * settings, and other configuration options.
 * 
 * The `TimerChannel` class does **not** perform any actual operations; it simply holds and organizes 
 * the configuration data to be passed to the timer during setup.
 * 
 * @note The actual initialization of GPIO pins and timer settings should be performed separately, typically 
 * in a higher-level function that uses this configuration.
 */
class TimerChannel : public ITimerChannel
{
public:

    TimerChannel() = default;

    /**
     * @brief Initializes the channel configuration with the specified capture/compare selection mode.
     * 
     * @param selection The capture/compare selection mode to be used for the timer channel.
     */
    void Init(Timer::eCaptureCompareSelection selection);
    
    /**
     * @brief Gets the pointer to the GPIO pin associated with this timer channel.
     */
    std::shared_ptr<PinBase> GetChannelPin() const override;
    
    /**
     * @brief Pointer to the GPIO pin associated with this timer channel.
     */
    std::shared_ptr<PinBase> mpChannelPin = nullptr;

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
