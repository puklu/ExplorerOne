#pragma once

// #include "stm32f303xc.h"

// #include "common/defines.hpp"
#include "common/PinDefinitions.hpp"
// #include "common/ringBuffer.hpp"
// #include "drivers/interfaces/IPin.hpp"
// #include "drivers/interfaces/PinBase.hpp"


class ITimer
{
public:
    virtual eGeneralStatus Start() = 0;
    virtual eGeneralStatus Stop() = 0;
    virtual eGeneralStatus Reset() = 0;
    virtual eGeneralStatus SetPeriodAndCount(uint32_t period, uint32_t count) = 0;
    virtual eGeneralStatus EnableInterrupt() = 0;
    virtual eGeneralStatus DisableInterrupt() = 0;
    virtual ~ITimer() = default;
    // virtual eGeneralStatus SetPrescalerValue() = 0;
    // virtual eGeneralStatus SetAutoReloadRegisterValue() = 0;
    // virtual eGeneralStatus ReadCounterValue() = 0;

};
