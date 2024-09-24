/**
 * @file
 */

#pragma once

#include <cstdint>  // for uint8_t

inline void delay(volatile uint32_t count)
{
    while (count--)
        ;
}