// threadsync.cpp containes examples of various thread synchronization mechanisms in C++20
// such as mutex, atomic, condition variable, and semaphore.


#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include<condition_variable>
#include<semaphore>


//int counter = 0;
std::mutex mu;
std::atomic<int> counter(0);
bool ready =false;
std::condition_variable cv;

std::atomic_flag flag =ATOMIC_FLAG_INIT; // lock free flag
//2 inside this what is that 2 means
//it means max 2 threads can access critical section at a time  

std::counting_semaphore<2> sem(2); // max 2 threads can access critical section

void worker(int id)
{
    sem.acquire(); // acquire a permit
    std::cout << "Worker " << id << " is in the critical section.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1)); // simulate work
    std::cout << "Worker " << id << " is leaving the critical section.\n";
    sem.release(); // release the permit
}

void wait_for_ready(){
    std::unique_lock<std::mutex> lock(mu);
    cv.wait(lock,[](){ return ready;});
    std::cout<<"Thread proceeding after signal"<<std::endl;

}

void notify_read(){
    
    {
        std::lock_guard<std::mutex> lock(mu);
        ready = true;
    }
    cv.notify_one();
}
void increament(){
     for(int i=0; i<10000;i++){
       // std::this_thread::yield();   // force context switch
       // std::lock_guard<std::mutex> lock(mu); // RAII style locking
       while(flag.test_and_set(std::memory_order_acquire))// spin until the flag is cleared
       {

       } // acquire lock
        counter++;
        flag.clear(std::memory_order_release); // release lock
      
     }        
        
}

int main(){

    std::thread t1(increament);
    std::thread t2(increament);
    std::thread t3(increament);
    std::thread t4(increament);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << "Final counter value: " << counter << std::endl;
    std::cout<<"Thread sync example"<<std::endl;

    
    std::thread waiter(wait_for_ready);
    notify_read();
    waiter.join();

    std::cout<<"Semaphore example"<<std::endl;
    std::thread workers[5];
    for(int i=0;i<5;i++){
        workers[i]= std::thread(worker,i+1);
    }   
    for(int i=0;i<5;i++){
        workers[i].join();
    }       
    

    return 0;
}