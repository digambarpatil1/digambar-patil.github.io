#include<iostream>
#include<string>
#include<memory>

class payment
{
    public:
    virtual void pay(int amount)=0;
    virtual ~payment()=default;
};

class creditcard:public payment{

    private:
    std::string cardnumber;
    public:
    creditcard(const std::string& number):cardnumber(number){}
    void pay(int amount)
    {
        std::cout << "Paid " << amount << " using Credit Card (****" 
        << cardnumber.substr(cardnumber.size() - 4) << ")\n";
    }
};

class paypal:public payment{

    private:
    std::string email;
    public:
    paypal(const std::string& mail):email(mail){}
    void pay(int amount)
    {
        std::cout << "Paid " << amount << " using paypal" << ")\n";
    }
};
class upi:public payment{

    private:
    std::string upiid;
    public:
    upi(const std::string& number):upiid(number){}
    void pay(int amount)
    {
        std::cout << "Paid " << amount << " using upi" <<"\n";
    }
};

class strategyclass{
    private:
    std::unique_ptr<payment> strategy;

    public:
    void setstrategy(std::unique_ptr<payment> s)
    {
        strategy=std::move(s);
    }

    void checkout(int amount) {
        if (strategy) {
            strategy->pay(amount);
        } else {
            std::cout << "No payment method selected!\n";
        }
    }
};



int main(){

    int number =123456890;
    creditcard cr(std::to_string(number));
    strategyclass st;
    st.setstrategy(std::make_unique<creditcard>(std::to_string(number)));
    st.checkout(123456);
    return 0;
}