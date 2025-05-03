#pragma once

class PinBase;

class ITimerChannel
{
public:
    virtual ~ITimerChannel() = default;

    /**
     * @brief Get the pin associated with this channel.
     * 
     * @return A pointer to the PinBase.
    */
    virtual std::shared_ptr<PinBase> GetChannelPin() const = 0;

protected:
};
