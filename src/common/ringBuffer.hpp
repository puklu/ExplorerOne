#pragma once
#include <cstdint>  // for unit8_t
#include "statusCodes.hpp"

class RingBuffer
{
    public:
        RingBuffer(uint8_t *pBuf, uint8_t size);
        eRingBufferStatus put(const uint8_t *pData);
        eRingBufferStatus take(uint8_t *const pTakeBuf);
        eRingBufferStatus peek(uint8_t *const pPeekBuf) const;

    private:
        bool isFull() const;
        bool isEmpty() const;
        uint8_t *mpBuffer;  // each element of the buffer is of 8 bits
        uint8_t  mSize;
        uint8_t  mHead;
        uint8_t  mTail;
        uint8_t  mCount;
};
