#include "../src/dynarr.hpp"
#include "../src/stack.hpp"
#include <gtest/gtest.h>

TEST(BasicTests, DynArrBasic){
    DynArr<int> arr;
    for (int i = 0; i < 10; i++)
        arr.pushBack(i);
    for (int i = 0; i < 10; i++)
        ASSERT_EQ(arr[i], i);
}

TEST(BasicTests, StackBasic){
    Stack<int> stack;
    for (int i = 0; i < 10; i++)
        stack.push(i);
    int target = 9;
    while (stack.size()){
        ASSERT_EQ(stack.pop(), target);
        target--;
    }
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}