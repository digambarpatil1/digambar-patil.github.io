#include<iostream>

class singleton{
    private:
        singleton(){
            std::cout<<"Singleton Instance created"<<std::endl;
        }
        singleton(const singleton&) = delete; // prevent copy
        singleton& operator=(const singleton&) = delete; // prevent assignment
    public:
    static singleton& getInstance()
    {
        static singleton instance;// created on first call, thread-safe in C++11 and later
        return instance;
    }
    void doSomething() {
        std::cout << "Doing something in Singleton instance." << std::endl;
    }
};

int main()
{
    std::cout<<"Singleton Design Pattern"<<std::endl;
    singleton& s1 = singleton::getInstance();
    singleton& s2 = singleton::getInstance();
    s1.doSomething();

    if (&s1 == &s2) {
        std::cout << "Both pointers point to the same instance.\n";
    }
    return 0;
}