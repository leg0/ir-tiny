// This test program mirrors whatever there is on PB0 onto PB2. 
// An example circuit to excercise this test:

/*

         SW1          IC1             D1    R1 
                     ____________
          /         |            |    |\ |
      +--/ ---------+ PB0    PB2 +----| >+--^v----+
      |             |            |    |/ |        |
      > R2          |____________|                |
      <                                          _+_
      |
     _+_

*/

#include "board.h"

#include <avr/interrupt.h>

using namespace test_int0;

// copyInputToOutput1 and copyInputToOutput2 should generate identical code
// (like bit-for-bit identical).

static void copyInputToOutput1()
{
    Board::OutputPin::write(Board::InputPin::isSet());
}

static void copyInputToOutput2()
{
    bool const isSet = PINB & (1<<PORTB0);
    if (isSet)
        PORTB |= (1<<PORTB2);
    else
        PORTB &= ~(1<<PORTB2);
}

ISR(PCINT0_vect, ISR_NAKED)
{
    copyInputToOutput1();
    reti();
}

//ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));
//ISR(PCINT2_vect, ISR_ALIASOF(PCINT0_vect));

int main(void)
{
    Board::init();
    copyInputToOutput1();
    sei();
    for (;;)
    { }
}