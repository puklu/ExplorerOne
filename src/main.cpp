// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "drivers/io/GPIOpin.hpp"
#include "drivers/leds/leds.hpp"
#include "drivers/io/IrqHandlers.cpp"
#include "mcuInit.hpp"
#include "drivers/usart/usart.hpp"


int main()
{

    SystemInit();

    IO::GpioPinInitStruct pinInit = {
        .pin_name      = IO::ePin::IO_UART4_TX,
        .mode          = IO::eMode::IO_MODE_ALT_FUNCTION,
        .pupd_resistor = IO::ePupdResistor::IO_RESISTOR_PULL_DOWN,
    };

    IO::GPIOpin *pin = IO::GPIOpin::CreatePin(pinInit);

    USART::UsartInitStruct usartInitStruct = {
        .pin = pin,
        .alternate_function = IO::eAlternateFunction::IO_AF5,
        .baud_rate = USART::eBaudRate::USART_BAUD_RATE_115200,
    };

    USART::Usart usart(usartInitStruct);

    // char *data = 'Hello';
    // // uintptr_t data = myChar;

    // for(int i=0; i<sizeof(*data); i++){
    // USART::TransmitData(data++);
    // }

    while (1)
    {
        usart.TransmitData("d");
        usart.TransmitData("u");
        usart.TransmitData("c");
        usart.TransmitData("k");
        usart.TransmitData("\n");
        delay(500000);
    }
}
