/**
 * @file
 */

#include "common/defines.hpp"
#include "common/time.hpp"

#pragma once

#include <cstdint>  // for uint8_t


// TODO: use a hardware timer here
inline void delayInTicks(volatile uint32_t ticks)
{
    while (ticks--)
        ;
}

inline void delay(Microseconds microsecs)
{
    uint32_t ticks = (SYS_CLK*microsecs.value)/1000000;
    delayInTicks(ticks);
}

inline void delay(Milliseconds millisecs)
{
    uint32_t ticks = (SYS_CLK*millisecs.value)/1000;
    delayInTicks(ticks);
}