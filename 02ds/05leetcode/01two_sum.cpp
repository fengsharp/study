/*
Given an array of integers, return indices of the two numbers such that they add up to a specific target.
You may assume that each input would have exactly one solution, and you may not use the same element twice.
Example:
Given nums = [2, 7, 11, 15], target = 9,
Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
*/

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <cassert>

using namespace std;

class Solution
{
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        std::unordered_map<int, int> map;
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            int curVal = nums[i];
            int targetVal = target - nums[i];
            std::unordered_map<int, int>::const_iterator findItem = map.find(targetVal);
            if (findItem != map.end()) {
                return {findItem->second, i};
            } else {
                map.insert(std::pair<int, int>(curVal, i));
            }
        }
        return {};
    }
};

int main()
{
    vector<int> nums {2, 7, 11, 15};
    int target = 9;
    vector<int> results = Solution().twoSum(nums, target);
    assert(results[0] == 0 && results[1] == 1);
    std::cout << "=== over ===" << std::endl;

    return 0;
}