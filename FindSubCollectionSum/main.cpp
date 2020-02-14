#include <iostream>

#include "find.h"

using namespace std;

int main()
{
    vector<uint32_t> I;
    uint32_t T = 11;
    vector<uint32_t> M;
    uint32_t S;

    cout << "Test 1 ." << endl;
    I = { 22, 4, 44, 2, 4, 2, 2, 2, 12, 10, 8, 10, 12, 22, 8, 4, 4, 49, 2, 2, 12, 2 };
    T = 55;
    find_largest_sum(T,I,M,S);
    print_result(T,I,M,S);

    cout << endl << "Test 2 ." << endl;
    I = { 1, 2, 3, 4, 5, 6, 7 };
    T = 11;
    find_largest_sum(T,I,M,S);
    print_result(T,I,M,S);

    cout << endl << "Test 3 ." << endl;
    I = { 21, 4, 43, 33, 91, 20, 11, 8, 19, 67, 0, 1, 5, 22, 15, 1, 13, 19, 30, 45, 17, 99 };
    T = 103;
    find_largest_sum(T,I,M,S);
    print_result(T,I,M,S);

    cout << endl << "Test 4 ." << endl;
    I = { 59, 2, 6, 37, 15, 13, 8, 28, 14, 65, 10, 3, 3, 2, 5, 14, 1, 1, 0, 4 };
    T = 48;
    find_largest_sum(T,I,M,S);
    print_result(T,I,M,S);

    cout << endl << "Test 5 ." << endl;
    I = { 8, 44, 64, 48, 20, 10, 86, 18, 6, 12, 8, 22, 16, 4, 14, 64, 32, 40, 70, 90 };
    T = 33;
    find_largest_sum(T,I,M,S);
    print_result(T,I,M,S);

    return EXIT_SUCCESS;
}
