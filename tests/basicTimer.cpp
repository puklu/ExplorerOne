#include "drivers/stm32f3discovery/timers/BasicTimer.hpp"

#include <cstdint>  // for uint8_t

#include "common/Delay.hpp"
#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "common/time.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/stm32f3discovery/common/DefaultPinConfigs.hpp"
#include "drivers/stm32f3discovery/leds/leds.hpp"
#include "drivers/stm32f3discovery/mcu/mcuInit.hpp"
#include "drivers/stm32f3discovery/timers/BasicTimerConfig.hpp"

int main()
{
    SystemInit();
    PostSystemInit();

    ASSERT(IsSystemInitialized());

    BasicTimerConfig timer_config;

    BasicTimer timer(timer_config);
    timer.Init();
    timer.SetPeriod(1_ms);

    GpioPinInitStruct ledInit =
        DefaultPinConfigs::Ld4Blue_Output_PushPull_PullDown;

    std::shared_ptr<PinBase> gpio_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, ledInit);
    std::dynamic_pointer_cast<GpioPin>(gpio_pin)->Init(ledInit);
    auto ledpin = std::dynamic_pointer_cast<IDigitalPin>(gpio_pin);

    while (true)
    {
        ledpin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);
        timer.Reset();
        timer.Start();
        while (timer.GetTimeElapsedInMillisecondsSinceStart() <= 2000_ms)
            ;

        ledpin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
        timer.Reset();
        timer.Start();
        while (timer.GetTimeElapsedInMillisecondsSinceStart() <= 3000_ms)
            ;
    }
}
