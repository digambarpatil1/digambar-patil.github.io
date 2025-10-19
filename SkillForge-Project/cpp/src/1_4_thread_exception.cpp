#include <iostream>
#include <thread>
#include <future>
#include <exception>

std::exception_ptr ptr = nullptr;

void worker()
{
    try
    {
        throw std::runtime_error("exception inside thread");
    }
    catch (...)
    {
        ptr = std::current_exception();  // capture exception
    }
}

int main()
{
    // --- async example ---
    try
    {
        auto f = std::async(std::launch::async, [] {
            throw std::runtime_error("runtime fail");
        });
        f.get(); // rethrows exception here
    }
    catch (const std::exception &e)
    {
        std::cout << "Caught from async: " << e.what() << std::endl;
    }

    // --- thread example ---
    std::thread t(worker);
    t.join();  // ✅ wait for thread to finish before checking exception

    if (ptr)
    {
        try
        {
            std::rethrow_exception(ptr);
        }
        catch (const std::exception &e)
        {
            std::cout << "Caught from thread: " << e.what() << std::endl;
        }
    }

    return 0;
}
