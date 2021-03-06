#pragma once

#include "utility.h"

unsigned intSqrt(const unsigned x)
{
    unsigned retval =
        LZ(x) & 1 ?
        0xffffffff >> (16 + ((LZ(x) + 1) >> 1))
        :
        1 << ((32 - LZ(x)) >> 1);
    while (true)
    {
        unsigned tmp = ((x / retval) + retval) >> 1;
        if (tmp >= retval)
            return retval;
        else
            retval = tmp;
    }
}