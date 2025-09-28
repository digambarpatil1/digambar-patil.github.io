#include<iostream>
#include<vector>
#include<unordered_map>
#include<algorithm>
//first pair sum =target
std::vector<int> twosum(std::vector<int> nums,int target)
{
    std::unordered_map<int,int> map;
    for(int i=0; i<nums.size(); i++)
    {
        int complement = target -nums[i];
        if(map.find(complement)!= map.end())
        {
            return {map[complement],i};
        }
        map[nums[i]] =i;
    }
    return {};
}
int main()
{
    std::vector<int> nums ={2,7,11,15};
    std::vector<int> result =twosum(nums,9);
    std::cout << "Indices: " << result[0] << " and " << result[1] << std::endl;
    std::cout << "Numbers: " << nums[result[0]] << " and " << nums[result[1]] << std::endl;

    return 0;
}