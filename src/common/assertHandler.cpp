#include "assertHandler.hpp"

#include "delay.hpp"
#include "drivers/io/GPIOpin.hpp"
#include "drivers/io/enums.hpp"

void assertHandler()
{
    // TODO: Improve me
    SOFTWARE_BREAKPOINT

    IO::GPIOpin pin = IO::GPIOpin(IO::ePin::IO_ASSERT_ENCOUNTERED_LED);

    pin.SetOutputType(IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL);
    pin.SetResistor(IO::ePupdResistor::IO_RESISTOR_PULL_DOWN);
    pin.SetMode(IO::eMode::IO_MODE_OUTPUT);

    while (1)
    {
        pin.WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        delay(2500000);
        pin.WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        delay(2500000);
    }
}