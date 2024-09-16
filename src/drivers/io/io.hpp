#pragma once
#include <cstdint>

/**
 * @file io.hpp
 * IO pins handling including pin mapping, initialization, and configuration.
 * This is a wrapper for registers defines provided by stm
 */

namespace IO
{

struct port_a
{
    enum class pin : uint8_t
    {
        IO_FRONT_MOTOR_RIGHT_A,
        IO_FRONT_MOTOR_RIGHT_B,
        IO_FRONT_MOTOR_LEFT_A,
        IO_FRONT_MOTOR_LEFT_B,
        IO_BACK_MOTOR_RIGHT_A,
        IO_BACK_MOTOR_RIGHT_B,
        IO_BACK_MOTOR_LEFT_A,
        IO_BACK_MOTOR_LEFT_B,
        IO_FRONT_ULTRASONIC_SENSOR_TRIG,
        IO_FRONT_ULTRASONIC_SENSOR_ECHO,
        IO_BACK_ULTRASONIC_SENSOR_TRIG,
        IO_BACK_ULTRASONIC_SENSOR_ECHO,
        IO_UNUSED_12,
        IO_UNUSED_13,
        IO_UNUSED_14,
        IO_UNUSED_15,
    };
};

struct port_b
{
    enum class pin : uint8_t
    {
        IO_UNUSED_0,
        IO_UNUSED_1,
        IO_UNUSED_2,
        IO_UNUSED_3,
        IO_UNUSED_4,
        IO_UNUSED_5,
        IO_UNUSED_6,
        IO_UNUSED_7,
        IO_UNUSED_8,
        IO_UNUSED_9,
        IO_UNUSED_10,
        IO_UNUSED_11,
        IO_UNUSED_12,
        IO_UNUSED_13,
        IO_UNUSED_14,
        IO_UNUSED_15,
    };
};

struct port_c
{
    enum class pin : uint8_t
    {
        IO_UNUSED_0,
        IO_UNUSED_1,
        IO_UNUSED_2,
        IO_UNUSED_3,
        IO_UNUSED_4,
        IO_UNUSED_5,
        IO_UNUSED_6,
        IO_UNUSED_7,
        IO_UNUSED_8,
        IO_UNUSED_9,
        IO_UNUSED_10,
        IO_UNUSED_11,
        IO_UNUSED_12,
        IO_UNUSED_13,
        IO_UNUSED_14,
        IO_UNUSED_15,
    };
};

struct port_d
{
    enum class pin : uint8_t
    {
        IO_UNUSED_0,
        IO_UNUSED_1,
        IO_UNUSED_2,
        IO_UNUSED_3,
        IO_UNUSED_4,
        IO_UNUSED_5,
        IO_UNUSED_6,
        IO_UNUSED_7,
        IO_UNUSED_8,
        IO_UNUSED_9,
        IO_UNUSED_10,
        IO_UNUSED_11,
        IO_UNUSED_12,
        IO_UNUSED_13,
        IO_UNUSED_14,
        IO_UNUSED_15,
    };
};

struct port_e
{
    enum class pin : uint8_t
    {
        IO_UNUSED_0,
        IO_UNUSED_1,
        IO_UNUSED_2,
        IO_UNUSED_3,
        IO_UNUSED_4,
        IO_UNUSED_5,
        IO_UNUSED_6,
        IO_TEST_LED,
        IO_UNUSED_8,
        IO_UNUSED_9,
        IO_UNUSED_10,
        IO_UNUSED_11,
        IO_UNUSED_12,
        IO_UNUSED_13,
        IO_UNUSED_14,
        IO_UNUSED_15,
    };
};

struct port_f
{
    enum class pin : uint8_t
    {
        IO_UNUSED_0,
        IO_UNUSED_1,
        IO_UNUSED_2,
        IO_UNUSED_3,
        IO_UNUSED_4,
        IO_UNUSED_5,
        IO_UNUSED_6,
        IO_UNUSED_7,
        IO_UNUSED_8,
        IO_UNUSED_9,
        IO_UNUSED_10,
        IO_UNUSED_11,
        IO_UNUSED_12,
        IO_UNUSED_13,
        IO_UNUSED_14,
        IO_UNUSED_15,
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