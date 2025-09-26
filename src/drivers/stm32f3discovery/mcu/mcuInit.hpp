#pragma once

extern "C" void SystemInit();

void PostSystemInit();

void InitializeConsolePrinting();

void InitializeDelaySystem();

void InitializeSystick();

bool IsSystemInitialized();
