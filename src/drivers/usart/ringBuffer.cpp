#include "ringBuffer.hpp"

RingBuffer::RingBuffer(uint8_t *pBuf, uint8_t size):
    mpBuffer(pBuf),
    mSize(size),
    mHead(0),
    mTail(0),
    mCount(0)
{
}

USART::eRingBufferStatus RingBuffer::put(const uint8_t *pData)
{
    if(isFull())
    {
        return USART::eRingBufferStatus::RING_BUFFER_STATUS_FULL;
    }

    // Place the data where the head is
    *(mpBuffer + mHead) = *pData;

    // Increase head (taking care of circular nature)
    mHead = (mHead + 1) % mSize;

    mCount++;

    return USART::eRingBufferStatus::RING_BUFFER_STATUS_SUCCESS;
}

USART::eRingBufferStatus RingBuffer::take(uint8_t *const pTakeBuf)
{
    if(isEmpty())
    {
        return USART::eRingBufferStatus::RING_BUFFER_STATUS_EMPTY;
    }

    // Populate the place pointed to by the pointer passed to the function
    *pTakeBuf = *(mpBuffer + mHead);

    // Increase tail (taking care of circular nature)
    mTail = (mTail + 1) % mSize;

    mCount--;

    return USART::eRingBufferStatus::RING_BUFFER_STATUS_SUCCESS;
}

USART::eRingBufferStatus RingBuffer::peek(uint8_t *const pPeekBuf)
{
    if(isEmpty())
    {
        return USART::eRingBufferStatus::RING_BUFFER_STATUS_EMPTY;
    }

    // Populate the place pointed to by the pointer passed to the function
    *pPeekBuf = *(mpBuffer + mHead);

    return USART::eRingBufferStatus::RING_BUFFER_STATUS_SUCCESS;
}

bool RingBuffer::isFull() const
{
    return mCount == mSize;
}

bool RingBuffer::isEmpty() const
{
    return mCount == 0;
}

