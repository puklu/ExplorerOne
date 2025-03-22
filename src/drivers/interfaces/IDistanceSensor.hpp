#pragma once

class IDistanceSensor
{
    virtual ~IDistanceSensor() = default;
    virtual float CalculateDistance() = 0;
};
