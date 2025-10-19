// threadsync.cpp containes examples of various thread synchronization mechanisms in C++20
// such as mutex, atomic, condition variable, and semaphore.


#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include<condition_variable>
#include<semaphore>

// Mutex example
std::mutex mu;
int counter = 0;
void increament(){
    for(int i=0;i<100000;i++){
        std::lock_guard<std::mutex> lock(mu); // locks automatically, unlocks at end of scope
        counter++;
    }
}
// Atomic example
std::atomic<int> atomic_counter(0);
void atomic_increament(){
    for(int i=0;i<100000;i++){
        atomic_counter++;
    }
}

// atomic flag example
std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;
void atomic_flag_increament(){
    for(int i=0;i<100000;i++){
        while(lock_flag.test_and_set(std::memory_order_acquire)); // spin until lock is acquired
        counter++;
        lock_flag.clear(std::memory_order_release); // release lock
    }
}

//semaphore example
std::counting_semaphore<3> sem(3); // allow up to 3 concurrent accesses
std::atomic<int> sem_counter(0);
void semaphore_increament(){
    for(int i=0;i<10;i++){
        sem.acquire(); // acquire a permit
        std::cout << "Thread " << std::this_thread::get_id()
                  << " in critical section " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        sem_counter++;
        sem.release(); // release the permit
    }
}

//condition variable example
std::condition_variable cv;
bool ready = false;
void wait_for_ready(){
    std::unique_lock<std::mutex> lock(mu);
    cv.wait(lock, []{return ready;}); // wait until ready is true
    // proceed with work
    std::cout << "Thread : Proceeding after signal.\n";
}   



int main(){

   
    std::cout << "Final counter value: " << counter << std::endl;
    std::cout<<"Thread sync example"<<std::endl;
    std::thread workers[5];
    for(int i=0;i<5;i++){
        //workers[i]= std::thread(increament); // mutex version
        //workers[i]= std::thread(atomic_increament); // atomic version
        //workers[i]= std::thread(atomic_flag_increament); // atomic flag version
        workers[i]= std::thread(semaphore_increament); // semaphore version
        //workers[i]= std::thread(wait_for_ready); // condition variable version
    }  
    //condition variable version
    // simulate some work in main thread
    /*
    std::this_thread::sleep_for(std::chrono::seconds(1)); // simulate some work 
    {
        std::lock_guard<std::mutex> lock(mu);
        ready = true; // set the condition
    }
    cv.notify_all(); // wake up all waiting threads */

    for(int i=0;i<5;i++){
        workers[i].join();
    }       
   // std::cout << "Final counter value: " << counter << std::endl;// mutex version
   // std::cout << "Final atomic counter value: " << atomic_counter.load() << std::endl;// atomic version
   // std::cout << "Final counter value using atomic flag: " << counter << std::endl;// atomic flag version 
    std::cout << "Final counter value using semaphore: " << sem_counter << std::endl;// semaphore version
   return 0;
}