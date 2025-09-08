#include<iostream>
#include<deque>
#include<string>

int main()
{
    std::deque<std::string>queue;

    queue.push_back("first");
    queue.push_back("second");
    queue.push_front("zero");   
    for(const auto& item:queue){
        std::cout<<item<<std::endl;
    }
    queue.pop_back();
    std::cout<<"After pop_back operation"<<std::endl;
    for(const auto& item:queue){
        std::cout<<item<<std::endl;
    }
    queue.pop_front();
    std::cout<<"After pop_front operation"<<std::endl;
    for(const auto& item:queue){
        std::cout<<item<<std::endl;
    }
    return 0;
}