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
    typedef Pin<PortB, PB0> IrDataPin;
    typedef Pin<PortB, PB1> UartTxPin;
    typedef Pin<PortB, PB2> LedPin;
    typedef Pin<PortB, PB3> Unused2;

    typedef Timer0 TheClock;
    //typedef UartTx<SoftwareUartTraits<SerialOutputPin, SerialClock>> SerialTx;

    static void init()
    {
        // Set system clock prescaler to 1 - run at 8 MHz.
        System::setClockPrescaler(System::ClockPrescaler::_1);

        #if defined(__AVR_ATtiny5__) || defined(__AVR_ATtiny10__)
        // Disable ADC - not going to use it. Save some power.
        PRR |= (1 << PRADC);
        #endif

        // According to data sheet, the unused pins should have defined level,
        // either 0 or 1, but advises against connecting the pins to GND or VCC
        // because that could accidentally cause some not so funny results.
        // Enable internal pull-up for the unused pins.
        LedPin::makeOutput();
        Unused2::enablePullUp();

        uint32_t const F_CPU = 8000000;
        uint32_t const SERIAL_BAUD = 115200;
        uint16_t const tickCount = F_CPU / SERIAL_BAUD;
        TheClock::setCtcMode(tickCount);

        // The IR-Receiver is active-low and it leaves the pin floating when
        // inactive. We need to drive the line high ourselves (or use external
        // pull-up resistor).
        //IrDataPin::makeInput(); // it's input by default
        IrDataPin::enablePullUp();
        // TODO: add features to lib so that we could enable interrupt like this:
        //IrDataPin::enablePinChangeInterrupt();
        PCICR |= (1 << PCIE0);  // Enable pin change interrupt controller 0
        PCMSK |= (1 << PCINT0); // Enable pin change interrupts for pin0

        UartTxPin::makeOutput();
    }
};
