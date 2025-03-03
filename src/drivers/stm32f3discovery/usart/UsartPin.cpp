#include "printf_redirect.h"
#include "common/assertHandler.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/stm32f3discovery/common/AlternateFunctionsTable.hpp"
#include "drivers/stm32f3discovery/common/registerArrays.hpp"
#include "drivers/stm32f3discovery/io/GpioPin.hpp"
#include "UsartPin.hpp"

UsartPin* activeUsartPin = nullptr;  // Global pointer for active UsartPin

std::shared_ptr<UsartPin> UsartPin::Create(const UsartPinInitStruct &pin_init_struct)
{
    return std::shared_ptr<UsartPin>(new UsartPin(pin_init_struct));
}

UsartPin::UsartPin(UsartPinInitStruct const &pin_init_struct):
    PinBase(pin_init_struct.pin_name),
    mAlternateFunction(pin_init_struct.alternate_function),
    mWordLength(pin_init_struct.word_length),
    mOversamplingMode(pin_init_struct.oversampling_mode),
    mParityControl(pin_init_struct.parity_control),
    mParitySelection(pin_init_struct.parity_selection),
    mTxEnable(pin_init_struct.tx_enable),
    mRxEnable(pin_init_struct.rx_enable),
    mUsartEnable(pin_init_struct.usart_enable),
    mBaudRate(pin_init_struct.baud_rate),
    mInterruptCallbackFunction(pin_init_struct.cb),
    mCriticalSectionGuard(), // initialize the critical section guard
    mpRingBuffer(std::make_shared<RingBuffer>(RING_BUFFER_SIZE, mCriticalSectionGuard))
{
    SetMode();
    SetAlternateFunction();
    SelectUsart();
    EnableClock();
    EnableInterrupts();
    SetBaudRate();
    SetControlRegister();

    mIsInitialized = true;
}

void UsartPin::SetMode()
{
    // Set mode as Alternate function
    mpPort->MODER |= IO::aModeRegisterBits[2 * mPinNumber + 1];
    mpPort->MODER &= ~IO::aModeRegisterBits[2 * mPinNumber];
    mMode = IO::eMode::IO_MODE_ALT_FUNCTION;
}

void UsartPin::ReceiveData(uintptr_t *const data_buffer) const
{
    // TODO: Something about various data sizes
    ASSERT(data_buffer != nullptr);

    // Wait until Read data register is not empty
    while(!(mpUsart->ISR & USART_ISR_RXNE));

    *data_buffer = mpUsart->RDR;
}

void UsartPin::TransmitData(char data)
{
    mTxData = data;

    if(mpRingBuffer->put(data) != eRingBufferStatus::RING_BUFFER_STATUS_SUCCESS)
    {
        ASSERT(0);
    }

    EnableTxRegisterEmptyInterrupt();
}

void UsartPin::TransmitDataPolling(char data)
{
    mTxData = data;

    while(!(mpUsart->ISR & USART_ISR_TXE));
    
    mpUsart->TDR = mTxData;
}

void UsartPin::EnableClock() const
{

    ASSERT(mpUsart != nullptr);

    if(mpUsart == USART1){
        mpRCC->APB2ENR |= IO::aUsartEnableRegistersMasks[0]; // USART1 (1<<14);
    }

    else if (mpUsart == USART2)
    {
       mpRCC->APB1ENR |= IO::aUsartEnableRegistersMasks[1]; // (1<<17);
    }

    else if (mpUsart == USART3)
    {
        mpRCC->APB1ENR |= IO::aUsartEnableRegistersMasks[2]; // (1<<18);
    }

    else if (mpUsart == UART4)
    {
        mpRCC->APB1ENR |= IO::aUsartEnableRegistersMasks[3]; // (1<<19);
    }

    else if (mpUsart == UART5)
    {
        mpRCC->APB1ENR |= IO::aUsartEnableRegistersMasks[4]; // (1<<20);
    } 
}


void UsartPin::SetAlternateFunction()
{
    ASSERT(mMode == IO::eMode::IO_MODE_ALT_FUNCTION);



    if(mPinNumber<8)
    {
        mpPort->AFR[0] &= ~(0xF << (mPinNumber * 4));
        mpPort->AFR[0] |= (static_cast<uint8_t>(mAlternateFunction) << (mPinNumber * 4));
    }
    else
    {
        uint8_t pin_number = mPinNumber; // Making a copy to use here
        pin_number -= 8;

        mpPort->AFR[1] &= ~(0xF << (pin_number * 4));
        mpPort->AFR[1] |= (static_cast<uint8_t>(mAlternateFunction) << (pin_number * 4));
    }

    // TODO: Replace the above code by something like below to use the set method
    // of the pin.
    // GpioPin *pin = static_cast<GpioPin*>(activePins[mPortNumber][mPinNumber]);

    // pin->SetAlternateFunction(mAlternateFunction);
}



void UsartPin::SelectUsart()
{
    const void *selectedUsart;  

    switch (mPortNumber)
    {
    case 0:
        selectedUsart = aAltFunctionsAdressesPortA[mPinNumber][static_cast<uint8_t>(mAlternateFunction)];
        break;
    
    case 1:
        selectedUsart = aAltFunctionsAdressesPortB[mPinNumber][static_cast<uint8_t>(mAlternateFunction)];
        break;
    
    case 2:
        selectedUsart = aAltFunctionsAdressesPortC[mPinNumber][static_cast<uint8_t>(mAlternateFunction)];
        break;
    
    case 3:
        selectedUsart = aAltFunctionsAdressesPortD[mPinNumber][static_cast<uint8_t>(mAlternateFunction)];
        break;
    
    case 4:
        selectedUsart = aAltFunctionsAdressesPortE[mPinNumber][static_cast<uint8_t>(mAlternateFunction)];
        break;
    
    case 5:
        selectedUsart = aAltFunctionsAdressesPortF[mPinNumber][static_cast<uint8_t>(mAlternateFunction)];
        break;
    
    default:
        ASSERT(0);
        break;
    }

    if(selectedUsart != nullptr)
    {
            mpUsart = const_cast<USART_TypeDef*>(reinterpret_cast<const USART_TypeDef*>(selectedUsart));
    }
    else
    {
        ASSERT(0);
    }
}

void UsartPin::SetControlRegister()
{
    ASSERT(!(mpUsart->CR1 & 0x1));

    // Set word length
    if(mWordLength == USART::eWordLength::USART_WORD_LEN_7BITS)
    {
        mpUsart->CR1 |= (1<<28); 
        mpUsart->CR1 &= ~(1<<12);
    }
    else if (mWordLength == USART::eWordLength::USART_WORD_LEN_8BITS)
    {
        mpUsart->CR1 &= ~(1<<28); 
        mpUsart->CR1 &= ~(1<<12);
    }
    else if (mWordLength == USART::eWordLength::USART_WORD_LEN_9BITS)
    {
        mpUsart->CR1 &= ~(1<<28); 
        mpUsart->CR1 |= (1<<12);
    } 

    // Set oversampling mode
    if(mOversamplingMode == USART::eOverSamplingMode::USART_OVERSAMPLING_BY_16)
    {
        mpUsart->CR1 &= ~(1<<15);
    }
    else if (mOversamplingMode == USART::eOverSamplingMode::USART_OVERSAMPLING_BY_8)
    {
        mpUsart->CR1 |= (1<<15);
    }  

    // Enable party control
    if(mParityControl == USART::eParityControlEnable::USART_PARITY_CONTROL_ENABLED)
    {
        mpUsart->CR1 |= (1<<10);
    }
    else if (mParityControl == USART::eParityControlEnable::USART_PARITY_CONTROL_DISABLED)
    {
        mpUsart->CR1 &= ~(1<<10);
    }

    // Set parity selection
    if(mpUsart->CR1 & (1<<10)) // Assure that Parity control is enabled
    {
        if(mParitySelection == USART::eParitySelection::USART_PARITY_EVEN)
        {
            mpUsart->CR1 &= ~(1<<9);
        }
        else if (mParitySelection == USART::eParitySelection::USART_PARITY_ODD)
        {
            mpUsart->CR1 |= (1<<9);
        }
    }
  
    // Enable transmitter 
    if(mTxEnable == USART::eTxEnable::USART_TX_ENABLE)
    {
        mpUsart->CR1 |= (1<<3);
    }
    else if (mTxEnable == USART::eTxEnable::USART_TX_DISABLE)
    {
        mpUsart->CR1 &= ~(1<<3);
    }  

    // Enable receiver   
    if(mRxEnable == USART::eRxEnable::USART_RX_ENABLE)
    {
        mpUsart->CR1 |= (1<<2);
    }
    else if (mRxEnable == USART::eRxEnable::USART_RX_DISABLE)
    {
        mpUsart->CR1 &= ~(1<<2);
    }  


    // Enable/disable USART
    if(mUsartEnable == USART::eUsartEnable::USART_ENABLE)
    {
        mpUsart->CR1 |= (1<<0);
    }
    else if (mUsartEnable == USART::eUsartEnable::USART_DISABLE)
    {
        mpUsart->CR1 &= ~(1<<0);
    }
}

void UsartPin::SetBaudRate()
{
    ASSERT(!(mpUsart->CR1 & 0x1));

    // TODO: This should change with the selected clock. Hardcoded for now.
    uintptr_t clock = SYS_CLK;
    uintptr_t desiredBaudRate = 0;

    switch (mBaudRate)
    {
    case USART::eBaudRate::USART_BAUD_RATE_9600:
        desiredBaudRate = 9600;
        break;

    case USART::eBaudRate::USART_BAUD_RATE_57600:
        desiredBaudRate = 57600;
        break;

    case USART::eBaudRate::USART_BAUD_RATE_38400:
        desiredBaudRate = 38400;
        break;

    case USART::eBaudRate::USART_BAUD_RATE_19200:
        desiredBaudRate = 19200;
        break;

    case USART::eBaudRate::USART_BAUD_RATE_115200:
        desiredBaudRate = 115200;
        break;

    default:
        ASSERT(0);
        return;
    }

    [[maybe_unused]] uintptr_t UsartDiv = 0;

    if (mOversamplingMode == USART::eOverSamplingMode::USART_OVERSAMPLING_BY_16)
    {
        UsartDiv = clock/desiredBaudRate;
        mpUsart->BRR |= UsartDiv;
    }
    else if (mOversamplingMode == USART::eOverSamplingMode::USART_OVERSAMPLING_BY_8)
    {
        UsartDiv = (2 * clock)/desiredBaudRate;
        mpUsart->BRR  = (UsartDiv & USART_BRR_DIV_FRACTION) >> 1;
        mpUsart->BRR |= (UsartDiv & USART_BRR_DIV_MANTISSA);
    }
    else
    {
        ASSERT(0);
        return;
    }
}

// void UsartPin::SetGuardTimeAndPrescaler(){}
// void UsartPin::SetReceiverTimeoutRegister(){}
// void UsartPin::SetRequestRegister(){}

uint32_t UsartPin::GetInterruptAndStatusRegister(USART::eIsrFlags const &flag) const
{
    ASSERT(flag != USART::eIsrFlags::RESERVED);
    return (mpUsart->ISR & (1<< static_cast<uint8_t>(flag)));
}

void UsartPin::SetInterruptClearFlagRegister(USART::eIcrFlags const &flag)
{
    if(flag != USART::eIcrFlags::RESERVED5 && flag != USART::eIcrFlags::RESERVED7 &&
       flag != USART::eIcrFlags::RESERVED10 && flag != USART::eIcrFlags::RESERVED13 &&
       flag != USART::eIcrFlags::RESERVED14 && flag != USART::eIcrFlags::RESERVED15 &&
       flag != USART::eIcrFlags::RESERVED16 && flag != USART::eIcrFlags::RESERVED17 &&
       flag != USART::eIcrFlags::RESERVED18)
    {
        mpUsart->ICR  |= 1 << static_cast<uint8_t>(flag);
    }

    else
    {
        ASSERT(0);
    }
}

void UsartPin::EnableTxRegisterEmptyInterrupt()
{
    if(!(mpUsart->CR1 & USART_CR1_TXEIE))
    {
        mpUsart->CR1 |=  USART_CR1_TXEIE;
    }    
}

void UsartPin::DisableTxRegisterEmptyInterrupt()
{
    if((mpUsart->CR1 & USART_CR1_TXEIE))
    {
        mpUsart->CR1 &= ~USART_CR1_TXEIE;
    }
}

void UsartPin::EnableTxCompleteInterrupt()
{
    mpUsart->CR1 |= USART_CR1_TCIE;    
}

void UsartPin::DisableTxCompleteInterrupt()
{
    mpUsart->CR1 &= ~USART_CR1_TCIE;
}

void UsartPin::EnableRxNotEmptyInterrupt()
{
    mpUsart->CR1 |= USART_CR1_RXNEIE;
}

void UsartPin::DisableRxNotEmptyInterrupt()
{
    mpUsart->CR1 &= ~USART_CR1_RXNEIE;
}

void UsartPin::EnableInterrupts()
{
    GetIRQn();
    EnableNVIC();
}

void UsartPin::EnableNVIC()
{
    NVIC_EnableIRQ(mIrqNumber);
    NVIC_SetPriority(mIrqNumber, PRIORITY_USART);  ///TODO: fix this priority
}

void UsartPin::GetIRQn()
{
    if(mpUsart == USART1)   
    {
        mIrqNumber = USART1_IRQn;
    }
    else if(mpUsart == USART2)
    {
        mIrqNumber = USART2_IRQn;
    }
    else if(mpUsart == USART3)
    {
        mIrqNumber = USART3_IRQn;
    }
    else if(mpUsart == UART4)
    {
        mIrqNumber = UART4_IRQn;
    }
    else if(mpUsart == UART5)
    {
        mIrqNumber = UART5_IRQn;
    }
    else
    {
        ASSERT(0);
        mIrqNumber = NonMaskableInt_IRQn;  // TODO: Fix me. return appopriate code or handle in some other way
    }
}

USART_TypeDef* UsartPin::GetSelectedUsart()
{
    return mpUsart;
}

char UsartPin::GetDataToTransmit()
{
    return mTxData;
}

std::shared_ptr<RingBuffer> UsartPin::GetRingBuffer()
{
    return mpRingBuffer;
}

void UsartPin::SetInterruptCallback(InterruptCallback cb)
{
    ASSERT(cb != nullptr);
    mInterruptCallbackFunction = cb;
}

InterruptCallback UsartPin::GetInterruptCallback()
{
    return mInterruptCallbackFunction;
}

void ActivateTraceForAssert()
{
    // Disable all interupts in case an assert occurs and simply use polling
    activePrintUsartPin->DisableRxNotEmptyInterrupt();
    activePrintUsartPin->DisableTxCompleteInterrupt();
    activePrintUsartPin->DisableTxRegisterEmptyInterrupt();

    // Change the _putchar function for the pin
    printf_set_putchar(UsartPutcharPolling);
}


void UsartPutchar(char character) 
{
    if (activePrintUsartPin)
    {
        activePrintUsartPin->TransmitData(character);
    }
}

void UsartPutcharPolling(char character)
{
    if (activePrintUsartPin)
    {
        activePrintUsartPin->TransmitDataPolling(character);
    }
}
