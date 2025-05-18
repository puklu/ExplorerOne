#pragma once

#include "common/PinDefinitions.hpp"

class IDigitalPin
{
public:
    virtual ~IDigitalPin() = default;
    
    // method to write a digital output value
    virtual void WriteOutputValue(IO::eValue value) = 0;
    virtual IO::eValue ReadInputValue() = 0;
};
