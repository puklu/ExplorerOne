#pragma once


/**
 * @class ICriticalSectionGuard
 * @brief Interface for managing critical sections.
 *
 * This interface defines methods for disabling and enabling interrupts,
 * which are used to implement platform-specific critical section guards.
 */

class ICriticalSectionGuard
{
public:
    virtual ~ICriticalSectionGuard() = default;

    /**
     * @brief Disables interrupts to enter a critical section.
     */
    virtual void DisableInterrupts() = 0;

    /**
     * @brief Enables interrupts to exit a critical section.
     */
    virtual void EnableInterrupts() = 0;
};
