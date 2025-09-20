#pragma once

class IDistanceSensor
{
public:    
    virtual ~IDistanceSensor() = default;
    virtual float CalculateDistance() = 0;
};
