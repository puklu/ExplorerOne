#include "CriticalSectionGuard.hpp"

CriticalSectionGuard::CriticalSectionGuard(ICriticalSectionGuard &guard)
    :mGuard(guard)
{
    mGuard.DisableInterrupts();
}

CriticalSectionGuard::~CriticalSectionGuard()
{
    mGuard.EnableInterrupts();
}
