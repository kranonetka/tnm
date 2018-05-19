#pragma once

#include <cstdlib>
#include "pow.h"
#include "utility.h"
#include <exception>

unsigned Gen(const unsigned short bits = 32)
{
    return ((rand() << 17) ^ (rand() << 2) ^ rand()) >> (32 - bits);
}

bool IsPrimeFarmTest(const unsigned number, const unsigned reliability)
{
    const unsigned
        degree = number - 1,
        genMod = number - 3;
    for (unsigned i = 0u; i < reliability; ++i)
    {
        const unsigned
            a = 2 + (Gen() % genMod);
        if (RLpow(a, degree, number) != 1)
            return false;
    }
    return true;
}

bool IsPrimeSSTest(const unsigned number, const unsigned reliability)
{
    const unsigned
        genMod = number - 3,
        degree = (number - 1) >> 1,
        minusOne = number - 1;
    for (unsigned i = 0; i < reliability; ++i)
    {
        const unsigned
            a = 2 + (Gen() % genMod);
        int r = RLpow(a, degree, number);
        if (r != 1 && r != minusOne)
        {
            return false;
        }
        short s = Jacobi(a, number);
        if (r == number - 1)
        {
            r = -1;
        }
        if (s != r)
        {
            return false;
        }
    }
    return true;
}

bool IsPrimeMRTest(const unsigned number, const unsigned reliability)
{
    const unsigned
        genMod = number - 3,
        minusOne = number - 1;
    short s = RZ(minusOne);
    const unsigned r = minusOne >> s;
    for (unsigned i = 0; i < reliability; ++i)
    {
        const unsigned
            a = 2 + (Gen() % genMod);
        unsigned b = RLpow(a, r, number);
        if (b != 1 && b != minusOne)
        {
            for (short j = 1; j < s && b != minusOne; ++j)
            {
                b = (b * b) % number;
                if (b == 1)
                    return false;
            }
            if (b != minusOne)
                return false;
        }
    }
    return true;
}

bool IsPrimeLukaTest(const unsigned number, const unsigned reliability)
{
    const unsigned
        genMod = number - 3,
        minusOne = number - 1,
        sqrtMinusOne = intSqrt(minusOne);
    unsigned
        subjNumber = minusOne >> RZ(minusOne),
        dividers[32] = { 2 },
        dividersCount = 1;

#pragma region factorization
    for (unsigned i = 3; i <= sqrtMinusOne; i += 2)
    {
        if (subjNumber % i == 0)
        {
            dividers[dividersCount] = i;
            do
            {
                subjNumber /= i;
            } while (subjNumber % i == 0);
            ++dividersCount;
        }
    }
    if (subjNumber > 1)
    {
        dividers[dividersCount++] = subjNumber;
    }
#pragma endregion

    for (unsigned i = 0; i < reliability; ++i)
    {
        const unsigned a = 2 + (Gen() % genMod);
        if (RLpow(a, minusOne, number) != 1)
        {
            return false;
        }
        for (unsigned j = 0; j < dividersCount; ++j)
        {
            if (RLpow(a, minusOne / dividers[j], number) != 1)
            {
                --dividersCount;
                dividers[j] = dividers[dividersCount];
            }
        }
        if (!dividersCount)
        {
            return true;
        }
    }
    return false;
}

unsigned OddGen(const unsigned short bits)
{
    return Gen(bits) | (1u << (bits - 1)) | 1u;
}

enum Test
{
    Farm,
    SS,
    MR,
    Luka
};

unsigned PrimeGen(const short bits, Test primeTest, unsigned reliability)
{
    if (primeTest < Test::Farm || primeTest > Test::Luka)
        throw std::exception("Invalid prime test parameter");

    static bool(*const tests[])(const unsigned, const unsigned) = { &IsPrimeFarmTest, &IsPrimeSSTest, &IsPrimeMRTest, &IsPrimeLukaTest };

    unsigned retval = OddGen(bits);

    while (tests[primeTest](retval, reliability) == false)
    {
        retval = OddGen(bits);
    }

    return retval;
}

unsigned long long StrongPrimeGen(const short bits)
{
    unsigned
        s = PrimeGen(bits, Test::MR, 5),
        t = PrimeGen(bits, Test::MR, 5);
    while (t == s)
    {
        t = PrimeGen(bits, Test::MR, 5);
    }
    unsigned i = Gen(31) + 1;
    unsigned long long r = 1 + ((i * t) << 1);
    while (IsPrimeMRTest(r, 5) == false)
    {
        ++i;
        r = 1 + ((i * t) << 1);
    }
    unsigned long long p0 = ((RLpow(s, r - 2, r) * s) << 1) - 1;
    i = Gen(31) + 1;
    unsigned long long p = p0 + ((i * r * s) << 1);
    while (IsPrimeMRTest(p, 5) == false)
    {
        ++i;
        p = p0 + ((i * r * s) << 1);
    }
    return p;
}