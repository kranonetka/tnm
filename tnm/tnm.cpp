// tnm.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <iostream>
#include <iomanip>
#include <ctime>

#include "pow.h"
#include "gcd.h"
#include "sqrt.h"
#include "generators.h"

#include <intrin.h>

#include <bitset>

#include <map>

using namespace std;

int main()
{
    srand(unsigned(time(NULL)));

    map<int, int> fails;
    map<int, int> rt;
    map<int, int> p0s;

    for (auto j = 0; j < USHRT_MAX; ++j)
    {
        for (auto i = 3; i <= 32; ++i)
        {
            auto p = StrongPrimeGen(i);
            if (p == 0 || p == 1)
            {
                if (p == 0)
                {
                    ++rt[i];
                }
                else
                {
                    ++p0s[i];
                }
                ++fails[i];
            }
        }
    }
    for (auto include : fails)
    {
        cout << include.first << " bits: " << include.second << " fail times" << endl;
    }
    for (auto include : rt)
    {
        cout << include.first << " bits: " << include.second << " r calculating fail times" << endl;
    }
    for (auto include : p0s)
    {
        cout << include.first << " bits: " << include.second << " p0 calculating fail times" << endl;
    }
    return 0;
}