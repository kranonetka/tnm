#pragma once

#include "utility.h"

unsigned RLpow(const unsigned base, unsigned degree, const unsigned mod)
{
    if (!degree)
        return 1ull;

    unsigned long long
        q = base,
        z = (degree & 1) ? base : 1ull;

    while (degree >>= 1)
    {
        q = (q * q) % mod;
        if (degree & 1)
        {
            z = (z * q) % mod;
        }
    }
    return (unsigned)z;
}

unsigned LRpow(const unsigned base, unsigned degree, const unsigned mod)
{
    if (!degree)
        return 1ull;

    unsigned long long z = base;
    unsigned char iterations = 31 - LZ(degree);

    degree <<= 32 - iterations;

    while (iterations > 0)
    {
        z = (z * z) % mod;
        if (degree & 0x80000000)
        {
            z = (z * base) % mod;
        }
        degree <<= 1;
        --iterations;
    }

    return (unsigned)z;
}