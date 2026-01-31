#pragma once

#include "drivers/interfaces/IDistanceSensor.hpp"

class TofSensor : public IDistanceSensor
{
   public:
    TofSensor();
    float CalculateDistance() override;
};
