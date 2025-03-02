# C++11

## Overview
C++11 includes the following new language features:
 - [move semantics](#move-semantics)
 - [Rvalue references](#Rvalue-references)
 - [ Variadic templates](#Variadic-templates)
 - [Initializer lists](#Initializer-lists)
 - [Static Assertions](#Static-Assertions)
 - [auto](#auto)
 - [Lambda expressions](#Lambda-expressions)
 - [decltype](#decltype)
 - [type aliases](#type-aliases)
 - [nullptr](#nullptr)
 - [strongly-typed enums](#strongly-typed-enums)
 - [attributes](#attributes)
 - [constexpr](#constexpr)
 - [delegating constructors](#delegating-constructors)
- [user-defined literals](#user-defined-literals)
- [explicit virtual overrides](#explicit-virtual-overrides)
- [final specifier](#final-specifier)
- [default functions](#default-functions)
- [deleted functions](#deleted-functions)
- [range-based for loops](#range-based-for-loops)
- [special member functions for move semantics](#special-member-functions-for-move-semantics)
- [explicit conversion functions](#explicit-conversion-functions)
- [inline-namespaces](#inline-namespaces)
- [non-static data member initializers](#non-static-data-member-initializers)
- [right angle brackets](#right-angle-brackets)
- [ref-qualified member functions](#ref-qualified-member-functions)
- [trailing return types](#trailing-return-types)
- [noexcept specifier](#noexcept-specifier)
- [char32_t and char16_t](#char32_t-and-char16_t)
- [raw string literals](#raw-string-literals)
   
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
```c++
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
```c++
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
### Static Assertions
Static assertions (introduced in C++11) allow you to perform compile-time checks to ensure certain conditions are met. They are evaluated at compile-time, and if the condition fails, the compiler produces an error message. This helps catch errors early in development, before the program even runs.
```c++
static_assert(condition, "error message");
constexpr int x = 0;
constexpr int y = 1;
static_assert(x == y, "x != y");
```
### auto
the compiler deduces auto-typed variables according to the type of their initializer.

```c++
auto a = 3.14; // double
auto b = 1; // int
auto& c = b; // int&
auto d = { 0 }; // std::initializer_list<int>
auto&& e = 1; // int&&
auto&& f = b; // int&
auto g = new auto(123); // int*
const auto h = 1; // const int
auto i = 1, j = 2, k = 3; // int, int, int
auto l = 1, m = true, n = 1.61; // error -- `l` deduced to be int, `m` is bool
auto o; // error -- `o` requires initializer
```
Extremely useful for readability, especially for complicated types:
```c++
std::vector<int> v = ...;
std::vector<int>::const_iterator cit = v.cbegin();
// vs.
auto cit = v.cbegin();
Functions can also deduce the return type using auto. In C++11, a return type must be specified either explicitly, or using decltype like so:

template <typename X, typename Y>
auto add(X x, Y y) -> decltype(x + y) {
  return x + y;
}
add(1, 2); // == 3
add(1, 2.0); // == 3.0
add(1.5, 1.5); // == 3.0
```
The trailing return type in the above example is the declared type (see section on decltype) of the expression x + y. For example, if x is an integer and y is a double, decltype(x + y) is a double. Therefore, the above function will deduce the type depending on what type the expression x + y yields. Notice that the trailing return type has access to its parameters, and this when appropriate.

### Lambda expressions
A lambda is an unnamed function object capable of capturing variables in scope. It features: a capture list; an optional set of parameters with an optional trailing return type; and a body. Examples of capture lists:

* `[]` - captures nothing.
* `[=]` - capture local objects (local variables, parameters) in scope by value.
* `[&]` - capture local objects (local variables, parameters) in scope by reference.
* `[this]` - capture `this` by reference.
* `[a, &b]` - capture objects `a` by value, `b` by reference.

```c++
int x = 1;
auto getX = [=] { return x; };
getX(); // == 1

auto addX = [=](int y) { return x + y; };
addX(1); // == 2

auto getXRef = [&]() -> int& { return x; };
getXRef(); // int& to `x`
```
By default, value-captures cannot be modified inside the lambda because the compiler-generated method is marked as const. The mutable keyword allows modifying captured variables. The keyword is placed after the parameter-list (which must be present even if it is empty).

```c++
int x = 1;

auto f1 = [&x] { x = 2; }; // OK: x is a reference and modifies the original

auto f2 = [x] { x = 2; }; // ERROR: the lambda can only perform const-operations on the captured value
// vs.
auto f3 = [x]() mutable { x = 2; }; // OK: the lambda can perform any operations on the captured value
```
* Short, one-time-use functions (avoid unnecessary function definitions).
* Custom comparisons in algorithms (std::sort, std::find_if).
* Callbacks and event handlers.
* Capturing local variables for short-lived operations.
* Threading with std::thread.
* Functional-style programming (std::for_each, std::transform).

### decltype
allows you to determine the type of an expression at compile time.
decltype is an operator which returns the declared type of an expression passed to it. cv-qualifiers and references are maintained if they are part of the expression. Examples of decltype:
```c++
int a = 1; // `a` is declared as type `int`
decltype(a) b = a; // `decltype(a)` is `int`
const int& c = a; // `c` is declared as type `const int&`
decltype(c) d = a; // `decltype(c)` is `const int&`
decltype(123) e = 123; // `decltype(123)` is `int`
int&& f = 1; // `f` is declared as type `int&&`
decltype(f) g = 1; // `decltype(f) is `int&&`
decltype((a)) h = g; // `decltype((a))` is int&
template <typename X, typename Y>
auto add(X x, Y y) -> decltype(x + y) {
  return x + y;
}
add(1, 2.0); // `decltype(x + y)` => `decltype(3.0)` => `double`
```
```c++
template <typename X, typename Y>
auto add(X x, Y y) -> decltype(x + y) {
  return x + y;
}
add(1, 2.0); // `decltype(x + y)` => `decltype(3.0)` => `double`
```
### Type aliases
Semantically similar to using a typedef however, type aliases with using are easier to read and are compatible with templates.
```c++
template <typename T>
using Vec = std::vector<T>;
Vec<int> v; // std::vector<int>

using String = std::string;
String s {"foo"};
```
### nullptr
C++11 introduces a new null pointer type designed to replace C's NULL macro. nullptr itself is of type std::nullptr_t and can be implicitly converted into pointer types, and unlike NULL, not convertible to integral types except bool.
```c++
void foo(int);
void foo(char*);
foo(NULL); // error -- ambiguous
foo(nullptr); // calls foo(char*)
```
### Strongly-typed enums
Type-safe enums that solve a variety of problems with C-style enums including: implicit conversions, inability to specify the underlying type, scope pollution.
```C++
// Specifying underlying type as `unsigned int`
enum class Color : unsigned int { Red = 0xff0000, Green = 0xff00, Blue = 0xff };
// `Red`/`Green` in `Alert` don't conflict with `Color`
enum class Alert : bool { Red, Green };
Color c = Color::Red;
```
### Attributes
Attributes provide a universal syntax over __attribute__(...), __declspec, etc.
```c++
// `noreturn` attribute indicates `f` doesn't return.
[[ noreturn ]] void f() {
  throw "error";
}
```
### constexpr
Constant expressions are expressions that are possibly evaluated by the compiler at compile-time. Only non-complex computations can be carried out in a constant expression (these rules are progressively relaxed in later versions). Use the constexpr specifier to indicate the variable, function, etc. is a constant expression.
improving performance and enabling optimizations.
```c++
constexpr int square(int x) {
  return x * x;
}

int square2(int x) {
  return x * x;
}

int a = square(2);  // mov DWORD PTR [rbp-4], 4

int b = square2(2); // mov edi, 2
                    // call square2(int)
                    // mov DWORD PTR [rbp-8], eax
```
In the previous snippet, notice that the computation when calling square is carried out at compile-time, and then the result is embedded in the code generation, while square2 is called at run-time.

constexpr values are those that the compiler can evaluate, but are not guaranteed to, at compile-time:
```c++
const int x = 123;
constexpr const int& y = x; // error -- constexpr variable `y` must be initialized by a constant expression
Constant expressions with classes:

struct Complex {
  constexpr Complex(double r, double i) : re{r}, im{i} { }
  constexpr double real() { return re; }
  constexpr double imag() { return im; }

private:
  double re;
  double im;
};

constexpr Complex I(0, 1);
```
### Delegating constructors
Constructors can now call other constructors in the same class using an initializer list.
```c++
struct Foo {
  int foo;
  Foo(int foo) : foo{foo} {}
  Foo() : Foo(0) {}
};

Foo foo;
foo.foo; // == 0
```

### User-defined literals
User-defined literals allow you to extend the language and add your own syntax. To create a literal, define a `T operator "" X(...) { ... }` function that returns a type `T`, with a name `X`. Note that the name of this function defines the name of the literal. Any literal names not starting with an underscore are reserved and won't be invoked. There are rules on what parameters a user-defined literal function should accept, according to what type the literal is called on.

Converting Celsius to Fahrenheit:
```c++
// `unsigned long long` parameter required for integer literal.
long long operator "" _celsius(unsigned long long tempCelsius) {
  return std::llround(tempCelsius * 1.8 + 32);
}
24_celsius; // == 75
```

String to integer conversion:
```c++
// `const char*` and `std::size_t` required as parameters.
int operator "" _int(const char* str, std::size_t) {
  return std::stoi(str);
}

"123"_int; // == 123, with type `int`
```

### Explicit virtual overrides
Specifies that a virtual function overrides another virtual function. If the virtual function does not override a parent's virtual function, throws a compiler error.
```c++
struct A {
  virtual void foo();
  void bar();
};

struct B : A {
  void foo() override; // correct -- B::foo overrides A::foo
  void bar() override; // error -- A::bar is not virtual
  void baz() override; // error -- B::baz does not override A::baz
};
```

### Final specifier
Specifies that a virtual function cannot be overridden in a derived class or that a class cannot be inherited from.
```c++
struct A {
  virtual void foo();
};

struct B : A {
  virtual void foo() final;
};

struct C : B {
  virtual void foo(); // error -- declaration of 'foo' overrides a 'final' function
};
```

Class cannot be inherited from.
```c++
struct A final {};
struct B : A {}; // error -- base 'A' is marked 'final'
```

### Default functions
A more elegant, efficient way to provide a default implementation of a function, such as a constructor.
```c++
struct A {
  A() = default;
  A(int x) : x{x} {}
  int x {1};
};
A a; // a.x == 1
A a2 {123}; // a.x == 123
```

With inheritance:
```c++
struct B {
  B() : x{1} {}
  int x;
};

struct C : B {
  // Calls B::B
  C() = default;
};

C c; // c.x == 1
```

### Deleted functions
A more elegant, efficient way to provide a deleted implementation of a function. Useful for preventing copies on objects.
```c++
class A {
  int x;

public:
  A(int x) : x{x} {};
  A(const A&) = delete;
  A& operator=(const A&) = delete;
};

A x {123};
A y = x; // error -- call to deleted copy constructor
y = x; // error -- operator= deleted
```

### Range-based for loops
Syntactic sugar for iterating over a container's elements.
```c++
std::array<int, 5> a {1, 2, 3, 4, 5};
for (int& x : a) x *= 2;
// a == { 2, 4, 6, 8, 10 }
```

Note the difference when using `int` as opposed to `int&`:
```c++
std::array<int, 5> a {1, 2, 3, 4, 5};
for (int x : a) x *= 2;
// a == { 1, 2, 3, 4, 5 }
```

### Special member functions for move semantics
The copy constructor and copy assignment operator are called when copies are made, and with C++11's introduction of move semantics, there is now a move constructor and move assignment operator for moves.
```c++
struct A {
  std::string s;
  A() : s{"test"} {}
  A(const A& o) : s{o.s} {}
  A(A&& o) : s{std::move(o.s)} {}
  A& operator=(A&& o) {
   s = std::move(o.s);
   return *this;
  }
};

A f(A a) {
  return a;
}

A a1 = f(A{}); // move-constructed from rvalue temporary
A a2 = std::move(a1); // move-constructed using std::move
A a3 = A{};
a2 = std::move(a3); // move-assignment using std::move
a1 = f(A{}); // move-assignment from rvalue temporary
```
