#pragma once

#include<memory>

#include "drivers/interfaces/IDrive.hpp"
#include "drivers/motion/Motor.hpp"

class Drive : public IDrive
{
public:
    Drive(std::unique_ptr<Motor> pFrontRightMotor, std::unique_ptr<Motor> pFrontLeftMotor, std::unique_ptr<Motor> pBackRightMotor, std::unique_ptr<Motor> pBackLeftMotor);
    eGeneralStatus Halt() override;
    eGeneralStatus Forward(int8_t speed_percent) override;
    eGeneralStatus Backward(int8_t speed_percent) override;
    eGeneralStatus Left(int8_t speed_percent, eTurnRadius turn_radius) override;
    eGeneralStatus Right(int8_t speed_percent, eTurnRadius turn_radius) override;

private:
    int8_t CalculateSpeedForSlowerSide(eTurnRadius turn_radius);

    std::unique_ptr<Motor> mpFrontRightMotor;
    std::unique_ptr<Motor> mpFrontLeftMotor;
    std::unique_ptr<Motor> mpBackRightMotor;
    std::unique_ptr<Motor> mpBackLeftMotor;
    eMotorDirection mLastMovingDirection = eMotorDirection::FORWARD;
    int8_t mCurrentSpeedPercent = 0;
};
