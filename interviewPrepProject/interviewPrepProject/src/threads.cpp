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

// function to compute square and set value in promise
// reference is used to avoid copying
void square(int& x, std::promise<int>& p) {
  x = x * x;
  p.set_value(x);
}

int add(int a, int b) {
  return a + b;
}

int main() {
  std::thread t1(threadfun, "hello thread");
  t1.join();
  std::cout << "Main function executing" << std::endl;

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

  // using async and future
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

  // using promise and future
  int value = 5;
  std::promise<int> prom;
  std::future<int> fut = prom.get_future();
  std::thread t5(square, std::ref(value), std::ref(prom));
  std::cout << "Square of value using promise/future: " << fut.get() << std::endl;
  t5.join();

  // using packaged_task
  std::packaged_task<int(int,int)> task(add);
  std::future<int> fut2 = task.get_future();
  std::thread t6(std::move(task), 10, 20);
  std::cout << "Sum using packaged_task: " << fut2.get() << std::endl;
  t6.join();

  const int number_of_threads = 4;
  //std::barrier sync_point(number_of_threads);
  std::latch sync_point(number_of_threads);
  std::vector<std::thread> threads;
  for (int i = 0; i < number_of_threads; ++i) {
    threads.emplace_back([i,&sync_point]() {
      //std::this_thread::sleep_for(std::chrono::milliseconds(10 * (i + 1))); // simulate work
      std::cout << "Thread " << i << " reached the barrier." << std::endl;
      sync_point.arrive_and_wait(); // wait for other threads
      std::cout << "Thread " << i << " passed the barrier." << std::endl;

    });
  }
  for (auto& t : threads) t.join();


  return 0;
}
