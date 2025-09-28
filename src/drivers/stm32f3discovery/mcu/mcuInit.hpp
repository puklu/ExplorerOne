#pragma once

extern "C" void SystemInit();

void SetupRcc();

void PostSystemInit();

void InitializeConsolePrinting();

void InitializeDelaySystem();

void InitializeSystick();

bool IsSystemInitialized();
