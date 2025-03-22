#pragma once

#include <memory>
#include <vector>
#include "common/time.hpp"
#include "ITimer.hpp"
#include "ITimerChannel.hpp"
 

class IPwm : public virtual ITimer
{
public:
    virtual std::vector<std::shared_ptr<ITimerChannel>> GetChannels() = 0;
    virtual eGeneralStatus SetPeriod(Milliseconds period_in_ms) = 0;
    virtual eGeneralStatus SetDutyCycle(uint32_t duty_cycle, uint8_t channel_index) = 0;
    virtual ~IPwm() = default;
};
 