// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "drivers/io/GpioPin.hpp"
#include "drivers/leds/leds.hpp"
#include "drivers/io/IrqHandlers.cpp"
#include "mcuInit.hpp"
#include "drivers/usart/UsartPin.hpp"
#include "drivers/interfaces/PinFactory.hpp"

#include "common/Trace.hpp"

int main()
{

    SystemInit();

    while (1)
    {
        TRACE_LOG("duck");
        delay(500000);
    }

}   
