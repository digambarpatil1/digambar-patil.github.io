#include<iostream>
#include<thread>
#include<vector>
#include<atomic>
static std::atomic_int counter = 0u;
//static auto counter = 0u;
constexpr unsigned int MAX_ITER_NUMBER = 20000000u;
 
 
void task()
{
    //auto tmp = counter;
    //++tmp;
    //counter = tmp;
    counter.fetch_add(1, std::memory_order_relaxed);
}
 
void worker()
{
    for(std::size_t idx=0;idx<MAX_ITER_NUMBER; ++idx)
    {
        task();
    }
}
int main()
{
    const auto threadNumber = std::thread::hardware_concurrency();
    std::cout<<"thread Number"<<threadNumber;
    std::vector<std::thread> threads;
    threads.reserve(threadNumber);
    for(std::size_t idx=0;idx<threadNumber; ++idx)
    {
        threads.emplace_back(worker);
    }
    for(auto&th : threads)
    {
        th.join();
    }
    std::cout<< "Expected calls: " <<threadNumber*MAX_ITER_NUMBER<<" Real calls: "<< counter<< std::endl;
        return 0;

    return 0;
}