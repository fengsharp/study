/*
You are given two non-empty linked lists representing two non-negative integers. 
The digits are stored in reverse order and each of their nodes contain a single digit. 
Add the two numbers and return it as a linked list.
You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Example:
Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
Explanation: 342 + 465 = 807.
*/

#include <iostream>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* pRet = nullptr;
        int carry = 0;
        ListNode* pCurNode = pRet;
        while (l1 != nullptr || l2!=nullptr)
        {
            int curVal = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + carry;
            carry = curVal / 10;
            curVal = curVal - carry * 10;

            ListNode* pNode = new ListNode(curVal);
            if (pRet == nullptr) {
                pRet = pNode;
                pCurNode = pNode;
            } else {
                pCurNode->next = new ListNode(curVal);
                pCurNode = pCurNode->next;
            }

            l1 = l1 ? l1->next : l1;
            l2 = l2 ? l2->next : l2;
        }

        if (carry != 0) {
            pCurNode->next = new ListNode(carry);
        }

        return pRet;
    }
};

void freeListNode(ListNode* pHeader)
{
    ListNode* pCursor = nullptr;
    while (pHeader != nullptr)
    {
        pCursor = pHeader;
        pHeader = pHeader->next;
        delete pCursor;
    }
}

void printListNode(ListNode* pHeader)
{
    ListNode* pCursor = pHeader;
    while (pCursor != nullptr)
    {
        std::cout << pCursor->val << " ";
        pCursor = pCursor->next;
    }

    std::cout << std::endl;
}

void test1()
{
    // test 0+3 => 3
    ListNode* l1 = new ListNode(0);
    ListNode* l2 = new ListNode(3);
    ListNode* ret = Solution().addTwoNumbers(l1, l2);
    std::cout << "test1. should display 3" << std::endl;
    printListNode(ret);
    freeListNode(l1);
    freeListNode(l2);
    freeListNode(ret);
}

void test2()
{
    ListNode* pCursor = nullptr;

    // test (2 -> 4 -> 3) + (5 -> 6 -> 4) => 7 -> 0 -> 8
    ListNode* l1 = new ListNode(2);
    pCursor = l1;
    pCursor->next = new ListNode(4);
    pCursor = pCursor->next;
    pCursor->next = new ListNode(3);

    ListNode* l2 = new ListNode(5);
    pCursor = l2;
    pCursor->next = new ListNode(6);
    pCursor = pCursor->next;
    pCursor->next = new ListNode(4);

    ListNode* ret = Solution().addTwoNumbers(l1, l2);
    std::cout << "test2. should display 7 0 8" << std::endl;
    printListNode(ret);
    freeListNode(l1);
    freeListNode(l2);
    freeListNode(ret);
}

void test3()
{
    ListNode* pCursor = nullptr;

    // test (2 -> 4 -> 8) + (5 -> 6 -> 8) => 7 -> 0 -> 7 -> 1
    ListNode* l1 = new ListNode(2);
    pCursor = l1;
    pCursor->next = new ListNode(4);
    pCursor = pCursor->next;
    pCursor->next = new ListNode(8);

    ListNode* l2 = new ListNode(5);
    pCursor = l2;
    pCursor->next = new ListNode(6);
    pCursor = pCursor->next;
    pCursor->next = new ListNode(8);

    ListNode* ret = Solution().addTwoNumbers(l1, l2);
    std::cout << "test3. should display 7 0 7 1" << std::endl;
    printListNode(ret);
    freeListNode(l1);
    freeListNode(l2);
    freeListNode(ret);
}

void test4()
{
    // test 0+[1,8] => [1,8]
    ListNode* l1 = new ListNode(0);
    ListNode* l2 = new ListNode(1);
    l2->next = new ListNode(8);

    ListNode* ret = Solution().addTwoNumbers(l1, l2);
    std::cout << "test4. should display 1 8" << std::endl;
    printListNode(ret);
    freeListNode(l1);
    freeListNode(l2);
    freeListNode(ret);
}

int main() {
    test1();
    test2();
    test3();
    test4();

    return 0;
}