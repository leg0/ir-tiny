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

ISR(PCINT0_vect, ISR_NAKED)
{
    Board::OutputPin::write(Board::InputPin::isSet());
    reti();
}

//ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));
//ISR(PCINT2_vect, ISR_ALIASOF(PCINT0_vect));

int main(void)
{
    Board::init();
    sei();
    for (;;)
    { }
}