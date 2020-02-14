#pragma once

#include <vector>
#include <algorithm>

using namespace std;

void find_largest_sum(const uint32_t T, const vector<uint32_t> &I, vector<uint32_t>& M, uint32_t &S)
{
    vector<uint32_t> sub_collection;
    uint32_t counter = 0;
    S = 0;

    while(S < T && counter < I.size())
    {

        sub_collection.clear();

        S =  [&T, &I, &M, &sub_collection, &counter]()->uint32_t{

            uint32_t sum = 0;

            for(auto i : I)
            {
                if (sum+i <= T)
                {
                    sum += i;
                    sub_collection.push_back(i);
                    M.push_back(i);
                }
                else if (i <= T){

                    sort(sub_collection.begin(), sub_collection.end());

                    for(auto ignored_interval = sub_collection.begin();
                        ignored_interval != sub_collection.end(); ++ignored_interval)
                    {
                        uint32_t _sum = i;
                        vector<uint32_t> _M;

                        for(auto p = ignored_interval; p != sub_collection.end(); ++p)
                        {
                            if (_sum + *p <= T)
                            {
                                _M.push_back(*p);
                                _sum += *p;
                            }
                            else
                                break;
                        }
                        for(auto p = ignored_interval; p != sub_collection.begin(); --p)
                        {
                            if (_sum + *p <= T)
                            {
                                _M.push_back(*p);
                                _sum += *p;
                            }
                        }

                        if (_sum > sum)
                        {
                            sum = _sum;
                            M = _M;
                            M.push_back(i);
                        }
                        if (_sum == T)
                            break;
                    }
                }

                counter++;

                if (sum==T)
                    break;
            }

            return sum;
        }();
    }

}

void print_result(const uint32_t T, const vector<uint32_t> &I, vector<uint32_t>& M, uint32_t &S)
{
    cout << "I: ";
    for(auto const& i: I)
    cout << i << " " ;
    cout << endl;

    cout << "T: " << T << endl;

    cout << "M: ";
    for(auto const& p: M)
    cout << p  << " ";


    cout << endl << "S: " << S << endl;
}
