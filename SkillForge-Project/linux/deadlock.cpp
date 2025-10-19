#include<iostream>
#include<chrono>
#include<mutex>
#include<thread>
static std::mutex mu1,mu2;
using namespace std::chrono_literals;

void threadfun1()
{
    std::cout<<"threadfun1"<<"\n";
    std::lock_guard<std::mutex>lock1(mu1);
    std::cout<<"threadfun1 after mutex"<<"\n";
    std::this_thread::sleep_for(1s);
    std::lock_guard<std::mutex>lock2(mu2);
}

void threadfun2()
{
    std::cout<<"threadfun2"<<"\n";
    std::lock_guard<std::mutex>lock1(mu2);
    std::cout<<"threadfun2 after mutex"<<"\n";
    std::this_thread::sleep_for(1s);
    std::lock_guard<std::mutex>lock2(mu1);
}


int main()
{
    std::cout<<"Demo for debuggig with gdb"<<"\n";
    std::thread t1(threadfun1);
    std::thread t2(threadfun2);

    t1.join();
    t2.join();

}