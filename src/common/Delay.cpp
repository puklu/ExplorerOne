#include "Delay.hpp"
#include "common/assertHandler.hpp"
#include "common/Trace.hpp"

// Initialize  static member
Delay* Delay::mpInstance = nullptr;


void Delay::Init(ITimer& rTimer)
{
    TRACE_LOG("Initializing delay");
    ASSERT(mpInstance == nullptr);
    static Delay instance(rTimer);
    mpInstance = &instance;
}

Delay& Delay::GetInstance()
{
    if(mpInstance == nullptr)
    {
        ASSERT(false);
    }
    return *mpInstance;
}

Delay::Delay(ITimer& rTimer):
    mrTimer(rTimer)
{
    mrTimer.Start();
}

void Delay::operator()(Milliseconds ms)
{
    mrTimer.Reset();
    Milliseconds start = mrTimer.GetTimeElapsedInMillisecondsSinceStart();
    while (mrTimer.GetTimeElapsedInMillisecondsSinceStart() < (ms - start));
}

void Delay::operator()(Microseconds us)
{
    mrTimer.Reset();
    Microseconds start = mrTimer.GetTimeElapsedInMicrosecondsSinceStart();
    while (mrTimer.GetTimeElapsedInMicrosecondsSinceStart() < (us - start));
}
