/**
 * @file ServiceISRs.hpp
 * @brief Interrupt Service Routines for GPIO pins.
 *
 * This file contains implementations of Interrupt Service Routines (ISRs)
 * for handling external interrupts on General Purpose Input/Output (GPIO)
 * pins. The ISRs check for active interrupts on specified pins and invoke
 * the associated callback functions if any are registered. 
 *
 * The functions defined in this file are:
 * - ServiceISR0_4: Handles interrupts for EXTI lines 0 to 4.
 * - ServiceISR5_15: Handles interrupts for EXTI lines 5 to 15.
 *
 * Each ISR iterates through the active GPIO pins, checks for pending
 * interrupts, and executes the registered callback functions while
 * ensuring that the interrupts are cleared after servicing.
 *
 * @note This file depends on GPIO pin definitions and the interrupt
 *       callback mechanism provided in the associated driver files.
 */

#pragma once

#include <cstdint>  // for uint8_t

/**
 * @brief Handles interrupts for EXTI lines 0 to 4.
 *
 * This function checks the active GPIO pins for any interrupts 
 * associated with the specified pin number. If an interrupt is 
 * detected, it invokes the registered callback function for that pin 
 * and clears the interrupt.
 *
 * @param pinNum The number of the pin for which the interrupt is 
 *               being serviced (0 to 4).
 */
void ServiceISR0_4(uint8_t pinNum);


/**
 * @brief Handles interrupts for EXTI lines 5 to 15.
 *
 * This function iterates over the active GPIO pins in the range 
 * specified by the start and end pin numbers. It checks for any 
 * interrupts and invokes the registered callback functions for 
 * each pin that has an interrupt present, then clears the interrupts.
 *
 * @param startPinNumber The starting pin number for the range 
 *                       (5 to 15).
 * @param endPinNumber The ending pin number for the range (5 to 15).
 */
void ServiceISR5_15(uint8_t startPinNumber, uint8_t endPinNumber);
