#pragma once

inline uint8_t high(uint16_t val)
{
    return val >> 8;
}

inline uint8_t low(uint16_t val)
{
    return val & 0xFF;
}
