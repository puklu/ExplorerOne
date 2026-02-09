#include "AlternateFunctionsTable.hpp"
#include "common/assertHandler.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/stm32f3discovery/io/GpioPin.hpp"

template <typename T>
T* FindAlternatePeripherelTypeDef(std::shared_ptr<GpioPin> pPin)
{
    ASSERT(activePins[pPin->GetPortNumber()][pPin->GetPinNumber()] != nullptr);

    // Compile-time check - fails if T is not a supported peripheral type
    static_assert(std::is_same<T, USART_TypeDef>::value ||
                      std::is_same<T, TIM_TypeDef>::value ||
                      std::is_same<T, I2C_TypeDef>::value,
                  "T must be a peripheral type (TIM_TypeDef, USART_TypeDef, "
                  "I2C_TypeDef)");

    uint8_t                portNumber          = pPin->GetPortNumber();
    uint8_t                pinNumber           = pPin->GetPinNumber();
    IO::eAlternateFunction alternateFunction   = pPin->GetAlternateFunction();
    const void*            pSelectedPeripherel = nullptr;

    switch (portNumber)
    {
        case 0:
            pSelectedPeripherel =
                aAltFunctionsAdressesPortA[pinNumber][static_cast<uint8_t>(
                    alternateFunction)];
            break;

        case 1:
            pSelectedPeripherel =
                aAltFunctionsAdressesPortB[pinNumber][static_cast<uint8_t>(
                    alternateFunction)];
            break;

        case 2:
            pSelectedPeripherel =
                aAltFunctionsAdressesPortC[pinNumber][static_cast<uint8_t>(
                    alternateFunction)];
            break;

        case 3:
            pSelectedPeripherel =
                aAltFunctionsAdressesPortD[pinNumber][static_cast<uint8_t>(
                    alternateFunction)];
            break;

        case 4:
            pSelectedPeripherel =
                aAltFunctionsAdressesPortE[pinNumber][static_cast<uint8_t>(
                    alternateFunction)];
            break;

        case 5:
            pSelectedPeripherel =
                aAltFunctionsAdressesPortF[pinNumber][static_cast<uint8_t>(
                    alternateFunction)];
            break;

        default:
            ASSERT(0);
            break;
    }

    return const_cast<T*>(reinterpret_cast<const T*>(pSelectedPeripherel));
}

void SetRegisterBits(volatile uint32_t& rRegister, const uint32_t& rMask)
{
    rRegister |= rMask;
}

void ClearRegisterBits(volatile uint32_t& rRegister, const uint32_t& rMask)
{
    rRegister &= ~rMask;
}