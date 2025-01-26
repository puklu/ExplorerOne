// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "common/Trace.hpp"
#include "drivers/motion/DriveFactory.hpp"
#include "drivers/motion/DriveImpl.hpp"
#include "drivers/motion/Motor.hpp"
#include "mcuInit.hpp"

#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/leds/leds.hpp"


int main()
{

    SystemInit();
    
    ASSERT(isSystemInitialized);

    auto drive = DriveFactory::CreateMotorDrivers();

    uint8_t speed = 80;

    drive->Forward(speed);

    while (speed > 0)   
    {
        delay(2000);
        drive->Right(speed, eTurnRadius::SMALL);
        delay(2000);
        speed = speed - 1;
    }
}   
