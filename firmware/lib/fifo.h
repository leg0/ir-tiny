#pragma once

#include <stdint.h>

template <uint32_t N>
struct IsEven
{
    static bool const value = (N&1) == 0;
};

template <uint32_t N>
struct IsPowerOf2
{
    static bool const value = IsEven<N>::value && IsPowerOf2<N / 2>::value;
};

template <>
struct IsPowerOf2<1>
{
    static bool const value = true;
};

template <>
struct IsPowerOf2<0>
{
    static bool const value = false;
};

template <uint8_t Size, typename VT = uint8_t>
class GenericFifo
{
    static_assert(IsPowerOf2<Size>::value, "Size should be a power of 2");

public:

    typedef VT ValueType;
    static uint8_t const BufferSize = Size;


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

