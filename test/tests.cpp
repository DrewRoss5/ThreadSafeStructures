#include <iostream>
#include <gtest/gtest.h>
#include "../src/dynarr.hpp"
#include "../src/stack.hpp"

// test the basic functionality of the DynArr class
TEST(BasicTests, DynArrBasic){
    DynArr<int> arr;
    for (int i = 0; i < 1000; i++){
        arr.pushBack(i);
    }
    for (int i = 0; i < 1000; i++)
        ASSERT_EQ(arr[i], i);
}

// test the basic functionality of the Stack class
TEST(BasicTests, StackBasic){
    Stack<int> stack;
    for (int i = 0; i < 1024; i++){
        stack.push(i);
    }
    int target = 1023;
    while (target > -1){
        ASSERT_EQ(stack.pop(), target);
        target--;
    }
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}