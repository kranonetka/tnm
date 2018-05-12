#pragma once

#include <cstdlib>
#include "pow.h"
#include "utility.h"
#include <exception>

unsigned Gen()
{
    return (rand() << 17) ^ (rand() << 2) ^ rand();
}

bool IsPrimeFarmTest(unsigned number, const unsigned reliability)
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

    for (unsigned i = 1; i < reliability; ++i)
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

/*
OLD bool IsPrimeLukaTest(const unsigned number, const unsigned reliability)
{
    const unsigned
        genMod = number - 3,
        minusOne = number - 1,
        sqrtMinusOne = intSqrt(minusOne);
    unsigned
        subjNumber = minusOne,
        dividers[32],
        count = 1;
    dividers[0] = 2;
    subjNumber >>= RZ(subjNumber);
    for (unsigned i = 3; i <= sqrtMinusOne; i += 2)
    {
        if (subjNumber % i == 0)
        {
            dividers[count] = i;
            do
            {
                subjNumber /= i;
            } while (subjNumber % i == 0);
            ++count;
        }
    }
    if (subjNumber > 1)
    {
        dividers[count++] = subjNumber;
    }

    for (unsigned i = 0; i < reliability; ++i)
    {
        const unsigned a = 2 + (Gen() % genMod);
        if (RLpow(a, minusOne, number) != 1)
        {
            return false;
        }
        bool flag = true;
        for (unsigned j = 0; j < count; ++j)
        {
            if (RLpow(a, minusOne / dividers[j], number) == 1)
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            return true;
        }
    }
    return false;
}
*/

unsigned OddGen(const unsigned bits)
{
    return (Gen() >> (32 - bits)) | (1u << (bits - 1)) | 1u;
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
    static bool(*tests[])(const unsigned, const unsigned) = { &IsPrimeFarmTest, &IsPrimeSSTest, &IsPrimeMRTest, &IsPrimeLukaTest };

    if (primeTest < Test::Farm || primeTest > Test::Luka)
        throw std::exception("Invalid prime test parameter");

    while (true)
    {
        unsigned retval = OddGen(bits);
        if (tests[primeTest](retval, reliability) == true)
        {
            return retval;
        }
    }
}