/**
 * @file statusCodes.hpp
 * @brief Defines status codes.
 */

#pragma once
#include <cstdint>  // for uint8_t

/**
 * @enum eRingBufferStatus
 * @brief Enumerates possible status codes for RingBuffer operations.
 *
 * The eRingBufferStatus enum represents the status of operations performed
 * by the RingBuffer class, such as adding, removing, or peeking at data.
 */
enum class eRingBufferStatus : uint8_t
{
    RING_BUFFER_STATUS_SUCCESS,
    RING_BUFFER_STATUS_EMPTY,
    RING_BUFFER_STATUS_FULL,
};
