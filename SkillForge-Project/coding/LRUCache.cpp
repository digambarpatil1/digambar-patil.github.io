#include<iostream>
#include<list>
#include<unordered_map>

struct LRUCache
{
    int capacity;
   std::list<std::pair<int,int>> list;
   std::unordered_map<int,  std::list<std::pair<int,int>>::iterator> cachemap;

   LRUCache(int cap):capacity(cap){}
   int get( int key)
   {
         auto it = cachemap.find(key);
        if(it==cachemap.end())
        {
            return -1;
        }
        list.splice(list.begin(),list,it->second);
        return it->second->second;
   }
   void put(int key ,int value)
    {
        auto it = cachemap.find(key);
        if(it!=cachemap.end())
        {
            it->second->second =value;//map update
            list.splice(list.begin(),list,it->second);//move to front
        }else
        {
            if((int)list.size()==capacity) //check for cache full or not
            {
                auto last = list.back();
                cachemap.erase(last.first);
                list.pop_back();
            }
            list.emplace_front(key, value);
            cachemap[key]=list.begin();

        }
    }

    void display() {
        std::cout << "Cache: ";
        for (auto& p : list) std::cout << "(" << p.first << "," << p.second << ") ";
        std::cout << "\n";
    }
};



int main(){
    std::cout<< "LRU implimentation\n";
    LRUCache cache(3);
    cache.put(1,3);
    cache.put(2,4);
    cache.put(3,5);
    cache.display();
    cache.put(4,6);
    cache.display();
}








