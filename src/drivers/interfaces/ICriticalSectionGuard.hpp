#pragma once

class ICriticalSectionGuard
{
public:
    virtual ~ICriticalSectionGuard() = default;
    virtual void DisableInterrupts() = 0;
    virtual void EnableInterrupts() = 0;
};
