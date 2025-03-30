#include <algorithm>
#include "BaseTimer.hpp"
#include "common/assertHandler.hpp"


BaseTimer::BaseTimer(uint16_t prescalerValue, uint32_t autoReloadRegisterValue, InterruptCallback cb):
    mPrescalerValue(prescalerValue),
    mAutoReloadRegisterValue(autoReloadRegisterValue),
    mCallBack(cb),
    mIrqNumber(static_cast<IRQn_Type>(0)), // Initialize to a valid IRQ number
    mIs32bitTimer(false),
    mIsInitialized(false),
    mIsTimerRunning(false),
    mPeriodOfCounterClockMicroSeconds(Microseconds{0}),
    mPeriodOfCounterClockMilliSeconds(Milliseconds{0}),
    mPeriodOfCounterClockSeconds(Seconds{0}),
    mCountOfOverflows(0)
{

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
TimeUnit BaseTimer::GetTimeElapsedSinceStart(const TimeUnit& period) const
{
    ASSERT(mpTimer);

    ASSERT(period != 0);
 
    double time = (period * static_cast<double>(mCountOfOverflows)) + (GetCounterValue() * (static_cast<double>(mPrescalerValue)/SYS_CLK));

    return TimeUnit{time};
}

eGeneralStatus BaseTimer::EnableNVIC()
{
    ASSERT(mpTimer);

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
    ASSERT(mpTimer);

    return GetCounterValue()/mPrescalerValue;
}

uint16_t BaseTimer::GetCounterValue() const
{
    ASSERT(mpTimer);
    return mpTimer->CNT;
}

Microseconds BaseTimer::GetTimeElapsedInMicrosecondsSinceStart() const
{
    ASSERT(mpTimer);

    Microseconds us;
    us = GetTimeElapsedSinceStart(mPeriodOfCounterClockMicroSeconds);
    return us;
}

Milliseconds BaseTimer::GetTimeElapsedInMillisecondsSinceStart() const
{
    ASSERT(mpTimer);

    Milliseconds ms;
    ms = GetTimeElapsedSinceStart(mPeriodOfCounterClockMilliSeconds);
    return ms;
}


eGeneralStatus BaseTimer::SetPeriod(Milliseconds period)
{
    ASSERT(mpTimer);

    // 0. Do everything in seconds
    const Seconds periodInSeconds{period};

    // 1. Calculate time period limits
    const Seconds MIN_POSSIBLE_PERIOD{1.0/SYS_CLK};
    ASSERT(periodInSeconds >= MIN_POSSIBLE_PERIOD);
    
    // 2. Calculate the ticks it will take to cover the desired period
    const uint64_t desiredTicks = static_cast<uint64_t>((SYS_CLK * periodInSeconds));
 
    // 3. Handle 16-bit and 320bit timer cases
    if(!mIs32bitTimer)
    {
        const Seconds MAX_POSSIBLE_PERIOD{(static_cast<double>(UINT16_MAX) * UINT16_MAX)/SYS_CLK};
        ASSERT(periodInSeconds <= MAX_POSSIBLE_PERIOD);

        mPrescalerValue = static_cast<uint16_t>((desiredTicks / 0xFFFF) + 1);
        mPrescalerValue = std::clamp<uint16_t>(mPrescalerValue, 1u, 0xFFFFu);

        //calculate the corresponding ARR value
        mAutoReloadRegisterValue = static_cast<uint16_t>((desiredTicks / (mPrescalerValue + 1)));
        mAutoReloadRegisterValue = std::clamp<uint16_t>(mAutoReloadRegisterValue, 1u, 0xFFFFu);
    
        ASSERT(mAutoReloadRegisterValue < 0xFFFFu);
    }
    else
    {
        const Seconds MAX_POSSIBLE_PERIOD{(static_cast<double>(UINT32_MAX) * UINT16_MAX)/SYS_CLK};
        ASSERT(periodInSeconds <= MAX_POSSIBLE_PERIOD);

        mPrescalerValue = 1u;
        mAutoReloadRegisterValue = static_cast<uint32_t>(desiredTicks / (mPrescalerValue + 1));
        
        ASSERT(mAutoReloadRegisterValue < 0xFFFFFFFFu);
    }

    ASSERT(mPrescalerValue < 0xFFFFu);
    ASSERT(mAutoReloadRegisterValue != 0u);

    // 4. Set the hardware registers with the calculated values
    SetPrescalerValue();
    SetAutoReloadRegisterValue();

    // 5. Store period in all units
    mPeriodOfCounterClockSeconds = Seconds(period);
    mPeriodOfCounterClockMilliSeconds = period;
    mPeriodOfCounterClockMicroSeconds = Microseconds(period);

    return eGeneralStatus::SUCCESS;
}

void BaseTimer::IncrementCountOfOverflows()
{
    ASSERT(mpTimer);

    mCountOfOverflows++;
}

bool BaseTimer::GetIsTimerRunning() const
{
    ASSERT(mpTimer);

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
