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

    drive->Forward(50);

    delete drive;

    while (1)   
    {

    }

}   
