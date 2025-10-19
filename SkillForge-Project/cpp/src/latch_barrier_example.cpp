#include <iostream>
#include<vector>
#include <thread>
#include <chrono>
#include <latch> // For std::latch, use <latch> in MSVC implementations
#include <barrier> // For std::latch, use <barrier> in GCC/libstdc++ implementations

void latch_example(){
  constexpr  int N=5;
  std::latch latch(N);
  std::vector<std::thread> threads;
  for (int i = 0; i < N; i++) {
     threads.emplace_back([=,&latch]{
          std::cout << "Thread " << i << " ready\n";
         latch.count_down();
         latch.wait();
           std::cout << "thread" << i << " started"
                    << std::this_thread::get_id() << "\n";
      }
     );
  }
  for( auto& t :threads)
  t.join();

  std::this_thread::sleep_for(std::chrono::seconds(4)); // wait for tasks
  std::cout << "All tasks completed.\n";
}
void barrier_example(){
  constexpr  int N=5;
  std::barrier barrier(N, [](){ std::cout << "All threads reached the barrier. Proceeding...\n"; });
  std::vector<std::thread> threads;
  for (int i = 0; i < N; i++) {
     threads.emplace_back([=,&barrier]{
          std::cout << "Thread " << i << " ready\n";
         barrier.arrive_and_wait();
           std::cout << "thread" << i << " started"
                    << std::this_thread::get_id() << "\n";
      }
     );
  }
  for( auto& t :threads)
  t.join();

  std::this_thread::sleep_for(std::chrono::seconds(4)); // wait for tasks
  std::cout << "All tasks completed.\n";
}

int main()
{

    latch_example();
    barrier_example();

  
  return 0;
}
//g++ -std=c++20 -pthread main.cpp -o main