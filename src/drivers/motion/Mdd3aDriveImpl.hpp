/**
 * @file DriveImpl.hpp
 * @brief Header file for the Mdd3aDrive class, which implements the IDrive interface for 
 * controlling a drive system using two MDD3A motor drivers.
 *
 * This file defines the Mdd3aDrive class, which provides functionality to control a drive system consisting of four motors.
 * It supports operations like moving forward, backward, turning left, and turning right, with adjustable speed and turn radius.
 */

#pragma once

#include<memory>

#include "drivers/interfaces/IDrive.hpp"
#include "drivers/motion/Motor.hpp"

/**
 * @class Mdd3aDrive
 * @brief Implements the IDrive interface for controlling a drive system with four motors
 * using two MDD3A motor drivers. Each driver can drive two motors. So two drivers are
 * used to drive four motors. The input logic for a MDD3A motor driver is:
 * 
 * ### MDD3A Input logic
 * | IN A     |  IN B    | MOTOR    |
 * |----------|----------|----------|
 * |    0     |    0     | Brake    |
 * |    0     |    1     | Reverse  |
 * |    1     |    0     | Forward  |
 * |    1     |    1     | Brake    |
 *  
 * This class provides functionality to control a drive system consisting of four motors (front-right, front-left,
 * back-right, and back-left). It supports operations like moving forward, backward, turning left, and turning right,
 * with adjustable speed and turn radius.
 */
class Mdd3aDrive : public IDrive
{
public:
    /**
     * @brief Constructs a Mdd3aDrive object.
     *
     * @param pFrontRightMotor A unique pointer to the front-right motor.
     * @param pFrontLeftMotor A unique pointer to the front-left motor.
     * @param pBackRightMotor A unique pointer to the back-right motor.
     * @param pBackLeftMotor A unique pointer to the back-left motor.
     */
    Mdd3aDrive(std::unique_ptr<Motor> pFrontRightMotor, std::unique_ptr<Motor> pFrontLeftMotor, std::unique_ptr<Motor> pBackRightMotor, std::unique_ptr<Motor> pBackLeftMotor);

    /**
     * @brief Stops all motors in the drive system.
     *
     * @return eGeneralStatus::SUCCESS if all motors are successfully halted.
     */
    eGeneralStatus Halt() override;

    /**
     * @brief Moves the drive system forward at the specified speed.
     *
     * @param speed_percent The speed of the drive system as a percentage (0-100).
     * @return eGeneralStatus::SUCCESS if the drive system is successfully moved forward.
     */    
    eGeneralStatus Forward(int8_t speed_percent) override;

    /**
     * @brief Moves the drive system backward at the specified speed.
     *
     * @param speed_percent The speed of the drive system as a percentage (0-100).
     * @return eGeneralStatus::SUCCESS if the drive system is successfully moved backward, otherwise an error code.
     */
    eGeneralStatus Backward(int8_t speed_percent) override;

    /**
     * @brief Turns the drive system left at the specified speed and turn radius.
     *
     * @param speed_percent The speed of the drive system as a percentage (0-100).
     * @param turn_radius The radius of the turn (e.g., NORMAL, LARGE, MEDIUM, SMALL, ZERO).
     * @return eGeneralStatus::SUCCESS if the drive system is successfully turned left, otherwise an error code.
     */
    eGeneralStatus Left(int8_t speed_percent, eTurnRadius turn_radius) override;

    /**
     * @brief Turns the drive system right at the specified speed and turn radius.
     *
     * @param speed_percent The speed of the drive system as a percentage (0-100).
     * @param turn_radius The radius of the turn (e.g., NORMAL, LARGE, MEDIUM, SMALL, ZERO).
     * @return eGeneralStatus::SUCCESS if the drive system is successfully turned right, otherwise an error code.
     */
    eGeneralStatus Right(int8_t speed_percent, eTurnRadius turn_radius) override;

private:
    /**
     * @brief Calculates the speed for the slower side of the drive system during a turn.
     *
     * This method is used internally to determine the speed of the slower side motors when turning left or right.
     *
     * @param turn_radius The radius of the turn (e.g., NORMAL, LARGE, MEDIUM, SMALL, ZERO).
     * @return The calculated speed for the slower side as a percentage.
     */
    int8_t CalculateSpeedForSlowerSide(eTurnRadius turn_radius);

    std::unique_ptr<Motor> mpFrontRightMotor;    ///< Unique pointer to the front-right motor.
    std::unique_ptr<Motor> mpFrontLeftMotor;     ///< Unique pointer to the front-left motor.
    std::unique_ptr<Motor> mpBackRightMotor;     ///< Unique pointer to the back-right motor.
    std::unique_ptr<Motor> mpBackLeftMotor;      ///< Unique pointer to the back-left motor.
    eMotorDirection mLastMovingDirection = eMotorDirection::FORWARD; ///< The last moving direction of the drive system.
    int8_t mCurrentSpeedPercent = 0;             ///< The current speed of the drive system as a percentage.
};
