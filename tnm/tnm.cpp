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

using namespace std;

int main()
{
    srand(time(NULL));
    IsPrimeLukaTest(15, 5);
        return 0;

    for (auto i = 3; i <= 32; ++i)
    {
        cout << i << ": " << PrimeGen(i, Test::Luka, 5) << endl;
    }
}