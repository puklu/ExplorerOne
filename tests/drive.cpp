// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/Delay.hpp"
#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "drivers/motion/DriveFactory.hpp"
#include "mcuInit.hpp"

int main()
{
    SystemInit();

    ASSERT(IsSystemInitialized());

    auto drive = DriveFactory::CreateMdd3aDrive();

    uint8_t speed = 80;

    while (1)
    {
        drive->Forward(speed);
        DELAY(1000_ms);
        drive->Backward(speed);
        DELAY(1000_ms);
        drive->Right(speed, eTurnRadius::SMALL);
        DELAY(1000_ms);
        drive->Left(speed, eTurnRadius::SMALL);
        DELAY(1000_ms);
        drive->Right(speed, eTurnRadius::ZERO);
        DELAY(1000_ms);
        drive->Left(speed, eTurnRadius::ZERO);
        DELAY(1000_ms);
        speed = speed - 1;

        if (speed <= 0)
        {
            speed = 80;
        }
    }
}
