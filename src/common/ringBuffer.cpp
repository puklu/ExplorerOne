#include "ringBuffer.hpp"


RingBuffer::RingBuffer(size_t size)
    : mBuffer(size), mHead(0), mTail(0), mCount(0)
{
}

eRingBufferStatus RingBuffer::put(char data)
{
    if(isFull())
    {
        return eRingBufferStatus::RING_BUFFER_STATUS_FULL;
    }

    // Place the data where the head is
    mBuffer[mHead] = data;

    // Increase head (taking care of circular nature)
    mHead = (mHead + 1) % mBuffer.size();

    mCount++;

    return eRingBufferStatus::RING_BUFFER_STATUS_SUCCESS;
}

eRingBufferStatus RingBuffer::take(char &data)
{
    if(isEmpty())
    {
        return eRingBufferStatus::RING_BUFFER_STATUS_EMPTY;
    }

    // Populate the place pointed to by the pointer passed to the function
    data = mBuffer[mTail];

    // Increase tail (taking care of circular nature)
    mTail = (mTail + 1) % mBuffer.size();

    mCount--;

    return eRingBufferStatus::RING_BUFFER_STATUS_SUCCESS;
}

eRingBufferStatus RingBuffer::peek(char &data) const
{
    if(isEmpty())
    {
        return eRingBufferStatus::RING_BUFFER_STATUS_EMPTY;
    }

    // Populate the place pointed to by the pointer passed to the function
    data = mBuffer[mTail];

    return eRingBufferStatus::RING_BUFFER_STATUS_SUCCESS;
}

bool RingBuffer::isFull() const
{
    return mCount == mBuffer.size();
}

bool RingBuffer::isEmpty() const
{
    return mCount == 0;
}

size_t RingBuffer::size() const
{
    return mCount; 
}

size_t RingBuffer::capacity() const
{
    return mBuffer.size();
}
