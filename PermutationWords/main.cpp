#include <iostream>
#include <map>

using namespace std;

// Returns true if one the string is a permutation of the other
bool isPermutation(string s, string t) {
    map<char, int> substring;
    map<char, int> raw;
    
    for(const char& c: s)
    {
     
      if(substring.find(c) == substring.end())
         substring.insert({c, 1});
      else
         substring[c]++;
    }
  
    for( const auto& c: t)
    {
      if(raw.find(c) == raw.end())
         raw.insert({c, 1});
      else
         raw[c]++;
    }
  
    for( auto& v: substring )
    {
       if(raw[v.first] < v.second)
       { 
         return false;
       } 
    }
  
    return true;
}

int main() {
   
    cout << isPermutation("abcd", "dcab");
    cout << isPermutation("abdd", "dcab");
    cout << isPermutation("2022", "2222222222");
    cout << isPermutation("2022", "22222222220");                  

    return 0;
}