# C++11

## Overview
C++11 includes the following new language features:
 - [move semantics](#move-semantics)
 - [Rvalue references](#Rvalue-references)

### Move semantics
Moving an object means to transfer ownership of some resource it manages to another object.

The first benefit of move semantics is performance optimization. When an object is about to reach the end of its lifetime, either because it's a temporary or by explicitly calling `std::move`, a move is often a cheaper way to transfer resources. For example, moving a `std::vector` is just copying some pointers and internal state over to the new vector -- copying would involve having to copy every single contained element in the vector, which is expensive and unnecessary if the old vector will soon be destroyed.

Moves also make it possible for non-copyable types such as `std::unique_ptr`s ([smart pointers](#smart-pointers)) to guarantee at the language level that there is only ever one instance of a resource being managed at a time, while being able to transfer an instance between scopes.
```c++
#include <iostream>
#include <vector>

class Data {
public:
    std::vector<int> vec;

    // Constructor
    Data(int size) : vec(size) {
        std::cout << "Constructor Called\n";
    }

    // Move Constructor
    Data(Data&& other) noexcept { 
        std::cout << "Move Constructor Called\n";// 
        vec = std::move(other.vec);//The large std::vector is moved instead of copied, 
                                                // avoiding a deep copy.
    }
    
      // Move Assignment Operator
    Data& operator=(Data&& other) noexcept {
        std::cout << "Move Assignment Called\n";
        if (this != &other) {
            vec = std::move(other.vec); //  The assignment operator moves the vector instead of copying it.
                                       // This ensures efficient resource transfer.
        }
        return *this;
    }
};

Data createData(int size) {
    return Data(size); // Move semantics applied automatically
}
int main() {
    Data d1(1000000);   // Constructor called
    Data d2 = std::move(d1);  // std::move(d1) converts d1 into an rvalue, triggering the move constructor
    d2 = std::move(d1);  // std::move(d1) converts d1 into an rvalue, triggering the move constructor
    Data D3 = createData(10000000);
    return 0;
}
```

### Rvalue references
C++11 introduces a new reference termed the rvalue reference. An rvalue reference to T, which is a non-template type parameter (such as int, or a user-defined type), is created with the syntax T&&. Rvalue references only bind to rvalues.

Type deduction with lvalues and rvalues:
```c++
int x = 0; // `x` is an lvalue of type `int`
int& xl = x; // `xl` is an lvalue of type `int&`
int&& xr = x; // compiler error -- `x` is an lvalue
int&& xr2 = 0; // `xr2` is an lvalue of type `int&&` -- binds to the rvalue temporary, `0`

void f(int& x) {}
void f(int&& x) {}

f(x);  // calls f(int&)
f(xl); // calls f(int&)
f(3);  // calls f(int&&)
f(std::move(x)); // calls f(int&&)

f(xr2);           // calls f(int&)
f(std::move(xr2)); // calls f(int&& x)
```
