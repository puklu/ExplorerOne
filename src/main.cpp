// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "common/Trace.hpp"
#include "drivers/motors/DriveFactory.hpp"
#include "drivers/motors/DriveImpl.hpp"
#include "drivers/motors/Motor.hpp"
#include "mcuInit.hpp"


int main()
{

    SystemInit();
    
    ASSERT(isSystemInitialized);

    Drive *drive = DriveFactory::CreateMotorDrivers();

    // drive->Forward(80);
    // // delay(20000);
    // drive->Right(80, eTurnRadius::LARGE);

    // delete drive;

    uint8_t speed = 80;
    while (1)   
    {
        drive->Forward(speed);
        delay(2000);
        drive->Right(speed, eTurnRadius::SMALL);
        delay(2000);
        speed = -speed;
    }

}   
