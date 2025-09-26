// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "common/time.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/stm32f3discovery/common/SysTickImpl.hpp"
#include "drivers/stm32f3discovery/leds/leds.hpp"
#include "drivers/stm32f3discovery/mcu/mcuInit.hpp"

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

    while (true)
    {
        pin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        SysTickImpl::GetInstance()->SystickDelay(800_ms);

        pin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        SysTickImpl::GetInstance()->SystickDelay(400_ms);
    }
}
