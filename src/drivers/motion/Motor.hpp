/**
 * @file Motor.hpp
 * @brief Header file for the Motor class, which controls a motor using a PWM and a digital pin.
 *
 * This file defines the Motor class, which provides functionality to control a motor using
 * a PWM timer for speed control and a digital pin for direction control. It supports operations
 * like moving forward, backward, and halting the motor.
 * 
 * An instance of this class represent one motor with two pins. The speed is controlled using a PWM
 * signal. To simplify the operation, only 1 PWM input is sufficient to drive the motor at desired
 * speed (in percentage of highest speed). The other pin of the motor can simply just be connected to
 * a digital pin.
 */

#pragma once

#include<memory>


/**
 * @class Motor
 * @brief Represents a motor controlled by a PWM signal and a digital pin.
 *
 * This class provides functionality to control a motor using a PWM timer for speed control
 * and a digital pin for direction control. It supports operations like moving forward,
 * backward, and halting the motor.
 */

class IDigitalOutputPin;
class ITimer;
class PinBase;
enum class eGeneralStatus : uint8_t;

class Motor
{
public:
    /**
     * @brief Constructs a Motor object.
     *
     * @param pwm_timer A shared pointer to the GeneralPurposeTimer used for PWM control.
     * @param pwm_channel_index The channel index of the PWM timer for this motor.
     * @param digital_pin A pointer to the GpioPin used for digital control (direction).
     */
    Motor(std::shared_ptr<ITimer> pwm_timer, uint8_t pwm_channel_index, std::shared_ptr<IDigitalOutputPin> digital_pin);

     /**
     * @brief Stops the motor by setting the PWM duty cycle to 0%.
     *
     * @return eGeneralStatus::SUCCESS if the motor is successfully halted.
     */
    eGeneralStatus Halt();

    /**
     * @brief Moves the motor forward at the specified speed.
     *
     * @param speed_percent The speed of the motor as a percentage (0-100).
     * @return eGeneralStatus::SUCCESS if the motor is successfully moved forward.
     */
    eGeneralStatus Forward(int8_t speed_percent);

    /**
     * @brief Moves the motor backward at the specified speed.
     *
     * @param speed_percent The speed of the motor as a percentage (0-100).
     * @return eGeneralStatus::SUCCESS if the motor is successfully moved backward, otherwise an error code.
     */
    eGeneralStatus Backward(int8_t speed_percent);

private:
    std::shared_ptr<PinBase> mpPwmChannelPin = nullptr; ///< Pointer to the GPIO pin used for PWM signal output.
    /// < Since the speed of a motor can be controlled by using only 1 PWM signal, other pin is a simple digital output pin
    std::shared_ptr<IDigitalOutputPin> mpDigitalPin = nullptr; 
    uint8_t  mPwmChannelIndex = 0;      ///< The channel index of the PWM timer for this motor.
    uint8_t  mPwmPeriodMs = 2;          ///< The period of the PWM signal in milliseconds.
     ///< Shared pointer to the PWM timer used for speed control. Shared pointer is used because different channels of the same PWM timer can be used with different motors.
    std::shared_ptr<ITimer> mpPwmTimer;
};
