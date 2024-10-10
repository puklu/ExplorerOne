#include "drivers/io/ServiceISRs.hpp"
#include "drivers/io/pinBank.hpp"
#include "common/defines.hpp"
#include "drivers/io/GPIOpin.hpp"


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