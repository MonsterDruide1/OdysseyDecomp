#pragma once

// implementation of the REV instruction for 16-bit registers
static inline short bswap16(int val)
{
    short ret;
    __asm("REV16 %w0, %w1" : "=r"(ret) : "r"(val));
    return ret;
}

// implementation of the REV instruction for 32-bit registers
static inline int bswap32(int val)
{
    int ret;
    __asm("REV %w0, %w1" : "=r"(ret) : "r"(val));
    return ret;
}