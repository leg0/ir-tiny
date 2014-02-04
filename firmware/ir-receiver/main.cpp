#include "board.h"
#include "fifo.h"
#include "util.h"

#include <avr/interrupt.h>

#if 0
    register uint16_t g_currentTime  asm("r16"); // r17:r16
    register uint16_t g_prevInt0Time asm("r18"); // r19:r18
    register uint8_t g_bitsRemaining asm("r28");
    register uint8_t g_serialShift   asm("r29");
#else
    uint16_t volatile g_currentTime;
    uint8_t volatile  g_bitsRemaining;
    uint16_t volatile g_serialShift;
#endif

typedef GenericFifo<8, uint8_t> Fifo;
static Fifo volatile g_queue;


ISR(TIM0_COMPA_vect, ISR_NAKED)
{
    // bit_time = 1/115200 s = ~8.681 µs (== 1 bit)
    // The IR-pulses are measured in units of bit_time. For example
    // a 400 µs pulse would be 400 µs*115200 bit/s = 46.08 bits
    ++g_currentTime;

    if (g_bitsRemaining == 0)
    {
        if (!g_queue.empty())
        {
            g_bitsRemaining = 11; // start + 8*data + 2*stop
            g_serialShift = (g_queue.pop() | 0x300) << 1;

        }
    }
    else
    {
        Board::UartTxPin::write(g_serialShift & 1);
        g_serialShift >>= 1;
        --g_bitsRemaining;
    }

    reti();
}

//ISR(INT0_vect, ISR_NAKED)
ISR(PCINT0_vect, ISR_NAKED)
{
    // works even when the values overflow.
    uint16_t pulseLength = g_currentTime;// - g_prevInt0Time;
    //g_prevInt0Time = g_currentTime;
    g_currentTime = 0;

    // Represent the low time as negative value, and high time
    // as a positive value.
    bool const irInput = Board::IrDataPin::isSet();
    if (!irInput)
        pulseLength *= -1;

    g_queue.push(high(pulseLength));
    g_queue.push(low(pulseLength));

    Board::LedPin::write(!irInput);

    reti();
}

int main ()
{
    //asm volatile ("ldi r28, 0"); //g_bitsRemaining = 0;
    //asm volatile ("ldi r28, 10"); //XXX: 
    g_bitsRemaining = 0;
    g_queue.init();

    g_queue.push(0x30);
    g_queue.push(0x31);
    g_queue.push(0x32);
    g_queue.push(0x33);

    Board::UartTxPin::write(true);
	Board::init();
    Board::LedPin::write(true);
    sei();
        
	for (;;)
    { }
}