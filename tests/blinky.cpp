#include "common/assertHandler.hpp"
#include "common/defines.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/interfaces/IDigitalPin.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "drivers/stm32f3discovery/io/GpioPin.hpp"
#include "drivers/stm32f3discovery/mcu/mcuInit.hpp"

void blinkyTestFunction(std::shared_ptr<IDigitalPin> pin)
{
    while (1)
    {
        pin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        DELAY(1000_ms);
        pin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        DELAY(1000_ms);
    }
}

int main()
{
    SystemInit();
    PostSystemInit();

    ASSERT(IsSystemInitialized());

    GpioPinInitStruct pinInit = {};
    pinInit.pin_name          = IO::ePin::IO_TEST_LED_LD4_BLUE;
    pinInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    pinInit.output_type       = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL;
    pinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    std::shared_ptr<PinBase> gpio_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, pinInit);
    auto pin = std::dynamic_pointer_cast<IDigitalPin>(gpio_pin);

    blinkyTestFunction(pin);
}
