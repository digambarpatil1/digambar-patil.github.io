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
