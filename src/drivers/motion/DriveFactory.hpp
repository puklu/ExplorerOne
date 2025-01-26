#pragma once

#include<memory>
#include "drivers/motion/DriveImpl.hpp"
#include "drivers/motion/Motor.hpp"

class DriveFactory
{
public:
    static std::unique_ptr<Drive> CreateMotorDrivers();
};
