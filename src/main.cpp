// src/main.cpp
#include <cstdint>  // for uint8_t

#include "common/assertHandler.hpp"
#include "common/delay.hpp"
#include "common/Trace.hpp"
#include "drivers/io/GpioPin.hpp"
#include "drivers/leds/leds.hpp"
#include "drivers/interfaces/pinBank.hpp"
#include "drivers/interfaces/PinFactory.hpp"
#include "drivers/timers/BasicTimer.hpp"
#include "drivers/timers/ITimer.hpp"
#include "drivers/usart/UsartPin.hpp"
#include "mcuInit.hpp"


int main()
{

    SystemInit();
    
    ASSERT(isSystemInitialized);

    TimerInitStruct timer_init_struct = {};
    // timer_init_struct.prescaler_value = 7999;
    // timer_init_struct.auto_reload_register_value = 2000;
    timer_init_struct.cb = InterruptLed;

    // ITimer *basictimer = new BasicTimer(timer_init_struct);
    BasicTimer basictimer(timer_init_struct);
    basictimer.SetPeriodAndCount(1, 3000);
    basictimer.Start();
    // basictimer.EnableInterrupt();
           
    while (1)   
    {
    }

}   
