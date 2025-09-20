#pragma once

#include <memory>
#include "common/PinDefinitions.hpp"
#include "drivers/interfaces/ITimer.hpp"


class TimerFactory
{
    public:
        static ITimer* CreateBasicTimer();
};