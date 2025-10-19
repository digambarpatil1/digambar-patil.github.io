#include<iostream>
#include<thread>
#include<vector>
#include<numeric>
#include <future>

using namespace std;

void square(int x, std::promise<int> p)
{
    x=x*x;
    p.set_value(x);
}

int main()
{
    std::promise<int> promise;
    int number = 5 ;
    
    std::future<int> f = promise.get_future();
    //while passing it must be std::move as promises are not copyiable
    std::thread t1(square,number,std::move(promise)); 
    
    std::cout<<"result ="<<f.get()<<endl;
    t1.join();

    
    std::packaged_task<int(int)> task([](int x) {   // takes int, returns int
        return x * x;
    });

    std::future<int> f2 = task.get_future();         // get future from task

    std::thread t(std::move(task), 7);              // run task in thread

    std::cout << "Result: " << f2.get() << std::endl; // wait + get result
    t.join();

    return 0;
}