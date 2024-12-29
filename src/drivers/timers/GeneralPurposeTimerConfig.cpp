#include "common/assertHandler.hpp"
#include "GeneralPurposeTimerConfig.hpp"


void ChannelConfig::Init(Timer::eCaptureCompareSelection selection)
{
    this->mSelection = selection;

    if (mSelection == Timer::eCaptureCompareSelection::OUTPUT)
    {
        mOutputCompareConfig.mOutputCompareEnable = Timer::eOutputCompareClearEnable::DISABLE;
        mOutputCompareConfig.mOutputComparePreloadEnable = Timer::eOutputComparePreloadEnable::DISABLE;
        mOutputCompareConfig.mOutputCompareMode = Timer::eOutputCompareMode::FROZEN;
    }
    else
    {
        mInputCaptureConfig.mInputCaptureFilter = Timer::eInputCaptureFilter::NO_FILTER;
        mInputCaptureConfig.mInputCapturePrescaler = Timer::eInputCapturePrescaler::NO_PRESCALER;
    }
}


GeneralPurposeTimerConfig::GeneralPurposeTimerConfig()
{
    // initialize each channel
    for(uint8_t i=0; i<GENERAL_PURPOSE_TIMER_NUM_CHANNELS; i++)
    {
        mChannels[i].Init(Timer::eCaptureCompareSelection::NOT_SELECTED);
    }
}
