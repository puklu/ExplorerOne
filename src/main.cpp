// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "common/Trace.hpp"
#include "drivers/io/GpioPin.hpp"
#include "drivers/leds/leds.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/usart/UsartPin.hpp"
#include "mcuInit.hpp"


int main()
{

    SystemInit();
    
    ASSERT(isSystemInitialized);
           
    int i = 0;

    while (1)
    {
        TRACE_LOG("duck %d", i);
        i++;
        delay(1000000);
    }

}   
