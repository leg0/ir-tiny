#pragma once
#include "port.h"

template <typename P, int N>
struct Pin
{
	enum { Number = N };
	typedef P Port;
	
	static void makeInput()
	{
		Port::makeInput(1<<Number);
	}
	
	static void makeOutput()
	{
		Port::makeOutput(1<<Number);
	}
	
    static void enablePullUp()
    {
        Port::enablePullUp(1<<Number);
    }

    static void disablePullUp()
    {
        Port::disablePullUp(1<<Number);
    }

	static void set()
	{
		Port::set(1<<Number);
	}
	
	static void clear()
	{
		Port::clear(1<<Number);
	}
	
    static void write(bool value)
    {
        if (value)
            Port::set(1<<Number);
        else
            Port::clear(1<<Number);
    }

	static bool isSet()
	{
		return (Port::readInput() & (1<<Number)) != 0;
	}
	
	static void toggle()
	{
		Port::toggle(1<<Number);
	}
};
