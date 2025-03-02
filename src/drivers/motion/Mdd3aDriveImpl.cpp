#include "common/assertHandler.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/motion/Mdd3aDriveImpl.hpp"


Mdd3aDrive::Mdd3aDrive(std::unique_ptr<Motor> pFrontRightMotor, std::unique_ptr<Motor> pFrontLeftMotor, std::unique_ptr<Motor> pBackRightMotor, std::unique_ptr<Motor> pBackLeftMotor):
   mpFrontRightMotor(std::move(pFrontRightMotor)),
   mpFrontLeftMotor(std::move(pFrontLeftMotor)),
   mpBackRightMotor(std::move(pBackRightMotor)),
   mpBackLeftMotor(std::move(pBackLeftMotor))       
{
    ASSERT(mpFrontRightMotor);
    ASSERT(mpFrontLeftMotor);
    ASSERT(mpBackRightMotor);
    ASSERT(mpBackLeftMotor);
}   

eGeneralStatus Mdd3aDrive::Halt()
{
    mpFrontRightMotor->Halt();
    mpFrontLeftMotor->Halt();
    mpBackRightMotor->Halt();
    mpBackLeftMotor->Halt();

    return eGeneralStatus::SUCCESS;         
}

eGeneralStatus Mdd3aDrive::Forward(int8_t speed_percent)
{
    mpFrontRightMotor->Forward(speed_percent);
    mpFrontLeftMotor->Forward(speed_percent);
    mpBackRightMotor->Forward(speed_percent);
    mpBackLeftMotor->Forward(speed_percent);
    
    mLastMovingDirection = eMotorDirection::FORWARD;
    mCurrentSpeedPercent = speed_percent;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus Mdd3aDrive::Backward(int8_t speed_percent)
{
    mpFrontRightMotor->Backward(speed_percent);
    mpFrontLeftMotor->Backward(speed_percent);
    mpBackRightMotor->Backward(speed_percent);
    mpBackLeftMotor->Backward(speed_percent);
    
    mLastMovingDirection = eMotorDirection::BACKWARD;
    mCurrentSpeedPercent = speed_percent;

    return eGeneralStatus::SUCCESS;
}


int8_t Mdd3aDrive::CalculateSpeedForSlowerSide(eTurnRadius turn_radius)
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

eGeneralStatus Mdd3aDrive::Left(int8_t speed_percent, eTurnRadius turn_radius)
{
    int8_t speed_for_slow_moving_side = CalculateSpeedForSlowerSide(turn_radius);

    if (mLastMovingDirection == eMotorDirection::FORWARD)
    {
        mpFrontRightMotor->Forward(speed_percent);
        mpFrontLeftMotor->Forward(speed_for_slow_moving_side);
        mpBackRightMotor->Forward(speed_percent);
        mpBackLeftMotor->Forward(speed_for_slow_moving_side);   
 
    }
    else if(mLastMovingDirection == eMotorDirection::BACKWARD)
    {
        mpFrontRightMotor->Backward(speed_percent);
        mpFrontLeftMotor->Backward(speed_for_slow_moving_side);
        mpBackRightMotor->Backward(speed_percent);
        mpBackLeftMotor->Backward(speed_for_slow_moving_side);
    }

    return eGeneralStatus::SUCCESS;

}

eGeneralStatus Mdd3aDrive::Right(int8_t speed_percent, eTurnRadius turn_radius)
{
    int8_t speed_for_slow_moving_side = CalculateSpeedForSlowerSide(turn_radius);

    if (mLastMovingDirection == eMotorDirection::FORWARD)
    {
        mpFrontRightMotor->Forward(speed_for_slow_moving_side);
        mpFrontLeftMotor->Forward(speed_percent);
        mpBackRightMotor->Forward(speed_for_slow_moving_side);
        mpBackLeftMotor->Forward(speed_percent);   
 
    }
    else if(mLastMovingDirection == eMotorDirection::BACKWARD)
    {
        mpFrontRightMotor->Backward(speed_for_slow_moving_side);
        mpFrontLeftMotor->Backward(speed_percent);
        mpBackRightMotor->Backward(speed_for_slow_moving_side);
        mpBackLeftMotor->Backward(speed_percent);
    }

    return eGeneralStatus::SUCCESS;    
}
