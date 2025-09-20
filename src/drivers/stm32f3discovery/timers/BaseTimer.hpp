#pragma once

#include "stm32f303xc.h"

#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "common/time.hpp"
#include "drivers/interfaces/ITimer.hpp"

using InterruptCallback = void(*)(void);

class BaseTimer : public virtual ITimer
{
public:
    BaseTimer(uint16_t prescalerValue, uint32_t autoReloadRegisterValue, InterruptCallback cb);

    virtual ~BaseTimer() = default;

    /**
     * @brief To get the sys clock ticks elapsed since timer started.
     * 
     * TODO: Fix me!
     * 
     * @warning Do not use without fixing me!
     *
     * @return Microseconds
     */
    uint32_t GetSysClockTicksElapsedSinceStart() const override;
    
    /**
     * @brief To get the time elapsed since timer started in microseconds.
     *
     * @return Microseconds
     */
    Microseconds GetTimeElapsedInMicrosecondsSinceStart() const override;
  
    /**
     * @brief To get the time elapsed since timer started in milliseconds.
     *
     * @return Milliseconds
     */
    Milliseconds GetTimeElapsedInMillisecondsSinceStart() const override;

    /**
     * @brief Keeps a count of overflows of ARR.
     * 
     * Whenever the count of the counter reaches ARR it overflows. This function
     * keeps track of how many times it has overflowed for calulation of time.
     * 
     * TODO: Works fine for now since it affects an int variable but maybe that is not
     * enough and smart enough? 
     *
     */
    void IncrementCountOfOverflows();

    /**
     * @brief To get if the timer is running.
     *
     * @return Returns true if the timer is running, else false.
     */
    bool GetIsTimerRunning() const override;

    // /**
    //  * @brief Configures the timer period and counter value.
    //  *
    //  * This function sets the timer's prescaler value and auto-reload register value based on the provided period (in milliseconds) and count.
    //  *
    //  * @param[in] period The desired timer period in milliseconds. This is used to calculate the prescaler value.
    //  * @param[in] count The desired counter value to be set in the auto-reload register.
    //  *
    //  * @return `eGeneralStatus::SUCCESS` if the configuration is applied successfully.
    //  *
    //  * @note The prescaler value is computed based on the system clock (`SYS_CLK`) to achieve the desired timing period. 
    //  *       The prescaler and auto-reload registers are updated using `SetPrescalerValue` and `SetAutoReloadRegisterValue`.
    //  *
    //  * @warning Ensure that the provided period and count are within valid ranges for the timer hardware. 
    //  *          The prescaler value must not exceed its hardware limit, and the count must match the timer's resolution.
    //  */
    // eGeneralStatus SetPeriodAndCount(Milliseconds period, uint32_t count) override;

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
     * @brief Configures the timer period for a specific channel.
     *
     * This function sets the timer's prescaler value, calculates the auto-reload register value based on the desired
     * period for the specified channel.
     *
     * Depending on whether the timer is 32-bit or not, it chooses the appropriate prescaler value.
     *
     * @param[in] period The desired timer period in milliseconds.
     * 
     * @warning Timing goes all out of whack when time period is set to 0.01_ms. Fix me
     * before using at such a low resolution
     * 
     * TODO: Fix me for very small periods (if possible).
     * 
     * TODO: Make it accept time period in any unit. 
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful.
     *
     * @see mPrescalerValue, mAutoReloadRegisterValue, mpChannels
     */
    eGeneralStatus SetPeriod(Milliseconds period) override;


protected:

    /**
     * @brief Returns the time elapsed since the timer started in seconds.
     */
    Seconds GetTimeElapsedSinceStart() const;


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
    eGeneralStatus EnableNVIC();

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

    
    uint32_t GetCounterValue() const;

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
    
    
    RCC_TypeDef                 *mpRCC = RCC;               ///< Pointer to the RCC (Reset and Clock Control) peripheral.
    TIM_TypeDef                 *mpTimer = nullptr;         ///< Pointer to the timer peripheral.
    uint16_t                     mPrescalerValue;
    uint32_t                     mAutoReloadRegisterValue;  // TODO: It is 16 bit for 16 bit bit timer and 32 for 32 bit timer. make it clean
    InterruptCallback            mCallBack;
    IRQn_Type                    mIrqNumber;
    bool                         mIs32bitTimer = false;
    bool                         mIsInitialized = false;
    bool                         mIsTimerRunning = false;
    Microseconds                 mPeriodOfCounterClockMicroSeconds;
    Milliseconds                 mPeriodOfCounterClockMilliSeconds;
    Seconds                      mPeriodOfCounterClockSeconds;
    volatile int                 mCountOfOverflows;
    
};
