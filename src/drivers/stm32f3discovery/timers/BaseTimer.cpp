#include <algorithm>
#include <climits>
#include "BaseTimer.hpp"
#include "common/assertHandler.hpp"
#include "common/Trace.hpp"
#include "drivers/stm32f3discovery/common/Rcc.hpp"


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

// TODO: Possibly some error is being introduced here. Verify and fix.
Seconds BaseTimer::GetTimeElapsedSinceStart() const
{
    ASSERT(mpTimer);

    ASSERT(mAutoReloadRegisterValue != 0);

    if(mCountOfOverflows == INT_MAX - 1)
    {
        TRACE_LOG("mCountOfOverflows is at max, about to overflow!!");
    }

    // TODO: This should change according to the timer being used?
    uint32_t clockFreq = GetPeripherelClockFrequency();
 
    double ticks_elapsed = (mAutoReloadRegisterValue * static_cast<double>(mCountOfOverflows)) + GetCounterValue();
    double time = ticks_elapsed * (static_cast<double>(mPrescalerValue+1.0)/clockFreq);

    // PRINT("Time elapsed in seconds: %f", time);

    return Seconds{time};
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
    ASSERT(mPrescalerValue >= 0 && mPrescalerValue <= 0xffff);
    mpTimer->PSC = mPrescalerValue;
    return eGeneralStatus::SUCCESS;
}

// TODO: Probably wrong. Not tested. Fix me when needed!
uint32_t BaseTimer::GetSysClockTicksElapsedSinceStart() const
{
    ASSERT(mpTimer);

    uint32_t ticksUntilLastOverflow = mCountOfOverflows * mpTimer->ARR;

    return (GetCounterValue() + ticksUntilLastOverflow);
}

uint32_t BaseTimer::GetCounterValue() const
{
    ASSERT(mpTimer);
    return mpTimer->CNT;
}

Microseconds BaseTimer::GetTimeElapsedInMicrosecondsSinceStart() const
{
    ASSERT(mpTimer);

    Seconds timeElapsed;
    timeElapsed = GetTimeElapsedSinceStart();
    return Microseconds{timeElapsed};
}

Milliseconds BaseTimer::GetTimeElapsedInMillisecondsSinceStart() const
{
    ASSERT(mpTimer);

    Seconds timeElapsed;
    timeElapsed = GetTimeElapsedSinceStart();
    return Milliseconds{timeElapsed};
}

uint32_t BaseTimer::GetPeripherelClockFrequency() const
{
    ASSERT(mpTimer != nullptr);

    uint32_t freq = 0;

    // TIM1
    if(mpTimer == aAdvancedControlTimersAddress[0]){
        freq = RccImpl::GetInstance()->GetTim1ClockFreq();
    }

    // TIM8
    else if (mpTimer == aAdvancedControlTimersAddress[1])
    {
        freq = RccImpl::GetInstance()->GetTim8ClockFreq();
    }

    // TIM2
    else if (mpTimer == aGeneralPurposeTimersAddress[0])
    {
        freq = RccImpl::GetInstance()->GetTim2ClockFreq();
    }

    // TIM3
    else if (mpTimer == aGeneralPurposeTimersAddress[1])
    {
        freq = RccImpl::GetInstance()->GetTim3ClockFreq();
    }

    // TIM4
    else if (mpTimer == aGeneralPurposeTimersAddress[2])
    {
        freq = RccImpl::GetInstance()->GetTim4ClockFreq();
    }

    // TIM15
    else if (mpTimer == aGeneralPurposeTimersAddress[3])
    {
        freq = RccImpl::GetInstance()->GetTim15ClockFreq();
    }

    // TIM16
    else if (mpTimer == aGeneralPurposeTimersAddress[4])
    {
        freq = RccImpl::GetInstance()->GetTim16ClockFreq();
    }

    // TIM17
    else if (mpTimer == aGeneralPurposeTimersAddress[5])
    {
        freq = RccImpl::GetInstance()->GetTim17ClockFreq();
    }

    // TIM6
    else if (mpTimer == aBasicTimersAddress[0])
    {
        freq = RccImpl::GetInstance()->GetTim6ClockFreq();
    }

    // TIM7
    else if (mpTimer == aBasicTimersAddress[1])
    {
        freq = RccImpl::GetInstance()->GetTim7ClockFreq();
    }

    ASSERT(freq != 0);

    return freq;
}

// TODO: Errors definitely introduced here, specially when period is 0.01_ms.
// Timing seems somewhat fine when the period is set at 1_ms but when the period
// is set to 0.01_ms (ARR calculated is 80) then 1 sec stretched out to like 10 
// seconds.
// TODO: Make it accept period in any unit
eGeneralStatus BaseTimer::SetPeriod(Milliseconds period)
{
    ASSERT(mpTimer);

    // TODO: This should change according to the timer being used?
    uint32_t SYS_CLK = GetPeripherelClockFrequency();

    // 0. Do everything in seconds
    const Seconds periodInSeconds{period};

    // 1. Calculate time period limits
    const Seconds MIN_POSSIBLE_PERIOD{1.0/SYS_CLK};
    ASSERT(periodInSeconds >= MIN_POSSIBLE_PERIOD);
    
    // 2. Calculate the ticks it will take to cover the desired period
    // const uint64_t desiredTicks = static_cast<uint64_t>((SYS_CLK * periodInSeconds));
    const long double desiredTicks = static_cast<long double>((SYS_CLK * periodInSeconds));
 
    // 3. Handle 16-bit and 32-bit timer cases
    if(!mIs32bitTimer)
    {
        const Seconds MAX_POSSIBLE_PERIOD{(static_cast<double>(UINT16_MAX) * UINT16_MAX)/SYS_CLK};
        ASSERT(periodInSeconds <= MAX_POSSIBLE_PERIOD);

        // find suitable prescaler and ARR
        uint32_t bestPrescaler = 0;
        uint32_t bestArr = 0;

        for(uint32_t presc = 0; presc <=UINT16_MAX; ++presc)
        {
            uint32_t arr = static_cast<uint16_t>((desiredTicks / (presc + 1)));

            if(arr >=1 && arr <=UINT16_MAX)
            {
                bestPrescaler = presc;
                bestArr = arr;
                break;
            }
        }

        ASSERT(bestArr >=1 && bestArr <=UINT16_MAX);

        // mPrescalerValue = static_cast<uint16_t>((desiredTicks / 0xFFFF));
        // mPrescalerValue = std::clamp<uint16_t>(mPrescalerValue, 0u, 0xFFFFu);

        mPrescalerValue = static_cast<uint16_t>(bestPrescaler);

        //calculate the corresponding ARR value
        // mAutoReloadRegisterValue = static_cast<uint16_t>((desiredTicks / (mPrescalerValue + 1)));
        // mAutoReloadRegisterValue = std::clamp<uint16_t>(mAutoReloadRegisterValue, 1u, 0xFFFFu);

        mAutoReloadRegisterValue = static_cast<uint16_t>(bestArr);
    
        ASSERT(mAutoReloadRegisterValue <= UINT16_MAX);
    }
    else
    {
        const Seconds MAX_POSSIBLE_PERIOD{(static_cast<double>(UINT32_MAX) * UINT16_MAX)/SYS_CLK};
        ASSERT(periodInSeconds <= MAX_POSSIBLE_PERIOD);

        mPrescalerValue = 0u;
        mAutoReloadRegisterValue = static_cast<uint32_t>(desiredTicks / (mPrescalerValue + 1));
        
        ASSERT(mAutoReloadRegisterValue <= UINT32_MAX);
    }

    ASSERT(mPrescalerValue <= UINT16_MAX);
    ASSERT(mAutoReloadRegisterValue != 0u);

    // 4. Set the hardware registers with the calculated values
    SetPrescalerValue();
    SetAutoReloadRegisterValue();

    // 5. Store period in all units
    mPeriodOfCounterClockSeconds = Seconds(mAutoReloadRegisterValue*(mPrescalerValue + 1)/SYS_CLK); // store the actual value instead of requested period
    mPeriodOfCounterClockMilliSeconds = Milliseconds(mPeriodOfCounterClockSeconds);
    mPeriodOfCounterClockMicroSeconds = Microseconds(mPeriodOfCounterClockSeconds);

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
