#include<iostream>
#include<ranges>
#include<vector>
#include<algorithm>
struct person{
    std::string name;
    int age;
};

int main(){

//c++ 20 : ranges
 std::vector<int> vect={1,4,2,3,5,6,7,8,9,10};
 std::ranges::sort(vect);
 for(const auto& val:vect){
    std::cout<<val<<" ";
 }
//projection example
std::vector<person> person={ {"abc",20}, {"def",30}, {"xyz",25} };
std::ranges::sort(person,{},&person::age);
std::cout<<"\n";
for(const auto& val:person){
    std::cout<<val.age<<" ";
 }
 //viewi`ng and filtering
 auto even = vect| std::views::filter([](int i){return i%2==0;})
                 |std::views::transform([](int i){return i*2;})
                 |std::views::take(3)
                 |std::views::reverse;
   
 
 std::cout<<"\nEven numbers: ";
    for (int i : even) std::cout << i << " ";
    std::cout << '\n';


    return 0;

}