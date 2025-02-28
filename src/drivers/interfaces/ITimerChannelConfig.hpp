#pragma once

#include "PinBase.hpp"

class ITimerChannelConfig
{
public:
    virtual ~ITimerChannelConfig() = default;

    /**
     * @brief Get the pin associated with this channel.
     * 
     * @return A pointer to the PinBase.
    */
    virtual std::shared_ptr<PinBase> GetChannelPin() const = 0;

protected:
};
