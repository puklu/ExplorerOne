#pragma once

#include <stm32f303xc.h>
#include "common/PinDefinitions.hpp"

class ISysTick
{
public:    
    virtual ~ISysTick() = default;
    virtual eGeneralStatus SystickSetup(uint32_t freq, uint32_t ahb_clock) = 0;
    virtual eGeneralStatus SystickDelay(Milliseconds ms) = 0;
};
