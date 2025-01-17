#pragma once

#include "drivers/motors/DriveImpl.hpp"
#include "drivers/motors/Motor.hpp"

class DriveFactory
{
public:
    static Drive * CreateMotorDrivers();
};
