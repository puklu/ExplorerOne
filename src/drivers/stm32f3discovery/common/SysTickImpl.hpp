#pragma once

#include <stm32f303xc.h>
#include "common/time.hpp"
#include "drivers/interfaces/ISystick.hpp"

/**
 * @brief Forward declaration of SysTick ISR with C linkage.
 * 
 * This function is called by the hardware timer interrupt.
 */
extern "C" void SysTick_Handler();

/**
 * @class SysTickImpl
 * @brief Singleton implementation of the SysTick timer interface.
 * 
 * Provides blocking delays and tick counting using the MCU's SysTick peripheral.
 * Inherits from ISysTick interface.
 */
class SysTickImpl : public ISysTick
{
public:
    /**
     * @brief Get the singleton instance of SysTickImpl.
     * 
     * @return Pointer to the SysTickImpl instance.
     */ 
    static SysTickImpl* GetInstance();

    /**
     * @brief Sets up the systick peripheral.
     * 
     * Sets up the SysTick to generate interrupts at the specified frequency
     * using the provided AHB clock frequency, enables the counter,
     * enables the interrupts
     * 
     * @param freq Desired interrupt frequency in Hz.
     * @param ahb_clock System AHB clock in Hz.
     * @return eGeneralStatus Status of the setup operation.
     */
    eGeneralStatus SystickSetup(uint32_t freq, uint32_t ahb_clock) override;

    /**
     * @brief Block execution for the specified delay.
     * 
     * Uses the SysTick tick counter to create a busy-wait delay.
     * 
     * @param ms Delay duration in milliseconds.
     * @return eGeneralStatus Status of the delay operation.
     */
    eGeneralStatus SystickDelay(Milliseconds ms) override;

private:
    SysTick_Type *mpSystick;         /// Pointer to the hardware SysTick peripheral registers
    static SysTickImpl *mpInstance;  /// Singleton instance
    static volatile uint64_t mTicks; /// Tick counter updated by the ISR

    /**
     * @brief Private constructor for the singleton.
     */
    SysTickImpl();
    
    /**
     * @brief Set the SysTick interrupt frequency.
     * 
     * @param freq Desired frequency in Hz.
     * @param ahb_clock System AHB clock in Hz.
     */
    void SystickSetInterruptFrequency(uint32_t freq, uint32_t ahb_clock);

    /**
     * @brief Enable the SysTick counter.
     */
    void SystickCounterEnable();

    /**
     * @brief Enable the SysTick interrupt.
     */
    void SystickInterruptEnable();

    /**
     * @brief Retrieve the current tick count.
     * 
     * @return Current tick count as uint64_t.
     */
    uint64_t GetTicks();

    /**
     * @brief Allow the SysTick ISR to access private members.
     */
    friend void ::SysTick_Handler();

    /// Delete copy constructor to prevent multiple instances
    SysTickImpl(const SysTickImpl&) = delete;

    /// Delete assignment operator to prevent multiple instances
    SysTickImpl& operator=(const SysTickImpl&) = delete;
};
