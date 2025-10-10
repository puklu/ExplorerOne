#include "common/assertHandler.hpp"
#include "common/defines.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/interfaces/IDigitalPin.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "drivers/stm32f3discovery/common/DefaultPinConfigs.hpp"
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

    GpioPinInitStruct pinInit =
        DefaultPinConfigs::Ld4Blue_Output_PushPull_PullDown;

    std::shared_ptr<PinBase> gpio_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, pinInit);
    std::dynamic_pointer_cast<GpioPin>(gpio_pin)->Init(pinInit);
    auto pin = std::dynamic_pointer_cast<IDigitalPin>(gpio_pin);

    blinkyTestFunction(pin);
}
