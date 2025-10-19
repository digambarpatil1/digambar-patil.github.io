#include<iostream>
using namespace std;

class Base{
    public:
    virtual void show(int x=10)  //virtual function
    {
        cout<<"Base class show function called x="<<x<<endl;
    }
    virtual ~Base(){
        cout<<"Base class destructor called"<<endl;

    } //virtual destructor

};
class Derived:public Base{
    public:
    
    void show(int x=20) override //override base class function
    {
        cout<<"Derived class show function called x"<<x<<endl;
    }
    ~Derived(){
        cout<<"Derived class destructor called"<<endl;
    } //derived class destructor
};


int main()
{
    Base* b = new Derived(); //base class pointer
    b->show(); //calls derived class show function because of virtual function
    delete b; //calls derived class destructor followed by base class destructor
    return 0;
}