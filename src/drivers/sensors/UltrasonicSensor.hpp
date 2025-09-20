#pragma once

#include <memory>
#include "drivers/interfaces/IDistanceSensor.hpp"
#include "drivers/interfaces/ITimer.hpp"

class IDigitalPin;

class UltrasonicSensor : public IDistanceSensor
{
public:    
    UltrasonicSensor(std::shared_ptr<ITimer> timer, std::shared_ptr<IDigitalPin> pTrigPin, std::shared_ptr<IDigitalPin> pEchoPin);
    float CalculateDistance() override;
private:
    void SendTriggerPulse();
    void WaitForEchoToGoHigh();
    Microseconds MeasurePulseDuration();

    std::shared_ptr<IDigitalPin> mpTrigPin;
    std::shared_ptr<IDigitalPin> mpEchoPin;
    std::shared_ptr<ITimer> mpTimer;
};
