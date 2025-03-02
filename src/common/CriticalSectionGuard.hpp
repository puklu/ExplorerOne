#pragma once

#include "drivers/interfaces/ICriticalSectionGuard.hpp"


/**
 * @class CriticalSectionGuard
 * @brief RAII-style guard for managing critical sections.
 *
 * This class ensures that interrupts are disabled when the guard is created
 * and re-enabled when the guard goes out of scope. It uses the provided
 * implementation of ICriticalSectionGuard (platform-specific implementation
 * is passed to the ICriticalSectionGuard parameter by whoever wants
 * to use this class) to handle platform-specific interrupt control. It
 * is the responsibility of the caller to pass the desired platform-specific
 * implementation.
 *
 * @note This class is non-copyable and non-assignable to prevent misuse.
 */
class CriticalSectionGuard
{
public:
    /**
     * @brief Constructs a CriticalSectionGuard and disables interrupts.
     * @param guard Reference to an implementation of ICriticalSectionGuard.
     */
    explicit CriticalSectionGuard(ICriticalSectionGuard &guard);

    /**
     * @brief Destructor. Re-enables interrupts when the guard goes out of scope.
     */
    ~CriticalSectionGuard();

    // prevent copying and assigning
    CriticalSectionGuard(const CriticalSectionGuard&) = delete;
    CriticalSectionGuard& operator=(const CriticalSectionGuard&) = delete;

private:
    ICriticalSectionGuard &mGuard;    ///< Reference to the platform-specific critical section implementation.
};
