#pragma once

#include "drivers/interfaces/ICriticalSectionGuard.hpp"
#include "stm32f303xc.h"

/**
 * @class Stm32f3CriticalSectionGuard
 * @brief Platform-specific implementation of ICriticalSectionGuard for STM32F3.
 *
 * This class provides methods to disable and enable interrupts on the STM32F3
 * microcontroller, ensuring atomic access to shared resources.
 */
class Stm32f3CriticalSectionGuard : public ICriticalSectionGuard
{
   public:
    void DisableInterrupts() override;
    void EnableInterrupts() override;
};
