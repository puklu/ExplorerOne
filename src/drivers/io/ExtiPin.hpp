/**
 * @file ExtiPin.hpp
 * @brief Defines the ExtiPin class for external interrupt pins and associated functionalities.
 *
 * This file contains the declaration of the ExtiPin class and its initialization structure, 
 * ExtiPinInitStruct, which provides configuration options for an external interrupt pin.
 */

#pragma once

#include "common/PinDefinitions.hpp"
#include "drivers/interfaces/PinBase.hpp"


/**
 * @typedef InterruptCallback
 * @brief Defines a type for interrupt callback functions.
 *
 * This type is a pointer to a function that takes no arguments and returns void.
 * It is used to define the callback function triggered by the interrupt.
 */
typedef void (*InterruptCallback)(void);


/**
 * @struct ExtiPinInitStruct
 * @brief Structure for initializing an ExtiPin with configuration options.
 *
 * Inherits from PinBaseInitStruct and adds specific configurations for an external interrupt pin.
 */
struct ExtiPinInitStruct: public PinBaseInitStruct
{
    IO::eMode mode;
    IO::eOutputType output_type = IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET;
    IO::eOutputSpeed output_speed = IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET;
    IO::ePupdResistor pupd_resistor = IO::ePupdResistor::IO_RESISTOR_NOT_SET;
};


/**
 * @class ExtiPin
 * @brief Class for handling external interrupt pins with configurable trigger settings and callback.
 *
 * The ExtiPin class extends PinBase to provide functionality specific to external interrupt pins,
 * such as enabling and disabling interrupts, selecting interrupt trigger conditions, and managing
 * interrupt callbacks.
 */
class ExtiPin : public PinBase
{
public:
    /**
     * @brief Enables the interrupt for the pin and assigns a callback function.
     *
     * This function sets up the interrupt for the pin and assigns the provided callback
     * function to be called when the interrupt is triggered.
     *
     * @param cb The callback function to be called on interrupt.
     */
    void EnableInterrupt(InterruptCallback cb);

    /**
     * @brief Disables the interrupt for the pin.
     *
     * This function disables the interrupt associated with the pin.
     */
    void DisableInterrupt();

    /**
     * @brief Selects the trigger condition for the interrupt.
     *
     * Sets the trigger edge (rising, falling, or both) that activates the interrupt.
     *
     * @param edge The edge trigger condition for the interrupt.
     */
    void SelectInterruptTrigger(IO::eTriggerEdge);

    /**
     * @brief Clears the interrupt flag.
     *
     * This function clears the interrupt pending flag, effectively resetting the interrupt status.
     */
    void ClearInterrupt();

    /**
     * @brief Checks if an interrupt has occurred.
     * @return True if the interrupt is pending, false otherwise.
     */
    bool isInterruptPresent() const;

    /**
     * @brief Retrieves the currently set interrupt callback function.
     * @return The callback function assigned to this interrupt pin.
     */
    InterruptCallback GetInterruptCallback();

private:
    /**
     * @brief Private constructor to initialize an ExtiPin with the specified settings.
     *
     * The constructor is private to enforce usage via the PinFactory class.
     *
     * @param pin_init_struct Initialization structure with pin settings.
     */
    explicit ExtiPin(const ExtiPinInitStruct &pin_init_struct);

    friend class PinFactory; // Allows PinFactory to access private constructor

    /**
     * @brief Enables the NVIC for handling the interrupt.
     *
     * Configures the Nested Vectored Interrupt Controller (NVIC) for the pin.
     */
    void EnableNVIC();

    /**
     * @brief Retrieves the IRQ number associated with this pin.
     * @return The IRQn_Type enum value representing the IRQ number for the pin.
     */
    IRQn_Type GetIRQn() const;
    SYSCFG_TypeDef   *mpSystemConfigController = SYSCFG;
    EXTI_TypeDef     *mpInterruptController    = EXTI;
    IRQn_Type         mIrqNumber;
    InterruptCallback mInterruptCallbackFunction = nullptr;
};
