// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "drivers/io/GPIOpin.hpp"
#include "drivers/leds/leds.hpp"
#include "drivers/io/IrqHandlers.cpp"


int main()
{

    IO::GpioPinInitStruct pinInit = {
        .pin_name = IO::ePin::IO_UNUSED_D0,
        .mode = IO::eMode::IO_MODE_INPUT,
        .pupd_resistor = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    IO::GPIOpin pin(pinInit);

    pin.EnableInterrupt(InterruptLed);
    pin.SelectInterruptTrigger(IO::eTriggerEdge::IO_INTERRUPT_TRIGGER_RISING_EDGE);

    while(1){}
}
