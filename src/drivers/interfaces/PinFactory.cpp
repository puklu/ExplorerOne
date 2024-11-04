#include "drivers/io/ExtiPin.hpp"
#include "drivers/io/GpioPin.hpp"
#include "drivers/usart/UsartPin.hpp"
#include "pinBank.hpp"
#include "PinFactory.hpp"

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
            break;
        case IO::ePinType::IO_PIN_TYPE_USART:
            pin = new UsartPin(static_cast<const UsartPinInitStruct&>(pin_init_struct));
            break;    
        default:
            break;  // TODO: handle error
    }

    activePins[port_num][pin_num] = pin;
    return pin;
}
