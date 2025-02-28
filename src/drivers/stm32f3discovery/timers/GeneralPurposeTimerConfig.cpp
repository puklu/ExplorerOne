#include "common/assertHandler.hpp"
#include "GeneralPurposeTimerConfig.hpp"


GeneralPurposeTimerConfig::GeneralPurposeTimerConfig()
{
    // initialize each channel
    for(uint8_t i=0; i<GENERAL_PURPOSE_TIMER_NUM_CHANNELS; i++)
    {
        // create a new ChannelConfig object
        auto channel = std::make_shared<ChannelConfig>();

        channel->Init(Timer::eCaptureCompareSelection::NOT_SELECTED);

        mChannels[i] = channel;
    }
}
