#include <iostream>
#include <chrono>
using namespace std;
vector<int> mergeSortedArray(vector<int>first,int m,vector<int>second,int n)
{
    std::vector<int> result;
    int i=0;int j=0;
    while(i< m && j< n)
    {
        if(first[i]<=second[j])
        {
            result.push_back(first[i++]);
        }
        else{
            result.push_back(second[j++]);
        }
    }
    while(i <m){result.push_back(first[i++]);}
    while(j<n){result.push_back(second[j++]);}
    
    return result; 
}

int main() {
    vector<int> A = {1, 3, 5, 7};
    vector<int> B = {2, 4, 6, 8, 9};

    vector<int> merged = mergeSortedArray(A,A.size(), B,B.size());

    cout << "Merged array: ";
    for (int num : merged) cout << num << " ";
    cout << endl;

    return 0;
}
