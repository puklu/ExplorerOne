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
#include <vector>
#include "statusCodes.hpp"

class ICriticalSectionGuard;

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
         * @param size The size of the buffer in number of elements.
         */
        explicit RingBuffer(size_t size, ICriticalSectionGuard &guard);

        /**
         * @brief Puts data into the buffer.
         * 
         * @param data The data to be added to the buffer (passed by copy here).
         * @return The status of the operation, as an eRingBufferStatus enum.
         *         It can indicate success or an error if the buffer is full.
         */
        eRingBufferStatus put(char data);

        /**
         * @brief Takes data from the buffer.
         * 
         * @param data A reference to the buffer where the taken data will be stored.
         * @return The status of the operation, as an eRingBufferStatus enum.
         *         It can indicate success or an error if the buffer is empty.
         */
        eRingBufferStatus take(char &data);

        /**
         * @brief Peeks at the next data element in the buffer without removing it.
         * 
         * @param data A reference to the buffer where the peeked data will be stored.
         * @return The status of the operation, as an eRingBufferStatus enum.
         *         It can indicate success or an error if the buffer is empty.
         */
        eRingBufferStatus peek(char &data);

        /**
         * @brief Returns the number of elements in the buffer.
         * 
         * @return Number of elements in the buffer.
         */
        size_t size() const;

        /**
         * @brief Returns the capacity of the buffer.
         * 
         * @return Capacity of the buffer.
         */
        size_t capacity() const;

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


        std::vector<char>              mBuffer;  /// A buffer vector for storing data.
        uint8_t                        mHead;     /// The index of the head (next insertion point).
        uint8_t                        mTail;     /// The index of the tail (next extraction point).
        uint8_t                        mCount;    /// The number of elements currently in the buffer.
        ICriticalSectionGuard         &mCriticalSectionGuard;
};
