#include<iostream>
#include<map>
#include <cstdlib>

struct Allocation {
    void* ptr;
    size_t size;
};

Allocation allocations[1024];
size_t count = 0;

void* operator new(size_t size)
{
   
    void *ptr = malloc(size);
    if(!ptr) throw std::bad_alloc();
    allocations[++count]={ptr,size};
    std::cout<<"Allocated: "<<size<<" bytes at "<<ptr<<std::endl;
    return ptr;
}
void operator delete(void* ptr) noexcept
{
    for (size_t i = 0; i < count; ++i) {
        if (allocations[i].ptr == ptr) {
            allocations[i] = allocations[--count];
            break;
        }
    }
    free(ptr);
}

void reportLeaks()
{
    if (count == 0) std::cout << "No leaks\n";
    else {
        std::cout << "Leaks:\n";
        for (size_t i = 0; i < count; ++i)
            std::cout << " Leaked " << allocations[i].size << " bytes at " << allocations[i].ptr << "\n";
    }
}

int main(){
    int* leak1 = new int[10]; // intentional leak
    double* leak2 = new double; // intentional leak
    int* noleak = new int;
    delete noleak;
    reportLeaks();  
    return 0;
}
