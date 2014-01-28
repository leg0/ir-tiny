#pragma once

#include <avr/io.h>

#define DECLARE_REGISTER(type, name) \
struct name ## _RegisterTraits { \
    typedef type ValueType; \
    static ValueType volatile& get() { return name; } \
}; \
typedef Register<name ## _RegisterTraits> name ## _Register

template <typename Tr>
struct Register
{
    typedef Tr Traits;
    typedef typename Traits::ValueType ValueType;
    
    /// Clear bits in register.
    /// @param mask - if bit in mask is 1, then this bit is cleared.
    static void clear(ValueType mask)
    {
        Traits::get() &= ~mask;
    }

    /// Set specified bits in register.
    static void set(ValueType mask)
    {
        Traits::get() |= mask;
    }

    static void write(ValueType value) { Traits::get() = value; }
    static ValueType read() { return Traits::get(); }
};

#if defined(__AVR_ATtiny4__)
    #include "attiny4/registers.h"
#elif defined(__AVR_ATtiny5__)
    #include "attiny5/registers.h"
#elif defined(__AVR_ATtiny9__)
    #include "attiny9/registers.h"
#elif defined(__AVR_ATtiny10__)
    #include "attiny10/registers.h"
#elif defined(__AVR_ATmega128__)
    #include "atmega128/registers.h"
#else
    #error "Unsupported device"
#endif

#undef DECLARE_REGISTER
//#undef DECLARE_BIT
