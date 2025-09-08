#include<iostream>
#include<map>
#include<vector>
#include<string>
int main()
{
    std::map<int,std::vector<std::string>,std::greater<int>> Learderboard;
    Learderboard[50] = {"alice","bob"};
    Learderboard[30] = {"charlie"};
    Learderboard[40] = {"david"};
    Learderboard[20] = {"eve","frank"};

    std::cout<<"Leaderboard:\n";
    for(const auto&[score, players]:Learderboard)
    {
        for(const auto& player:players)
        {
            std::cout<<player<<" : "<<score<<std::endl;
        }   
    }
}