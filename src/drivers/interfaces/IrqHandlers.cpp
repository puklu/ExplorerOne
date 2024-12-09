/**
 * @file IrqHandlers.cpp
 * @brief Interrupt Service Routines for GPIO pin interrupts.
 *
 * This file contains the implementations of interrupt service routines 
 * (ISRs) that handle interrupts for various interrupt 
 * lines.
 *
 */

#include "common/assertHandler.hpp"
#include "common/Trace.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/io/ServiceISRs.hpp"
#include "drivers/timers/ServiceISRs.hpp"
#include "drivers/usart/ServiceISRs.hpp"
#include "drivers/usart/UsartPin.hpp"


// #ifdef __cplusplus
// extern "C" {
// #endif


/**
 * @brief External Interrupt Handler for EXTI line 0.
 *
 * This function is called when an interrupt occurs on EXTI line 0. 
 * It delegates the handling of the interrupt to the ExtiServiceISR0_4 
 * function for processing.
 */
extern "C" void EXTI0_IRQHandler(){
    ExtiServiceISR0_4(0);
}

/**
 * @brief External Interrupt Handler for EXTI line 1.
 *
 * This function is called when an interrupt occurs on EXTI line 1. 
 * It delegates the handling of the interrupt to the ExtiServiceISR0_4 
 * function for processing.
 */
extern "C" void EXTI1_IRQHandler(){
    ExtiServiceISR0_4(1);
}

/**
 * @brief External Interrupt Handler for EXTI line 2.
 *
 * This function is called when an interrupt occurs on EXTI line 2. 
 * It delegates the handling of the interrupt to the ExtiServiceISR0_4 
 * function for processing.
 */
extern "C" void EXTI2_TSC_IRQHandler(){
     ExtiServiceISR0_4(2);
}

/**
 * @brief External Interrupt Handler for EXTI line 3.
 *
 * This function is called when an interrupt occurs on EXTI line 3. 
 * It delegates the handling of the interrupt to the ExtiServiceISR0_4 
 * function for processing.
 */
extern "C" void EXTI3_IRQHandler(){
     ExtiServiceISR0_4(3);
}

/**
 * @brief External Interrupt Handler for EXTI line 4.
 *
 * This function is called when an interrupt occurs on EXTI line 4. 
 * It delegates the handling of the interrupt to the ExtiServiceISR0_4 
 * function for processing.
 */
extern "C" void EXTI4_IRQHandler(){
     ExtiServiceISR0_4(4);
}

/**
 * @brief External Interrupt Handler for EXTI lines 5 to 9.
 *
 * This function is called when an interrupt occurs on EXTI lines 
 * 5 through 9. It delegates the handling of the interrupts to the 
 * ExtiServiceISR5_15 function for processing.
 */
extern "C" void EXTI9_5_IRQHandler(){
     ExtiServiceISR5_15(5, 9);
}

/**
 * @brief External Interrupt Handler for EXTI lines 10 to 15.
 *
 * This function is called when an interrupt occurs on EXTI lines 
 * 10 through 15. It delegates the handling of the interrupts to 
 * the ExtiServiceISR5_15 function for processing.
 */
extern "C" void EXTI15_10_IRQHandler(){
    ExtiServiceISR5_15(10, 15);
}

/**
 * @brief Interrupt Service Routine for USART1.
 *
 * This ISR is triggered when an interrupt occurs on USART1.
 * It calls the common handler `UsartServiceISR()` for processing.
 */
extern "C" void USART1_IRQHandler()
{
	UsartServiceISR();
}

/**
 * @brief Interrupt Service Routine for USART2.
 *
 * This ISR is triggered when an interrupt occurs on USART2.
 * It calls the common handler `UsartServiceISR()` for processing.
 */
extern "C" void USART2_IRQHandler()
{
	UsartServiceISR();
}

/**
 * @brief Interrupt Service Routine for USART3.
 *
 * This ISR is triggered when an interrupt occurs on USART3.
 * It calls the common handler `UsartServiceISR()` for processing.
 */
extern "C" void USART3_IRQHandler()
{
	UsartServiceISR();
}

/**
 * @brief Interrupt Service Routine for UART4.
 *
 * This ISR is triggered when an interrupt occurs on UART4.
 * It calls the common handler `UsartServiceISR()` for processing.
 */
extern "C" void UART4_IRQHandler()
{
     UsartServiceISR();
}

/**
 * @brief Interrupt Service Routine for UART5.
 *
 * This ISR is triggered when an interrupt occurs on UART5.
 * It calls the common handler `UsartServiceISR()` for processing.
 */
extern "C" void UART5_IRQHandler()
{
	UsartServiceISR();
}

extern "C" void TIM6_DAC_IRQHandler()
{
     ASSERT(0);
}


#include "drivers/leds/leds.hpp"

extern "C" void TIM7_IRQHandler()
{
     BasicTimersServiceISR();
}

// #ifdef __cplusplus
// }
// #endif
