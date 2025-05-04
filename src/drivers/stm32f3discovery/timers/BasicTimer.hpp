#pragma once

#include "stm32f303xc.h"

#include "BaseTimer.hpp"
#include "BasicTimerConfig.hpp"
#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "drivers/interfaces/ITimer.hpp"


/**
 * @brief Typedef for an interrupt callback function.
 * 
 * This type defines the signature of the interrupt callback function.
 */
using InterruptCallback = void(*)(void);


/**
 * @brief Class representing a basic timer.
 * 
 * This class provides an interface for configuring, starting, stopping, resetting, 
 * and handling interrupts for basic timers. It extends the `ITimer` interface and 
 * interacts with STM32F303 hardware timers.
 */
class BasicTimer : public BaseTimer
{
public:
    /**
     * @brief Constructor for BasicTimer.
     * 
     * Stores the provided configuration in internal member variables but does not 
     * perform any hardware-level initialization. Use Init() to apply the configuration 
     * and initialize the timer hardware.
     * 
     * @param timer_config Reference to the BasicTimerConfig containing configuration.
     */
    explicit BasicTimer(BasicTimerConfig  const &timer_config);

    /**
     * @brief Initializes the timer hardware using the stored configuration.
     *
     * Applies the configuration previously set in the constructor. This typically involves
     * setting up timer registers, enabling the clock, and configuring interrupts or callbacks
     * as necessary.
     *
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */
    eGeneralStatus Init() override;

    /**
     * @brief Starts the basic timer.
     * 
     * Configures the timer control registers and enables the interrupts.
     * 
     */
    eGeneralStatus Start() override;

    /**
     * @brief Stops the basic timer.
     * 
     * Disables the timer and stops the counting.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */
    eGeneralStatus Stop() override;

    /**
     * @brief Resets the basic timer.
     * 
     * Manually triggers an update event to reset the timer.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */   
    eGeneralStatus Reset() override;

    /**
     * @brief Enables the timer interrupts.
     * 
     * Enables DMA requests and interrupt handling for the timer.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */
    eGeneralStatus EnableInterrupt() override;

    /**
     * @brief Disables the timer interrupts.
     * 
     * Disables DMA requests and interrupts associated with the timer.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */    
    eGeneralStatus DisableInterrupt() override;

    /**
     * @brief Retrieves the interrupt callback function.
     * 
     * Returns the function pointer to the interrupt callback that is triggered when the timer event occurs.
     * 
     * @return The interrupt callback function.
     */    
    InterruptCallback GetInterruptCallback();

    /**
     * @brief Clears the timer interrupt.
     * 
     * Clears the interrupt flag after handling an interrupt event.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */    
    eGeneralStatus ClearInterrupt();

    /**
     * @brief Destructor for BasicTimer.
     * 
     * Releases any resources and cleans up the timer instance.
     */
    ~BasicTimer() override;

private:
    /**
     * @brief Configures the control registers of the timer.
     * 
     * Configures the necessary control registers to enable auto-reload, update requests, 
     * and starts the timer.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */
    eGeneralStatus SetControlRegisters();

    /**
     * @brief Enables DMA.
     * 
     * Configures the timer's DMA enable registers.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */    
    eGeneralStatus EnableDma();

        /**
     * @brief Enables timer interrupts.
     * 
     * Configures the timer's interrupt enable registers.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */    
    eGeneralStatus EnableInterrupts();

    /**
     * @brief Disables DMA.
     * 
     * Disables DMA related to the timer.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */    
    eGeneralStatus DisableDma();

        /**
     * @brief Disables timer interrupts.
     * 
     * Disables interrupt handling related to the timer.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */    
    eGeneralStatus DisableInterrupts();

    /**
     * @brief Triggers an update event to reload the timer configuration.
     * 
     * Manually triggers an update event which reloads the configuration settings.
     */    
    void TriggerUpdateEvent();



    BasicTimerConfig const &mrTimerConfig; 
};
