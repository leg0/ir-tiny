#pragma once

#include "pin.h"

namespace test_int0
{

    struct Board
    {
        typedef Pin<PortB, PB0> InputPin;
        typedef Pin<PortB, PB1> Unused1;
        typedef Pin<PortB, PB2> OutputPin;
        typedef Pin<PortB, PB3> Unused3;

        static void init()
        {
            Unused1::enablePullUp();
            Unused3::enablePullUp();

            InputPin::enablePullUp();
            OutputPin::makeOutput();

            // Enable pin change interrupt controller 0
            PCICR |= (1 << PCIE0);

            // Enable pin change interrupts for pin0
            PCMSK |= (1 << PCINT0);
        }
    };
}