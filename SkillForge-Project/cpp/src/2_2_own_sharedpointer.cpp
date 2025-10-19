#include <iostream>
template <typename T>
class shared_ptr
{
    private:
    T* ptr ;//pointer to managed object
    size_t* ref_count;//reference count
    void release()
    {
        if(--(*ref_count) == 0)
        {
            delete ptr;
            delete ref_count;
        }
    }
public:
    explicit shared_ptr( T* sptr=nullptr):ptr(sptr),ref_count(new size_t(1)){
    }
    shared_ptr(const shared_ptr& other)
    {
        ptr = other.ptr;
        ref_count = other.ref_count;
        ++(*ref_count); //increment referece count
    }
    shared_ptr& operator=(shared_ptr& other)
    {
        if(this!= &other)
        {
            release();
            ptr = other.ptr;
           ref_count = other.ref_count;
          ++(*ref_count); //increment referece count
        }
        return *this; 
    }

    T* operator->(){ return ptr;}
    T& operator*(){return *ptr;}

    size_t use_count() const{
        return *ref_count;
    }

    ~shared_ptr()
    {
        release();
    }
};
class A{
    private:
    int val=10;
    public:
    A(){std::cout<<"A is created\n";}
    ~A(){std::cout<<"A is distructed\n";}
    void hello(){std::cout<<"hello\n";}
};

int main()
{
    shared_ptr<A> sptr(new A());
    shared_ptr<A> sptr1= sptr;
    sptr1->hello(); 
    std::cout<<sptr.use_count()<<std::endl;
    return 0;
}
