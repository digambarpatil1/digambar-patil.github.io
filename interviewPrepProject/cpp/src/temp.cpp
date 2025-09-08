#include<iostream>
#include<vector>
using namespace std;

void merge(vector<int>& num1, int m, vector<int>& num2, int n) 
{
    int val;
        for(int i=0;i<m;i++)
        {
            val=num1[i];
            auto it =std::lower_bound(num2.begin(),num2.end(),val);
           // std:cout<<*it;
             num2.insert(it,val);
        }
          for(auto& i:num2){
             std::cout<<i<< " "; 
          }
        std::cout<<endl;


}

int main(){
    std::cout<<"Hello World"<<std::endl;
    vector<int> nums1{1,2,3,0,0,0}; int  m = 3; vector<int> nums2{2,5,6}; int  n = 3;
    merge(nums1,m,nums2,n);
    std::views::
    return 0;
}

