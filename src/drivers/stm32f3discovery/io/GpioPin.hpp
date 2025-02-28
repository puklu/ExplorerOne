/**
 * @file io.hpp
 * IO pins handling including pin mapping, initialization, and configuration.
 */

#pragma once

#include <memory>
#include <cstdint>
#include "stm32f303xc.h"

#include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
#include "drivers/interfaces/IDigitalOutputPin.hpp"
#include "drivers/interfaces/IPin.hpp"
#include "drivers/interfaces/PinBase.hpp"


struct GpioPinInitStruct: public PinBaseInitStruct
{
    IO::eMode mode;
    IO::eOutputType output_type = IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET;
    IO::eOutputSpeed output_speed = IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET;
    IO::ePupdResistor pupd_resistor = IO::ePupdResistor::IO_RESISTOR_NOT_SET;
};


/**
 * @class GpioPin
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
 * 
 *  GpioPinInitStruct ledPinInit = {};
 *  ledPinInit.pin_name      = IO::ePin::IO_TEST_LED_LD5_ORANGE;
 *  ledPinInit.mode          = IO::eMode::IO_MODE_OUTPUT;
 *  ledPinInit.output_type   = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL;
 *  ledPinInit.pupd_resistor = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;
 *
 *  PinBase *gpio_pin = PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, ledPinInit);
 *  auto ledPin = static_cast<GpioPin*>(gpio_pin);
 *
 *  ledPin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
 * @endcode
 */
class GpioPin : public PinBase, public IDigitalOutputPin
{
public:
    /**
     * @brief Creates an instance of GPIO pin.
     *
     * @param pin_init_struct Struct containing the init data.
     */
    static std::shared_ptr<GpioPin> Create(const GpioPinInitStruct &pin_init_struct);

    /**
     * @brief Sets the mode of the GPIO pin.
     *
     * @param mode The mode to set for the GPIO pin (input, output,
     * alternate function, or analog).
     */
    void SetMode(IO::eMode mode);

    /**
     * @brief Gets the mode of the GPIO pin.
     */
    IO::eMode GetMode();

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
    void SetOutputType(IO::eOutputType outType);

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
    void SetOutputSpeed(IO::eOutputSpeed outSpeed);

    /**
     * @brief Configures the pull-up/pull-down resistor for the GPIO pin.
     *
     * @param updown The resistor configuration to apply (none, pull-up,
     * pull-down, or reserved).
     */
    void SetResistor(IO::ePupdResistor updown);

    /**
     * @brief Reads the input value from the GPIO pin.
     *
     * @return The current value of the pin (high or low).
     */
    IO::eValue ReadInputValue();

    /**
     * @brief Reads the output value from the GPIO pin.
     *
     * @return The last value written to the pin (high or low).
     */
    IO::eValue ReadOutputValue();

    /**
     * @brief Writes a value to the GPIO pin.
     *
     * @param value The value to write to the pin (high or low).
     */
    void WriteOutputValue(IO::eValue value) override;

    /**
     * @brief Sets the alternate function for the pin.
     *
     * Configures the alternate function setting on this pin.
     * 
     * @param af The alternate function to be set for the pin.
     * 
     */
    void SetAlternateFunction(IO::eAlternateFunction af);


private:
    
    /**
     * Private constructor. An instance can be created only through
     * CreatePin function.
    */
    explicit GpioPin(const GpioPinInitStruct &pin_init_struct);

    friend class PinFactory; // Allows PinFactory to access private constructor

    IO::eMode           mMode          = IO::eMode::IO_MODE_NOT_SET;
    IO::eOutputType     mOutputType    = IO::eOutputType::IO_OUTPUT_TYPE_NOT_SET;
    IO::eOutputSpeed    mOutputSpeed   = IO::eOutputSpeed::IO_OUTPUT_SPEED_NOT_SET;
    IO::ePupdResistor   mPupdResistor  = IO::ePupdResistor::IO_RESISTOR_NO_PUPD;
    IO::eValue          mValueAtPin    = IO::eValue::IO_VALUE_UNKNOWN;
};
