#include "drivers/io/ExtiPin.hpp"
#include "drivers/io/GpioPin.hpp"
#include "drivers/usart/ServiceISRs.hpp"
#include "drivers/usart/UsartPin.hpp"
#include "pinBank.hpp"
#include "PinFactory.hpp"
#include "printf_redirect.h"

PinBase* PinFactory::CreatePin(IO::ePinType pin_type, const PinBaseInitStruct &pin_init_struct)
{
    // Get the port number and pin number
    uint8_t port_num = (pin_init_struct.pin_name & IO_PORT_MASK) >> IO_PORT_OFFSET;
    uint8_t pin_num = pin_init_struct.pin_name & IO_PIN_MASK;

    // Check if an instance of the pin already exists.
    // if it does, dont create an instance for the pin
    if(activePins[port_num][pin_num] != nullptr){
        return activePins[port_num][pin_num];
    }

    // otherwise create an instance and return it
    PinBase *pin = nullptr;

    switch (pin_type) {
        case IO::ePinType::IO_PIN_TYPE_GPIO:
            pin = new GpioPin(static_cast<const GpioPinInitStruct&>(pin_init_struct));
            break;
        case IO::ePinType::IO_PIN_TYPE_EXTI:
            pin = new ExtiPin(static_cast<const ExtiPinInitStruct&>(pin_init_struct));
            activeExtiPins[port_num][pin_num] = pin;
            break;
        case IO::ePinType::IO_PIN_TYPE_USART:{
            auto usart_pin = new UsartPin(static_cast<const UsartPinInitStruct&>(pin_init_struct));

            if(usart_pin->mpUsart == USART1)   
            {
                usart_pin->SetInterruptCallback(Usart1ServiceISR);
            }
            else if(usart_pin->mpUsart == USART2)
            {
                usart_pin->SetInterruptCallback(Usart2ServiceISR);
            }
            else if(usart_pin->mpUsart == USART3)
            {
                usart_pin->SetInterruptCallback(Usart3ServiceISR);
            }
            else if(usart_pin->mpUsart == UART4)
            {
                usart_pin->SetInterruptCallback(Uart4ServiceISR);
            }
            else if(usart_pin->mpUsart == UART5)
            {
                usart_pin->SetInterruptCallback(Uart5ServiceISR);
            }

            pin = usart_pin;
            activeUsartPins[port_num][pin_num] = pin;
            break;
        } 
        case IO::ePinType::IO_PIN_TYPE_PRINTING_USART:{
            auto usart_pin = new UsartPin(static_cast<const UsartPinInitStruct&>(pin_init_struct));
            usart_pin->SetInterruptCallback(UsartServiceISR_Printing);
            pin = usart_pin;
            activeUsartPins[port_num][pin_num] = usart_pin;
            activePrintUsartPin = usart_pin;

            // Register the global UsartPutchar function
            printf_set_putchar(UsartPutchar);
            
            break;
        }
        default:
            break;  // TODO: handle error
    }

    activePins[port_num][pin_num] = pin;
    return pin;
}
