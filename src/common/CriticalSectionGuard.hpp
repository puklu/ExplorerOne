#pragma once

#include "drivers/interfaces/ICriticalSectionGuard.hpp"

class CriticalSectionGuard
{
public:
    explicit CriticalSectionGuard(ICriticalSectionGuard &guard);

    ~CriticalSectionGuard();

    // prevent copying and assigning
    CriticalSectionGuard(const CriticalSectionGuard&) = delete;
    CriticalSectionGuard& operator=(const CriticalSectionGuard&) = delete;

private:
    ICriticalSectionGuard &mGuard;    
};
