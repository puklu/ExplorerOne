/**
 * @file
 */

#pragma once

#include <cstdint>  // for uint8_t
#include "common/defines.hpp"
#include "common/time.hpp"
#include "drivers/interfaces/ITimer.hpp"

class Delay
{
public:
    static void Init(ITimer& rTimer); // singleton pattern

    /// @brief to be able to use the instance like a function,
    /// so the user can just call delay like `delay(1000_ms)`
    void operator()(Milliseconds ms); 
    void operator()(Microseconds us);

    // Public getter for the instance
    static Delay& GetInstance();

    Delay(const Delay&) = delete; // delete copy constructor
    Delay& operator=(const Delay&) = delete; // delete assignment operator


private:
    explicit Delay(ITimer& rTimer);
    ITimer& mrTimer;
    static Delay* mpInstance; // singleton instance pointer
};
