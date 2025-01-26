#pragma once

#include<memory>
#include "drivers/motors/DriveImpl.hpp"
#include "drivers/motors/Motor.hpp"

class DriveFactory
{
public:
    static std::unique_ptr<Drive> CreateMotorDrivers();
};
