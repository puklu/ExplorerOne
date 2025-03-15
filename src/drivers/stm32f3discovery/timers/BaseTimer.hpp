#pragma once

#include "stm32f303xc.h"

#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "common/time.hpp"
#include "drivers/interfaces/ITimer.hpp"

using InterruptCallback = void(*)(void);

class BaseTimer : public ITimer
{
public:
    BaseTimer(uint16_t prescalerValue, uint32_t autoReloadRegisterValue, InterruptCallback cb);
    
    float GetSysClockTicksElapsed() const override;
    
    Microseconds GetTimeElapsedInMicroseconds() const override;
    
    Milliseconds GetTimeElapsedInMilliseconds() const override;
    
    void IncrementNumberOfTimesHighestValueReached();

    /**
     * @brief To get if the timer is running.
     *
     * @return Returns true if the timer is running, else false.
     */
    bool GetIsTimerRunning() const override;


protected:

    template<typename TimeUnit>
    TimeUnit GetTimeElapsed(const TimeUnit& period) const;

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

    
    uint16_t GetCounterValue() const;

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
    uint32_t                     mAutoReloadRegisterValue;
    InterruptCallback            mCallBack;
    Timer::eUpdateRequestSource  mUpdateRequestSource;        
    IRQn_Type                    mIrqNumber;
    bool                         mIs32bitTimer = false;
    bool                         mIsInitialized = false;
    bool                         mIsTimerRunning = false;
    Microseconds                 mPeriodOfCounterClockMicroSeconds;
    Milliseconds                 mPeriodOfCounterClockMilliSeconds;
    Seconds                      mPeriodOfCounterClockSeconds;
    int                          mNumberOfTimesHighestValueReached;
    
};
