#pragma once

#include "pin.h"
#include "port.h"
#include "registers.h"
#include "timer.h"

struct System
{
    enum class ClockPrescaler : CLKPSR_Register::ValueType
    {
        _1   = 0,
        _2   = (1 << CLKPS0),
        _4   = (1 << CLKPS1),
        _8   = (1 << CLKPS1) | (1 << CLKPS0),
        _16  = (1 << CLKPS2),
        _32  = (1 << CLKPS2) | (1 << CLKPS0),
        _64  = (1 << CLKPS2) | (1 << CLKPS1),
        _128 = (1 << CLKPS2) | (1 << CLKPS1) | (1 << CLKPS0),
        _256 = (1 << CLKPS3),
    };

    static void setClockPrescaler(ClockPrescaler cp)
    {
        CCP = 0xD8;
        CLKPSR = static_cast<uint8_t>(cp);
    }
};

struct Board
{
    typedef Pin<PortB, PB2> LedPin;

    typedef Timer0 TheClock;

    static void init()
    {
        // Set system clock prescaler to 1 - run at 8 MHz.
        System::setClockPrescaler(System::ClockPrescaler::_1);

        //uint32_t const F_CPU = 8000000;
        uint16_t const tickCount = 50000;
        TheClock::setCtcMode(tickCount);

        // The IR-Receiver is active-low and it leaves the pin floating when
        // inactive. We need to drive the line high ourselves (or use external
        // pull-up resistor).
        LedPin::makeOutput();
    }
};
