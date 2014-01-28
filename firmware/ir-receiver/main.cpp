#include "board.h"
#include "fifo.h"
#include "util.h"

#include <avr/interrupt.h>

register uint16_t g_currentTime  asm("r16"); // r17:r16
register uint16_t g_prevInt0Time asm("r18"); // r19:r18

register uint8_t g_bitsRemaining asm("r28");
register uint8_t g_serialShift   asm("r29");

static Fifo g_queue;

ISR(TIM0_COMPA_vect, ISR_NAKED)
{
    // bit_time = 1/115200 s = ~8.681 µs (== 1 bit)
    // The IR-pulses are measured in units of bit_time. For example
    // a 400 µs pulse would be 400 µs*115200 bit/s= 46,08 bit
   ++g_currentTime;

    if (g_bitsRemaining == 0 && !g_queue.empty())
    {
          g_bitsRemaining = 10; // start + 8*data + stop
          g_serialShift = g_queue.pop();
    }

    if (g_bitsRemaining > 0)
    {
        bool const bit = g_serialShift & 1;
        if (g_bitsRemaining == 10)
            Board::UartTxPin::write(false);
        else if (g_bitsRemaining == 1)
            Board::UartTxPin::write(true);
        else
        {
            Board::UartTxPin::write(bit);
            g_serialShift >>= 1;
        }
        --g_bitsRemaining;
    }

    reti();
}

ISR(INT0_vect, ISR_NAKED)
{
    // works even when the values overflow.
    int16_t pulseLength = g_currentTime - g_prevInt0Time;
    g_prevInt0Time = g_currentTime;

    // Represent the low time as negative value, and high time
    // as a positive value.
    if (!Board::IrDataPin::isSet())
        pulseLength = -pulseLength;

    g_queue.push(high(pulseLength));
    g_queue.push(low(pulseLength));
    
    reti();
}

int main ()
{
    asm volatile ("ldi r28, 0"); //g_bitsRemaining = 0;
    g_queue.init();
	Board::init();
    Board::UartTxPin::write(true);
    sei();
        
	for (;;)
    { }
}