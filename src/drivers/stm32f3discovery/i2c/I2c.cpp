#include "I2c.hpp"

#include "common/Trace.hpp"
#include "common/assertHandler.hpp"
#include "drivers/stm32f3discovery/common/RccImpl.hpp"
#include "drivers/stm32f3discovery/common/utils.cpp"

I2c::I2c(I2cInitStruct const &i2cInitStruct)
    : mpSclPin(i2cInitStruct.scl_pin),
      mpSdaPin(i2cInitStruct.sda_pin),
      mSlaveAddress(i2cInitStruct.slave_address),
      mAddressMode(i2cInitStruct.AddressMode),
      mTransferDirection(i2cInitStruct.TransferDirection)
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

    SetTransferDirection(mTransferDirection);

    SetAddressMode(eI2cAddressMode::ADDR_7BIT);

    SetSlaveAddress(mSlaveAddress);

    SetTimingRegister();

    EnableI2c();

    mIsInitialized = true;

    TRACE_LOG("I2c initialised");

    return eGeneralStatus::SUCCESS;
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

eGeneralStatus I2c::SetSlaveAddress(uint32_t slave_address)
{
    ASSERT(mpI2c != nullptr);

    // make sure address is 7 bits
    ASSERT((slave_address & 0x7F) == slave_address);

    SetRegisterBits(mpI2c->CR2, slave_address);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::SetNumBytes(uint32_t num_bytes)
{
    ASSERT(mpI2c != nullptr);
    ASSERT(mIsInitialized);

    ASSERT(num_bytes);

    ASSERT(false);

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

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::SetTimingRegister()
{
    uint32_t mask = 0xB0420F13;  // from example for fI2CCLK = 48MHZ AND 100KHz
                                 // standard mode
    SetRegisterBits(mpI2c->TIMINGR, mask);

    return eGeneralStatus::SUCCESS;
}

uint32_t I2c::ReadData()
{
    ASSERT(mpI2c != nullptr);
    ASSERT(mIsInitialized);

    uint32_t data = mpI2c->RXDR;

    ASSERT(false);

    return data;
}
