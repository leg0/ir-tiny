#pragma once

#include "pin.h"
#include "port.h"
#include "registers.h"
#include "timer.h"

namespace test_serial
{
    struct Board
    {
        typedef Pin<PortB, PB0> Unused0;
        typedef Pin<PortB, PB1> TxPin;
        typedef Pin<PortB, PB2> Unused2;
        typedef Pin<PortB, PB3> Unused3;
        
        typedef Timer0 TheClock;

        static void init()
        {
            Unused0::enablePullUp();
            Unused2::enablePullUp();
            Unused3::enablePullUp();
            TxPin::makeOutput();

            // system clock prescaler = 1
            CCP = 0xD8;
            CLKPSR = 0;

            uint32_t const F_CPU = 8000000;
            uint32_t const SERIAL_BAUD = 115200;
            uint16_t const tickCount = F_CPU / SERIAL_BAUD;
            TheClock::setCtcMode(tickCount);
        }
    };
}