// src/main.cpp

#include <climits>
#include <cstdint>  // for uint8_t

#include "common/Delay.hpp"
#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "common/time.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/interfaces/IDistanceSensor.hpp"
#include "drivers/motion/DriveFactory.hpp"
#include "drivers/motion/Mdd3aDriveImpl.hpp"
#include "drivers/motion/Motor.hpp"
#include "drivers/sensors/UltrasonicSensor.hpp"
#include "drivers/stm32f3discovery/common/DefaultPinConfigs.hpp"
#include "drivers/stm32f3discovery/common/SysTickImpl.hpp"
#include "drivers/stm32f3discovery/leds/leds.hpp"
#include "drivers/stm32f3discovery/mcu/mcuInit.hpp"
#include "drivers/stm32f3discovery/timers/BasicTimer.hpp"
#include "drivers/stm32f3discovery/timers/BasicTimerConfig.hpp"
#include "drivers/stm32f3discovery/timers/GeneralPurposeTimer.hpp"
#include "drivers/stm32f3discovery/timers/GeneralPurposeTimerConfig.hpp"

int main()
{
    SystemInit();
    PostSystemInit();

    ASSERT(IsSystemInitialized());

    TRACE_LOG("Hello from main");

    GpioPinInitStruct pinInit =
        DefaultPinConfigs::Ld4Blue_Output_PushPull_PullDown;

    std::shared_ptr<PinBase> gpio_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, pinInit);
    std::dynamic_pointer_cast<GpioPin>(gpio_pin)->Init(pinInit);
    auto pin = std::dynamic_pointer_cast<IDigitalPin>(gpio_pin);

    while (true)
    {
        pin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        SysTickImpl::GetInstance()->SystickDelay(1000_ms);

        pin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        SysTickImpl::GetInstance()->SystickDelay(1000_ms);
    }
}
