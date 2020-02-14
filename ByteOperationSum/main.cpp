
#include <bits/stdc++.h>
#include <stdexcept>

#include <iostream>


int add(int a, int b)
{
    int _a = a;
    int _b = b;

    while (b != 0)
    {
        int bits = a & b;

        a = a ^ b;

        b = bits << 1;
    }

    if( ((_a ^ a) & (_b ^ a)) < 0 && a < 0)
        throw std::overflow_error("Overflow");

    if( ((_a ^ a) & (_b ^ a)) < 0 && a > 0)
        throw std::underflow_error("Underflow");

    return a;
}

int main()
{

    std::cout << "Test 1 ." << std::endl;
    int A = 1000000, B = 9000000;
    int sum = add( A, B );
    std::cout << A << " + " << B << " = " << sum << std::endl;

    std::cout << std::endl << "Test 2 ." << std::endl;
    A = INT_MIN + 10, B = 10;
    sum = add( INT_MIN + 10, 10);
    std::cout << A << " + " << B << " = " << sum << std::endl;


    std::cout << std::endl << "Test 3 . overflow expected" << std::endl;
    A = INT_MAX-10, B = 11;
    try {

       sum = add(A, B);

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    std::cout << std::endl << "Test 4 . cross over minimal int limit. error expected" << std::endl;
    A = INT_MIN + 10, B = -100;
    try {

       sum = add(A, B);

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    std::cout << std::endl << "Test 5 ." << std::endl;
    A = 1000, B = -2000;
    sum = add(A, B);
    std::cout << A << " + " << B << " = " << sum << std::endl;

    return 0;
}
