#include "VL53L1X.hpp"

#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "drivers/stm32f3discovery/i2c/I2c.hpp"

// === Only suppress warnings for the external header ===
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcpp"

#include "vl53l1_api.h"
#include "vl53l1_platform.h"

#pragma GCC diagnostic pop

std::unordered_map<VL53L1_DEV, VL53L1X*> VL53L1X::sVl53l1xInstancesMap;

// =======================================================

VL53L1X::VL53L1X(I2c& i2c_bus, uint32_t slave_address)
    : mrI2cBus(i2c_bus), mSlaveAddress(slave_address)
{
    ASSERT(mrI2cBus.IsInitialized());

    mpDevice->i2c_slave_address               = slave_address;
    mpDevice->comms_type                      = VL53L1_I2C;
    mpDevice->comms_speed_khz                 = 100;
    mpDevice->new_data_ready_poll_duration_ms = 10;

    // register this instance
    sVl53l1xInstancesMap[mpDevice] = this;
}

VL53L1X::~VL53L1X()
{
    // remove this instance from the map
    sVl53l1xInstancesMap.erase(mpDevice);
}

eGeneralStatus VL53L1X::Init()
{
    // ===== OPTIONAL =====
    // allows to ensure that the device is booted and ready.
    // It is a blocking function because there is an internal polling
    VL53L1_WaitDeviceBooted(mpDevice);

    SetTimingBudget(66000);

    // ====================

    // perform the device initialization, called once and only once after device
    // is brought out of reset.
    VL53L1_DataInit(mpDevice);

    // allow to load device settings specific for a given use case.
    VL53L1_StaticInit(mpDevice);

    mIsInitialized = true;

    return eGeneralStatus::SUCCESS;
}

VL53L1_Error VL53L1X::SetTimingBudget(float us)
{
    [[maybe_unused]] VL53L1_Error stop_status =
        VL53L1_StopMeasurement(mpDevice);
    VL53L1_Error status =
        VL53L1_SetMeasurementTimingBudgetMicroSeconds(mpDevice, us);

    return status;
}

I2c& VL53L1X::GetI2cBus() const
{
    return mrI2cBus;
}

float VL53L1X::CalculateDistance()
{
    ASSERT(mIsInitialized);

    // start a measurement
    VL53L1_Error start_status = VL53L1_StartMeasurement(mpDevice);
    TRACE_LOG("VL53L1_StartMeasurement status = %s", start_status);

    // poll on a function to ask if a the ranging data is available
    uint8_t      isMeasurementDataReady = 0;
    VL53L1_Error measurement_ready_status =
        VL53L1_GetMeasurementDataReady(mpDevice, &isMeasurementDataReady);
    // TODO: use interrupt instead
    TRACE_LOG("VL53L1_GetMeasurementDataReady status = %s",
              measurement_ready_status);

    // get a ranging data
    VL53L1_RangingMeasurementData_t measurementData;
    VL53L1_Error                    measurement_data_status =
        VL53L1_GetRangingMeasurementData(mpDevice, &measurementData);
    TRACE_LOG("VL53L1_GetRangingMeasurementData status = %s",
              measurement_data_status);

    // Interrupt must be cleared by calling driver function, after reading
    // ranging data
    VL53L1_ClearInterruptAndStartMeasurement(mpDevice);
    VL53L1_Error stop_status = VL53L1_StopMeasurement(mpDevice);
    TRACE_LOG("VL53L1_StopMeasurement status = %s", stop_status);

    return measurementData.RangeMilliMeter;
}

VL53L1_Error VL53L1_WrByte(VL53L1_Dev_t* pdev, uint16_t index, uint8_t data)
{
    return VL53L1_WriteMulti(pdev, index, &data, 1);
}

VL53L1_Error VL53L1_WriteMulti(VL53L1_Dev_t* pdev, uint16_t index,
                               uint8_t* pdata, uint32_t count)
{
    if (!pdev || !pdata || count == 0)
    {
        return VL53L1_ERROR_INVALID_PARAMS;
    }

    if (count > 255)
    {
        return VL53L1_ERROR_INVALID_PARAMS;
    }

    // create a buffer to hold register address + data
    uint8_t buffer[2 + count];  // 2 bytes for register address + data bytes

    // fill the buffer with register address (data is trasmitted as MSB first)
    buffer[0] = (index >> 8) & 0xFF;  // high byte
    buffer[1] = index & 0xFF;         // low byte

    // copy the data into buffer
    for (uint32_t i = 0; i < count; i++)
    {
        buffer[2 + i] = pdata[i];
    }

    // find the VL53L1X class instance from the static memory
    auto it = VL53L1X::sVl53l1xInstancesMap.find(pdev);
    if (it == VL53L1X::sVl53l1xInstancesMap.end())
    {
        return VL53L1_ERROR_UNDEFINED;
    }

    VL53L1X* pSensor = it->second;

    if (!pSensor)
    {
        return VL53L1_ERROR_UNDEFINED;
    }

    // write the data using I2c
    eGeneralStatus status = pSensor->GetI2cBus().WriteData(
        pdev->i2c_slave_address, buffer, 2 + count);

    return (status == eGeneralStatus::SUCCESS ? VL53L1_ERROR_NONE
                                              : VL53L1_ERROR_CONTROL_INTERFACE);
}

VL53L1_Error VL53L1_ReadMulti(VL53L1_Dev_t* pdev, uint16_t index,
                              uint8_t* pdata, uint32_t count)
{
    if (!pdev || !pdata || count == 0)
    {
        return VL53L1_ERROR_INVALID_PARAMS;
    }

    if (count > 255)
    {
        return VL53L1_ERROR_INVALID_PARAMS;
    }

    // create a buffer to hold register address
    uint8_t buffer[2];  // 2 bytes for register address

    // fill the buffer with register address (data is trasmitted as MSB first)
    buffer[0] = (index >> 8) & 0xFF;  // high byte
    buffer[1] = index & 0xFF;         // low byte

    // find the VL53L1X class instance from the static memory
    auto it = VL53L1X::sVl53l1xInstancesMap.find(pdev);
    if (it == VL53L1X::sVl53l1xInstancesMap.end())
    {
        return VL53L1_ERROR_UNDEFINED;
    }

    VL53L1X* pSensor = it->second;

    if (!pSensor)
    {
        return VL53L1_ERROR_UNDEFINED;
    }

    // write the data using I2c
    eGeneralStatus status = pSensor->GetI2cBus().ReadData(
        pdev->i2c_slave_address, buffer, 2, pdata, count);

    return (status == eGeneralStatus::SUCCESS ? VL53L1_ERROR_NONE
                                              : VL53L1_ERROR_CONTROL_INTERFACE);
}

VL53L1_Error VL53L1_RdByte(VL53L1_Dev_t* pdev, uint16_t index, uint8_t* pdata)
{
    return VL53L1_ReadMulti(pdev, index, pdata, 1);
}