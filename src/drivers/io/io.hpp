/**
 * @file io.hpp
 * IO pins handling including pin mapping, initialization, and configuration.
 * This is a wrapper for registers defines provided by stm
 */

#pragma once
#include <cstdint>

/**
 * @namespace IO
 * @brief All IO related variables, constants and functions should fall
 * under this namespace
 */
namespace IO
{

/**
 * @enum
 * @brief The purpose of this enum is to extract port number and pin number
 * from an enum. There are 6(ports) times 16(pin in each port) numbers in this
 * enum.
 * @details The number represented by PB1 is 17, which is 0001 0001
 *          Since there are 16 pins per port, we need 4 bits. The last
 *          4 bits can be used to extract pin number. And there are 6
 *          ports so the next 3 bits can be used to extract port.
 *
 *          PB1 = 0001 0001
 *          pin = PB1 & 0x0F = 0000 0001, which is 1 (pin indexing starts from
 * 0) here 0X0F is PIN_MASK
 *
 *          port = (PB1 & 0X70) >> 4 = 0000 0001, which is 1 (port indexing
 * starts from 0) here 0x70 is PORT_MASK, and 4 is PORT_OFFSET
 *
 */

typedef enum : uint8_t
{
    // clang-format off
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
    PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
    PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
    PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
    PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,
    // clang-format on
} eGeneric_port;

/**
 * @enum
 * @brief Enum for the user-friendly names of each pin on the board.
 */
typedef enum : uint8_t
{
    IO_FRONT_MOTOR_RIGHT_A          = PA0,
    IO_FRONT_MOTOR_RIGHT_B          = PA1,
    IO_FRONT_MOTOR_LEFT_A           = PA2,
    IO_FRONT_MOTOR_LEFT_B           = PA3,
    IO_BACK_MOTOR_RIGHT_A           = PA4,
    IO_BACK_MOTOR_RIGHT_B           = PA5,
    IO_BACK_MOTOR_LEFT_A            = PA6,
    IO_BACK_MOTOR_LEFT_B            = PA7,
    IO_FRONT_ULTRASONIC_SENSOR_TRIG = PA8,
    IO_FRONT_ULTRASONIC_SENSOR_ECHO = PA9,
    IO_BACK_ULTRASONIC_SENSOR_TRIG  = PA10,
    IO_BACK_ULTRASONIC_SENSOR_ECHO  = PA11,
    IO_UNUSED_A12                   = PA12,
    IO_UNUSED_A13                   = PA13,
    IO_UNUSED_A14                   = PA14,
    IO_UNUSED_A15                   = PA15,
    IO_UNUSED_B0                    = PB0,
    IO_UNUSED_B1                    = PB1,
    IO_UNUSED_B2                    = PB2,
    IO_UNUSED_B3                    = PB3,
    IO_UNUSED_B4                    = PB4,
    IO_UNUSED_B5                    = PB5,
    IO_UNUSED_B6                    = PB6,
    IO_UNUSED_B7                    = PB7,
    IO_UNUSED_B8                    = PB8,
    IO_UNUSED_B9                    = PB9,
    IO_UNUSED_B10                   = PB10,
    IO_UNUSED_B11                   = PB11,
    IO_UNUSED_B12                   = PB12,
    IO_UNUSED_B13                   = PB13,
    IO_UNUSED_B14                   = PB14,
    IO_UNUSED_B15                   = PB15,
    IO_UNUSED_C0                    = PC0,
    IO_UNUSED_C1                    = PC1,
    IO_UNUSED_C2                    = PC2,
    IO_UNUSED_C3                    = PC3,
    IO_UNUSED_C4                    = PC4,
    IO_UNUSED_C5                    = PC5,
    IO_UNUSED_C6                    = PC6,
    IO_UNUSED_C7                    = PC7,
    IO_UNUSED_C8                    = PC8,
    IO_UNUSED_C9                    = PC9,
    IO_UNUSED_C10                   = PC10,
    IO_UNUSED_C11                   = PC11,
    IO_UNUSED_C12                   = PC12,
    IO_UNUSED_C13                   = PC13,
    IO_UNUSED_C14                   = PC14,
    IO_UNUSED_C15                   = PC15,
    IO_UNUSED_D0                    = PD0,
    IO_UNUSED_D1                    = PD1,
    IO_UNUSED_D2                    = PD2,
    IO_UNUSED_D3                    = PD3,
    IO_UNUSED_D4                    = PD4,
    IO_UNUSED_D5                    = PD5,
    IO_UNUSED_D6                    = PD6,
    IO_UNUSED_D7                    = PD7,
    IO_UNUSED_D8                    = PD8,
    IO_UNUSED_D9                    = PD9,
    IO_UNUSED_D10                   = PD10,
    IO_UNUSED_D11                   = PD11,
    IO_UNUSED_D12                   = PD12,
    IO_UNUSED_D13                   = PD13,
    IO_UNUSED_D14                   = PD14,
    IO_UNUSED_D15                   = PD15,
    IO_UNUSED_E0                    = PE0,
    IO_UNUSED_E1                    = PE1,
    IO_UNUSED_E2                    = PE2,
    IO_UNUSED_E3                    = PE3,
    IO_UNUSED_E4                    = PE4,
    IO_UNUSED_E5                    = PE5,
    IO_UNUSED_E6                    = PE6,
    IO_UNUSED_E7                    = PE7,
    IO_TEST_LED_LD4                 = PE8,
    IO_TEST_LED_LD3                 = PE9,
    IO_TEST_LED_LD5                 = PE10,
    IO_TEST_LED_LD7                 = PE11,
    IO_TEST_LED_LD9                 = PE12,
    IO_TEST_LED_LD10                = PE13,
    IO_TEST_LED_LD8                 = PE14,
    IO_TEST_LED_LD6                 = PE15,
    IO_UNUSED_F0                    = PF0,
    IO_UNUSED_F1                    = PF1,
    IO_UNUSED_F2                    = PF2,
    IO_UNUSED_F3                    = PF3,
    IO_UNUSED_F4                    = PF4,
    IO_UNUSED_F5                    = PF5,
    IO_UNUSED_F6                    = PF6,
    IO_UNUSED_F7                    = PF7,
    IO_UNUSED_F8                    = PF8,
    IO_UNUSED_F9                    = PF9,
    IO_UNUSED_F10                   = PF10,
    IO_UNUSED_F11                   = PF11,
    IO_UNUSED_F12                   = PF12,
    IO_UNUSED_F13                   = PF13,
    IO_UNUSED_F14                   = PF14,
    IO_UNUSED_F15                   = PF15,
} ePin;

/**
 * @enum
 * @brief Enum for direction of an IO pin.
 */
enum class eDirection : uint8_t
{
    IO_DIRECTION_INPUT,
    IO_DIRECTION_OUTPUT,
    IO_DIRECTION_ALT_FUNCTION_MODE,
    IO_DIRECTION_ANALOG_MODE,
};

/**
 * @enum
 * @brief Enum for output type of an IO pin.
 */
enum class eOutputType : uint8_t
{
    IO_OUTPUT_PUSH_PULL,
    IO_OUTPUT_OPEN_DRAIN,
};

/**
 * @enum
 * @brief Enum for output speed of an IO pin.
 */
enum class eOutputSpeed : uint8_t
{
    IO_OUTPUT_LOW_SPEED,
    IO_OUTPUT_MEDIUM_SPEED,
    IO_OUTPUT_HIGH_SPEED,
};

/**
 * @enum
 * @brief Enum for pull-up/pull-down resistor direction of an IO pin.
 */
enum class ePupdResistor : uint8_t
{
    IO_RESISTOR_NO_PUPD,
    IO_RESISTOR_PULL_UP,
    IO_RESISTOR_PULL_DOWN,
    IO_RESISTOR_RESERVED,
};

/**
 * @enum
 * @brief Enum for digital values at an IO pin, i.e low or high.
 */
typedef enum : uint32_t
{
    IO_VALUE_LOW,
    IO_VALUE_HIGH
} eValue;

class GPIOpin
{
   public:
    GPIOpin(IO::ePin pinName);

   private:
    uint8_t GetPortNumber(ePin pin_name);
    uint8_t GetPinNumber(ePin pin_name);
    void    Enable(ePin pin_name);
    void    SetDirection(ePin pin_name, eDirection dir);
    void    SetResistor(ePin pin_name, ePupdResistor updown);
    eValue  ReadInputValue(ePin pin_name);
    eValue  ReadOutputValue(ePin pin_name);
    void    WriteOutputValue(ePin pin_name, eValue value);

    uint8_t       pinNumber;
    uint8_t       portNumber;
    bool          isInitialized;
    eDirection    direction;
    ePupdResistor pupdResistor;
    eValue        valueAtPin;
};

/**
 * @brief Returns the port number from the name of a given pin.
 *
 * @param One of the pin names from ePin enum given to a pin, for example
 * IO_TEST_LED
 * @return Port numbers (numbers start from 0)
 */
uint8_t GetPortNumber(ePin pin_name);

/**
 * @brief Returns the pin number from the name of a given pin.
 *
 * @param  One of the pin names from ePin enum given to a pin, for example
 * IO_TEST_LED
 * @return Pin numbers (numbers start from 0)
 */
uint8_t GetPinNumber(ePin pin_name);

/**
 * @brief Enables the port to which the pin belongs.
 * @param One of the pin names from ePin enum given to a pin, for example
 * IO_TEST_LED
 */
void Enable(ePin pin_name);

/**
 * @brief Sets the direction of pin, whether it is being used as an input
 * or an output pin?
 * @param One of the pin names from ePin enum given to a pin, for example
 * IO_TEST_LED.
 * @param One of the directions from eDirection enum.
 */
void SetDirection(ePin pin_name, eDirection dir);

/**
 * @brief Sets the pull-up pull-down resistor of the pin, whether the pin should
 * be pulled-up or pulled-down is set using this.
 * @param One of the pin names from ePin enum given to a pin, for example
 * IO_TEST_LED.
 * @param One of the values from ePupdResistor enum.
 */
void SetResistor(ePin pin_name, ePupdResistor updown);

/**
 * @brief Reads value present at an input pin
 * @param One of the pin names from ePin enum given to a pin from which
 * the value is to be read, for example IO_TEST_LED.
 * @returns Value present at the input pin
 */
eValue ReadInputValue(ePin pin_name);

/**
 * @brief Reads value present at an output pin
 * @param One of the pin names from ePin enum given to a pin from which
 * the value is to be read, for example IO_TEST_LED.
 * @returns Value present at the output pin
 */
eValue ReadOutputValue(ePin pin_name);

/**
 * @brief Writes value to an output pin
 * @param One of the pin names from ePin enum given to a pin to which
 * the value is to be written, for example IO_TEST_LED.
 * @param Value to be written to the pin, high or low.
 * @returns Value present at the output pin
 */
void WriteOutputValue(ePin pin_name, eValue value);

}  // namespace IO