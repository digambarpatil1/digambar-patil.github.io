initialize thread
 - [Initialize thread](#initializethread)
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
 - [promise and future](#promise_and_future)
 - [memory ordering constraints](#memory-ordering-constraints)
 - [Fences-atomic_thread_fence](#Fences-atomic_thread_fence)
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
###  lock_guard
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
### shared_timed_mutex
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
### call_once
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
### condition_variable
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
### promise_and_future
 The sender is called promise, the receiver – future
sender can provide the value for more than one future. Besides a value, the sender can also provide a notification or an exception.
Tasks are available in three variations. 
asynchronous function call with std::async
simple wrapper for a callable with std::packaged_task
They communicate between threads by passing values or exceptions between threads.
std::promise is used to set a value or exception to be retrieved later by a std::future
Once a std::promise has a value set (via set_value() or set_exception()), the associated std::future can retrieve that value.
```c++
int res;
std::thread t([&]{res= 3+4;});
t.join();
std::cout << res << std:::endl;

auto fut=std::async([]{return 3+4;});
std::cout << fut.get() << std::endl;

**promise_and_future**
void producer(std::promise<int>& prom) {
    // Simulate some computation and set the value
    std::this_thread::sleep_for(std::chrono::seconds(2));
    prom.set_value(42);  // Setting the value
}
void consumer(std::future<int>& fut) {
    // Get the value from the future, which is set by the producer
    int result = fut.get();  // This will block until the value is available
    std::cout << "Received result: " << result << std::endl;
}
    // Create a promise and a future
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();  // Get future associated with the promise

    // Launch the producer and consumer threads
    std::thread t1(producer, std::ref(prom));
    std::thread t2(consumer, std::ref(fut));

    // Join the threads
    t1.join();
    t2.join();
```
### async
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
### packaged_task
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

### shared_future
std::shared_future<int> shared_fut= prodResult.share();
Multiple threads can call .get() multiple times.
Remains valid even after .get().
```C++
std::shared_future<int> shared_fut = fut.share();
```
# memory ordering constraints
Modern compilers and CPUs often optimize code for better performance by reordering operations, especially when they are not explicitly dependent on each other. This is great for single-threaded performance, but in multi-threaded code, it can lead to issues if operations that need to be synchronized across threads are reordered.

- **`memory_order_relaxed`**:
  - Use for maximum performance when strict ordering between threads is not necessary.
  - Ideal for atomic operations like counters or flags where only atomicity is required.
  
- **`memory_order_acquire`** and **`memory_order_release`**:
  - Use for straightforward synchronization when data depends on the state of an atomic variable.
  - **`memory_order_acquire`**: Ensures that all operations before the atomic load are visible after the load.
  - **`memory_order_release`**: Ensures that all operations before the atomic store are visible before the store.

- **`memory_order_acq_rel`**:
  - Use when an operation involves both reading and writing, and you want synchronization on both sides.
  - Combines the effects of **`memory_order_acquire`** and **`memory_order_release`** in one operation.

- **`memory_order_seq_cst`**:
  - Use when you need global consistency across threads.
  - Ensures that all threads see operations in the same order.
  - Provides the strictest ordering, useful when sequential consistency is critical.

## When to Use Lock-Free (Atomics)
- **High performance & low latency**: Avoids context switching and blocking.
- **Minimal contention**: Suitable when threads infrequently access the same resource.
- **Simple atomic operations**: Works well for counters, flags, and single-variable state tracking.
- **Non-blocking algorithms**: Used in lock-free data structures (e.g., queues, stacks).
- **Scalability**: Reduces contention in multi-threaded environments with many CPU cores.
- **Avoiding priority inversion**: Ensures that high-priority threads are not blocked by lower-priority ones.

## When to Use Mutexes
- **Complex critical sections**: When multiple operations need to be performed atomically as a unit.
- **Protecting shared resources**: When multiple threads need synchronized access to complex data structures.
- **Blocking synchronization**: When threads must wait for an event or condition to be met.
- **Simpler to implement & maintain**: Easier to reason about than lock-free programming, which requires careful ordering and atomic operations.
- **Preventing data races in non-trivial cases**: Atomics only work well for simple shared data; mutexes provide broader safety for more complex cases.
### Fences-atomic_thread_fence
When using non-atomic shared variables that need ordering guarantees but aren't protected by atomics.
When atomic variables are not enough: You need ordering across multiple memory operations beyond just atomic reads/writes.
When ensuring ordering constraints across separate atomic operations instead of a single one.
For implementing higher-level synchronization primitives (e.g., spinlocks, lock-free data structures).
