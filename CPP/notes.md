# C++11

## Overview
C++11 includes the following new language features:
 - [move semantics](#move-semantics)
 - [Rvalue references](#Rvalue-references)
 - [ Variadic templates](#Variadic-templates)
 - [Initializer lists](#Initializer-lists)

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
### Variadic templates
Variadic templates enable functions or classes to accept a variable number of argument
The ... syntax creates a parameter pack or expands one. A template parameter pack is a template parameter that accepts zero or more template arguments (non-types, types, or templates). A template with at least one parameter pack is called a variadic template.
```
template <typename... Args>
void print(Args... args) {
    (std::cout << ... << args) << "\n";  // Fold expression (C++17)
}

int main() {
    print(1, 2.5, "Hello", 'A');  // Outputs: 12.5HelloA
}

template <typename... T>
struct arity {
  constexpr static int value = sizeof...(T);
};
static_assert(arity<>::value == 0);
static_assert(arity<char, short, int>::value == 3);
An interesting use for this is creating an initializer list from a parameter pack to iterate over variadic function arguments.

template <typename First, typename... Args>
auto sum(const First first, const Args... args) -> decltype(first) {
  const auto values = {first, args...};
  return std::accumulate(values.begin(), values.end(), First{0});
}

sum(1, 2, 3, 4, 5); // 15
sum(1, 2, 3);       // 6
sum(1.5, 2.0, 3.7); // 7.2
```
### Initializer lists
An initializer list in C++ is a container that holds a collection of values that are initialized in a uniform manner. It is a part of the C++ standard library, introduced in C++11, and is typically used for initialization purposes, such as initializing arrays or containers.
A lightweight array-like container of elements created using a "braced list" syntax. For example, { 1, 2, 3 } creates a sequences of integers, that has type std::initializer_list<int>. Useful as a replacement to passing a vector of objects to a function.
```
int sum(const std::initializer_list<int>& list) {
  int total = 0;
  for (auto& e : list) {
    total += e;
  }

  return total;
}

auto list = {1, 2, 3};
sum(list); // == 6
sum({1, 2, 3}); // == 6
sum({}); // == 0
```
