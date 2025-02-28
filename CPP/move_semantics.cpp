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

