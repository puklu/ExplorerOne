#include "ISRs.hpp"
#include "GPIOpin.hpp"
#include "common/assertHandler.hpp"
#include "drivers/leds/leds.hpp"
#include "drivers/io/pinBank.hpp"


void ServiceISR0_4(uint8_t pinNum){
    uint8_t portIdx = 0;
    IO::GPIOpin *pin = nullptr;

    for(portIdx = 0; portIdx < IO_PORT_COUNT; portIdx++){
        pin = activePins[portIdx][pinNum];
        if(pin != nullptr && pin->isInterruptPresent()) 
        {
            IO::InterruptCallback callback = pin->getInterruptCallback();
            if(callback != nullptr){
               callback();
            }
            pin->ClearInterrupt();
        }
    }
}


void ServiceISR5_15(uint8_t startPinNumber, uint8_t endPinNumber){
     uint8_t portIdx = 0;
     IO::GPIOpin *pin = nullptr;

     for(portIdx = 0; portIdx < IO_PORT_COUNT; portIdx++){
          for(uint8_t pinIdx = startPinNumber; pinIdx <= endPinNumber; pinIdx++){
               pin = activePins[portIdx][pinIdx];
               if(pin != nullptr && pin->isInterruptPresent())
               {
                    IO::InterruptCallback callback = pin->getInterruptCallback();
                    if(callback != nullptr){
                    callback();
               }
               pin->ClearInterrupt();
        }
     }
    }
}

extern "C" void EXTI0_IRQHandler(){
    ServiceISR0_4(0);
}

extern "C" void EXTI1_IRQHandler(){
    ServiceISR0_4(1);
}

extern "C" void EXTI2_TSC_IRQHandler(){
     ServiceISR0_4(2);
}

extern "C" void EXTI3_IRQHandler(){
     ServiceISR0_4(3);
}

extern "C" void EXTI4_IRQHandler(){
     ServiceISR0_4(4);
}

extern "C" void EXTI9_5_IRQHandler(){
     ServiceISR5_15(5, 9);
}

extern "C" void EXTI15_10_IRQHandler(){
    ServiceISR5_15(10, 15);
}
