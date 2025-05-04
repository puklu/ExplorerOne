#include "mcuInit.hpp"
#include "stm32f303xc.h"

#include "common/Delay.hpp"
#include "drivers/factory/PinFactory.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "drivers/stm32f3discovery/common/IrqHandlers.cpp"
#include "drivers/stm32f3discovery/timers/BasicTimer.hpp"

static bool isSystemInitialized = false;

void SystemInit()
{
    // TODO: Handle watchdog here
    // WWDG->CR &= ~ (1<<7);  //Disable watchdog

    // DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;  // Freeze IWDG in debug mode
    // DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_WWDG_STOP;  // Freeze WWDG in debug mode

}

bool IsSystemInitialized()
{
    return isSystemInitialized;
}

void PostSystemInit()
{
    InitializeConsolePrinting();

    InitializeDelaySystem();

    isSystemInitialized = true;

    TRACE_LOG("Post System Init done");

}

void InitializeConsolePrinting()
{
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

void InitializeDelaySystem()
{
    // timer to use for delay functionality
    static BasicTimerConfig delayTimerConfig;
    static std::unique_ptr<BasicTimer> gpDelayTimer = std::make_unique<BasicTimer>(delayTimerConfig);
    
    gpDelayTimer->Init();
    
    // // Initialize the Delay singleton
    Delay::Init(*gpDelayTimer);
}
