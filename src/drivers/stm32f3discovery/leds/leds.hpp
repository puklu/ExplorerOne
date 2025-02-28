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

#include "common/delay.hpp"
#include "drivers/stm32f3discovery/io/GpioPin.hpp"


/**
 * @brief Interrupt handler for LED operations.
 *
 * This function is called the user wants to blink an LED
 * when an interrupt occurs.
 */
void InterruptLed();

/**
 * @brief Blinks an LED connected to a specified GPIO pin.
 *
 * This function blinks an LED. It takes a 
 * reference to a GpioPin object representing the pin connected to the 
 * LED and toggles its state at a specified interval, causing the LED 
 * to blink.
 *
 * @param pin A reference to the GpioPin object representing the LED pin.
 */
void BlinkLed(GpioPin *pin);
