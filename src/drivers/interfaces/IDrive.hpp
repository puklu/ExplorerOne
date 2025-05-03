#pragma once

#include "common/PinDefinitions.hpp"

class IDrive
{
public:
    virtual eGeneralStatus Halt() = 0;
    virtual eGeneralStatus Forward(int8_t speed_percent) = 0;
    virtual eGeneralStatus Backward(int8_t speed_percent) = 0;
    virtual eGeneralStatus Left(int8_t speed_percent, eTurnRadius turn_radius) = 0;
    virtual eGeneralStatus Right(int8_t speed_percent, eTurnRadius turn_radius) = 0;
    virtual ~IDrive() = default;
};
