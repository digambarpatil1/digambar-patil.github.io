#include <iostream>
#include <set>
#include <vector>
using namespace std;

void productExceptSelf(vector<int> &nums)
{
    for(int i=0; i< nums.size();i++)
    {
        int multi =1;
        for(int j=0;j<nums.size();j++)
        {
            if(i!=j)
            {
               multi = multi *nums[j];
            }
        }
        std::cout<<multi << " ";
    }
    std::cout<<std::endl;
}


int main() {
    vector<int> A = {1,3,5,2};
    productExceptSelf(A);
    return 0;
}
// Output: 30 10 6 15