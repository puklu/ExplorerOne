/**
 * @file io.hpp
 * IO pins handling including pin mapping, initialization, and configuration.
 * This is a wrapper for registers defines provided by stm
 */

#pragma once
#include <cstdint>

#include "enums.hpp"
#include "stm32f303xc.h"

/**
 * @namespace IO
 * @brief All IO related variables, constants and functions should fall
 * under this namespace
 */
namespace IO
{

/**
 * @class GPIOpin
 *
 * @brief Represents a General Purpose Input/Output (GPIO) pin on the
 * microcontroller.
 *
 * This class provides an interface to configure and control the GPIO pins,
 * including setting the pin mode, enabling pull-up/down resistors,
 * and reading/writing output values. The user can initialize a pin by
 * specifying its identifier, which allows for easy interaction with the
 * hardware.
 *
 * Example usage:
 * @code
 * IO::GPIOpin ledPin(IO::ePin::IO_TEST_LED_LD7);
 * ledPin.SetResistor(IO::ePupdResistor::IO_RESISTOR_PULL_DOWN);
 * ledPin.SetMode(IO::eMode::IO_MODE_OUTPUT);
 * ledPin.WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
 * @endcode
 */
class GPIOpin
{
   public:
    explicit GPIOpin(ePin pin_name);

    /**
     * @brief Sets the mode of the GPIO pin.
     *
     * @param mode The mode to set for the GPIO pin (input, output,
     * alternate function, or analog).
     */
    void SetMode(eMode mode);

    /**
     * @brief Configures the output type of the GPIO pin.
     *
     * This function sets the output type of the GPIO pin to either push-pull or
     * open-drain.
     *
     * @param outType The desired output type for the GPIO pin.
     *                - `IO::eOutputType::PUSH_PULL`: Configures the pin for
     * push-pull output mode.
     *                - `IO::eOutputType::OPEN_DRAIN`: Configures the pin for
     * open-drain output mode.
     *
     * @note This function should be called after the pin has been initialized.
     */
    void SetOutputType(eOutputType outType);

    /**
     * @brief Configures the output speed of the GPIO pin.
     *
     * This function sets the desired output speed for the GPIO pin.
     * The speed determines how fast the pin can toggle between high and low
     * states.
     *
     * @param outSpeed The desired output speed for the GPIO pin.
     *                 - `IO::eOutputSpeed::LOW_SPEED`: Low-speed operation.
     *                 - `IO::eOutputSpeed::MEDIUM_SPEED`: Medium-speed
     * operation.
     *                 - `IO::eOutputSpeed::HIGH_SPEED`: High-speed operation.
     *
     * @note The output speed should be chosen based on the application
     * requirements, considering power consumption and signal integrity.
     */
    void SetOutputSpeed(eOutputSpeed outSpeed);

    /**
     * @brief Configures the pull-up/pull-down resistor for the GPIO pin.
     *
     * @param updown The resistor configuration to apply (none, pull-up,
     * pull-down, or reserved).
     */
    void SetResistor(ePupdResistor updown);

    /**
     * @brief Reads the input value from the GPIO pin.
     *
     * @return The current value of the pin (high or low).
     */
    eValue ReadInputValue();

    /**
     * @brief Reads the output value from the GPIO pin.
     *
     * @return The last value written to the pin (high or low).
     */
    eValue ReadOutputValue();

    /**
     * @brief Writes a value to the GPIO pin.
     *
     * @param value The value to write to the pin (high or low).
     */
    void WriteOutputValue(eValue value);


    void EnableInterrupt();
    void SelectInterruptTrigger(eTriggerEdge);

   private:
    /**
     * @brief Enables the GPIO clock for the pin's port.
     *
     * This function ensures that the clock for the port containing the
     * specified pin is enabled, allowing for further configuration and
     * usage of the pin.
     */
    void Enable();

    /**
     * @brief Sets the port number based on the specified GPIO pin.
     *
     * This function extracts the port number from the pin name and
     * stores it in the member variable `portNumber`. It must be called
     * before using the GPIOpin instance to ensure proper configuration.
     */
    void SetPortNumber();

    /**
     * @brief Sets the pin number based on the specified GPIO pin.
     *
     * This function extracts the pin number from the pin name and
     * stores it in the member variable `pinNumber`. It must be called
     * before using the GPIOpin instance to ensure proper configuration.
     */
    void SetPinNumber();

    ePin            mPinName;
    uint8_t         mPinNumber     = UINT8_MAX;
    uint8_t         mPortNumber    = UINT8_MAX;
    bool            mIsInitialized = false;
    eMode           mMode          = IO::eMode::IO_MODE_NOT_SET;
    eOutputType     mOutputType    = IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET;
    eOutputSpeed    mOutputSpeed   = IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET;
    ePupdResistor   mPupdResistor  = IO::ePupdResistor::IO_RESISTOR_NO_PUPD;
    eValue          mValueAtPin    = IO::eValue::IO_VALUE_UNKNOWN;
    GPIO_TypeDef   *mpPort;
    RCC_TypeDef    *mpRCC          = RCC;
    SYSCFG_TypeDef *mpSystemConfigController = SYSCFG;
    EXTI_TypeDef   *mpInterruptController = EXTI;
};

}  // namespace IO