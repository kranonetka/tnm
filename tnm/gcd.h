#pragma once

#include "utility.h"

unsigned BinaryGCD(unsigned lhs, unsigned rhs)
{
    unsigned i = RZ(lhs), j = RZ(rhs);
    lhs >>= i;
    rhs >>= j;
    while (lhs != rhs)
    {
        if (lhs < rhs)
        {
            lhs ^= (rhs ^= (lhs ^= rhs));
        }
        lhs -= rhs;
        lhs >>= RZ(lhs);
    }
    return lhs << Minimum(i, j);
}