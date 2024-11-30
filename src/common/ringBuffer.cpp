#include "ringBuffer.hpp"


RingBuffer::RingBuffer(char *pBuf, uint8_t size):
    mpBuffer(pBuf),
    mSize(size),
    mHead(0),
    mTail(0),
    mCount(0)
{
}

eRingBufferStatus RingBuffer::put(const char *pData)
{
    if(isFull())
    {
        return eRingBufferStatus::RING_BUFFER_STATUS_FULL;
    }

    // Place the data where the head is
    *(mpBuffer + mHead) = *pData;

    // Increase head (taking care of circular nature)
    mHead = (mHead + 1) % mSize;

    mCount++;

    return eRingBufferStatus::RING_BUFFER_STATUS_SUCCESS;
}

eRingBufferStatus RingBuffer::take(char *const pTakeBuf)
{
    if(isEmpty())
    {
        return eRingBufferStatus::RING_BUFFER_STATUS_EMPTY;
    }

    // Populate the place pointed to by the pointer passed to the function
    *pTakeBuf = *(mpBuffer + mHead);

    // Increase tail (taking care of circular nature)
    mTail = (mTail + 1) % mSize;

    mCount--;

    return eRingBufferStatus::RING_BUFFER_STATUS_SUCCESS;
}

eRingBufferStatus RingBuffer::peek(char *const pPeekBuf) const
{
    if(isEmpty())
    {
        return eRingBufferStatus::RING_BUFFER_STATUS_EMPTY;
    }

    // Populate the place pointed to by the pointer passed to the function
    *pPeekBuf = *(mpBuffer + mHead);

    return eRingBufferStatus::RING_BUFFER_STATUS_SUCCESS;
}

bool RingBuffer::isFull() const
{
    return mCount == mSize;
}

bool RingBuffer::isEmpty() const
{
    return mCount == 0;
}
