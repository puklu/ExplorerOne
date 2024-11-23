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
            assertHandler(__FILE__, __LINE__); \
        }                    \
    } while (0)

void assertHandler(const char* file, int line);
