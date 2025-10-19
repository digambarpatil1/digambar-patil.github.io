#include <iostream>
#include <vector>

class shape
{
public:
    // Base class data member
    std::string name = "shape";

    // Virtual function: enables runtime polymorphism
    // If you uncomment "=0", this becomes a pure virtual function → abstract class
    // virtual void draw() = 0;
    virtual void draw()
    {
        std::cout << "drawing shape" << std::endl;
    }
};

class circle : public shape
{
public:
    // Hides base class "name" (not overriding, just hiding)
    std::string name = "circle";

    // Override keyword ensures compiler checks correctness
    void draw() override
    {
        std::cout << "drawing circle" << std::endl;
    }

    // Circle-specific function (not in base class)
    void radius()
    {
        std::cout << "circle radius" << std::endl;
    }
};

class square : public shape
{
public:
    void draw() override
    {
        std::cout << "drawing square" << std::endl;
    }
};

int main()
{
    // Vector of shape pointers (polymorphic storage)
    std::vector<shape*> shapes;
    shapes.push_back(new circle()); // Upcasting (circle* → shape*)
    shapes.push_back(new square()); // Upcasting (square* → shape*)

    // Polymorphic call → virtual function dispatch
    for (auto s : shapes)
    {
        s->draw(); // Resolves to circle::draw() or square::draw()
    }

    // Demonstrating upcasting without slicing
    circle c;       
    shape& s = c;   // Reference to base, bound to a circle object
    s.draw();       // Calls circle::draw() due to virtual function

    // Note: If "draw" were not virtual, this would call shape::draw()

    // Cleanup (avoid memory leaks in real code → use smart pointers)
    for (auto s : shapes)
    {
        delete s;
    }

    return 0;
}
