/**
 * @file io.hpp
 * IO pins handling including pin mapping, initialization, and configuration.
 * This is a wrapper for registers defines provided by stm
 */

#pragma once
#include <cstdint>

#include "enums.hpp"

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
 * including setting the pin direction, enabling pull-up/down resistors,
 * and reading/writing output values. The user can initialize a pin by
 * specifying its identifier, which allows for easy interaction with the
 * hardware.
 *
 * Example usage:
 * @code
 * IO::GPIOpin ledPin(IO::ePin::IO_TEST_LED_LD7);
 * ledPin.SetResistor(IO::ePupdResistor::IO_RESISTOR_PULL_DOWN);
 * ledPin.SetDirection(IO::eDirection::IO_DIRECTION_OUTPUT);
 * ledPin.WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
 * @endcode
 */
class GPIOpin
{
   public:
    explicit GPIOpin(ePin pin_name);

    /**
     * @brief Sets the direction of the GPIO pin.
     *
     * @param dir The direction to set for the GPIO pin (input, output,
     * alternate function, or analog).
     */
    void SetDirection(eDirection dir);

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

   private:
    /**
     * @brief Enables the GPIO clock for the pin's port.
     *
     * This function ensures that the clock for the port containing the
     * specified pin is enabled, allowing for further configuration and
     * usage of the pin.
     */
    void Enable() const;

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

    ePin       mPinName;
    uint8_t    mPinNumber     = UINT8_MAX;
    uint8_t    mPortNumber    = UINT8_MAX;
    bool       mIsInitialized = false;
    eDirection mDirection     = IO::eDirection::IO_DIRECTION_NOT_SET;
    // eOutputType   mOutputType;
    // eOutputSpeed  mOutputSpeed;
    ePupdResistor mPupdResistor = IO::ePupdResistor::IO_RESISTOR_NO_PUPD;
    eValue        mValueAtPin   = IO::eValue::IO_VALUE_UNKNOWN;
};

}  // namespace IO