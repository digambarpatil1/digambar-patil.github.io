
How to initialize thread using callable object
1. function
2. function object
3. lambda

t.join()
t.detach()

### Data Race
    A data race is a state in which at least two threads access shared data simultaneously, and at least one of the threads is a writer.
### Critical Section
    A critical section is a section of the code, that not more than one thread should access at any point in time.
 
std::mutex 
  lock()
  unlock()
  
### Deadlock(issue with mutex)
   A deadlock is a state in which at least two threads are blocked because each thread is waiting for the release of some resource with which the other thread works before it releases its resource.

std::mutex m;
m.lock();
sharedVariable= getVar();//exception throw by getVar()
m.unlock();

solution:
Locks:
 lock automatically binds its mutex in the constructor and releases it in the destructor. 

{
  std::mutex m,
  std::lock_guard<std::mutex> lockGuard(m);
  sharedVariable= getVar();
}

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


std::lock_guard
