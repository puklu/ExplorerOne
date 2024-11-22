// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "drivers/io/GpioPin.hpp"
#include "drivers/leds/leds.hpp"
#include "drivers/io/IrqHandlers.cpp"
#include "mcuInit.hpp"
#include "drivers/usart/UsartPin.hpp"
#include "drivers/interfaces/PinFactory.hpp"

#include "printf.h"


int main()
{

    SystemInit();

    UsartPinInitStruct pinInit = {};
    pinInit.pin_name           = IO::ePin::IO_UART4_TX;
    pinInit.alternate_function = IO::eAlternateFunction::IO_AF5;
    pinInit.baud_rate          = USART::eBaudRate::USART_BAUD_RATE_115200;

    [[maybe_unused]] PinBase const *usart_pin =
        PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_USART, pinInit);

    while (1)
    {
        printf("duck %d\n", 2025);
        delay(500000);
    }
}
