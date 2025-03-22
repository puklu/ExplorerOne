#pragma once

#include <memory>
#include "drivers/interfaces/IDistanceSensor.hpp"
#include "drivers/interfaces/ITimer.hpp"

class UltrasonicSensor : public IDistanceSensor
{
public:    
    UltrasonicSensor(std::unique_ptr<ITimer> timer, std::unique_ptr<PinBase> pTrigPin, std::unique_ptr<PinBase> pEchoPin);
    float CalculateDistance() override;
private:
    std::shared_ptr<PinBase> mpTrigPin;
    std::shared_ptr<PinBase> mpEchoPin;    
}