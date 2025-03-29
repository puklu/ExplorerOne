/**
 * @file ServiceISR.cpp
 * 
 * @brief Implementation of interrupt service routines (ISRs) for timers.
 * 
 * This file contains the definitions of interrupt service routines (ISRs) for handling 
 * interrupts from timers. These ISRs ensure proper processing of timer interrupts, executing 
 * the associated callback functions and clearing the interrupts after execution.
 */

#pragma once

/**
 * @brief Interrupt Service Routine (ISR) for basic timers.
 * 
 * This ISR processes interrupts from basic timers. It iterates through each basic timer 
 * in the `basicTimers` array, invokes the associated callback function if it exists, 
 * and then clears the interrupt.
 * 
 * It handles multiple basic timers, executing their interrupt callbacks and ensuring 
 * the interrupts are cleared after processing.
 * 
 * @param timer_index The index of the basic timer being serviced.
 * 
 * @note This function assumes that `basicTimers` is an array of pointers to `BasicTimer` instances.
 */
void BasicTimersServiceISR(uint8_t timer_index);


/**
 * @brief Interrupt Service Routine (ISR) for general-purpose timers.
 * 
 * This ISR processes interrupts from general-purpose timers. It takes the timer index as 
 * an argument and iterates through all channels of the specified timer. The callback 
 * function assigned to each channel is executed if it exists.
 * 
 * After handling each interrupt, the ISR clears the interrupt flag for the timer.
 * 
 * @param timer_index The index of the general-purpose timer being serviced.
 * 
 * @note This function assumes that `generalPurposeTimers` is an array of pointers to 
 *       `GeneralPurposeTimer` instances.
 */
void GeneralPurposeTimersServiceISR(uint8_t timer_index);
