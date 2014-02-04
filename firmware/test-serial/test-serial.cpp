/*
 * test_serial.cpp
 *
 * Created: 2.02.2014 18:45:02
 *  Author: lego
 */ 


#include "board.h"

#include <avr/interrupt.h>
//#include <atomic>

using namespace test_serial;

volatile uint8_t Character = 'A';
volatile uint8_t bitsRemaining = 0;
volatile uint16_t sendBuffer = 0;

ISR(TIM0_COMPA_vect, ISR_NAKED)
{
    //__sync_synchronize();
    if (bitsRemaining == 0)
    {
        bitsRemaining = 10;
        sendBuffer = (Character << 1) | (1 << 9);
        if (++Character > 'Z')
            Character = '\x0d';
        else if (Character == '\x0e')
            Character = 'A';
    }
    else
    {
        Board::TxPin::write(sendBuffer & 1);
        --bitsRemaining;
        sendBuffer >>= 1;
    }

    reti();
}

int main()
{
    Board::init();
    sei();
    while(1)
    { }
}