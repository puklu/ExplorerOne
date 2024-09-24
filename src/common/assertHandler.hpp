/**
 * @file assertHandler.hpp
 */

#pragma once

#define SOFTWARE_BREAKPOINT __asm__("BKPT");

#define ASSERT(expression)   \
    do                       \
    {                        \
        if (!(expression))   \
        {                    \
            assertHandler(); \
        }                    \
    } while (0)

void assertHandler();
