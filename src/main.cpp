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

    GpioPinInitStruct i2cSclConfig = DefaultPinConfigs::I2c1Scl;

    GpioPinInitStruct i2cSdaConfig = DefaultPinConfigs::I2c1Sda;

    std::shared_ptr<PinBase> gpio_pin_scl =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, i2cSclConfig);
    std::dynamic_pointer_cast<GpioPin>(gpio_pin_scl)->Init(i2cSclConfig);

    std::shared_ptr<PinBase> gpio_pin_sda =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_GPIO, i2cSdaConfig);
    std::dynamic_pointer_cast<GpioPin>(gpio_pin_sda)->Init(i2cSdaConfig);

    I2cInitStruct i2cConfig = {
        .scl_pin           = std::dynamic_pointer_cast<GpioPin>(gpio_pin_scl),
        .sda_pin           = std::dynamic_pointer_cast<GpioPin>(gpio_pin_sda),
        .AddressMode       = eI2cAddressMode::ADDR_7BIT,
        .TransferDirection = eI2cTransferDirection::MASTER_READ,
        .cb                = nullptr,
        .slave_address     = 0x52};

    I2c i2c(i2cConfig);
    i2c.Init();

    while (true)
    {
    }
}
