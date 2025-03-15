# C++ Templates Overview

C++ templates are a powerful feature, enabling:

- [Generic Programming (Function/Class Templates)](#generic-programming-functionclass-templates)
- [Type-Specific Customization (Specialization)](#type-specific-customization-specialization)
- [Compile-Time Computations (Metaprogramming)](#compile-time-computations-metaprogramming)
- [Enforcing Constraints (SFINAE, Concepts)](#enforcing-constraints-sfinae-concepts)
- [Efficient Static Polymorphism (CRTP)](#efficient-static-polymorphism-crtp)

---

### Generic Programming Function/Class Templates
Templates allow writing code that works with multiple data types, increasing reusability.
```cpp
// Function Template
template <typename T>
T sum(T a, T b) {
    return a + b;
}

// Class Template
template <typename T>
class test {
public:
    T sum(const T& a, const T& b) {
        return a + b;
    }
};
  test<int> t;
  std::cout << "sum = " << t.sum(30, 20) << std::endl;
```
## Type-Specific Customization (Specialization)
Specialization allows defining custom implementations for specific types.

```cpp
// Specialization for std::string
template <>
class test<std::string> {
public:
    std::string sum(const std::string& a, const std::string& b) {
        return a + " " + b;  // Concatenation with space
    }
};
    test<std::string> strSum("");
   std::cout << "string concatenate sum = " << strSum.sum("digambar", "patil") << std::endl;
```
 **Partial Specialization **
 type is pointer /constant 
 ```C++
 template <typename T>
class Box<T*> {  // Specialization for pointer types
    T* value;
public:
    Box(T* val) : value(val) {}
    T* getValue() { return value; }
};
```
### Variadic Templates (C++11)
Allows templates with a variable number of arguments
```C++
void print()
{
}
template<typename Args ...>
void print(Args... arg)
{
  std::court<<"argument is = "<<arg<<endl;
   print(arg...)
}
```

### Compile-Time Computations (Metaprogramming)
Metaprogramming in C++ allows computations to be performed at compile-time using templates, constexpr, and type traits. This improves performance by eliminating runtime computations and reducing overhead.

```C++
template<int N>
struct factorial
{
    static constexpr int val = N * factorial<N-1>::val;
};

template<>
struct factorial<0>
{
    static constexpr int val = 1;
};
 std::cout << "Factorial of 5: " << factorial<5>::val << std::endl;
```
### Enforcing Constraints (SFINAE, Concepts)
SFINAE is a rule in C++ that allows the compiler to discard invalid template specializations without producing an error. It enables overload selection based on type traits.
Concepts provide a more readable and compiler-friendly way to enforce constraints on template parameters compared to SFINAE. They enable better error messages and improve code clarity.
SFINAE (enable_if) allows fine-grained template constraints but can be verbose.
-  Concepts (C++20) provide cleaner, more readable syntax with better error messages.
- Both techniques prevent invalid template instantiations at compile time.

```C++
#include <iostream>
#include <type_traits>
#include <concepts>
// Enable this function only if T is an arithmetic type (int, float, double, etc.)
template <typename T>
typename std::enable_if_t<std::is_arithmetic_v<T>, T>
add(T a, T b) {
    return a + b;
}
// Enable class only for integral types
template <typename T, typename Enable = void>
class TestClass_enif;

// Specialization for integral types
template <typename T>
class TestClass_enif<T, typename std::enable_if_t<std::is_integral_v<T>>> {
public:
    TestClass_enif(T value) { std::cout << "Integral type: " << value << std::endl; }
};
//concept creation for arithmatic
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;
// Function restricted to Numeric types
template <Numeric T>
T add(T a, T b) {
    return a + b;
}

// Class restricted to integral types
template <Numeric T>
class TestClass {
public:
    TestClass(T value) { std::cout << "Integral type: " << value << std::endl; }
};
int main() {
   std::cout << add(5, 10) << std::endl;       
   std::cout << add(2.5, 3.5) << std::endl;
  // std::cout << add("hi", "world");
   
   TestClass t{3};
   //TestClass tf{"test"};
    return 0;
}


template <typename T>
requires std::integral<T>  // Cleaner and more readable
T square(T x) {
    return x * x;
}

```
### Efficient Static Polymorphism (CRTP)
Curiously Recurring Template Pattern (CRTP) is a powerful technique in C++ that enables static polymorphism, avoiding the runtime overhead of traditional dynamic polymorphism (i.e., virtual functions). It is widely used in performance-critical applications, zero-cost abstractions, and compile-time optimizations
CRTP is a template pattern where a base class takes a derived class as a template parameter..

```C++
//Base<T> is a template class that takes a derived class as a parameter.
template <typename Derived> 
class Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
};

class Derived : public Base<Derived> {
public:
    void implementation() {
        std::cout << "Derived::implementation()" << std::endl;
    }
};
```
