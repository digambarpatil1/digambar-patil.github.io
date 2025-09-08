//move_semantic.cpp contains an example of move semantics in C++20
// It demonstrates move constructor and move assignment operator.
#include<iostream>
#include<algorithm>




class buffer{
    size_t size;
    char* data;
    public:
    buffer(size_t s):size(s),data(new char[s]){
        std::cout<<"bfuffer of size "<<size<<" created."<<std::endl;
    }
    ~buffer(){
        delete[] data;
        std::cout<<"buffer of size "<<size<<" destroyed."<<std::endl;
    }
    buffer(const buffer& other):size(other.size),data(new char[other.size]){
        std::copy(other.data, other.data + other.size, data);
        std::cout<<"buffer of size "<<size<<" copied."<<std::endl;
    }
    buffer& operator=(const buffer& other){
        if(this !=&other)
        {
          delete[] data;
          size = other.size;
          data = new char[other.size];
          std::copy(other.data, other.data + other.size, data);
          std::cout<<"buffer of size "<<size<<" copy assigned."<<std::endl;
        } 
        return *this;  
    }
    buffer(buffer&& other)  noexcept:size(other.size),data(other.data){
        other.size =0;
        other.data = nullptr;
        std::cout<<"buffer of size"<<size<<"moved."<<std::endl;
    }       
    buffer& operator=(buffer&& other) noexcept{
        if(this != &other){
            delete[] data;
            size = other.size;
            data = other.data;
            other.size =0;
            other.data = nullptr;
            std::cout<<"buffer of size "<<size<<" move assigned."<<std::endl;
        }
        return *this;
    }
    
};

int main()
{
    buffer bf1(1024);
    buffer bf2 = std::move(bf1); // Move constructor
    buffer bf3(2048);
    bf3 = std::move(bf2); // Move assignment
    return 0;

}

//create notes on short notes of above code
//Move semantics in C++ allows the resources of temporary objects to be moved rather than copied,
//which can lead to significant performance improvements, especially for objects that manage dynamic memory or other resources.
//In the above code, we define a class buffer that manages a dynamic array of characters.
//The class includes a constructor, destructor, copy constructor, copy assignment operator, move constructor,
//and move assignment operator to handle resource management correctly.
//The move constructor and move assignment operator transfer ownership of the resource from the source object to the
//destination object, leaving the source object in a valid but unspecified state.
//This is done by transferring the pointer to the data and setting the source object's pointer to nullptr
//and size to 0, ensuring that the destructor of the source object does not attempt to free the resource.
//In the main function, we demonstrate the use of move semantics by creating a buffer object and then moving it to another buffer object using std::move.
//This avoids the overhead of copying the data, leading to more efficient code.     
//Note: The noexcept specifier is used in the move constructor and move assignment operator to indicate that these functions do not throw exceptions.
//This is important for performance optimization, as it allows certain standard library operations to use move semantics more effectively.  
