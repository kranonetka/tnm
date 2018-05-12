#pragma once

template <typename T>
T Minimum(const T lhs, const T rhs)
{
    return lhs < rhs ? lhs : rhs;
}

//the number of lower zeros
short RZ(unsigned vector)
{
    if (!vector)
        return -1;

    short count = 0;

    if (vector << 16 == 0)
    {
        count += 16;
        vector >>= 16;
    }

    if (vector << 24 == 0)
    {
        count += 8;
        vector >>= 8;
    }

    if (vector << 28 == 0)
    {
        count += 4;
        vector >>= 4;
    }

    if (vector << 30 == 0)
    {
        count += 2;
        vector >>= 2;
    }

    if (vector << 31 == 0)
    {
        ++count;
    }

    return count;
}

//the number of leading zeros
short LZ(unsigned vector)
{
    if (!vector)
        return -1;

    short count = 0;

    if (vector >> 16 == 0)
    {
        count += 16;
        vector <<= 16;
    }

    if (vector >> 24 == 0)
    {
        count += 8;
        vector <<= 8;
    }

    if (vector >> 28 == 0)
    {
        count += 4;
        vector <<= 4;
    }

    if (vector >> 30 == 0)
    {
        count += 2;
        vector <<= 2;
    }

    if (vector >> 31 == 0)
    {
        ++count;
    }

    return count;
}

short Jacobi(const unsigned base, const unsigned mod)
{
    if (!base)
        return 0;
    if (base == 1)
        return 1;
    const short s = RZ(base);
    const unsigned r = base >> s;
    short k;
    if (!(s & 1) || ((mod & 7) == 1 || (mod & 7) == 7))
    {
        k = 1;
    }
    else
    {
        k = -1;
    }
    if (r == 1)
        return k;
    if ((r & 3) == 3 && (mod & 3) == 3)
        k = -k;
    return k * Jacobi(mod % r, r);
}