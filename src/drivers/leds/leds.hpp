#pragma once

#include "common/delay.hpp"
#include "drivers/io/GPIOpin.hpp"

void InterruptLed();
void BlinkLed(IO::GPIOpin &pin);
