#pragma once
#include <cstdint>  // for uintx_t

enum class eRingBufferStatus : uintptr_t
{
    RING_BUFFER_STATUS_SUCCESS,
    RING_BUFFER_STATUS_EMPTY,
    RING_BUFFER_STATUS_FULL,
};
