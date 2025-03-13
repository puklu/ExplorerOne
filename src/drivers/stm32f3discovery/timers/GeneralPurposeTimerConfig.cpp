#include "common/assertHandler.hpp"
#include "GeneralPurposeTimerConfig.hpp"


GeneralPurposeTimerConfig::GeneralPurposeTimerConfig()
{
    // initialize each channel
    for(uint8_t i=0; i<GENERAL_PURPOSE_TIMER_NUM_CHANNELS; i++)
    {
        // create a new TimerChannel object
        auto channel = std::make_unique<TimerChannel>();

        channel->Init(Timer::eCaptureCompareSelection::NOT_SELECTED);

        mChannels.push_back(std::move(channel));
    }
}
