/*
Given a string, find the length of the longest substring without repeating characters.

Example 1:
Input: "abcabcbb"
Output: 3 
Explanation: The answer is "abc", with the length of 3. 

Example 2:
Input: "bbbbb"
Output: 1
Explanation: The answer is "b", with the length of 1.
Example 3:

Input: "pwwkew"
Output: 3
Explanation: The answer is "wke", with the length of 3. 
             Note that the answer must be a substring, "pwke" is a _subsequence_ and not a substring.
*/

#include <stdio.h>
#include <string>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        std::unordered_map<char, size_t> maps;
        size_t startIndex = 0;
        size_t maxLen = 0;
        for (size_t index = 0; index < s.length(); ++index) {
            const char& curChar = s.at(index);
            unordered_map<char, size_t>::iterator findItem = maps.find(curChar);
            if (findItem != maps.end() && findItem->second >= startIndex) {
                maxLen = std::max(maxLen, index - startIndex);
                startIndex = findItem->second + 1;
                findItem->second = index;
            } else {
                maps[curChar] = index;
            }
        }
        return std::max(maxLen, s.length() - startIndex);
    }
};

int main() {
    printf("abcabcbb %d\n", (Solution()).lengthOfLongestSubstring("abcabcbb"));
    printf("bbbbb %d\n", (Solution()).lengthOfLongestSubstring("bbbbb"));
    printf("pwwkew %d\n", (Solution()).lengthOfLongestSubstring("pwwkew"));
    printf("abcdef %d\n", (Solution()).lengthOfLongestSubstring("abcdef"));

    return 0;
}