#pragma once

#include "registers.h"

struct Timer0Traits
{
	typedef TCCR0A_Register controlRegisterA;
	typedef TCCR0B_Register controlRegisterB;
	typedef TCCR0C_Register controlRegisterC;
	//typedef GTCCR_Register  generalControlRegister;
	typedef TIMSK0_Register inerruptMaskRegister;
	typedef TIFR0_Register  interruptFlagRegister;
	typedef ICR0_Register   inputCaptureRegister;
	typedef OCR0A_Register  outputCompareRegisterA;
	typedef OCR0B_Register  outputCompareRegisterB;
	typedef TCNT0_Register  counterRegister;
};

template <typename Tr>
struct Timer
{
	typedef Tr Traits;
	
	enum class Prescaler
	{
		Off				= 0,
		_1				= (1<<CS00),
		_8				= (1<<CS01),
		_64				= (1<<CS01) | (1<<CS00),
		_256			= (1<<CS02),
		_1024			= (1<<CS02) | (1<<CS00),
	};
	enum class Wgm
	{
		Normal = 0,
		PwmPhaseCorrect8 = 1,
		PwmPhaseCorrect9 = 2,
		PwmPhaseCorrect10 = 3,
		ClearTimerOnCompare_A = 4,
		FastPwm8 = 5,
		FastPwm9 = 6,
		FastPwm10 = 7,
		PwmPhaseFreqCorrect_B = 8,
		PwmPhaseFreqCorrect_A = 9,
		PwmPhaseCorrect_B = 10,
		PwmPhaseCorrect_A = 11,
		ClearTimerOnCompare_B = 12,
		FastPwm_B = 14,
		FastPwm_A = 15
	};
	
	static void enable()
	{
		PRR &= ~(1 << PRTIM0);
	}

    static void disable()
    {
        PRR |= (1 << PRTIM0);
    }

	static void setPrescaler(Prescaler p)
	{
		auto const val = static_cast<uint8_t>(p);
		auto oldValue = Traits::controlRegisterA::get() & ~((1<<CS02) | (1<<CS01) | (1<<CS00));
		Traits::controlRegisterA::get() = oldValue | ((val & 4) << (CS02-2)) | ((val & 2) << (CS01-1)) | ((val & 1) << CS00);
	}
	static void setMode(Wgm mode)
	{
		//Traits::controlRegisterA::get() &= ~((1<<WGM01) | (1<<WGM00);
		//Traits::controlRegisterB::get() &= ~((1<<WGM03) | (1<<WGM02);
	}

    /// Set the timer to CTC mode and use reset-value in ICR0.
    /// When timer counter reaches \a resetValue, then the timer counter is
    /// reset to zero. When this happens TIM0_COMPA interrupt is triggered.
    static void setCtcMode(uint16_t resetValue)
    {
        TIMSK0 |= (1 << OCIE0A);
        ICR0 = resetValue;
        TCCR0A = 0;
        TCCR0B = (1<<WGM03) | (1<<WGM02) | (1<<CS00);
    }
	
	typedef typename Traits::counterRegister::ValueType CounterType;
	CounterType volatile& getCounter() { return Traits::counterRegister::get(); }
};

typedef Timer<Timer0Traits> Timer0;
