#include<iostream>
#include<vector>
#include<stack>
#include<stdexcept>
#include<algorithm>

class genericPool {
    std::vector<char> buffer;     // one big memory block
    std::stack<size_t> freeList;  // free slot indices
    size_t index = 0;
    size_t objSize;
    size_t capacity;

public:
genericPool(size_t objSize, size_t size) 
        : buffer(objSize * size), objSize(objSize), capacity(size) 
    {
        std::cout << "Generic Pool of capacity " << capacity 
                  << " for objects of size " << objSize << " created\n";
    }

    void* allocate() {
        if (!freeList.empty()) {
            size_t idx = freeList.top();
            freeList.pop();
            std::cout << "Reusing slot " << idx << std::endl;
            return &buffer[idx * objSize];
        }
        if (index >= capacity) {
            throw std::out_of_range("Pool exhausted");
        }
        std::cout << "Allocating slot " << index << std::endl;
        return &buffer[index++ * objSize];
    }

    void deallocate(void* ptr) {
        char* cptr = reinterpret_cast<char*>(ptr);
        if (cptr < buffer.data() || cptr >= buffer.data() + buffer.size()) {
            throw std::out_of_range("Invalid deallocation");
        }
        size_t idx = (cptr - buffer.data()) / objSize;
        std::cout << "Deallocating slot " << idx << std::endl;
        freeList.push(idx);
        // NOTE: No free(ptr) → memory belongs to pool
    }
};

/* 
Simple pool for integers 
class pool{
    std::vector<int> data;
    std::stack<size_t> freeList; // store indices of freed slots
    size_t index=0;
    public:
    pool(size_t size):data(size){
        std::cout<<"Pool of size "<<size<<" created"<<std::endl;
    }
    int& allocate(){
        if(!freeList.empty()){
            size_t idx = freeList.top();
            freeList.pop();
            std::cout<<"Reusing slot "<<idx<<std::endl;
            return data[idx];
        }
       /// static int index=0;
        if(index >= data.size()){
            throw std::out_of_range("Pool exhausted");
            //handle pool exhaustion    
        }
        return data[index++];
    }
    void deallocate(int& ref){
        size_t idx = &ref - &data[0]; // calculate index
        if (idx >= data.size()) {
            throw std::out_of_range("Invalid deallocation");
        }
        //simple deallocation logic
        std::cout<<"Deallocating "<<ref<<std::endl;
        //in real scenario, we would add ref back to free list
        //here we just simulate deallocation
        ref = -1; //mark as deallocated 
        freeList.push(idx);  
    } 
};
*/
struct MyObj {
    int x;
    MyObj(int val) : x(val) { std::cout << "MyObj("<<val<<") constructed\n"; }
    ~MyObj() { std::cout << "MyObj("<<x<<") destroyed\n"; }
};
int main()
{
    std::cout<<"Memory Optimization"<<std::endl;
   /* pool p(5); //pool of 5 integers
    try{
        int& a = p.allocate();  a=10;
        int& b = p.allocate();  b=20;
        std::cout<<"Allocated: "<<a<<" "<<b<<std::endl;
        p.deallocate(a);
        int& c = p.allocate();  c=30;
        std::cout<<"Allocated: "<<b<<" "<<c<<std::endl;
        //allocate more than pool size to test exhaustion
        int& d = p.allocate();  d=40;
        int& e = p.allocate();  e=50;
        int& f = p.allocate();  f=60;   
        int& g = p.allocate();  g=70; //should throw exception  
    }catch(const std::out_of_range& e){
        std::cerr<<e.what()<<std::endl;
    }*/
    genericPool pool(sizeof(int),5); //generic pool of 5 integers
    void* p1 = pool.allocate();
    MyObj* a = new (p1) MyObj(10);

    void* p2 = pool.allocate();
    MyObj* b = new (p2) MyObj(20);

    a->~MyObj(); pool.deallocate(a);
    b->~MyObj(); pool.deallocate(b);

    void* p3 = pool.allocate(); // reuse freed slot
    MyObj* c = new (p3) MyObj(30);

    c->~MyObj(); pool.deallocate(c);

   // Pre-allocate raw memory
    char buffer[sizeof(MyObj)];
    // Use placement new to construct inside buffer
    MyObj* obj = new (buffer) MyObj(42);

    std::cout << "Object value = " << obj->x << "\n";

    // Call destructor explicitly
    obj->~MyObj();


    return 0;   
}