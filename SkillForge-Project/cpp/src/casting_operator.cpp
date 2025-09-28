#include<iostream>
class Base {
public:
    virtual void show() { std::cout << "Base class show function called." << std::endl; }
    virtual ~Base() = default;
};
class Derived : public Base {
public:
    void show() override { std::cout << "Derived class show function called." << std::endl; }
};


int main() {
    int a = 10;
    double b = static_cast<double>(a); // Explicit cast from int to double
    std::cout << "Integer: " << a << ", Double: " << b << std::endl;

    void* ptr = &a;
    int* intPtr = static_cast<int*>(ptr); // Safe cast from void* to int*
    std::cout << "Value via intPtr: " << *intPtr << std::endl;

    Base* basePtr = new Base() ;
    Derived* derivedPtr = dynamic_cast<Derived*>(basePtr); // Safe downcast
    if (derivedPtr) {
        derivedPtr->show();
    } else {
        std::cout << "Dynamic cast failed." << std::endl;
    }
    delete basePtr;

    

    return 0;
}