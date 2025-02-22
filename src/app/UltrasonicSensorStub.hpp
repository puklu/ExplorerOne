#pragma once
#include <memory>

class IDistanceSensor
{
public:
    IDistanceSensor();
    virtual ~IDistanceSensor() = default;
    virtual float GetDistanceInMm() = 0;
};

class UltransonicSensorStub : public IDistanceSensor
{
public:    
    UltransonicSensorStub();
    float GetDistanceInMm() override;
private:
    float mDistance;    
};

class SensorFactory
{
public:    
    static std::unique_ptr<UltransonicSensorStub> CreateUltrasonicSensor();
};
