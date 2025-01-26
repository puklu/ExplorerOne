/**
 * @file DriveFactory.hpp
 * @brief Header file for the DriveFactory class, which provides a factory method for creating instances of the Mdd3aDrive class.
 *
 * This file defines the DriveFactory class, which is responsible for creating and initializing instances of the Mdd3aDrive class.
 * The factory method encapsulates the logic for setting up the motors and their dependencies, making it easier to create and manage drive systems.
 */

#pragma once

#include<memory>
#include "drivers/motion/Mdd3aDriveImpl.hpp"
#include "drivers/motion/Motor.hpp"


/**
 * @class DriveFactory
 * @brief Provides a factory method for creating instances of the Mdd3aDrive class.
 *
 * This class is responsible for creating and initializing instances of the Mdd3aDrive class, which controls a drive system using two MDD3A motor drivers.
 * The factory method encapsulates the logic for setting up the motors and their dependencies, making it easier to create and manage drive systems.
 */

class DriveFactory
{
public:
    /**
     * @brief Creates and initializes an instance of the Mdd3aDrive class.
     *
     * This method sets up the four motors (front-right, front-left, back-right, and back-left) and their dependencies,
     * then returns a unique pointer to the created Mdd3aDrive instance.
     *
     * ### Steps:
     * 1. **Initialize PWM and Digital Pins**:
     *    - Configure the a GPIO pin for PWM and a digital ouptut GPIO pin for each motor.
     * 2. **Configure the PWM Timer**:
     *    - Set up the GeneralPurposeTimer with the appropriate channels and configurations.
     * 3. **Create Motor Instances**:
     *    - Initialize the Motor objects using the configured PWM timer and GPIO pins.
     * 4. **Create the Mdd3aDrive Instance**:
     *    - Initialize the Mdd3aDrive object with the four motor instances.
     * 5. **Return the Drive System**:
     *    - Return the created Mdd3aDrive instance.
     *
     * @return A unique pointer to the created Mdd3aDrive instance.
     */
    static std::unique_ptr<Mdd3aDrive> CreateMdd3aDrive();
};
