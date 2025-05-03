/**
 * @file defines.hpp
 * @brief Defines constants and macros.
 */

#pragma once

#include <cstdint>  // for uint_t
#include "Delay.hpp"

constexpr uint32_t SYS_CLK = 8000000;

#define IO_PIN_MASK           (0x0F)
#define IO_PORT_OFFSET        (4u)
#define IO_PORT_MASK          (0x70)
#define IO_PORT_COUNT         (6u)
#define IO_PIN_COUNT_PER_PORT (16u)
#define IO_ALT_FUNC_COUNT_FOR_PORT_A (16u)
#define IO_ALT_FUNC_COUNT_FOR_PORT_B (16u)
#define IO_ALT_FUNC_COUNT_FOR_PORT_C (7u)
#define IO_ALT_FUNC_COUNT_FOR_PORT_D (7u)
#define IO_ALT_FUNC_COUNT_FOR_PORT_E (7u)
#define IO_ALT_FUNC_COUNT_FOR_PORT_F (7u)
#define IO_BITS_PER_REGISTER  (32u)
#define IO_EXTI_LINES_COUNT   (36u)

#define COUNT_OF_USARTS (5u)

#define RING_BUFFER_SIZE (8u)

#define PRIORITY_EXTI  (2u)
#define PRIORITY_USART (3u)
#define PRIORITY_TIMER (2u)

#define NUMBER_OF_BASIC_TIMERS (2u)
#define NUMBER_OF_GENERAL_PURPOSE_TIMERS (3u)
#define GENERAL_PURPOSE_TIMER_NUM_CHANNELS (4u)

#define UNUSED(x) (void)(x)

// Macro to allow delay(100_ms) directly
#define DELAY(delayTime) Delay::GetInstance()(delayTime)
