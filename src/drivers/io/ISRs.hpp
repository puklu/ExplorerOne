/**
 * @file ISRs.hpp
 * @brief Interrupt Service Routines for GPIO pin interrupts.
 *
 * This file contains the implementations of interrupt service routines 
 * (ISRs) that handle GPIO pin interrupts for various external interrupt 
 * lines. The service functions check for active GPIO pins associated 
 * with the interrupt and invoke the corresponding callback functions 
 * if an interrupt is detected.
 *
 * The ISRs are designed to work with the GPIOpin class and utilize a 
 * global array of active GPIO pin instances (`activePins`) to manage 
 * interrupt handling efficiently.
 * 
 * The following ISRs are defined:
 * - `EXTI0_IRQHandler`: Handles interrupts for EXTI line 0 (associated with pin 0).
 * - `EXTI1_IRQHandler`: Handles interrupts for EXTI line 1 (associated with pin 1).
 * - `EXTI2_TSC_IRQHandler`: Handles interrupts for EXTI line 2 (associated with pin 2).
 * - `EXTI3_IRQHandler`: Handles interrupts for EXTI line 3 (associated with pin 3).
 * - `EXTI4_IRQHandler`: Handles interrupts for EXTI line 4 (associated with pin 4).
 * - `EXTI9_5_IRQHandler`: Handles interrupts for EXTI lines 5 to 9.
 * - `EXTI15_10_IRQHandler`: Handles interrupts for EXTI lines 10 to 15.
 *
 * The `ServiceISR0_4` function manages interrupts for EXTI lines 0 to 4, 
 * while the `ServiceISR5_15` function manages interrupts for EXTI lines 
 * 5 to 15. Each service function iterates through the active GPIO pins 
 * and executes their registered callback functions upon interrupt detection.
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


/**
 * @brief External Interrupt Handler for EXTI line 0.
 *
 * This function is called when an interrupt occurs on EXTI line 0. 
 * It delegates the handling of the interrupt to the ServiceISR0_4 
 * function for processing.
 */
extern "C" void EXTI0_IRQHandler();


/**
 * @brief External Interrupt Handler for EXTI line 1.
 *
 * This function is called when an interrupt occurs on EXTI line 1. 
 * It delegates the handling of the interrupt to the ServiceISR0_4 
 * function for processing.
 */
extern "C" void EXTI1_IRQHandler();


/**
 * @brief External Interrupt Handler for EXTI line 2.
 *
 * This function is called when an interrupt occurs on EXTI line 2. 
 * It delegates the handling of the interrupt to the ServiceISR0_4 
 * function for processing.
 */
extern "C" void EXTI2_TSC_IRQHandler();


/**
 * @brief External Interrupt Handler for EXTI line 3.
 *
 * This function is called when an interrupt occurs on EXTI line 3. 
 * It delegates the handling of the interrupt to the ServiceISR0_4 
 * function for processing.
 */
extern "C" void EXTI3_IRQHandler();


/**
 * @brief External Interrupt Handler for EXTI line 4.
 *
 * This function is called when an interrupt occurs on EXTI line 4. 
 * It delegates the handling of the interrupt to the ServiceISR0_4 
 * function for processing.
 */
extern "C" void EXTI4_IRQHandler();


/**
 * @brief External Interrupt Handler for EXTI lines 5 to 9.
 *
 * This function is called when an interrupt occurs on EXTI lines 
 * 5 through 9. It delegates the handling of the interrupts to the 
 * ServiceISR5_15 function for processing.
 */
extern "C" void EXTI9_5_IRQHandler();


/**
 * @brief External Interrupt Handler for EXTI lines 10 to 15.
 *
 * This function is called when an interrupt occurs on EXTI lines 
 * 10 through 15. It delegates the handling of the interrupts to 
 * the ServiceISR5_15 function for processing.
 */
extern "C" void EXTI15_10_IRQHandler();
