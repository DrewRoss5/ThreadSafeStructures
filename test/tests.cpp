#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <gtest/gtest.h>
#include "../src/dynarr.hpp"
#include "../src/stack.hpp"

DynArr<int> globalArr;

// populates a DynArr with all integers within the range [start, end)
void populateArr(int start, int end){
    for (int i = start; i < end; i++)
        globalArr.pushBack(i);
}

// test the basic functionality of the DynArr class
TEST(BasicTests, DynArrBasic){
    DynArr<int> arr;
    for (int i = 0; i < 1000; i++)
        arr.pushBack(i);
    ASSERT_EQ(arr.size(), 1000);
    for (int i = 0; i < 1000; i++)
        ASSERT_EQ(arr[i], i);
}

// test the basic functionality of the Stack class
TEST(BasicTests, StackBasic){
    Stack<int> stack;
    for (int i = 0; i < 1024; i++)
        stack.push(i);
    int target = 1023;
    while (target > -1){
        ASSERT_EQ(stack.pop(), target);
        target--;
    }
}

// check for race conditions in the DynArr class
TEST(ThreadSafetyTests, DynArrSafety){
    int threadCount = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    // place 1000 elements from each thread, making a race condition likely
    for (int i = 0; i < threadCount; i++)
        threads.emplace_back(populateArr, i * 10000, (i+1) * 10000);
    for (int i = 0; i < threadCount; i++)
        threads[i].join();
    // check that each element in the array occurs exactly once, if any doesn't appear that means it was overwritten via race condition
    bool safe {true};
    for (int i = 0; i < threadCount * 10000; i++){
        unsigned count = std::count_if(&globalArr[0], &globalArr[10000*threadCount], [i] (int x) {return x == i;});
        if (count != 1){
            std::cout << "Race condition detected! " << i << " appears " << count << " times." << std::endl;
            safe = false;
            break;
        }
    }
    ASSERT_TRUE(safe);
}
  
int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
