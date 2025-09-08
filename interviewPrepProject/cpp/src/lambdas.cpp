//lambdas.cpp contains examples of lambda functions in C++20
// It demonstrates captures, generic lambdas, and usage with STL algorithms.
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <typeinfo>
#include <numeric>
#include<algorithm>
int main(){
    std::vector<int> numbers ={1,2,3,4,5,6,7,8,9,10};
    int counter = 0;
    int even_count = std::count_if(numbers.begin(), numbers.end(),  [&](int n){++counter; return n % 2 == 0;});
    std::cout<<"Count of even numbers: "<<even_count<<"counter :"<<counter<<std::endl;

    //generic lambda
    auto print=[](auto x){ std::cout<<"type of x:"<< x<<typeid(x).name()<<std::endl;};
    print(10);
    print(10.5);
    print("Hello");
    print(std::string("World"));

    std::transform(numbers.begin(),numbers.end(),numbers.begin(),[](auto n){ return n * n;});
    std::cout<<"Squares of numbers: ";
    for(const auto& num : numbers){
        std::cout<<num<<" ";
    }
   std::cout<<std::endl;
    std::for_each(numbers.begin(),numbers.end(),[](auto n){ std::cout<<n<<" ";});
    std::cout<<std::endl;

    std::vector<int> v = {1, 2, 3, 4};
    //auto result= std::accumulate(v.begin(),v.end(),1,std::multiplies<int>());
    //std::cout<<"result:"<<result<<std::endl;

    std::vector<std::string> words = {"C++", " ", "is", " ", "powerful"};
    auto sentence = std::accumulate(words.begin(), words.end(), std::string(""));
    std::cout << "Sentence: " << sentence << std::endl;

    
    auto avg = std::accumulate(v.begin(), v.end(), 0.0) / v.size();
    std::cout << "Average: " << avg << std::endl;

    std::vector<int> v1 = {10, 20, 30, 40}; //inserted sorted vector 25
    auto  it = std::lower_bound(v1.begin(),v1.end(),25);
    v1.insert(it,25);
    std::cout<<"After inserting 25: ";
    for(const auto& num : v1){
        std::cout<<num<<" ";
    }
    std::cout<<std::endl;

    std::cout<<"lower bound of 24 is :"<<*std::lower_bound(v1.begin(),v1.end(),20)<<std::endl;
    std::cout<<"upper bound of 24 is :"<<*std::upper_bound(v1.begin(),v1.end(),20)<<std::endl;
    std::vector<int> v2 = {1, 2, 3,3,3, 4};
    auto range = std::equal_range(v2.begin(), v2.end(), 3);
    std::cout << "3 appears from index " 
          << (range.first - v2.begin()) << " to "
          << (range.second - v2.begin() - 1) << "\n"; // 1 to 2
    
    return 0;
}

