#include <iostream>
template <typename T>
class unique_ptr
{
    private:
    T* ptr ;//pointer to managed object
public:
    explicit unique_ptr( T* uptr=nullptr):ptr(uptr){
    }
    unique_ptr(const unique_ptr& other)=delete;
    unique_ptr& operator=(unique_ptr& other)=delete;
    unique_ptr( unique_ptr&& other):ptr(other.ptr)
    {
       other.ptr =nullptr;
    }

    unique_ptr& operator=(unique_ptr&& other)
    {
        if(this != &other)
        {
            delete ptr;
           ptr= other.ptr;
           other.ptr =nullptr;
        }
        return *ptr;
    }

    T* operator->(){ return ptr;}
    T& operator*(){return *ptr;}
   
   void reset(T* p=nullptr)//delte old pointer and take ownership
   {
       delete ptr;
       ptr =p;
   }

   T* release() //Release ownership without deleting
   {
      T* temp =ptr;
      delete ptr;
      return temp;
   }

    ~unique_ptr()
    {
        if(!ptr)delete ptr;
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
    unique_ptr<A> uptr(new A());
    //unique_ptr<A> sptr1= sptr;
    unique_ptr<A> uptr1 =std::move(uptr);
    uptr1->hello(); 
    uptr.reset();
    uptr1.release();
    return 0;
}
