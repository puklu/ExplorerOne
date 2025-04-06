#include "UltrasonicSensor.hpp"

   
UltrasonicSensor::UltrasonicSensor(std::unique_ptr<ITimer> timer, std::unique_ptr<PinBase> pTrigPin, std::unique_ptr<PinBase> pEchoPin):
    mpTrigPin(std::move(pTrigPin)),
    mpEchoPin(std::move(pEchoPin))
{

}

float UltrasonicSensor::CalculateDistance()
{
    return 0.0f; // TODO: Implement me 
}

