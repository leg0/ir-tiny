#pragma once

#include <avr/io.h>
#include "registers.h"

/// @param Dd - Data direction register.
/// @param Out - output register
/// @param In - Input register
/// @param Pu - Pull-up configuration register.
template <typename Dd, typename Out, typename In, typename Pu>
struct GenericPort
{
private:
    typedef Dd dd;
    typedef Out out;
    typedef In in;
    typedef Pu pue;

public:
	static void makeInput(typename dd::ValueType mask)
	{
		dd::clear(mask);
	}
	
	static void makeOutput(typename dd::ValueType mask)
	{
		dd::set(mask);
	}
	
    static void enablePullUp(typename pue::ValueType mask)
    {
        pue::set(mask);
    }

    static void disablePullUp(typename pue::ValueType mask)
    {
        pue::clear(mask);
    }

	static void set(typename out::ValueType mask)
	{
		out::set(mask);
	}
	
	static void clear(typename out::ValueType mask)
	{
		out::clear(mask);
	}
	
	static void toggle(typename in::ValueType mask)
	{
		in::set(mask);
	}
	
	static typename in::ValueType readInput()
    {
        return in::read();
    }

    static typename out::ValueType readOutput()
    {
        return out::read();
    }
};

// TODO: this should be in device-specific header.
#if defined(__AVR_ATmega128__)
// If PORTxn is written logic one when the pin is configured as an
// input pin, the pull-up resistor is activated.
    typedef GenericPort<DDRA_Register, PORTA_Register, PINA_Register, PORTA_Register> PortA;
    typedef GenericPort<DDRB_Register, PORTB_Register, PINB_Register, PORTB_Register> PortB;
    typedef GenericPort<DDRC_Register, PORTC_Register, PINC_Register, PORTC_Register> PortC;
    typedef GenericPort<DDRD_Register, PORTD_Register, PIND_Register, PORTD_Register> PortD;
    typedef GenericPort<DDRE_Register, PORTE_Register, PINE_Register, PORTE_Register> PortE;
    typedef GenericPort<DDRF_Register, PORTF_Register, PINF_Register, PORTF_Register> PortF;
    typedef GenericPort<DDRG_Register, PORTG_Register, PING_Register, PORTG_Register> PortG;
#else
    typedef GenericPort<DDRB_Register, PORTB_Register, PINB_Register, PUEB_Register> PortB;
#endif
