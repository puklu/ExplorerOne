#include "I2c.hpp"

#include "common/assertHandler.hpp"
#include "common/utils.cpp"

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
}

eGeneralStatus I2c::Init()
{
}

eGeneralStatus I2c::SetSlaveAddress(uint32_t slave_address)
{
}

eGeneralStatus I2c::SetNumBytes(uint32_t num_bytes)
{
}

eGeneralStatus I2c::SetAddressMode(eI2cAddressMode num_bytes)
{
}

eGeneralStatus I2c::SetTransferDirection(eI2cTransferDirection direction)
{
}

uint32_t I2c::ReadData()
{
}
