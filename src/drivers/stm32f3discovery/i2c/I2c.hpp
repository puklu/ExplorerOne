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
    uint8_t                  slave_address = 0x52;
};

class I2c
{
   public:
    explicit I2c(I2cInitStruct const &i2cInitStruct);
    eGeneralStatus Init();
    eGeneralStatus SetSlaveAddress(uint8_t slave_address);
    eGeneralStatus SetNumBytes(uint8_t num_bytes);
    eGeneralStatus SetAddressMode(eI2cAddressMode address_modes);
    eGeneralStatus SetTransferDirection(eI2cTransferDirection direction);
    eGeneralStatus WriteData(uint8_t slave_address, const uint8_t *pData,
                             const uint32_t len);
    eGeneralStatus WriteDataByte(uint8_t slave_address, const uint8_t *pData);
    eGeneralStatus ReadData(uint8_t slave_address, const uint8_t *pWrite_data,
                            const uint32_t write_len, uint8_t *pRead_buf,
                            uint32_t read_len);
    eGeneralStatus ReadDataByte(uint8_t        slave_address,
                                const uint8_t *pWrite_data,
                                const uint32_t write_len, uint8_t *pRead_buf);
    bool           IsInitialized() const;

   private:
    eGeneralStatus SelectI2c();
    eGeneralStatus StartTransmission() const;
    eGeneralStatus StartTransmissionWithAutoend() const;
    eGeneralStatus StopTransmission() const;
    eGeneralStatus EnableAutoReload() const;
    eGeneralStatus EnableClock() const;
    eGeneralStatus EnableI2c() const;
    eGeneralStatus DisableI2c() const;
    eGeneralStatus EnableInterrupts(uint32_t interruptsMask) const;
    eGeneralStatus SetTimingRegister();

    I2C_TypeDef             *mpI2c = nullptr;
    std::shared_ptr<GpioPin> mpSclPin;
    std::shared_ptr<GpioPin> mpSdaPin;
    uint32_t                 mSlaveAddress = 0u;
    eI2cAddressMode          mAddressMode;
    eI2cTransferDirection    mTransferDirection;
    bool                     mIsInitialized = false;
};
