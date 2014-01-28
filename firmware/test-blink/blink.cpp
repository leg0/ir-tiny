 #include "board.h"
 #include <avr/interrupt.h>

uint8_t g_counter = 0;
bool g_b = false;

ISR(TIM0_COMPA_vect, ISR_NAKED)
{
    ++g_counter;
    if (g_counter > 160)
    {
        g_counter = 0;
        if (g_b ^= true)
        {
            Board::LedPin::makeInput();
        }
        else
        {
            Board::LedPin::makeOutput();
        }
    }
    reti();
}

int main()
{
    Board::init();
    Board::LedPin::write(0);
    sei();
    while(1)
    { }
}