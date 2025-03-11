initialize thread
 - [initialize thread](#initializethread)
 - [DataRace](#DataRace)
 - [CriticalSection](#CriticalSection)
 - [Deadlock](#Deadlock)
 - [lock_guard](#lock_guard)
 - [unique_lock](#unique_lock)
 - [shared_timed_mutex](#shared_timed_mutex)
 - [call_once](#call_once)
 - [async](#async)
 - [packaged_task](#packaged_task)
 - [shared_future](#shared_future)
     
 - [thread_local](#thread_local)
 - [](#)
 - [](#)
 - [](#)
 - [](#)
 - [](#)
 - [](#)
 - [](#)
 - [](#)
 - [](#)
 - 

### initialize thread  
initialize thread using callable object
1. function
2. function object
3. lambda
```c++
#include <atomic>
#include <thread>
void function()
{
  std::cout << "Hello C++11 from function." << std::endl;
}
class functionobj
{
public:
        void operator()() const
        {
                std::cout << "Hello C++11 from functionobj." << std::endl;
        }
};

int main() {
      std::cout << std::endl;
       std::thread t1(function);
       functionobj obj;
       std::thread t2(obj);
       std::thread t3([]() {
               std::cout << "Hello C++11 from lambda." << std::endl;
       });
       // ensure that t1, t2 and t3 have finished before main terminates
       t1.join();
       t2.join();
       t3.join();
       std::cout << std::endl;
}
```
t.join()
t.detach()

### DataRace
   A data race is a state in which at least two threads access shared data simultaneously, and at least one of the threads is a writer.
### CriticalSection
   A critical section is a section of the code, that not more than one thread should access at any point in time.
```c++
std::mutex 
  lock()
  unlock()
```
### Deadlock
  A deadlock is a state in which at least two threads are blocked because each thread is waiting for the release of some resource with which the other thread works before it releases its resource.

```c++
std::mutex m;
m.lock();
sharedVariable= getVar();//exception throw by getVar()
m.unlock();
```
solution:
Locks:
###  std::lock_guard
 - Use it when you need automatic locking and unlocking without any control over unlocking.
 - Fast and lightweight, as it only acquires the lock and releases it when out of scope.
 lock automatically binds its mutex in the constructor and releases it in the destructor.
```c++
{
  std::mutex m,
  std::lock_guard<std::mutex> lockGuard(m);
  sharedVariable= getVar();
}
```

```c++
struct CriticalData{
  std::mutex mut;
};
  a.mut.lock();
  std::cout << "get the first mutex" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  b.mut.lock();
  std::cout << "get the second mutex" << std::endl;
  // do something with a and b
  a.mut.unlock();
  b.mut.unlock();
```
 ### unique_lock
Use std::unique_lock when you need more flexibility, such as:
- Deferring locking (std::defer_lock)
- Manually unlocking and relocking
- Moving ownership of the lock

```C++
void deadLock(CriticalData& a, CriticalData& b){

  std::unique_lock<std::mutex>guard1(a.mut,std::defer_lock);
  std::cout << "Thread: " << std::this_thread::get_id() << " first mutex" <<  std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(1));

  std::unique_lock<std::mutex>guard2(b.mut,std::defer_lock);
  std::cout << "    Thread: " << std::this_thread::get_id() << " second mutex" <<  std::endl;

  std::cout << "        Thread: " << std::this_thread::get_id() << " get both mutex" << std::endl;
  std::lock(guard1,guard2);
  // do something with a and b
}
```
### std::shared_timed_mutex
read-write mutex that allows:
Multiple threads to acquire a shared (read) lock at the same time.
Only one thread to acquire an exclusive (write) lock at a time.
Timed locking functions for both shared and exclusive access.
#include <iostream>
#include <thread>
#include <shared_mutex>
#include <chrono>

```C++
std::shared_timed_mutex rw_mutex;
int shared_data = 0;

void reader(int id) {
    std::shared_lock<std::shared_timed_mutex> lock(rw_mutex);  // Shared (read) lock
    std::cout << "Reader " << id << " read value: " << shared_data << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void writer(int id) {
    std::unique_lock<std::shared_timed_mutex> lock(rw_mutex);  // Exclusive (write) lock
    shared_data++;
    std::cout << "Writer " << id << " updated value to: " << shared_data << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
    std::thread t1(reader, 1);
    std::thread t2(reader, 2);
    std::thread t3(writer, 1);
    std::thread t4(reader, 3);
    std::thread t5(writer, 2);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
```
### std::call_once
 Using the std::call_once function, you can register all callables. The std::once_flag ensures that only one registered function will be invoked. 

 ```C++
std::once_flag onceFlag;
 
void do_once(){
  std::call_once(onceFlag, [](){ std::cout << "Only once." << std::endl; });
}
  std::thread t1(do_once);
  std::thread t2(do_once);
  std::thread t3(do_once);
  std::thread t4(do_once);
 ```
### thread_local
When needed, thread local data will be created for each thread. Thread-local data exclusively belongs to the thread and behaves like static data. That means it will be created at its first usage, and its lifetime is bound to the thread’s lifetime.

```c++
thread_local std::string s("hello from ");
```
### std::condition_variable
Condition variables allow us to synchronize threads via notifications.
The receiver waits for the sender’s notification. If the receiver gets the notification, it continues its work.
```c++
std::mutex mutex_;
std::condition_variable condVar;
bool dataReady;
void doTheWork(){
  std::cout << "Processing shared data." << std::endl;
}
void waitingForWork(){
    std::unique_lock<std::mutex> lck(mutex_);
    condVar.wait(lck,[]{ return dataReady;}); // check spurious wakeup (an unexpected wakeup without notification).
    doTheWork();
}
void setDataReady(){
    {
     std::lock_guard<std::mutex> lck(mutex_);
     dataReady=true;
    }
    condVar.notify_one();
}
  std::thread t1(waitingForWork);
  std::thread t2(setDataReady);
  t1.join();
  t2.join();
```
### std::promise and std::future
 The sender is called promise, the receiver – future
sender can provide the value for more than one future. Besides a value, the sender can also provide a notification or an exception.
Tasks are available in three variations. 
asynchronous function call with std::async
simple wrapper for a callable with std::packaged_task
```c++
int res;
std::thread t([&]{res= 3+4;});
t.join();
std::cout << res << std:::endl;

auto fut=std::async([]{return 3+4;});
std::cout << fut.get() << std::endl;
```
### std::async
std::async gets a callable as a work package( it’s a function, a function object, or a lambda function.)
The promise immediately starts to execute its work package
With the flag std::launch::async std::async will run its work package in a new thread
std::launch::deferred expresses that std::async runs in the same thread.
std::async call generates a data channel with both endpoints fut and std::async. 
fut is a future, std::async  is a promise.
A task will not automatically generate a thread. Specifically, the C++ runtime decides if a thread should be created.
Reasons for the decision are: How heavy is the payload? How many cores are available? How high is the system load?
```c++
 std::future<int> fut = std::async(std::launch::async, compute, 5);
 std::cout << "Result = " << fut.get() << std::endl; // Retrieve the result
```
### std::packaged_task
wraps a callable (like a function, lambda, or functor) and allows its result to be retrieved asynchronously via a std::futur
```c++
  std::packaged_task<int(int)> task(compute); // Wrap the task
    std::future<int> fut = task.get_future();   // Get the future

    std::thread t(std::move(task), 5);          // Run it in a new thread

    std::cout << "Result = " << fut.get() << std::endl; // Retrieve the result

    t.join();  // Join the thread
```
| Feature                | std::packaged_task                                | std::async                                           |
|------------------------|---------------------------------------------------|------------------------------------------------------|
| Control                | Manual execution control                          | Auto-scheduled execution                             |
| Thread Management      | Manually assign to thread or execute directly     | Auto thread management (lazy or eager)               |
| Flexibility            | Ideal for custom thread pools                     | Less flexible, for simple async tasks                |
| Ease of Use            | Requires explicit setup                          | Simple, one-liner execution                          |
| Execution Guarantee    | Runs only when explicitly invoked                | Immediate or lazy, based on launch policy            |
| Launch Policy          | Manual control, no built-in policy               | Supports `std::launch::async`                        |
| Return Value           | Returns `std::future`                             | Automatically returns `std::future`                  |
| Thread Pool Usage      | Preferred for custom pools                        | Less ideal due to automatic thread creation          |

### std::shared_future
std::shared_future<int> shared_fut= prodResult.share();
Multiple threads can call .get() multiple times.
Remains valid even after .get().
```C++
std::shared_future<int> shared_fut = fut.share();
```
