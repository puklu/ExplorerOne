#include "mcuInit.hpp"
#include "stm32f303xc.h"

#include "drivers/interfaces/IrqHandlers.cpp"

bool isSystemInitialized = false;

void SystemInit()
{
    // TODO: Handle watchdog here
    // WWDG->CR &= ~ (1<<7);  //Disable watchdog

    // DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;  // Freeze IWDG in debug mode
    // DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_WWDG_STOP;  // Freeze WWDG in debug mode

    isSystemInitialized = true;

}
