#pragma once

#include "drivers/interfaces/IDrive.hpp"
#include "drivers/motors/Motor.hpp"

class Drive : public IDrive
{
public:
    Drive(Motor *frontRight, Motor *frontLeft, Motor *backRight, Motor *backLeft);
    eGeneralStatus Halt() override;
    eGeneralStatus Forward(int8_t speed_percent) override;
    eGeneralStatus Backward(int8_t speed_percent) override;
    eGeneralStatus Left(int8_t speed_percent, eTurnRadius turn_radius) override;
    eGeneralStatus Right(int8_t speed_percent, eTurnRadius turn_radius) override;

    ~Drive();

private:
    int8_t CalculateSpeedForSlowerSide(eTurnRadius turn_radius);

    Motor *mpFrontRight;
    Motor *mpFrontLeft;
    Motor *mpBackRight;
    Motor *mpBackLeft;
    eMotorDirection mLastMovingDirection = eMotorDirection::FORWARD;
    int8_t mCurrentSpeedPercent = 0;
};
