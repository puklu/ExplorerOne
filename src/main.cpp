// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "common/Trace.hpp"
#include "drivers/motion/DriveFactory.hpp"
#include "drivers/motion/Mdd3aDriveImpl.hpp"
#include "drivers/motion/Motor.hpp"
#include "mcuInit.hpp"

#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/leds/leds.hpp"

#include "app/Bot.hpp"


int main()
{

    SystemInit();
    
    ASSERT(isSystemInitialized);

    Bot* bot = Bot::GetInstance();

    while(1)
    {
        bot->Run();
        delay(1000000);
    }
}   
