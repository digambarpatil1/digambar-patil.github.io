#include<iostream>
#include<vector>
#include<algorithm>
class Iobserver
{
    public: 
    virtual void update()=0;
    virtual ~Iobserver() {}
};
class subject
{
    private:
    int state;
    private:
    std::vector<Iobserver*> observers;
    public:
    void registerObserver(Iobserver* obs){
        observers.push_back(obs);
    }
    void unregisterObserver(Iobserver* obs){
        std::cout<<"Unregistering observer"<<std::endl;
        observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
    }
    void setState(){
        //change state logic
        state++;
        notifyObservers();
    }
    int  getstate()const{
        return state;
    }
    void notifyObservers(){
        for(auto obs : observers){
            obs->update();
        }
    }
};


class observerA : public Iobserver
{
    public:
    void update() override {
        std::cout<<"Observer A updated"<<std::endl;
    }

};
class observerB : public Iobserver
{
    public:
    void update() override {
        std::cout<<"Observer B updated"<<std::endl;
    }
};


int main()
{
    std::cout<<"Observer Design Pattern"<<std::endl;
    subject sub;
    observerA a;
    sub.registerObserver(&a);
    observerB b;
    sub.registerObserver(&b);

    sub.setState();
    sub.unregisterObserver(&a);

   
    sub.setState();
    return 0;
}