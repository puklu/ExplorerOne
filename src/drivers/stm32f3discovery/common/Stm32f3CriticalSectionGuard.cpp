#include "Stm32f3CriticalSectionGuard.hpp"


void Stm32f3CriticalSectionGuard::DisableInterrupts()
{
    __disable_irq();
}

void Stm32f3CriticalSectionGuard::EnableInterrupts()
{
    __enable_irq();
}