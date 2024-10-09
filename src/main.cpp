// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "drivers/io/GPIOpin.hpp"
#include "drivers/leds/leds.hpp"
#include "drivers/io/ISRs.cpp"


int main()
{
    IO::GPIOpin pin = IO::GPIOpin(IO::ePin::IO_UNUSED_B15);
    pin.SetMode(IO::eMode::IO_MODE_INPUT);
    pin.SetResistor(IO::ePupdResistor::IO_RESISTOR_PULL_DOWN);
    pin.EnableInterrupt(InterruptLed);
    pin.SelectInterruptTrigger(IO::eTriggerEdge::IO_INTERRUPT_TRIGGER_RISING_EDGE);

    while(1){}
}
