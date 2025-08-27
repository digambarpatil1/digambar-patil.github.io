//producer_consumer.cpp contains a simple producer-consumer implementation in C++20
// It demonstrates task synchronization using mutexes.
#include<iostream>
#include<thread>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<chrono>    
#include<cstring>
#include<string>

std::queue<int> q;
std::mutex mu;
char* buffer;


char* nonreentrant_function(const char* buf){
   // std::cout<<"Reentrant function using buffer: "<<buf<<std::endl;
    //modify buffer
    buffer =new char[20];
    std::strcpy(buffer,buf);
   // std::cout<<"Buffer inside reentrant function: "<<buffer<<std::endl;
    return buffer;
   
}

void producer(){

    for(int i=0;i<10;i++){
       // std::this_thread::sleep_for(std::chrono::milliseconds(100)); // simulate work
       {
        std::unique_lock<std::mutex> lock(mu);
        std::cout<<"Producing item "<<i<<std::endl;
        q.push(i);
       }
    }

}
void consumer(){
    for(int i=0;i<10;i++){

         {
          std::lock_guard<std::mutex> lock(mu);
          if(!q.empty()){
               int item = q.front();
               q.pop();
             std::cout<<"Consuming item "<<item<<std::endl;
          }
         
         }
       
    }

}

int main()
{
    
    char* res1= nonreentrant_function("Hello");
    char* res2= nonreentrant_function("World");
    std::cout<<"Buffer in main after reentrant calls: "<<res1<<", "<<res2<<std::endl;
   std::thread p(producer);
   std::thread c(consumer);
   p.join();
   c.join();
    return 0;
}