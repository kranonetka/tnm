#pragma once

#include "utility.h"

unsigned intSqrt(const unsigned x)
{
    const unsigned leadingZeros = LZ(x);
    unsigned retval;
    if (leadingZeros & 1)
    {
        retval = 0xffffffff >> (16 + ((leadingZeros + 1) >> 1));
    }
    else
    {
        retval = 1 << ((32 - leadingZeros) >> 1);
    }
    while (true)
    {
        unsigned tmp = ((x / retval) + retval) >> 1;
        if (tmp >= retval)
            return retval;
        else
            retval = tmp;
    }
}