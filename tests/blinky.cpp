#include "blinky.hpp"

#include "common/assertHandler.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/mcu/mcuInit.hpp"

void blinkyTestFunction(GpioPin *pin)
{
    while (1)
    {
        pin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        delay(250000);
        pin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        delay(250000);
    }
}

int main()
{
    SystemInit();

    ASSERT(isSystemInitialized);

    GpioPinInitStruct pinInit = {};
    pinInit.pin_name          = IO::ePin::IO_TEST_LED_LD4_BLUE;
    pinInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    pinInit.output_type       = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL;
    pinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    PinBase *gpio_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, pinInit);
    auto pin = static_cast<GpioPin *>(gpio_pin);

    blinkyTestFunction(pin);
}
