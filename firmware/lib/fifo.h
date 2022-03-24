#pragma once

#include <stdint.h>

constexpr bool IsEven(uint32_t N)
{
    return (N&1) == 0;
}

constexpr bool IsPowerOf2(uint32_t N)
{
    return N == 0 ? false
         : N == 1 ? true
         : IsEven(N) && IsPowerOf2(N/2);
};

template <uint8_t Size, typename VT = uint8_t>
class GenericFifo
{
    static_assert(IsPowerOf2(Size), "Size should be a power of 2");

public:

    using ValueType = VT;
    static constexpr uint8_t BufferSize = Size;


    void init() volatile
    {
        head = tail = 0;
    }

    /// Check if the queue is empty.
    bool empty() const volatile
    {
        return head == tail;
    }

    /// Check if the queue is full. 
    bool full() const volatile
    {
        return (head - tail) == BufferSize;
    }

    /// Push a value to the queue.
    /// @pre !full()
    void push(ValueType val) volatile
    {
        if (!full())
        {
            uint8_t const idx = head & (BufferSize-1);
            ++head;
            buffer[idx] = val;
        }
    }

    /// Remove the oldest value from the queue.
    /// @pre !empty()
    ValueType pop() volatile
    {
        uint8_t const idx = tail & (BufferSize - 1);
        ++tail;
        return buffer[idx];
    }

private:
    uint8_t head;
    uint8_t tail;
    ValueType buffer[BufferSize];
};

