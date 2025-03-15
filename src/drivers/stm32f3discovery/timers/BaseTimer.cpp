#include "BaseTimer.hpp"
#include "common/assertHandler.hpp"


BaseTimer::BaseTimer(uint16_t prescalerValue, uint32_t autoReloadRegisterValue, InterruptCallback cb):
    mPrescalerValue(prescalerValue),
    mAutoReloadRegisterValue(autoReloadRegisterValue),
    mCallBack(cb),
    mNumberOfTimesHighestValueReached(0U)
{

}

template<typename TimeUnit>
TimeUnit BaseTimer::GetTimeElapsed(const TimeUnit& period) const
{
    TimeUnit time;
    if(mNumberOfTimesHighestValueReached != 0)
    {
        time.value = period.value * (mAutoReloadRegisterValue * mNumberOfTimesHighestValueReached + GetCounterValue());
    }
    else
    {
        time.value = period.value * GetCounterValue();
    }

    return time;
}

eGeneralStatus BaseTimer::EnableNVIC()
{
    NVIC_EnableIRQ(mIrqNumber);
    NVIC_SetPriority(mIrqNumber, PRIORITY_TIMER);  ///TODO: fix this priority

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BaseTimer::SetPrescalerValue()
{
    ASSERT(mpTimer);
    ASSERT(mPrescalerValue >= 1 && mPrescalerValue <= 0xffff);
    mpTimer->PSC = mPrescalerValue;
    return eGeneralStatus::SUCCESS;
}

float BaseTimer::GetSysClockTicksElapsed() const
{
    return GetCounterValue()/mPrescalerValue;
}

uint16_t BaseTimer::GetCounterValue() const
{
    return mpTimer->CNT;
}

Microseconds BaseTimer::GetTimeElapsedInMicroseconds() const
{
    Microseconds us;
    us = GetTimeElapsed(mPeriodOfCounterClockMicroSeconds);
    return us;
}

Milliseconds BaseTimer::GetTimeElapsedInMilliseconds() const
{
    Milliseconds ms;
    ms = GetTimeElapsed(mPeriodOfCounterClockMilliSeconds);
    return ms;
}

void BaseTimer::IncrementNumberOfTimesHighestValueReached()
{
    mNumberOfTimesHighestValueReached++;
}

bool BaseTimer::GetIsTimerRunning() const
{
    return mIsTimerRunning;
}

void BaseTimer::SetBits(volatile uint32_t& rRegister, const uint32_t& rMask) const
{
    rRegister |= rMask;
}

void BaseTimer::ResetBits(volatile uint32_t& rRegister, const uint32_t& rMask) const
{
    rRegister &= ~rMask;
}

