#include <random>
#include "UltrasonicSensorStub.hpp"

IDistanceSensor::IDistanceSensor(){}
   
UltransonicSensorStub::UltransonicSensorStub()
    :mDistance(0.0f)
{
}

float UltransonicSensorStub::GetDistanceInMm()
{
    static std::random_device rd; //seed
    static std::mt19937 gen(rd()); // random number generator
    static std::uniform_real_distribution<float> dist(5.0f, 200.0f); //range: 5mm to 200mm

    mDistance = dist(gen); // generate new random distance
    return mDistance;
}

std::unique_ptr<UltransonicSensorStub> SensorFactory::CreateUltrasonicSensor()
{
    return std::make_unique<UltransonicSensorStub>();
}



