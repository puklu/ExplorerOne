#pragma once

#include <unordered_map>

#include "drivers/interfaces/IDistanceSensor.hpp"
#include "drivers/stm32f3discovery/i2c/I2c.hpp"
#include "vl53l1_platform_user_data.h"  // for VL53L1_Dev_t

class VL53L1X : public IDistanceSensor
{
   public:
    VL53L1X(I2c& i2c_bus, uint32_t slave_address);
    ~VL53L1X();
    eGeneralStatus Init();
    float          CalculateDistance() override;
    VL53L1_Error   SetTimingBudget(float us);
    I2c&           GetI2cBus() const;

    static std::unordered_map<VL53L1_DEV, VL53L1X*> sVl53l1xInstancesMap;

   private:
    VL53L1_DEV mpDevice;
    I2c&       mrI2cBus;
    uint32_t   mSlaveAddress;
    bool       mIsInitialized = false;
};
