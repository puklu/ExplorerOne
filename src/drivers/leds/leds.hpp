#pragma once

#include "delay.hpp"
#include "GPIOpin.hpp"

void InterruptLed();
void BlinkLed(IO::GPIOpin &pin);
