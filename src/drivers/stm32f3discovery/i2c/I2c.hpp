#pragma once

#include "common/PinDefinitions.hpp"
#include "drivers/stm32f3discovery/io/GpioPin.hpp"
#include "stm32f303xc.h"

using InterruptCallback = void (*)(void);

struct I2cInitStruct
{
    std::shared_ptr<GpioPin> scl_pin;
    std::shared_ptr<GpioPin> sda_pin;
    eI2cAddressMode          AddressMode;
    eI2cTransferDirection    TransferDirection;
    InterruptCallback        cb            = nullptr;
    int32_t                  slave_address = 0x52;
};

class I2c
{
   public:
    explicit I2c(I2cInitStruct const &i2cInitStruct);
    eGeneralStatus Init();
    eGeneralStatus SetSlaveAddress(uint32_t slave_address);
    eGeneralStatus SetNumBytes(uint32_t num_bytes);
    eGeneralStatus SetAddressMode(eI2cAddressMode address_modes);
    eGeneralStatus SetTransferDirection(eI2cTransferDirection direction);
    uint32_t       ReadData();

   private:
    eGeneralStatus SelectI2c();
    eGeneralStatus EnableClock() const;
    eGeneralStatus EnableI2c() const;
    eGeneralStatus EnableInterrupts(uint32_t interruptsMask) const;
    eGeneralStatus SetTimingRegister();

    I2C_TypeDef             *mpI2c = nullptr;
    std::shared_ptr<GpioPin> mpSclPin;
    std::shared_ptr<GpioPin> mpSdaPin;
    uint32_t                 mSlaveAddress;
    eI2cAddressMode          mAddressMode;
    eI2cTransferDirection    mTransferDirection;
    bool                     mIsInitialized = false;
};
