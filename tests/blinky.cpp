#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/interfaces/IDigitalOutputPin.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "drivers/stm32f3discovery/io/GpioPin.hpp"
#include "drivers/stm32f3discovery/mcu/mcuInit.hpp"

void blinkyTestFunction(std::shared_ptr<IDigitalOutputPin> pin)
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

    std::shared_ptr<PinBase> gpio_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, pinInit);
    auto pin = std::dynamic_pointer_cast<IDigitalOutputPin>(gpio_pin);

    blinkyTestFunction(pin);
}
