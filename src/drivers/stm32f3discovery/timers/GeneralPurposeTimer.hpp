/**
 * @file GeneralPurpose.hpp
 * 
 * @brief Header file for the GeneralPurposeTimer class, providing general-purpose timer functionalities.
 *
 * This file defines the `GeneralPurposeTimer` class, which is responsible for configuring,
 * managing, and controlling general-purpose timers used in embedded systems. The class provides
 * support for output compare modes, input capture, PWM generation, and callback functionality.
 * 
 * The `GeneralPurposeTimer` class can be used to set up timers for various purposes such as LED blinking,
 * motor control, and pulse-width modulation (PWM) generation. It offers extensive configuration options,
 * including timer modes, output compare settings, input capture settings, and interrupt handling.
 * 
 * Example usage:
 * 
 * ```cpp
 * GpioPinInitStruct pinInit = {};
 * pinInit.pin_name          = IO::ePin::IO_UNUSED_B0;
 * pinInit.mode              = IO::eMode::IO_MODE_ALT_FUNCTION;
 * pinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;
 *
 * PinBase *tim2_ch2_pin = 
 *     PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, pinInit);
 * 
 * auto pin = static_cast<GpioPin *>(tim2_ch2_pin);
 *
 * uint8_t channel_index = 2;
 *
 * GeneralPurposeTimerConfig gptimer_config;
 * gptimer_config.mChannels[channel_index].mpChannelPin = pin;
 * gptimer_config.mChannels[channel_index].mAlternateFunction = IO::eAlternateFunction::IO_AF2;
 * gptimer_config.mChannels[channel_index].mSelection = Timer::eCaptureCompareSelection::OUTPUT;
 * gptimer_config.mChannels[channel_index].mOutputCompareConfig.mOutputCompareMode = Timer::eOutputCompareMode::PWM_MODE_1;
 * gptimer_config.mChannels[channel_index].mCaptureCompareEnable = Timer::eCaptureCompare::ENABLE;
 * gptimer_config.mChannels[channel_index].mCaptureCompareCallbackFunction = InterruptLed;
 * gptimer_config.mChannels[channel_index].mOutputCompareConfig.mPwmDutyCyclePercent = 50;
 * gptimer_config.mChannels[channel_index].mOutputCompareConfig.mPwmPeriodMs = 1000;
 * gptimer_config.mChannels[channel_index].mOutputCompareConfig.mOutputComparePreloadEnable = Timer::eOutputComparePreloadEnable::ENABLE;
 *
 * GeneralPurposeTimer gp_timer(gptimer_config);
 *
 * gp_timer.Start();
 * ```
 * 
 * In this example, a timer configured is for PWM mode. A GPIO pin is set up
 * and connected to the timer's channel. The timer configuration includes PWM mode, output compare,
 * duty cycle, period, and callback function. The timer is then started, allowing PWM generation.
 */

#pragma once

#include <array>
#include <memory>
#include "stm32f303xc.h"
#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "drivers/stm32f3discovery/io/GpioPin.hpp"
#include "drivers/interfaces/ITimer.hpp"
#include "drivers/interfaces/ITimerChannelConfig.hpp"
#include "ChannelConfig.hpp"
#include "GeneralPurposeTimerConfig.hpp"

typedef void (*InterruptCallback)(void);

/**
 * @class GeneralPurposeTimer
 * 
 * @brief A class representing a general-purpose timer.
 * 
 * This class implements the `ITimer` interface and provides functionalities for configuring, 
 * managing, and controlling general-purpose timers in embedded systems. The `GeneralPurposeTimer` 
 * class supports features such as output compare, input capture, PWM generation, timer interrupts, 
 * and callback handling.
 * 
 * It allows users to set up timers with various configurations, including output compare modes, 
 * input capture settings, auto-reload values, and interrupt handling. The class handles interactions 
 * with the timer hardware, translating high-level configurations into appropriate register settings 
 * and initiating timer operations.
 */
class GeneralPurposeTimer : public ITimer
{
public:
    /**
     * @brief Constructs a GeneralPurposeTimer object with the specified configuration.
     *
     * This constructor initializes the timer based on the provided configuration. It sets up 
     * the prescaler, auto-reload value, and capture/compare registers. Additionally, it configures 
     * the timer channels and ensures proper hardware initialization.
     *
     * @param timer_config A reference to the configuration structure containing initialization parameters:
     *                     - `mPrescalerValue`: Prescaler value for the timer.
     *                     - `mAutoReloadRegisterValue`: Auto-reload value for the timer.
     *                     - `mCb`: Callback function for timer interrupts.
     *                     - `mChannels`: Array of channel configurations.
     */
    explicit GeneralPurposeTimer(GeneralPurposeTimerConfig  const &timer_config);

    /**
     * @brief Starts the General Purpose Timer.
     *
     * This function begins the operation of the timer by triggering an update event, 
     * setting the necessary control registers, and enabling the interrupt for the timer.
     *
     * @return `eGeneralStatus::SUCCESS` if the timer is successfully started.
     *
     * @note The function asserts that the timer has been initialized before starting it.
     *       Ensure that `mIsInitialized` is set to `true` during initialization.
     *
     * @note The following actions are performed:
     *       - Triggers an update event to synchronize the timer.
     *       - Configures the control registers for the timer's operation.
     *       - Enables the interrupt to handle timer-related events.
     *
     * @warning This function must be called only after the timer has been properly initialized.
     *          Calling it without initialization will result in an assertion failure.
     */
    eGeneralStatus Start() override;

    /**
     * @brief Stops the General Purpose Timer.
     *
     * This function disables the timer by clearing the enable bit in the control register (CR1).
     *
     * @return `eGeneralStatus::SUCCESS` if the timer is successfully stopped.
     *
     * @note The function modifies the `CR1` register to disable the timer:
     *       - Clears the bit at position 0 (`CR1[0]`) to stop the timer.
     *
     * @warning Ensure that no critical operations are relying on the timer before calling this function.
     */
    eGeneralStatus Stop() override;

    /**
     * @brief Resets the General Purpose Timer.
     *
     * This function triggers an update event to reset the timer counter and reload the prescaler and auto-reload register values.
     *
     * @return `eGeneralStatus::SUCCESS` if the timer is successfully reset.
     *
     * @note The update event is triggered by invoking the `TriggerUpdateEvent` function, which ensures the timer's internal state is reset.
     *       This operation does not stop the timer; it merely resets its counter and configuration.
     *
     * @warning Ensure that triggering a reset does not interfere with ongoing timer operations or associated callbacks.
     */
    eGeneralStatus Reset() override;

    /**
     * @brief Configures the timer period and counter value.
     *
     * This function sets the timer's prescaler value and auto-reload register value based on the provided period (in milliseconds) and count.
     *
     * @param[in] period_in_ms The desired timer period in milliseconds. This is used to calculate the prescaler value.
     * @param[in] count The desired counter value to be set in the auto-reload register.
     *
     * @return `eGeneralStatus::SUCCESS` if the configuration is applied successfully.
     *
     * @note The prescaler value is computed based on the system clock (`SYS_CLK`) to achieve the desired timing period. 
     *       The prescaler and auto-reload registers are updated using `SetPrescalerValue` and `SetAutoReloadRegisterValue`.
     *
     * @warning Ensure that the provided period and count are within valid ranges for the timer hardware. 
     *          The prescaler value must not exceed its hardware limit, and the count must match the timer's resolution.
     */
    eGeneralStatus SetPeriodAndCount(uint32_t period_in_ms, uint32_t count) override;

    /**
     * @brief Enables interrupts for the timer.
     *
     * This function activates the necessary configurations to enable timer-related interrupts, including DMA and NVIC settings.
     *
     * @return `eGeneralStatus::SUCCESS` if the interrupts are successfully enabled.
     *
     * @note This function ensures that both the DMA and NVIC configurations are set up correctly to handle timer interrupts.
     *
     * @see EnableInterrupts()
     * @see EnableNVIC()
     */
    eGeneralStatus EnableInterrupt() override;

    /**
     * @brief Disables interrupts for the timer.
     *
     * This function deactivates the configurations related to timer interrupts, including DMA and NVIC settings.
     *
     * @return `eGeneralStatus::SUCCESS` if the interrupts are successfully disabled.
     *
     * @note This function ensures that both the DMA and NVIC configurations are appropriately disabled to prevent further timer interrupts.
     *
     * @see DisableInterrupts()
     */
    eGeneralStatus DisableInterrupt() override;

    /**
     * @brief Sets the prescaler value for the timer.
     *
     * Configures the timer's prescaler to divide the input clock frequency as per the specified value. 
     * The prescaler determines the rate at which the timer increments.
     *
     * @return `eGeneralStatus::SUCCESS` if the prescaler value is successfully set.
     *
     * @pre `mpTimer` must be a valid timer instance.
     * @pre `mPrescalerValue` must be in the range [1, 0xFFFF].
     *
     * @note This function directly updates the timer's prescaler register (PSC).
     *       Ensure that `mPrescalerValue` is set correctly before calling this function.
     *
     * @see mPrescalerValue
     */
    eGeneralStatus SetPrescalerValue();

    /**
     * @brief Sets the Auto-Reload Register (ARR) value for the timer.
     *
     * Configures the timer's Auto-Reload Register to determine the period of the timer.
     * The ARR value defines the count at which the timer resets back to zero.
     *
     * @return `eGeneralStatus::SUCCESS` if the ARR value is successfully set.
     *
     * @pre `mpTimer` must be a valid timer instance.
     * @pre The appropriate limit is checked based on whether the timer is 32-bit or 16-bit.
     *
     * @note This function directly updates the timer's ARR register.
     *       Ensure that `mAutoReloadRegisterValue` is set correctly before calling this function.
     *
     * @see mAutoReloadRegisterValue
     */
    eGeneralStatus SetAutoReloadRegisterValue();

    /**
     * @brief Configures the Capture/Compare registers for each timer channel.
     *
     * This function sets up the necessary configurations for each capture/compare channel in the timer.
     * It configures the CCMR register, CCR register, and the associated functionality (output compare or input capture).
     * 
     * @return `eGeneralStatus::SUCCESS` if the configuration is successfully applied.
     *
     * @pre `mpTimer` must be a valid timer instance.
     * @pre Each channel must have a valid pin associated (`channel.mpChannelPin` should not be `nullptr`).
     *
     * @note This function handles both output and input capture configurations, based on the channel selection.
     * 
     * @see ChannelConfig, Timer, SetAlternateFunction, ConfigureCaptureCompareSelection, ConfigureOutputComparePreloadEnable,
     *      ConfigureOutputCompareMode, EnableOutputCompare, ConfigureInputCapturePrescaler, ConfigureInputCaptureFilter,
     *      EnableInputCapture
     */   
    eGeneralStatus ConfigureCaptureCompareRegisters();

    /**
     * @brief Retrieves the interrupt callback function associated with the timer.
     *
     * This function returns the interrupt callback function that was configured during the timer setup.
     * The callback function is triggered when the timer's interrupt occurs.
     *
     * @return The interrupt callback function (`InterruptCallback`).
     *
     * @pre The timer must have been initialized with a valid callback function.
     * 
     * @see InterruptCallback, mCallBack
     */
    InterruptCallback GetInterruptCallback();

    /**
     * @brief Retrieves the array of channel configurations associated with the timer.
     *
     * This function returns a pointer to the array containing the configurations for each channel of the timer.
     * The returned array can be used to access individual channel settings such as capture/compare registers, 
     * pins, and their respective configurations.
     *
     * @return Pointer to the array of channel configurations (`ChannelConfig`).
     *
     * @pre The timer must have been initialized with valid channel configurations.
     *
     * @see ChannelConfig, mChannels
     */
    std::array<std::shared_ptr<ITimerChannelConfig>, GENERAL_PURPOSE_TIMER_NUM_CHANNELS> GetChannels() override;

    /**
     * @brief Clears the interrupt flags for the timer.
     *
     * This function resets various interrupt flags in the status register (`SR`) of the timer.
     * It clears flags associated with capture compares, trigger events, and updates.
     *
     * @note The specific bits are cleared based on the timer's interrupt sources, including:
     *       - Capture Compare 1, 2, 3, and 4 interrupt flags
     *       - Trigger interrupt flag
     *       - Update interrupt flag
     *
     * This function should be called after handling the corresponding interrupts to acknowledge and clear them.
     *
     * @return `eGeneralStatus::SUCCESS` indicating successful execution.
     */
    eGeneralStatus ClearInterrupt();

    /**
     * @brief Configures the timer period and duty cycle for a specific channel.
     *
     * This function sets the timer's prescaler value, calculates the auto-reload register value based on the desired
     * period, and then sets the capture/compare register value corresponding to the duty cycle for the specified channel.
     *
     * Depending on whether the timer is 32-bit or not, it chooses the appropriate prescaler value.
     *
     * @param[in] period_in_ms The desired timer period in milliseconds.
     * @param[in] duty_cycle The desired duty cycle in percentage (0-100).
     * @param[in] channel_index The index of the channel for which the configuration is to be applied.
     *
     * @note This function calculates the CCR value based on the given duty cycle and sets it in the specified channel.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful.
     *
     * @see mPrescalerValue, mAutoReloadRegisterValue, mChannels
     */
    eGeneralStatus SetPeriodAndDutyCycle(uint32_t period_in_ms, uint32_t duty_cycle, uint8_t channel_index) override;

    /**
     * @brief Destructor for the `GeneralPurposeTimer` class.
     *
     * This destructor cleans up the timer instance by removing its reference from the global `generalPurposeTimers` array.
     * It sets the appropriate slot in the global timer array to `nullptr`, indicating that this timer instance is no longer active.
     *
     * @note This is the cleanup function that is automatically called when an object of this class goes out of scope.
     *
     * @see generalPurposeTimers
     */
    ~GeneralPurposeTimer();

    /**
     * @brief To get if the timer is running.
     *
     * @return Returns true if the timer is running, else false.
     */
    bool GetIsTimerRunning() const override;

private:
    /**
     * @brief Configures the control registers for the general-purpose timer.
     *
     * This function sets up various control aspects of the timer based on predefined configurations:
     * - Disables clock division (no division).
     * - Enables auto-reload.
     * - Configures edge-aligned mode.
     * - Sets the timer direction (up counting).
     * - Disables update request source.
     * - Enables update event.
     * - Sets the TI1 selection (input capture source).
     * - Enables the timer counter.
     *
     * This configuration is intended to initialize the timer in a basic mode. Adjustments may be made based on specific use cases.
     *
     * @note This function assumes that the timer instance (`mpTimer`) has been previously initialized.
     *
     * @return `eGeneralStatus::SUCCESS` on successful configuration, or an appropriate error code otherwise.
     */
    eGeneralStatus SetControlRegisters();

    /**
     * @brief Enables interrupt functionality for the timer.
     *
     * This function configures the timer to trigger interrupt handling.
     * - Enables trigger interrupt for the timer.
     * - Enables interrupts for capture compare channels 1, 2, 3, and 4.
     * - Enables the general interrupt for the timer.
     *
     * @note This function assumes that the timer instance (`mpTimer`) has been previously initialized.
     *
     * @return `eGeneralStatus::SUCCESS` on successful enabling, or an appropriate error code otherwise.
     *
     * @see mpTimer, eGeneralStatus
     */
    eGeneralStatus EnableInterrupts();

    /**
     * @brief Enables DMA functionality for the timer.
     *
     * This function configures the timer to handle DMA requests.
     * - Enables DMA trigger request for the timer.
     * - Enables DMA requests for capture compare channels 1, 2, 3, and 4.
     * - Enables the general DMA request for the timer.
     *
     * @note This function assumes that the timer instance (`mpTimer`) has been previously initialized.
     *
     * @return `eGeneralStatus::SUCCESS` on successful enabling, or an appropriate error code otherwise.
     *
     * @see mpTimer, eGeneralStatus
     */
    eGeneralStatus EnableDma();

    /**
     * @brief Disables interrupt functionality for the timer.
     *
     * This function clears the enable flags for  interrupts in the timer's DIER register.
     * - Disables trigger interrupt for the timer.
     * - Disables interrupts for capture compare channels 1, 2, 3, and 4.
     * - Disables general interrupt for the timer.
     *
     * @note This function assumes that the timer instance (`mpTimer`) has been previously initialized.
     *
     * @return `eGeneralStatus::SUCCESS` on successful disabling, or an appropriate error code otherwise.
     */  
    eGeneralStatus DisableInterrupts();

    /**
     * @brief Disables DMA functionality for the timer.
     *
     *  This function clears the enable flags for DMA requests in the timer's DIER register.
     * - Disables DMA trigger request for the timer.
     * - Disables DMA requests for capture compare channels 1, 2, 3, and 4.
     * - Disables the general DMA request for the timer.
     *
     * @note This function assumes that the timer instance (`mpTimer`) has been previously initialized.
     *
     * @return `eGeneralStatus::SUCCESS` on successful disabling, or an appropriate error code otherwise.
     *
     * @see mpTimer, eGeneralStatus
     */
    eGeneralStatus DisableDma();

    /**
     * @brief Triggers various update events in the timer.
     *
     * This function sets the necessary flags in the timer's EGR (Event Generation Register) to trigger 
     * update events such as:
     * - Update generation
     * - Capture compare 1, 2, 3, and 4 events generation
     *
     * Triggering these events can be used to reload auto-reload registers, update counters, 
     * or synchronize other peripheral actions.
     *
     * This function assumes that the timer instance (`mpTimer`) has been previously initialized.
     *
     * @note The actual effect depends on the configuration and the current state of the timer.
     */
    void TriggerUpdateEvent();

    /**
     * @brief Enables the Nested Vector Interrupt Controller (NVIC) for the timer's interrupt.
     *
     * This function configures the NVIC to enable the interrupt associated with the timer and sets its priority.
     * It assumes that the IRQ number (`mIrqNumber`) has been set during the timer's initialization.
     * 
     * The priority is set based on a pre-defined macro `PRIORITY_TIMER`, which can be adjusted as needed.
     *
     * @note The NVIC (Nested Vector Interrupt Controller) must be properly configured for the interrupt handling 
     * to work as expected.
     */
    void EnableNVIC();

    /**
     * @brief Selects the TIM (Timer) peripheral based on the GPIO pin and alternate function.
     *
     * This function maps a given GPIO pin and its alternate function to the appropriate TIM peripheral. 
     * It uses lookup tables to find the TIM instance corresponding to the specified GPIO port and pin.
     * 
     * The TIM peripheral is set based on the mapping and the function checks if all channels belong to the same TIM instance.
     * If they do not, an assertion is triggered.
     *
     * @note This function assumes that each GPIO port has a mapping table (`aAltFunctionsAdressesPortX`) 
     * that defines the alternate functions (AF) and the corresponding TIM instances.
     *
     * @param channel_pin A std::shared_ptr<PinBase> pointer to the GPIO pin object.
     * @param af Alternate function to be mapped to the TIM.
     * 
     * @return `eGeneralStatus::SUCCESS` if the TIM is successfully selected, otherwise asserts in case of failure.
     */
    eGeneralStatus SelectTIM(std::shared_ptr<PinBase>, IO::eAlternateFunction af);

    /**
     * @brief Configures the capture/compare selection for a specific timer channel.
     *
     * This function sets up the appropriate capture/compare mode for a given channel based on the selection provided.
     * The selection defines whether the channel will operate in output mode, input capture mode, or other specialized modes.
     * The function adjusts the corresponding capture/compare mode register (`CCMR`) accordingly.
     * 
     * It also performs bit manipulation on the appropriate CCMR register to enable or disable capture/compare functionalities.
     * 
     * @param selection The capture/compare selection (e.g., output, input, etc.) to configure for the timer channel.
     * @param channel_index Index of the timer channel to configure.
     * 
     * @return `eGeneralStatus::SUCCESS` if the configuration is successfully set, 
     *         `eGeneralStatus::FAILURE` if an unknown or unsupported capture/compare selection is passed.
     * 
     * @note The function assumes predefined masks for input and output configurations based on `aGeneralPurposeTimerCcmrOutputCompareRegisterMasks`
     *       and `aGeneralPurposeTimerCcmrInputCaptureRegisterMasks` arrays.
     * 
     * @see Timer::eCaptureCompareSelection, aGeneralPurposeTimerCcmrOutputCompareRegisterMasks, aGeneralPurposeTimerCcmrInputCaptureRegisterMasks
     */
    eGeneralStatus ConfigureCaptureCompareSelection(Timer::eCaptureCompareSelection selection, uint8_t channel_index);

    /**
     * @brief Configures the input capture prescaler for a specific timer channel.
     *
     * This function sets the input capture prescaler configuration for a given timer channel.
     * The prescaler controls how often the input capture event is triggered.
     *
     * Based on the provided `prescaler` value, the function manipulates the appropriate register (`CCMR`)
     * to select one of the following modes:
     * - No prescaler
     * - Capture once every 2 events
     * - Capture once every 4 events
     * - Capture once every 8 events
     *
     * The function modifies specific bits in the `CCMR` register to apply the desired prescaler setting.
     * 
     * @param prescaler The input capture prescaler to configure.
     * @param channel_index Index of the timer channel to configure.
     * 
     * @return `eGeneralStatus::SUCCESS` if the prescaler configuration is successfully set,
     *         `eGeneralStatus::FAILURE` if an unknown or unsupported prescaler value is passed.
     *
     * @note The function assumes predefined masks for input capture configurations based on
     *       `aGeneralPurposeTimerCcmrInputCaptureRegisterMasks` array.
     */
    eGeneralStatus ConfigureInputCapturePrescaler(Timer::eInputCapturePrescaler prescaler, uint8_t channel_index);

    /**
     * @brief Configures the input capture filter for a specific timer channel.
     *
     * This function sets the input capture filter configuration for a given timer channel.
     * 
     * The appropriate bits in the `CCMR` register are manipulated based on the provided `filter` value to apply the desired filter setting.
     *
     * @param filter The input capture filter to configure.
     * @param channel_index Index of the timer channel to configure.
     * 
     * @return `eGeneralStatus::SUCCESS` if the filter configuration is successfully set,
     *         `eGeneralStatus::FAILURE` if an unknown or unsupported filter value is passed.
     *
     * @note The function uses predefined masks for input capture configurations from 
     *       `aGeneralPurposeTimerCcmrInputCaptureRegisterMasks` array.
     */
    eGeneralStatus ConfigureInputCaptureFilter(Timer::eInputCaptureFilter filter, uint8_t channel_index);

    /**
     * @brief Configures the output compare mode for a specific timer channel.
     * 
     * This function sets the output compare mode of a specified timer channel. 
     * It updates the CCMR (Capture/Compare Mode Register) based on the provided mode.
     * 
     * @param mode The desired output compare mode. It can be one of the values from the Timer::eOutputCompareMode enum.
     * @param channel_index The index of the timer channel to configure.
     * 
     * @return eGeneralStatus::SUCCESS if the configuration is successful,
     *         eGeneralStatus::FAILURE if an unknown mode is provided.
     */
    eGeneralStatus ConfigureOutputCompareMode(Timer::eOutputCompareMode mode, uint8_t channel_index);

    /**
     * @brief Configures the output compare preload enable setting for a specific timer channel.
     * 
     * This function sets whether output compare preload is enabled or disabled for a given timer channel.
     * It updates the CCMR (Capture/Compare Mode Register) based on the specified preload enable mode.
     * 
     * @param preload_enable The desired preload enable setting. It can be one of the values from the Timer::eOutputComparePreloadEnable enum.
     * @param channel_index The index of the timer channel to configure.
     * 
     * @return eGeneralStatus::SUCCESS if the configuration is successful,
     *         eGeneralStatus::FAILURE if an unknown preload enable mode is provided.
     */
    eGeneralStatus ConfigureOutputComparePreloadEnable(Timer::eOutputComparePreloadEnable preload_enable, uint8_t channel_index);

    /**
     * @brief Enables or disables the output compare function for a specific timer channel.
     * 
     * This function controls the activation of the output compare feature on a given timer channel.
     * It updates the CCMR (Capture/Compare Mode Register) and associated control registers based on 
     * the specified enable mode.
     * 
     * @param enable The desired capture/compare function state. It can be either `ENABLE` or `DISABLE` from the Timer::eCaptureCompare enum.
     * @param channel_index The index of the timer channel to configure.
     * 
     * @return eGeneralStatus::SUCCESS if the configuration is applied successfully,
     *         eGeneralStatus::FAILURE if an unknown enable mode is provided.
     */
    eGeneralStatus EnableOutputCompare(Timer::eCaptureCompare enable, uint8_t channel_index);

    /**
     * @brief Enables or disables the input capture function for a specific timer channel.
     * 
     * This function controls the activation of the input capture feature on a given timer channel.
     * It updates the CCMR (Capture/Compare Mode Register) and associated control registers based on 
     * the specified enable mode. Currently, only non-inverted rising edge detection is implemented.
     * 
     * @param enable The desired capture/compare function state. It can be either `ENABLE` or `DISABLE` from the Timer::eCaptureCompare enum.
     * @param channel_index The index of the timer channel to configure.
     * 
     * @return eGeneralStatus::SUCCESS if the configuration is applied successfully,
     *         eGeneralStatus::FAILURE if an unknown enable mode is provided.
     */
    eGeneralStatus EnableInputCapture(Timer::eCaptureCompare enable, uint8_t channel_index);

    /**
     * @brief Configures the alternate function for a specified timer channel pin.
     * 
     * This function sets the alternate function mode for a given timer channel pin. 
     * It ensures that the provided pin is valid and supports alternate functions. 
     * The function first verifies the pin mode and then sets the appropriate alternate function.
     * 
     * @param channel_config A structure containing the pin and the desired alternate function.
     *                        It includes `mpChannelPin` (the pin object) and `mAlternateFunction` (the desired function).
     * 
     * @return eGeneralStatus::SUCCESS if the configuration is successfully applied,
     *         eGeneralStatus::FAILURE if the configuration fails (invalid pin or function).
     */
    static eGeneralStatus SetAlternateFunction(ChannelConfig channel_config);

    /**
     * @brief Sets up the timer for operation.
     * 
     * This function initializes the timer based on its index. It performs checks to 
     * ensure the timer's auto-reload register value is within a valid range and then 
     * configures clock enable, IRQ settings, and adds the instance to the global array.
     * 
     * @return eGeneralStatus::SUCCESS if the timer is successfully configured,
     *         eGeneralStatus::FAILURE if any of the setup steps fail.
     */
    eGeneralStatus SetUpTimer();

    /**
     * @brief Determines the index of the timer based on the pointer to the timer instance.
     * 
     * This function checks the provided timer instance and returns the index corresponding 
     * to the specific timer (TIM2, TIM3, TIM4). If the timer is not recognized, it asserts an error.
     * 
     * @return uint8_t The index of the timer (0 for TIM2, 1 for TIM3, 2 for TIM4).
     *         If an invalid timer is provided, it asserts an error and returns a fallback value 
     *         (NUMBER_OF_GENERAL_PURPOSE_TIMERS).
     */
    uint8_t GetTimerIndex();

    /**
     * @brief Sets (enables) specific bits in a register.
     *
     * This function performs a bitwise OR operation on the target register with the provided mask.
     * The bits corresponding to `1`s in the mask will be set to `1` in the register, while other bits remain unchanged.
     *
     * @param rRegister Reference to the target register (volatile uint32_t).
     * @param rMask Mask specifying the bits to set. Bits set to `1` in the mask will be set in the register.
     *
     * @note The register is modified in place.
     * @note The function assumes the register is a 32-bit unsigned integer.
     */
    void SetBits(volatile uint32_t& rRegister, const uint32_t& rMask) const;

    /**
     * @brief Resets (clears) specific bits in a register.
     *
     * This function performs a bitwise AND operation on the target register with the complement of the provided mask.
     * The bits corresponding to `1`s in the mask will be cleared (set to `0`) in the register, while other bits remain unchanged.
     *a
     * @param rRegister Reference to the target register (volatile uint32_t).
     * @param rMask Mask specifying the bits to clear. Bits set to `1` in the mask will be cleared in the register.
     *
     * @note The register is modified in place.
     * @note The function assumes the register is a 32-bit unsigned integer.
     */
    void ResetBits(volatile uint32_t& rRegister, const uint32_t& rMask) const;


    RCC_TypeDef                         *mpRCC = RCC;
    TIM_TypeDef                         *mpTimer = nullptr;
    uint16_t                             mPrescalerValue;
    uint32_t                             mAutoReloadRegisterValue;
    Timer::eUpdateRequestSource          mUpdateRequestSource;
    InterruptCallback                    mCallBack;
    IRQn_Type                            mIrqNumber;
    std::array<std::shared_ptr<ITimerChannelConfig>, GENERAL_PURPOSE_TIMER_NUM_CHANNELS>     mChannels;
    bool                                 mIs32bitTimer = false;
    bool                                 mIsInitialized = false;
    bool                                 mIsTimerRunning = false;

};
