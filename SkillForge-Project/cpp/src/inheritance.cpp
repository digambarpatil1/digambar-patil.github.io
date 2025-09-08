#include<iostream>
#include<vector>
class shape
{
    public:
    //virtual void draw()=0;
    std::string name="shape";
    virtual void draw()
    {
        std::cout<<"drawing shape"<<std::endl;
    }
};
class circle:public shape{
    public:
    std::string name="circle";
    void draw() override
    {
        std::cout<<"drawing circle"<<std::endl;
    }
    void radius()
    {
        std::cout<<"circle radius"<<std::endl;
    }
};
class square:public shape{
    public:
    void draw() override
    {
        std::cout<<"drawing square"<<std::endl;
    }
};
int main()
{
    std::vector<shape*> shapes;
    shapes.push_back(new circle());
    shapes.push_back(new square());
    for(auto s:shapes)
    {
        s->draw();
    }
    

    circle c;
    shape& s = c; //upcasting avoid slicing
    s.draw();


    return 0;
}