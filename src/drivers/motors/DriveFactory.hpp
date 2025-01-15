#pragma once

#include "drivers/motors/Motor.hpp"

class DriveFactory
{
public:
    void PrepareMotorsForDriving(Motor *frontRight, Motor *frontLeft, Motor *backRight, Motor *backLeft);
};
