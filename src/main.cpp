// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/Delay.hpp"
#include "common/time.hpp"
#include "common/Trace.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/motion/DriveFactory.hpp"
#include "drivers/motion/Mdd3aDriveImpl.hpp"
#include "drivers/motion/Motor.hpp"
#include "drivers/stm32f3discovery/leds/leds.hpp"
#include "drivers/stm32f3discovery/mcu/mcuInit.hpp"

#include "drivers/stm32f3discovery/timers/BasicTimer.hpp"
#include "drivers/stm32f3discovery/timers/BasicTimerConfig.hpp"
#include "drivers/stm32f3discovery/timers/GeneralPurposeTimer.hpp"
#include "drivers/stm32f3discovery/timers/GeneralPurposeTimerConfig.hpp"

#include "drivers/interfaces/IDistanceSensor.hpp"
#include "drivers/sensors/UltrasonicSensor.hpp"

#include <climits>

// #include "app/Bot.hpp"


int main()
{

    SystemInit();
    PostSystemInit();
    
    ASSERT(IsSystemInitialized());

    TRACE_LOG("Hello from main");

    // // timer for sensor
    // BasicTimerConfig timer_config;
    // timer_config.mUpdateInterrupt = Timer::eUpdateInterrupt::DISABLE;


    // std::shared_ptr<ITimer> pTimer = std::make_unique<BasicTimer>(timer_config);
    // [[maybe_unused]] auto* raw_ptr = pTimer.get();
    // pTimer->Init();
    // pTimer->SetPeriod(0.01_ms);

    
    // GpioPinInitStruct trigPinInit = {};
    // trigPinInit.pin_name          = IO::ePin::IO_FRONT_ULTRASONIC_SENSOR_TRIG;
    // trigPinInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    // trigPinInit.output_type       = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL;
    // trigPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;
    
    // std::shared_ptr<PinBase> pTrigPinGpio =
    // PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, trigPinInit);
    // auto pTrigPin = std::dynamic_pointer_cast<IDigitalPin>(pTrigPinGpio);

    // GpioPinInitStruct echoPinInit = {};
    // echoPinInit.pin_name          = IO::ePin::IO_FRONT_ULTRASONIC_SENSOR_ECHO;
    // echoPinInit.mode              = IO::eMode::IO_MODE_INPUT;
    // echoPinInit.output_type       = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL;
    // echoPinInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;
    
    // std::shared_ptr<PinBase> pEchoPinGpio =
    // PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, echoPinInit);
    // auto pEchoPin = std::dynamic_pointer_cast<IDigitalPin>(pEchoPinGpio);
    
    // std::unique_ptr<IDistanceSensor> ultrasonicSensor = std::make_unique<UltrasonicSensor>(pTimer, pTrigPin, pEchoPin);
    
    // while (true)
    // {
    //     [[maybe_unused]] float distance = ultrasonicSensor->CalculateDistance();
    //     // TRACE_LOG("Distance is: %f", distance);
    //     DELAY(500_ms);
    // }


    BasicTimerConfig timer_config;

    BasicTimer timer(timer_config);
    timer.Init();
    timer.SetPeriod(0.01_ms);

    GpioPinInitStruct ledInit = {};
    ledInit.pin_name          = IO::ePin::IO_TEST_LED_LD4_BLUE;
    ledInit.mode              = IO::eMode::IO_MODE_OUTPUT;
    ledInit.output_type       = IO::eOutputType::IO_OUTPUT_TYPE_PUSH_PULL;
    ledInit.pupd_resistor     = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN;

    std::shared_ptr<PinBase> gpio_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, ledInit);
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
