#include "interrupt.hpp"

#include "common/delay.hpp"
#include "drivers/io/ISRs.hpp"
#include "drivers/leds/leds.hpp"

void interruptTestFunction()
{
    IO::GPIOpin pin = IO::GPIOpin(IO::ePin::IO_UNUSED_B15);
    pin.SetMode(IO::eMode::IO_MODE_INPUT);
    pin.SetResistor(IO::ePupdResistor::IO_RESISTOR_PULL_DOWN);
    pin.EnableInterrupt(InterruptLed);
    pin.SelectInterruptTrigger(
        IO::eTriggerEdge::IO_INTERRUPT_TRIGGER_RISING_EDGE);
}
