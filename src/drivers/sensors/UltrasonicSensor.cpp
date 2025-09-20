#include "common/Delay.hpp"
#include "common/Trace.hpp"
#include "drivers/interfaces/IDigitalPin.hpp"
#include "drivers/interfaces/PinBase.hpp"
#include "UltrasonicSensor.hpp"

   
UltrasonicSensor::UltrasonicSensor(std::shared_ptr<ITimer> timer, std::shared_ptr<IDigitalPin> pTrigPin, std::shared_ptr<IDigitalPin> pEchoPin):
    mpTrigPin(std::move(pTrigPin)),
    mpEchoPin(std::move(pEchoPin)),
    mpTimer(std::move(timer))
{
    mpTimer->Start();
}

float UltrasonicSensor::CalculateDistance()
{
    const float SPEED_OF_SOUND = 0.0343; // cm/us

    SendTriggerPulse();

    WaitForEchoToGoHigh();

    Microseconds duration = MeasurePulseDuration();

    // TRACE_LOG("Pulse duration: %f", duration);

    float distance = (SPEED_OF_SOUND * duration)/2;

    return distance;

}

void UltrasonicSensor::SendTriggerPulse()
{
    mpTrigPin->WriteOutputValue(IO::eValue::IO_VALUE_HIGH);

    Microseconds startTime = mpTimer->GetTimeElapsedInMicrosecondsSinceStart();
    while(mpTimer->GetTimeElapsedInMicrosecondsSinceStart() - startTime < 10_us);
    Microseconds endTime = mpTimer->GetTimeElapsedInMicrosecondsSinceStart();

    TRACE_LOG("Trigger pulse duration: %f", endTime - startTime);

    // DELAY(10_us);
    mpTrigPin->WriteOutputValue(IO::eValue::IO_VALUE_LOW);
}

void UltrasonicSensor::WaitForEchoToGoHigh()
{
    while(mpEchoPin->ReadInputValue() == IO::eValue::IO_VALUE_LOW)
    {
        // TRACE_LOG("mpEchoPin->ReadInputValue(): %d", mpEchoPin->ReadInputValue());
    }
    // TRACE_LOG("mpEchoPin->ReadInputValue(): %d", mpEchoPin->ReadInputValue());
}

Microseconds UltrasonicSensor::MeasurePulseDuration()
{
    Microseconds startTime = mpTimer->GetTimeElapsedInMicrosecondsSinceStart();
    while(mpEchoPin->ReadInputValue() == IO::eValue::IO_VALUE_HIGH);
    Microseconds stopTime = mpTimer->GetTimeElapsedInMicrosecondsSinceStart();
    return stopTime - startTime;
}

