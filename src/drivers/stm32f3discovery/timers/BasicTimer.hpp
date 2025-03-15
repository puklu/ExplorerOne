#pragma once

#include "stm32f303xc.h"

#include "BaseTimer.hpp"
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
 * @brief Struct representing the initialization configuration for a basic timer.
 * 
 * This struct encapsulates the necessary configuration parameters for initializing
 * a basic timer, including prescaler value, auto-reload register value, interrupt
 * callback, counter mode, preload settings, update sources, and more.
 */
struct BasicTimerConfig
{
    uint16_t prescaler_value = 7999;
    uint16_t auto_reload_register_value = 1000;
    InterruptCallback cb;
    Timer::eOnePulseMode one_pulse_mode = Timer::eOnePulseMode::DISABLE_ONE_PULSE_MODE;
    Timer::eAutoReloadPreload enable_auto_reload_preload = Timer::eAutoReloadPreload::ARR_BUFFERED;
    Timer::eUpdateRequestSource update_request_source = Timer::eUpdateRequestSource::ONLY_OVERFLOW_UNDERFLOW;
    Timer::eUpdateEvent enable_update_event = Timer::eUpdateEvent::ENABLE_EVENT_GENERATION;
    Timer::eCounterEnable enable_counter = Timer::eCounterEnable::ENABLE;
    Timer::eMasterModeSelection master_mode_selection = Timer::eMasterModeSelection::SEND_RESET;
    Timer::eUpdateDmaRequest update_dma_request = Timer::eUpdateDmaRequest::DISABLE;
    Timer::eUpdateInterrupt enable_update_interrupt = Timer::eUpdateInterrupt::ENABLE; 
};


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
     * Initializes the timer using the provided BasicTimerConfig configuration.
     * It sets up the timer's prescaler, auto-reload values, callback, and enables clock 
     * to the timer based on the global timer arrays.
     * 
     * @param timer_init_struct Reference to the BasicTimerConfig containing configuration.
     */
    explicit BasicTimer(BasicTimerConfig  const &timer_init_struct);

    /**
     * @brief Starts the basic timer.
     * 
     * Configures the timer control registers and enables the interrupts.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
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
     * @brief Sets the timer period and count.
     * 
     * Configures the timer with the provided period in milliseconds and the highest count
     * value before the counter resets to 0.
     * 
     * @param period The desired timer period in milliseconds.
     * @param count The desired highest timer count value.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */
    eGeneralStatus SetPeriodAndCount(Milliseconds period, uint32_t count) override;

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
     * @brief Sets the auto-reload register value for the timer.
     * 
     * Configures the auto-reload register value based on the desired period.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */    
    eGeneralStatus SetAutoReloadRegisterValue();

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
    ~BasicTimer();

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
     * @brief Enables DMA and timer interrupts.
     * 
     * Configures the timer's DMA and interrupt enable registers.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */    
    eGeneralStatus EnableDmaAndInterrupt();

    /**
     * @brief Disables DMA and timer interrupts.
     * 
     * Disables DMA and interrupt handling related to the timer.
     * 
     * @return `eGeneralStatus::SUCCESS` if the operation is successful, otherwise an error status.
     */    
    eGeneralStatus DisableDmaAndInterrupt();

    /**
     * @brief Triggers an update event to reload the timer configuration.
     * 
     * Manually triggers an update event which reloads the configuration settings.
     */    
    void TriggerUpdateEvent();
 
};
