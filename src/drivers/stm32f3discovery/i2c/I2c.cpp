#include "I2c.hpp"

#include "common/assertHandler.hpp"
#include "drivers/stm32f3discovery/common/RccImpl.hpp"
#include "drivers/stm32f3discovery/common/utils.cpp"

I2c::I2c(I2cInitStruct const &i2cInitStruct)
    : mpSclPin(i2cInitStruct.pScaPin),
      mpSdaPin(i2cInitStruct.pSdaPin),
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
    ASSERT(mIsInitialized);
    RccImpl::GetInstance()->SelectSystemClock(
        eRccClockSource::RCC_CLOCK_SOURCE_PLL);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::EnableI2c() const
{
    SetRegisterBits(mpI2c->CR1, 1 << 0);

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::SetSlaveAddress(uint32_t slave_address)
{
    ASSERT(mpI2c != nullptr);
    ASSERT(mIsInitialized);

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

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::SetAddressMode(eI2cAddressMode address_modes)
{
    ASSERT(mpI2c != nullptr);
    ASSERT(mIsInitialized);

    SetRegisterBits(mpI2c->CR2, static_cast<uint32_t>(address_modes));

    return eGeneralStatus::SUCCESS;
}

eGeneralStatus I2c::SetTransferDirection(eI2cTransferDirection direction)
{
    ASSERT(mpI2c != nullptr);
    ASSERT(mIsInitialized);

    SetRegisterBits(mpI2c->CR2, static_cast<uint32_t>(direction));

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

    return 0;
}
