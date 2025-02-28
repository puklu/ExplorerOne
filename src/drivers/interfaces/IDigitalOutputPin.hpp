#pragma once

#include "common/PinDefinitions.hpp"

class IDigitalOutputPin
{
public:
    virtual ~IDigitalOutputPin() = default;
    
    // method to write a digital output value
    virtual void WriteOutputValue(IO::eValue value) = 0;
};
