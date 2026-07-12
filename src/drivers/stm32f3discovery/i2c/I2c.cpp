#include "I2c.hpp"

#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "drivers/stm32f3discovery/common/RccImpl.hpp"
#include "drivers/stm32f3discovery/common/utils.cpp"
#include "vl53l1_platform.h"

I2c::I2c(I2cInitStruct const &i2cInitStruct)
    : mpSclPin(i2cInitStruct.scl_pin),
      mpSdaPin(i2cInitStruct.sda_pin),
      //   mSlaveAddress(i2cInitStruct.slave_address),
      mAddressMode(i2cInitStruct.AddressMode),
      mTransferDirection(eI2cTransferDirection::NOT_SET)
{
    SelectI2c();
}

eGeneralStatus I2c::SelectI2c()
{
    I2C_TypeDef *pSdaType =
        FindAlternatePeripherelTypeDef<I2C_TypeDef>(mpSdaPin);
    I2C_TypeDef *pSclType =
        FindAlternatePeripherelTypeDef<I2C_TypeDef>(mpSclPin);
    if (pSclType == pSdaType)
    {
        mpI2c = pSdaType;
    }
    else
    {
        ASSERT(false);
    }

    TRACE_LOG("I2c selected");

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::Init()
{
    ASSERT(!mIsInitialized);
    ASSERT(mpI2c != nullptr);

    EnableClock();

    DisableI2c();

    // Create a mask for interrupts
    uint32_t TxInterruptMask               = 1 << 1;
    uint32_t RxInterruptMask               = 1 << 2;
    uint32_t NackInterruptMask             = 1 << 4;
    uint32_t StopInterruptMask             = 1 << 5;
    uint32_t TransferCompleteInterruptMask = 1 << 6;
    uint32_t ErrorInterruptMask            = 1 << 7;

    uint32_t interruptsMask =
        TxInterruptMask | RxInterruptMask | NackInterruptMask |
        StopInterruptMask | TransferCompleteInterruptMask | ErrorInterruptMask;

    EnableInterrupts(interruptsMask);

    SetAddressMode(eI2cAddressMode::ADDR_7BIT);

    // SetSlaveAddress(mSlaveAddress);

    // SetTransferDirection(eI2cTransferDirection::MASTER_WRITE);

    SetTimingRegister();

    EnableI2c();

    mIsInitialized = true;

    TRACE_LOG("I2c initialised");

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::StartTransmission() const
{
    // It is cleared by hardware after the START condition followed by the
    // address sequence is sent, by an arbitration loss, by a timeout error
    // detection, or when PE = 0.
    SetRegisterBits(mpI2c->CR2, I2C_CR2_START_Msk);
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::StartTransmissionWithAutoend() const
{
    SetRegisterBits(mpI2c->CR2, I2C_CR2_START_Msk | I2C_CR2_AUTOEND_Msk);
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::StopTransmission() const
{
    SetRegisterBits(mpI2c->CR2, I2C_CR2_STOP_Msk);
    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::EnableAutoReload() const
{
    SetRegisterBits(mpI2c->CR2, I2C_CR2_RELOAD_Msk);

    return eGeneralStatus::SUCCESS;
}

bool I2c::IsInitialized() const
{
    return mIsInitialized;
}

eGeneralStatus I2c::EnableInterrupts(uint32_t interruptsMask) const
{
    SetRegisterBits(mpI2c->CR1, interruptsMask);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::EnableClock() const
{
    ASSERT(mpI2c != nullptr);

    if (mpI2c == I2C1)
    {
        RccImpl::GetInstance()->EnableApb1I2c1();
    }
    else if (mpI2c == I2C2)
    {
        RccImpl::GetInstance()->EnableApb1I2c2();
    }
    else
    {
        ASSERT(false);
    }

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::EnableI2c() const
{
    uint32_t pe_mask = 1 << I2C_CR1_PE_Pos;
    SetRegisterBits(mpI2c->CR1, pe_mask);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::DisableI2c() const
{
    uint32_t pe_mask = 1 << I2C_CR1_PE_Pos;
    ClearRegisterBits(mpI2c->CR1, pe_mask);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::SetSlaveAddress(uint8_t slave_address)
{
    ASSERT(mpI2c != nullptr);

    // make sure address is 7 bits
    ASSERT((slave_address & 0x7F) == slave_address);

    SetRegisterBits(mpI2c->CR2, slave_address);

    mSlaveAddress = slave_address;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::SetNumBytes(uint8_t num_bytes)
{
    ASSERT(mpI2c != nullptr);
    ASSERT(mIsInitialized);

    uint32_t mask = num_bytes << 16;
    ASSERT(num_bytes);

    SetRegisterBits(mpI2c->CR2, mask);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::SetAddressMode(eI2cAddressMode address_modes)
{
    ASSERT(mpI2c != nullptr);

    uint32_t address_mode_mask = static_cast<uint32_t>(address_modes)
                                 << I2C_CR2_ADD10_Pos;

    SetRegisterBits(mpI2c->CR2, address_mode_mask);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::SetTransferDirection(eI2cTransferDirection direction)
{
    ASSERT(mpI2c != nullptr);

    uint32_t direction_mask = static_cast<uint32_t>(direction)
                              << I2C_CR2_RD_WRN_Pos;

    SetRegisterBits(mpI2c->CR2, direction_mask);

    mTransferDirection = direction;

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::SetTimingRegister()
{
    uint32_t mask = 0x00300B29;  // 400KHz

    SetRegisterBits(mpI2c->TIMINGR, mask);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::WriteData(uint8_t slave_address, const uint8_t *pData,
                              const uint32_t len)
{
    ASSERT(mpI2c != nullptr);
    ASSERT(mIsInitialized);
    ASSERT(pData);
    ASSERT(len > 0);

    // TXE must be 1
    ASSERT((mpI2c->ISR & 0x00000001));

    SetSlaveAddress(slave_address);

    // make the controller be the transmitter
    SetTransferDirection(eI2cTransferDirection::MASTER_WRITE);

    SetNumBytes(len);

    // generate start condition (HW sends START + ADDRESS)
    StartTransmissionWithAutoend();

    for (uint32_t i = 0; i < len; i++)
    {
        BusyWaitForFlagToBeSet(mpI2c->ISR, I2C_ISR_TXE_Msk);
        // write to tx register of i2c
        mpI2c->TXDR = pData[i];
    }

    BusyWaitForFlagToBeSet(mpI2c->ISR, I2C_ISR_TC_Msk);

    // STOP is sent automatically after NUMBYTES are sent

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::WriteDataByte(uint8_t slave_address, const uint8_t *pData)
{
    return WriteData(slave_address, pData, 1);
}

eGeneralStatus I2c::ReadData(uint8_t slave_address, const uint8_t *pWrite_data,
                             const uint32_t write_len, uint8_t *pRead_buf,
                             uint32_t read_len)
{
    ASSERT(mpI2c != nullptr);
    ASSERT(mIsInitialized);
    ASSERT(pWrite_data);
    ASSERT(pRead_buf);
    ASSERT(write_len > 0);
    ASSERT(read_len > 0);

    // to be able to rad first, slave address and register address of the sensor
    // ======== is transmitted by master first
    SetSlaveAddress(slave_address);
    SetTransferDirection(eI2cTransferDirection::MASTER_WRITE);
    SetNumBytes(write_len);

    // tell the hardware more bytes are coming after this writing phase
    EnableAutoReload();

    // generate start condition (HW sends START + ADDRESS)
    StartTransmission();

    for (uint32_t i = 0; i < write_len; i++)
    {
        BusyWaitForFlagToBeSet(mpI2c->ISR, I2C_ISR_TXE_Msk);
        // write to tx register of i2c
        mpI2c->TXDR = pWrite_data[i];
    }
    // wait for transfer complete reload
    BusyWaitForFlagToBeSet(mpI2c->ISR, I2C_ISR_TCR_Msk);

    // start reading now
    SetSlaveAddress(slave_address);
    SetTransferDirection(eI2cTransferDirection::MASTER_READ);
    SetNumBytes(read_len);
    // generate start condition (HW sends START + ADDRESS)
    StartTransmissionWithAutoend();

    for (uint32_t i = 0; i < read_len; i++)
    {
        BusyWaitForFlagToBeSet(mpI2c->ISR, I2C_ISR_RXNE_Msk);
        // read from rx register of i2c
        pRead_buf[i] = mpI2c->RXDR;
    }
    // wait for transfer complete
    BusyWaitForFlagToBeSet(mpI2c->ISR, I2C_ISR_TC_Msk);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::ReadDataByte(uint8_t        slave_address,
                                 const uint8_t *pWrite_data,
                                 const uint32_t write_len, uint8_t *pRead_buf)
{
    return ReadData(slave_address, pWrite_data, write_len, pRead_buf, 1);
}