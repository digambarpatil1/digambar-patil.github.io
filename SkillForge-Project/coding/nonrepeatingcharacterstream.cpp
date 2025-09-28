#include<iostream>
#include<queue>
#include<unordered_map>
#include<string>

int main(){
    std::string str = "aabcadbec";
    std::unordered_map<char,int>charcountermap;
    std::queue<char>characterqueue;
    for(const char& ch:str){
        charcountermap[ch]++;
        if(charcountermap[ch]==1){
            characterqueue.push(ch);
        }
        while(!characterqueue.empty() &&  charcountermap[characterqueue.front()]>1)
        {
            characterqueue.pop();
        }

        if (!characterqueue.empty())
            std::cout << "First non-repeating so far: " << characterqueue.front() << std::endl;
        else
            std::cout << "No non-repeating character yet" << std::endl;
    }
    return 0;
}