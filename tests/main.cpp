#include "blinky.hpp"
#include "drivers/io/GPIOpin.hpp"
#include "interrupt.hpp"

int main()
{
    // IO::GpioPinInitStruct pinInit = {
    //     .pin_name      = IO::ePin::IO_TEST_LED_LD4_BLUE,
    //     .mode          = IO::eMode::IO_MODE_OUTPUT,
    //     .output_type   = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL,
    //     .pupd_resistor = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    // };

    // IO::GPIOpin pin(pinInit);

    // blinkyTestFunction(pin);

    interruptTestFunction();
}
