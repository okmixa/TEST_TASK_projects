
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>


using namespace std;


int findIntervalSumZero(vector<int>& A)
{

    unordered_map<int, int> subSum;

    int res = 0;


    int sum = 0;

    for (int i = 0; i < A.size(); ++i) {

        sum += A[i];


        if (sum == 0)
            res++;


        if (subSum.find(sum - 0) != subSum.end())
            res += (subSum[sum - 0]);

        subSum[sum]++;
    }

    if (res < 1'000'000)
        return res;
    else
        return -1;
}

int main()
{
    vector<int> A = { -1, 1, -3, 2, 5 , 10, 2, -2, -20, 10, -6 , 1, 5 };

    cout << findIntervalSumZero(A) <<endl;;
    return 0;
}
