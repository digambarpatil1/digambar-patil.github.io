#include<iostream>
#include<unordered_map>
#include<string>
#include<sstream>

int main(){
    std::string str = "this is a test this is only a test";
    std::unordered_map<std::string, int> freqMap;
    std::istringstream  iss(std::move(str));
    std::string word;
    while(iss >> word){
        freqMap[word]++;
    }
  for(const auto&[key, value]:freqMap){
      std::cout<<key<<" : "<<value<<std::endl;
  }

    return 0;
}