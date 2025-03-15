// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "drivers/motion/DriveFactory.hpp"
#include "mcuInit.hpp"

int main()
{
    SystemInit();

    ASSERT(isSystemInitialized);

    auto drive = DriveFactory::CreateMdd3aDrive();

    uint8_t speed = 80;

    while (1)
    {
        drive->Forward(speed);
        delay(Milliseconds{1000});
        drive->Backward(speed);
        delay(Milliseconds{1000});
        drive->Right(speed, eTurnRadius::SMALL);
        delay(Milliseconds{1000});
        drive->Left(speed, eTurnRadius::SMALL);
        delay(Milliseconds{1000});
        drive->Right(speed, eTurnRadius::ZERO);
        delay(Milliseconds{1000});
        drive->Left(speed, eTurnRadius::ZERO);
        delay(Milliseconds{1000});
        speed = speed - 1;

        if (speed <= 0)
        {
            speed = 80;
        }
    }
}
