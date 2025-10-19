//threadpool.cpp contains a simple thread pool implementation in C++20
// It demonstrates task queuing, worker threads, and synchronization.

#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

class ThreadPool {
    std::queue<std::function<void()>> tasks;// Task queue

    std::vector<std::thread> threads;// Worker threads
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> stop;

public:
    ThreadPool(size_t size) : stop(false) {
        for (size_t i = 0; i < size; ++i) {
            threads.emplace_back([this] { workerThread(); });
        }
    }

    void workerThread() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this] { return stop || !tasks.empty(); });
                if (stop && tasks.empty()) return;
                task = tasks.front();
                tasks.pop();
            }
            task(); // Execute task outside lock
        }
    }

    void enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks.push(task);
        }
        cv.notify_one();
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        for (auto &t : threads) t.join();
    }
};

// Simulated work (with delay)
void printNumber(int n) {
    std::cout << "Task " << n << " started in thread "
              << std::this_thread::get_id() << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work
    std::cout << "Task " << n << " finished in thread "
              << std::this_thread::get_id() << "\n";
}

int main() {
    ThreadPool pool(3); // 3 worker threads

    for (int i = 1; i <= 6; ++i) {
        pool.enqueue([i] { printNumber(i); });
    }

    std::this_thread::sleep_for(std::chrono::seconds(4)); // wait for tasks
    return 0;
}
/*
Task Queue       Worker 1        Worker 2        Worker 3
--------------------------------------------------------------
enqueue(Task1)   waiting         waiting         waiting
→ notify         pick Task1      waiting         waiting
                 run(Task1)      
enqueue(Task2)   busy            waiting         waiting
→ notify                        pick Task2      
                               run(Task2)       
enqueue(Task3)   busy            busy            waiting
→ notify                                      pick Task3
                                               run(Task3)
...
Stop signal      exit loop       exit loop       exit loop
--------------------------------------------------------------
*/