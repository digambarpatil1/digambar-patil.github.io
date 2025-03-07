initialize thread
 - [initialize thread](#initializethread)
 - [DataRace](#DataRace)
 - [CriticalSection](#CriticalSection)
 - [Deadlock](#Deadlock)
    - [std::lock_guard](#std::lock_guard)
    - [std::unique_lock](#std::unique_lock)
 - [](#)
 - [](#)
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
### Deadlock(issue with mutex)
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
 ### std::unique_lock
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
