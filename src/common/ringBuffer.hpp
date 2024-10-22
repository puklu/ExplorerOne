/**
 * @file ringBuffer.hpp
 * @brief Declaration of the RingBuffer class for managing a circular buffer.
 *
 * This file contains the declaration of the RingBuffer class, which provides
 * functionality for a circular buffer data structure. It supports adding (putting)
 * data, taking data out, and peeking at data within the buffer.
 */

#pragma once
#include <cstdint>  // for unit8_t
#include "statusCodes.hpp"

/**
 * @class RingBuffer
 * @brief A class representing a circular buffer for managing data.
 *
 * The RingBuffer class implements a circular buffer data structure that allows
 * for adding data (putting), removing data (taking), and peeking at the data
 * within the buffer. It supports checking if the buffer is full or empty.
 */
class RingBuffer
{
    public:
        /**
         * @brief Constructs a RingBuffer with a given buffer and size.
         * 
         * @param pBuf A pointer to the buffer where data will be stored.
         * @param size The size of the buffer in number of elements.
         */
        RingBuffer(uint8_t *pBuf, uint8_t size);

        /**
         * @brief Puts data into the buffer.
         * 
         * @param pData A pointer to the data to be added to the buffer.
         * @return The status of the operation, as an eRingBufferStatus enum.
         *         It can indicate success or an error if the buffer is full.
         */
        eRingBufferStatus put(const uint8_t *pData);

        /**
         * @brief Takes data from the buffer.
         * 
         * @param pTakeBuf A pointer to the buffer where the taken data will be stored.
         * @return The status of the operation, as an eRingBufferStatus enum.
         *         It can indicate success or an error if the buffer is empty.
         */
        eRingBufferStatus take(uint8_t *const pTakeBuf);

        /**
         * @brief Peeks at the next data element in the buffer without removing it.
         * 
         * @param pPeekBuf A pointer to the buffer where the peeked data will be stored.
         * @return The status of the operation, as an eRingBufferStatus enum.
         *         It can indicate success or an error if the buffer is empty.
         */
        eRingBufferStatus peek(uint8_t *const pPeekBuf) const;

    private:
        /**
         * @brief Checks if the buffer is full.
         * 
         * @return True if the buffer is full, false otherwise.
         */
        bool isFull() const;

        /**
         * @brief Checks if the buffer is empty.
         * 
         * @return True if the buffer is empty, false otherwise.
         */
        bool isEmpty() const;
        uint8_t *mpBuffer;  /// A pointer to the buffer array for storing data.
        uint8_t  mSize;     /// The size of the buffer (number of elements).
        uint8_t  mHead;     /// The index of the head (next insertion point).
        uint8_t  mTail;     /// The index of the tail (next extraction point).
        uint8_t  mCount;    /// The number of elements currently in the buffer.
};
