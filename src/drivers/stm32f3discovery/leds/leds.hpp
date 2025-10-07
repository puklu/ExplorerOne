/**
 * @file leds.hpp
 * @brief Header file for LED control functions.
 *
 * This header file declares functions for controlling LED behavior,
 * including interrupt-driven LED operations and blinking functionality.
 * The functions are designed to work with the GpioPin class to manage
 * GPIO pins connected to LEDs.
 */

#pragma once

#include "drivers/stm32f3discovery/io/GpioPin.hpp"

class Led
{
   public:
    Led(std::shared_ptr<GpioPin> pin);

    /**
     * @brief An interrupt can call this function to indicate that
     * it want to blink the led
     */
    void RequestBlink();

    /**
     * @brief If someone has request to blink an led then this function
     * is called to process the request of blinking
     */
    void Process();

    /**
     * @brief Blinks the LED forever.
     */
    void BlinkLedForever();

   private:
    std::shared_ptr<GpioPin> mPin;
    volatile bool            mBlinkPending;
};
