#include "blinky.hpp"
#include "drivers/io/GPIOpin.hpp"
#include "interrupt.hpp"

int main()
{
    // IO::GPIOpin pin = IO::GPIOpin(IO::ePin::IO_TEST_LED_LD4);
    // blinkyTestFunction(pin);

    interruptTestFunction();
}
