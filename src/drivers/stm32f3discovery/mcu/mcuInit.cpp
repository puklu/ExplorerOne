#include "mcuInit.hpp"
#include "stm32f303xc.h"

#include "drivers/interfaces/IrqHandlers.cpp"
#include "drivers/interfaces/PinBase.hpp"
#include "drivers/interfaces/PinFactory.hpp"

bool isSystemInitialized = false;

void SystemInit()
{
    // TODO: Handle watchdog here
    // WWDG->CR &= ~ (1<<7);  //Disable watchdog

    // DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;  // Freeze IWDG in debug mode
    // DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_WWDG_STOP;  // Freeze WWDG in debug mode

    isSystemInitialized = true;

    // create a usart pin for printing via usart
    if(!activePrintUsartPin)
    {
        UsartPinInitStruct pinInit = {};
        pinInit.pin_name           = IO::ePin::IO_UART4_TX_PRINT;
        pinInit.alternate_function = IO::eAlternateFunction::IO_AF5;
        pinInit.baud_rate          = USART::eBaudRate::USART_BAUD_RATE_115200;

        [[maybe_unused]] std::shared_ptr<PinBase> const usart_print_pin =
            PinFactory::CreatePin(IO::ePinType::IO_PIN_TYPE_PRINTING_USART, pinInit);
    }

}
