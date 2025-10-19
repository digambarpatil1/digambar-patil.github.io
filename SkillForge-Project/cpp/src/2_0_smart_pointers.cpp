//smart_pointers.cpp contains examples of smart pointers in C++20
// It demonstrates unique_ptr, shared_ptr, weak_ptr, and RAII principles.

#include<iostream>
#include<memory>
#include<string>

class file {
    std::string filename;
    FILE* filePtr; // Simulate a file pointer
public:
    file(const std::string& name): filename(name) {
        std::cout<<"File object created: "<<filename<<std::endl;
        filePtr = fopen64(filename.c_str(), "w"); // Simulate file open   
        if(!filePtr) {
            std::cerr<<"Failed to open file: "<<filename<<std::endl;
        }
    }

    ~file() {
        std::cout<<"File object destroyed: "<<filename<<std::endl;
        // Simulate file close
        if(filePtr) 
        {
            fclose(filePtr);
            filePtr = nullptr;
            std::cout<<"File closed: "<<filename<<std::endl;
        }
          
    }
    void display() const {
        std::cout<<"File name: "<<filename<<std::endl;
    }
};

class car;
class person {
    std::string name;
    std::shared_ptr<car> carPtr; // Shared ownership of Car
public:
    person(const std::string& name): name(name) {
        std::cout<<"Person object created: "<<name<<std::endl;
    }
    ~person() {
        std::cout<<"Person object destroyed: "<<name<<std::endl;
    }
    void greet() const {
        std::cout<<"Hello, my name is "<<name<<std::endl;
    }
    void setcar(const std::shared_ptr<car>& p) {
        carPtr = p;  // Set the owner of the car to the given person object
    }
};
class car{
    std::string model;
    //std::shared_ptr<person> owner; // Shared ownership of Person
    std::weak_ptr<person> owner; // Weak reference to avoid circular dependency
public:
    car(const std::string& model): model(model) {
        std::cout<<"Car object created: "<<model<<std::endl;
    }
    ~car() {
        std::cout<<"Car object destroyed: "<<model<<std::endl;
    }
    void setOwner(const std::shared_ptr<person>& p) {
        owner = p;  // Set the owner of the car to the given person object
    }
    
};


int main() {
    std::cout << "Smart Pointers Example" << std::endl;
    std::unique_ptr<int> uinque_ptr = std::make_unique<int>(42);
    std::unique_ptr<int> uinque_ptr2 = std::move(uinque_ptr);
    if(!uinque_ptr) {
        std::cout<<"uinque_ptr is null after move"<<std::endl;
    }
    std::cout<<"Value from uinque_ptr2: "<<*uinque_ptr2<<std::endl;
  
   //create shared pointers for person and car
   std::shared_ptr<person> personPtr = std::make_shared<person>("Alice ");
    std::shared_ptr<car> carPtr = std::make_shared<car>("Toyota ");
   //create circular reference
     personPtr->setcar(carPtr);
    carPtr->setOwner(personPtr);
    personPtr->greet();
   
   
    // Demonstrate RAII with file class
   // {
        file myFile("example.txt");
        myFile.display();
   // }
 std::cout<<"Exiting main function"<<std::endl;
    return 0;
}


//RAII - Resource Acquisition Is Initialization
//Smart pointers are template classes provided by the C++ Standard Library to manage dynamic memory automatically.
//They help prevent memory leaks and dangling pointers by ensuring that memory is properly released when it is no longer needed.
//There are three main types of smart pointers in C++: std::unique_ptr, std::shared_ptr, and std::weak_ptr. 
//1. std::unique_ptr: This smart pointer represents exclusive ownership of a dynamically allocated object.
//   Only one std::unique_ptr can own a particular object at a time. When the std::unique_ptr goes out of scope, it automatically deletes the object it owns.
//2. std    ::shared_ptr: This smart pointer allows multiple pointers to share ownership of a dynamically allocated object.
//   It maintains a reference count to keep track of how many std::shared_ptr instances point to the same object.
//   When the last std::shared_ptr pointing to the object is destroyed or reset, the object is automatically deleted.
//3. std::weak_ptr: This smart pointer is used in conjunction with std::shared_ptr to break circular references.
//   It provides a non-owning reference to an object managed by std::shared_ptr.
//   A std::weak_ptr does not affect the reference count of the object, and it can be used to check if the object still exists before accessing it.
//In the example above, we define two classes: Person and Car. A Person can own a Car, and a Car can have an owner (Person).
//We use std::shared_ptr to manage the ownership of Person and Car objects. This allows multiple Person objects to share ownership of the same Car object.
//To avoid circular references that could lead to memory leaks, we use std::weak_ptr for the owner reference in the Car class.
//This way, not contribute to the reference count of the Person, preventing a situation where neither object can be deleted because they reference each other.
//In the main function, we create instances of Person and Car using std::make_shared, which is a convenient way to create shared pointers.
//We then set up the ownership relationships and demonstrate the use of smart pointers.
//When the main function exits, the destructors for Person and Car are called automatically, and the memory is released properly without any memory leaks.      
