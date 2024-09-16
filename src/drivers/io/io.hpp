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

#define PIN_MASK (0x0F)
#define PORT_OFFSET (4u)
#define PORT_MASK (0x70)

/**
 * @enum
 * @brief The purpose of this enum is to extract port number and pin number
 * from an enum. There are 6(ports) times 16(pin in each port) numbers in this
 * enum. 
 * @example The number represented by PB1 is 17, which is 0001 0001
 *          Since there are 16 pins per port, we need 4 bits. The last
 *          4 bits can be used to extract pin number. And there are 6 
 *          ports so the next 3 bits can be used to extract port.
 *          
 *          PB1 = 0001 0001
 *          pin = PB1 & 0x0F = 0000 0001, which is 1 (pin indexing starts from 0)
 *                here 0X0F is PIN_MASK
 * 
 *          port = (PB1 & 0X70) >> 4 = 0000 0001, which is 1 (port indexing starts from 0)
 *                 here 0x70 is PORT_MASK, and 4 is PORT_OFFSET
 *  
*/

typedef enum : uint8_t {
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
    PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
    PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
    PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
    PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,
} io_generic_port;


struct port_a
{
    enum class pin : uint8_t
    {
        IO_FRONT_MOTOR_RIGHT_A = PA0,
        IO_FRONT_MOTOR_RIGHT_B = PA1,
        IO_FRONT_MOTOR_LEFT_A = PA2,
        IO_FRONT_MOTOR_LEFT_B = PA3,
        IO_BACK_MOTOR_RIGHT_A = PA4,
        IO_BACK_MOTOR_RIGHT_B = PA5,
        IO_BACK_MOTOR_LEFT_A = PA6,
        IO_BACK_MOTOR_LEFT_B = PA7,
        IO_FRONT_ULTRASONIC_SENSOR_TRIG = PA8,
        IO_FRONT_ULTRASONIC_SENSOR_ECHO = PA9,
        IO_BACK_ULTRASONIC_SENSOR_TRIG = PA10,
        IO_BACK_ULTRASONIC_SENSOR_ECHO = PA11,
        IO_UNUSED_12 = PA12,
        IO_UNUSED_13 = PA13,
        IO_UNUSED_14 = PA14,
        IO_UNUSED_15 = PA15,
    };
};

struct port_b
{
    enum class pin : uint8_t
    {
        IO_UNUSED_0 = PB0,
        IO_UNUSED_1 = PB1,
        IO_UNUSED_2 = PB2,
        IO_UNUSED_3 = PB3,
        IO_UNUSED_4 = PB4,
        IO_UNUSED_5 = PB5,
        IO_UNUSED_6 = PB6,
        IO_UNUSED_7 = PB7,
        IO_UNUSED_8 = PB8,
        IO_UNUSED_9 = PB9,
        IO_UNUSED_10 = PB10,
        IO_UNUSED_11 = PB11,
        IO_UNUSED_12 = PB12,
        IO_UNUSED_13 = PB13,
        IO_UNUSED_14 = PB14,
        IO_UNUSED_15 = PB15,
    };
};

struct port_c
{
    enum class pin : uint8_t
    {
        IO_UNUSED_0 = PC0,
        IO_UNUSED_1 = PC1,
        IO_UNUSED_2 = PC2,
        IO_UNUSED_3 = PC3,
        IO_UNUSED_4 = PC4,
        IO_UNUSED_5 = PC5,
        IO_UNUSED_6 = PC6,
        IO_UNUSED_7 = PC7,
        IO_UNUSED_8 = PC8,
        IO_UNUSED_9 = PC9,
        IO_UNUSED_10 = PC10,
        IO_UNUSED_11 = PC11,
        IO_UNUSED_12 = PC12,
        IO_UNUSED_13 = PC13,
        IO_UNUSED_14 = PC14,
        IO_UNUSED_15 = PC15,
    };
};

struct port_d
{
    enum class pin : uint8_t
    {
        IO_UNUSED_0 = PD0,
        IO_UNUSED_1 = PD1,
        IO_UNUSED_2 = PD2,
        IO_UNUSED_3 = PD3,
        IO_UNUSED_4 = PD4,
        IO_UNUSED_5 = PD5,
        IO_UNUSED_6 = PD6,
        IO_UNUSED_7 = PD7,
        IO_UNUSED_8 = PD8,
        IO_UNUSED_9 = PD9,
        IO_UNUSED_10 = PD10,
        IO_UNUSED_11 = PD11,
        IO_UNUSED_12 = PD12,
        IO_UNUSED_13 = PD13,
        IO_UNUSED_14 = PD14,
        IO_UNUSED_15 = PD15,
    };
};

struct port_e
{
    enum class pin : uint8_t
    {
        IO_UNUSED_0 = PE0,
        IO_UNUSED_1 = PE1,
        IO_UNUSED_2 = PE2,
        IO_UNUSED_3 = PE3,
        IO_UNUSED_4 = PE4,
        IO_UNUSED_5 = PE5,
        IO_UNUSED_6 = PE6,
        IO_TEST_LED = PE7,
        IO_UNUSED_8 = PE8,
        IO_UNUSED_9 = PE9,
        IO_UNUSED_10 = PE10,
        IO_UNUSED_11 = PE11,
        IO_UNUSED_12 = PE12,
        IO_UNUSED_13 = PE13,
        IO_UNUSED_14 = PE14,
        IO_UNUSED_15 = PE15,
    };
};

struct port_f
{
    enum class pin : uint8_t
    {
        IO_UNUSED_0 = PF0,
        IO_UNUSED_1 = PF1,
        IO_UNUSED_2 = PF2,
        IO_UNUSED_3 = PF3,
        IO_UNUSED_4 = PF4,
        IO_UNUSED_5 = PF5,
        IO_UNUSED_6 = PF6,
        IO_UNUSED_7 = PF7,
        IO_UNUSED_8 = PF8,
        IO_UNUSED_9 = PF9,
        IO_UNUSED_10 = PF10,
        IO_UNUSED_11 = PF11,
        IO_UNUSED_12 = PF12,
        IO_UNUSED_13 = PF13,
        IO_UNUSED_14 = PF14,
        IO_UNUSED_15 = PF15,
    };
};

enum class direction : uint8_t
{
    IO_DIRECTION_OUTPUT,
    IO_DIRECTION_INPUT,
};

enum class pupd_resistor : uint8_t
{
    IO_RESISTOR_PULL_DOWN,
    IO_RESISTOR_PULL_UP,
};

template <typename T>
void enable(T port);

template <typename T>
void set_direction(T port, typename T::pin pin, direction dir);

template <typename T>
void set_resistor(T port, typename T::pin pin, pupd_resistor resistor);

}  // namespace IO