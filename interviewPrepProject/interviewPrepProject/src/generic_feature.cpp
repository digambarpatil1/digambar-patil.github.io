//generic_feature.cpp contains examples of generic features in C++20
// It demonstrates const expressions, enum class, and structured bindings.  
#include<iostream>
#include<map>
#include <utility>
#include <type_traits>
#include <typeinfo>

//const expressions c++11 and above
constexpr int square(int x) {
    return x * x;
}

//constvalue function c++20 and above
consteval int cube(int x) { return x * x * x; }

//enum class
enum class Color { Red, Green, Blue };
void printColor(Color color) {
    switch (color) {
        case Color::Red:
            std::cout << "Red" << std::endl;
            break;
        case Color::Green:
            std::cout << "Green" << std::endl;
            break;
        case Color::Blue:
            std::cout << "Blue" << std::endl;
            break;
    }
}

int main() {
    constexpr auto val = 5;
    decltype(val)  result = square(val);
    std::cout << "Square of " << val << " is " << result << std::endl;
   
    constexpr int  val1 = 5;
    constexpr int cube1 = cube(val1); // Valid: evaluated at compile time
    std::cout << "Cube of " << val << " is " << cube1 << std::endl;
    
   // printColor(Color::Green);
    std::map<std::string, int> fruits
    {
        {"Apple", 1},
        {"Banana", 2},
        {"Orange", 3}
    };

    for (const auto& [key, value] : fruits) {
        std::cout << key << ": " << value << std::endl;
    }

    printColor(Color::Red);
 
     
    
    return 0;
}

//exmplaination about const expressions
//Const expressions are expressions that can be evaluated at compile time. They are defined using the constexpr keyword in C++. 
//This allows for optimizations and ensures that certain values are constant throughout the program's execution.
//In the example above, the function square is defined as a constexpr function, which means it can be evaluated at compile time if its arguments are also constexpr.
//The variable val is declared as constexpr, making it a compile-time constant. When we call square(val), the compiler can evaluate this expression
//at compile time, resulting in the value being computed before the program runs.
//The decltype keyword is used to deduce the type of the variable result based on the type of val, ensuring type safety.
//Overall, using constexpr can lead to more efficient code by allowing certain computations to be performed at compile time rather than at runtime. 

//explaination about enum class explainenation with version of features in c++17
//An enum class (also known as a scoped enumeration) is a feature in C++ that provides a way to define enumerations with better type safety and scope control compared to traditional enums.
//In the example above, we define an enum class named Color with three enumerators: Red, Green, 
//and Blue. The enum class is scoped, meaning that the enumerators are accessed using the scope resolution operator (::),
//which prevents name clashes with other identifiers in the same scope.
//This is different from traditional enums, where the enumerators are placed in the surrounding scope, which can lead to naming conflicts.
//Enum classes also provide better type safety because they do not implicitly convert
//to integers or other types. This means that you cannot accidentally assign an enum class value to an integer variable or vice versa,
//which helps prevent bugs in the code.
//In the printColor function, we use a switch statement to handle different values of the Color enum class. Each case corresponds to one of the enumerators defined in the Color enum class.
//Overall, enum classes are a useful feature in C++ that enhance code readability, maintainability, and safety by providing scoped and strongly typed enumerations. //This feature was introduced in C++11 and is widely used in modern C++ programming.    
//explaination about structured bindings with version of features in c++17
//Structured bindings are a feature introduced in C++17 that allows you to unpack and bind multiple values from a tuple-like object (such as std::pair, std::tuple, or structs)
//into individual variables in a more convenient and readable way.
//In the example above, we have a std::map named fruits that contains key-value pairs representing fruit names and their corresponding quantities.
//Using structured bindings, we can iterate over the map and unpack each key-value pair into two separate variables, key and value, in the for loop.
//The syntax for (const auto& [key, value] : fruits) uses structured bindings to declare key and value as references to the elements of each pair in the map.
//This allows us to access the key and value directly within the loop body without needing to use std::get or other access methods.
//Structured bindings improve code readability and reduce boilerplate code when working with tuples or pairs.
//They also help avoid the need for temporary variables, making the code cleaner and more concise.
//Overall, structured bindings are a powerful feature in C++17 that enhances the way we work with multiple values and improves code clarity. 
