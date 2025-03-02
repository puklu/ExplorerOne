#pragma once

#include "stm32f303xc.h"
#include "drivers/interfaces/ICriticalSectionGuard.hpp"

class Stm32f3CriticalSectionGuard : public ICriticalSectionGuard
{
public:
    void DisableInterrupts() override;
    void EnableInterrupts() override;
};
