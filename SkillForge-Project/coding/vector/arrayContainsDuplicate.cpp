#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

std::vector<int> duplicateElementArray(vector<int> &nums)
{
    std::set<int> result;
    std::vector<int> vec;
    for(int num:nums)
    {
        if(result.count(num) && std::find(vec.begin(), vec.end(), num) == vec.end() )
        {
           vec.push_back(num);
        }
        result.insert(num);
    }
   return  vec;
}


int main() {
    vector<int> A = {1, 3,3,5,5, 5, 7};
    std::vector<int> result = duplicateElementArray(A);
    for(auto& i :result)
    std::cout<<i<<" ";
    return 0;
}
