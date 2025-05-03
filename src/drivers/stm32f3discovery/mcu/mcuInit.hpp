#pragma once

extern "C" void SystemInit();

void PostSystemInit();


void InitializeConsolePrinting();

void InitializeDelaySystem();

bool IsSystemInitialized();
