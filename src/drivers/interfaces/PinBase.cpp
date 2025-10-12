#include "PinBase.hpp"

#include "common/assertHandler.hpp"
#include "drivers/stm32f3discovery/common/RccImpl.hpp"  // TODO: Get rid of this. shouldnt need platfrom specific include?
#include "drivers/stm32f3discovery/common/registerArrays.hpp"  // TODO: Get rid of this. shouldnt need platfrom specific include?
#include "pinBank.hpp"

PinBase::PinBase(IO::ePin pin_name) : mPinName(pin_name)
{
    SetPortNumber();
    SetPinNumber();
}

void PinBase::Init()
{
    ASSERT(!mIsInitialized);
    EnableClock();
}

void PinBase::SetPortNumber()
{
    mPortNumber = (mPinName & IO_PORT_MASK) >> IO_PORT_OFFSET;
    ASSERT(mPortNumber < IO_PORT_COUNT);
    mpPort = IO::aPorts[mPortNumber];
}

void PinBase::SetPinNumber()
{
    mPinNumber = mPinName & IO_PIN_MASK;
    ASSERT(mPinNumber < IO_PIN_COUNT_PER_PORT);
}

uint8_t PinBase::GetPortNumber()
{
    ASSERT(mPortNumber != UINT8_MAX);
    return mPortNumber;
}

GPIO_TypeDef* PinBase::GetPort()
{
    return mpPort;
}

uint8_t PinBase::GetPinNumber()
{
    ASSERT(mPinNumber != UINT8_MAX);
    return mPinNumber;
}

void PinBase::EnableClock() const
{
    // Enable the clock for the port
    switch (mPortNumber)
    {
        case 0:
            RccImpl::GetInstance()->EnableAhbGpioA();
            break;

        case 1:
            RccImpl::GetInstance()->EnableAhbGpioB();
            break;

        case 2:
            RccImpl::GetInstance()->EnableAhbGpioC();
            break;

        case 3:
            RccImpl::GetInstance()->EnableAhbGpioD();
            break;

        case 4:
            RccImpl::GetInstance()->EnableAhbGpioE();
            break;

        case 5:
            RccImpl::GetInstance()->EnableAhbGpioF();
            break;

        default:
            ASSERT(false);
            break;
    }
}

PinBase::~PinBase()
{
    activePins[mPortNumber][mPinNumber] = nullptr;
}

void PinBase::DeletePin(PinBase* pin)
{
    if (pin != nullptr)
    {
        delete pin;
    }
}
