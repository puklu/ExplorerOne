#include "common/defines.hpp"

extern "C" void SystemInit()
{
}

void JumpToApplication()
{
    using Function = void (*)();
    uint32_t appStackAddress =
        *reinterpret_cast<volatile uint32_t*>(APP_START_ADDRESS);
    Function appResetHandler =
        *reinterpret_cast<Function*>(APP_START_ADDRESS + 4);

    appResetHandler();
}

int main()
{
    JumpToApplication();

    return 0;
}