#include "common/defines.hpp"

extern "C" void SystemInit()
{
}

void JumpToApplication()
{
    using app_entry_t = void (*)();
    [[maybe_unused]] uint32_t appStackAddress =
        *reinterpret_cast<volatile uint32_t*>(APP_START_ADDRESS);
    app_entry_t appResetVector =
        *reinterpret_cast<app_entry_t*>(APP_START_ADDRESS + 4);

    appResetVector();
}

int main()
{
    JumpToApplication();

    return 0;
}