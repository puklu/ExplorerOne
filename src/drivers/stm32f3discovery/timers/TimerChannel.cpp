#include "TimerChannel.hpp"
#include "common/assertHandler.hpp"

void TimerChannel::Init(Timer::eCaptureCompareSelection selection)
{
    this->mSelection = selection;

    if (mSelection == Timer::eCaptureCompareSelection::OUTPUT)
    {
        mOutputCompareConfig.mOutputCompareEnable = Timer::eOutputCompareClearEnable::DISABLE;
        mOutputCompareConfig.mOutputComparePreloadEnable = Timer::eOutputComparePreloadEnable::DISABLE;
        mOutputCompareConfig.mOutputCompareMode = Timer::eOutputCompareMode::FROZEN;
        if(mOutputCompareConfig.mOutputCompareMode == Timer::eOutputCompareMode::PWM_MODE_1 || 
           mOutputCompareConfig.mOutputCompareMode == Timer::eOutputCompareMode::PWM_MODE_2 )
           {
                mOutputCompareConfig.mPwmDutyCyclePercent = 50;
                mOutputCompareConfig.mPwmPeriodMs = Milliseconds{1000};
           }
    }
    else
    {
        mInputCaptureConfig.mInputCaptureFilter = Timer::eInputCaptureFilter::NO_FILTER;
        mInputCaptureConfig.mInputCapturePrescaler = Timer::eInputCapturePrescaler::NO_PRESCALER;
    }
}

std::shared_ptr<PinBase> TimerChannel::GetChannelPin() const
{
    ASSERT(mpChannelPin != nullptr);
    return mpChannelPin;    
}
