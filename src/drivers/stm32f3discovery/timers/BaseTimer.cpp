#include "BaseTimer.hpp"
#include "common/assertHandler.hpp"


BaseTimer::BaseTimer(uint16_t prescalerValue, uint32_t autoReloadRegisterValue, InterruptCallback cb):
    mPrescalerValue(prescalerValue),
    mAutoReloadRegisterValue(autoReloadRegisterValue),
    mCallBack(cb),
    // mUpdateRequestSource(Timer::eUpdateRequestSource::ANY_EVENT),
    mIrqNumber(static_cast<IRQn_Type>(0)), // Initialize to a valid IRQ number
    mIs32bitTimer(false),
    mIsInitialized(false),
    mIsTimerRunning(false),
    // mPeriodOfCounterClockMicroSeconds(Microseconds{0}),
    // mPeriodOfCounterClockMilliSeconds(Milliseconds{0}),
    // mPeriodOfCounterClockSeconds(Seconds{0}),
    mNumberOfTimesHighestValueReached(0)
{

}

eGeneralStatus BaseTimer::SetPeriodAndCount(Milliseconds period, uint32_t count)
{
    ASSERT(mpTimer);

    if(mIs32bitTimer)
    {
        ASSERT(period.value < ((float(UINT32_MAX)/SYS_CLK)*1000));
        ASSERT(count < UINT32_MAX);       
    }
    else
    {
        ASSERT(period.value <= ((float(UINT16_MAX)/SYS_CLK)*1000));
        ASSERT(count <= UINT16_MAX);
    }
    
    mPrescalerValue = (period.value*SYS_CLK)/1000 - 1;
    SetPrescalerValue();

    mAutoReloadRegisterValue = count;
    SetAutoReloadRegisterValue();

    mPeriodOfCounterClockSeconds.value = period.value/1000;
    mPeriodOfCounterClockMilliSeconds.value = period.value;
    mPeriodOfCounterClockMicroSeconds.value = period.value*1000;
    
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus BaseTimer::SetAutoReloadRegisterValue()
{
    ASSERT(mpTimer);

    if(mIs32bitTimer)
    {
        ASSERT(mAutoReloadRegisterValue < 0xFFFFFFFF);
    }
    else
    {
        ASSERT(mAutoReloadRegisterValue < 0xFFFF);
    }

    mpTimer->ARR = mAutoReloadRegisterValue;
    return eGeneralStatus::SUCCESS;
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

