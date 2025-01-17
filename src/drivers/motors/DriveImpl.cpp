#include "common/assertHandler.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/motors/DriveImpl.hpp"
#include "drivers/motors/Motor.hpp"


Drive::Drive(Motor *frontRight, Motor *frontLeft, Motor *backRight, Motor *backLeft):
    mpFrontRight(frontRight),
    mpFrontLeft(frontLeft),
    mpBackRight(backRight),
    mpBackLeft(backLeft)
{
    ASSERT(frontRight);
    ASSERT(frontLeft);
    ASSERT(backRight);
    ASSERT(backLeft);
}   

eGeneralStatus Drive::Halt()
{
    mpFrontRight->Halt();
    mpFrontLeft->Halt();
    mpBackRight->Halt();
    mpBackLeft->Halt();

    return eGeneralStatus::SUCCESS;         
}

eGeneralStatus Drive::Forward(int8_t speed_percent)
{
    mpFrontRight->Forward(speed_percent);
    mpFrontLeft->Forward(speed_percent);
    mpBackRight->Forward(speed_percent);
    mpBackLeft->Forward(speed_percent);
    
    mLastMovingDirection = eMotorDirection::FORWARD;
    mCurrentSpeedPercent = speed_percent;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus Drive::Backward(int8_t speed_percent)
{
    mpFrontRight->Backward(speed_percent);
    mpFrontLeft->Backward(speed_percent);
    mpBackRight->Backward(speed_percent);
    mpBackLeft->Backward(speed_percent);
    
    mLastMovingDirection = eMotorDirection::BACKWARD;
    mCurrentSpeedPercent = speed_percent;

    return eGeneralStatus::SUCCESS;
}


int8_t Drive::CalculateSpeedForSlowerSide(eTurnRadius turn_radius)
{
    float speed_for_slow_moving_side = 0;

    switch (turn_radius)
    {
    case eTurnRadius::NORMAL:
        speed_for_slow_moving_side = static_cast<float>(mCurrentSpeedPercent) * 0.35f;
        break;

    case eTurnRadius::LARGE:
        speed_for_slow_moving_side = static_cast<float>(mCurrentSpeedPercent) * 0.75f;
        break;

    case eTurnRadius::MEDIUM:
        speed_for_slow_moving_side = static_cast<float>(mCurrentSpeedPercent) * 0.55f;
        break;

    case eTurnRadius::SMALL:
        speed_for_slow_moving_side = static_cast<float>(mCurrentSpeedPercent) * 0.10f;
        break;

    case eTurnRadius::ZERO:
        speed_for_slow_moving_side = static_cast<float>(mCurrentSpeedPercent) * -1.0f;
        break;
    }

    return static_cast<int8_t>(speed_for_slow_moving_side);   
}

eGeneralStatus Drive::Left(int8_t speed_percent, eTurnRadius turn_radius)
{
    int8_t speed_for_slow_moving_side = CalculateSpeedForSlowerSide(turn_radius);

    if (mLastMovingDirection == eMotorDirection::FORWARD)
    {
        mpFrontRight->Forward(speed_percent);
        mpFrontLeft->Forward(speed_for_slow_moving_side);
        mpBackRight->Forward(speed_percent);
        mpBackLeft->Forward(speed_for_slow_moving_side);   
 
    }
    else if(mLastMovingDirection == eMotorDirection::BACKWARD)
    {
        mpFrontRight->Backward(speed_percent);
        mpFrontLeft->Backward(speed_for_slow_moving_side);
        mpBackRight->Backward(speed_percent);
        mpBackLeft->Backward(speed_for_slow_moving_side);
    }

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus Drive::Right(int8_t speed_percent, eTurnRadius turn_radius)
{
    int8_t speed_for_slow_moving_side = CalculateSpeedForSlowerSide(turn_radius);

    if (mLastMovingDirection == eMotorDirection::FORWARD)
    {
        mpFrontRight->Forward(speed_for_slow_moving_side);
        mpFrontLeft->Forward(speed_percent);
        mpBackRight->Forward(speed_for_slow_moving_side);
        mpBackLeft->Forward(speed_percent);   
 
    }
    else if(mLastMovingDirection == eMotorDirection::BACKWARD)
    {
        mpFrontRight->Backward(speed_for_slow_moving_side);
        mpFrontLeft->Backward(speed_percent);
        mpBackRight->Backward(speed_for_slow_moving_side);
        mpBackLeft->Backward(speed_percent);
    }

    return eGeneralStatus::SUCCESS;    
}

Drive::~Drive()
{
    delete mpFrontRight;
    delete mpFrontLeft;
    delete mpBackRight;
    delete mpBackLeft;
}
