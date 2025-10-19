#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <numeric>
#include <future>
#include <barrier>
#include <latch>

void threadfun(const std::string& msg) {
  std::cout << "Thread function executing" << msg << std::endl;
}

int sum(std::vector<int>& v, auto start, auto end) {
  return std::accumulate(v.begin() + start, v.begin() + end, 0);
}


int main() {
  // using normal function as thread function
  std::thread t1(threadfun, "hello thread");
  t1.join();
  std::cout << "Main function executing" << std::endl;

  // using lambda function as thread function
  std::thread t2([](const std::string& msg) {
    std::cout << "Lambda thread function executing: " << msg << std::endl;
  }, "Hello lambda thread");
  t2.join();

  
  int result1 = 0, result2 = 0;
  auto startthread = std::chrono::high_resolution_clock::now();

  // using multiple threads to sum elements of a large vector
  std::vector<int> vec(1000000, 1);
  std::thread t3([&]() {
    result1 = sum(vec, 0, vec.size() / 2);
  });

  std::thread t4([&]() {
    result2 = sum(vec, vec.size() / 2, vec.size());
  });

  t3.join();
  t4.join();
  std::cout << "Sum of vector elements: " << result1 + result2 << std::endl;
  auto endthread = std::chrono::high_resolution_clock::now();

  // using promise and future to get result from thread
  std::cout << "Time taken using threads: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endthread - startthread).count()
        << " ms" << std::endl;
  auto starttask = std::chrono::high_resolution_clock::now();

  auto f1 = std::async(std::launch::async, [&]() {
    return sum(vec, 0, vec.size() / 2);
  });

  auto f2 = std::async(std::launch::async, [&]() {
    return sum(vec, vec.size() / 2, vec.size());
  });

  std::cout << "Sum of vector elements using task: " << f1.get() + f2.get() << std::endl;
  auto endtask = std::chrono::high_resolution_clock::now();
  std::cout << "Time taken using tasks: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(endtask - starttask).count()
        << " ms" << std::endl;
  return 0;
}
