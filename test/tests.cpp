#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <gtest/gtest.h>
#include <chrono>
#include "../src/dynarr.hpp"
#include "../src/stack.hpp"
#include "../src/linkedlist.hpp"

DynArr<int> globalArr;
Stack<int> globalStack;
LinkedList<int> globalList;

// populates the global DynArr with all integers within the range [start, end)
void populateArr(int start, int end){
    for (int i = start; i < end; i++)
        globalArr.pushBack(i);
}

// populates the global Stack with all integers within the range [start, end)
void populateStack(int start, int end){
    for (int i = start; i < end; i++)
        globalStack.push(i);
}

// populates the global LinkedList with all integers within the range [start, end)
void populateList(int start, int end){
    for (int i = start; i < end; i++)
        globalList.pushBack(i);
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

// test the basic functionality of the LinkedList class
TEST(BasicTests, LinkedListBasic){
    // ensure basic functionality
    LinkedList<int> list;
    list.pushBack(3);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 3);
    list.pushBack(1);
    list.insert(2, 1);
    for (int i = 0; i < 3; i++)
        EXPECT_EQ(list.at(i), 3-i); 
    // ensure that iterators work
    list.clear();
    for (int i = 0; i < 50; i++)
        list.pushBack(i);
    int target {0};
    for (auto itt = list.begin(); itt != list.end(); ++itt){
        EXPECT_EQ(*itt, target);
        ++target;
    }
    // test larger operations
    list.clear();
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100'000; i++)
        list.pushFront(i);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Pushed 100,000 elements in " << duration/1000 << "ms." << std::endl;
    t1 = std::chrono::high_resolution_clock::now();
    EXPECT_EQ(list.at(10), 99'989);
    t2 = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Accessed item at position 10 in " << duration/1000 << "ms." << std::endl;
    t1 = std::chrono::high_resolution_clock::now();
    EXPECT_EQ(list.at(99'990), 9);
    t2 = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "Accessed item at position 99,990 in " << duration/1000 << "ms." << std::endl;  
}

// check for race conditions in the DynArr   class
TEST(ThreadSafetyTests, DynArrSafety){
    int threadCount = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    // place 10000 elements from each thread, making a race condition likely
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
  
// check for race conditions affecting the Stack class
TEST(ThreadSafetyTests, StackSafety){
    int threadCount = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    // place 1000 elements from each thread, making a race condition likely
    for (int i = 0; i < threadCount; i++)
        threads.emplace_back(populateStack, i * 1000, (i+1) * 1000);
    for (int i = 0; i < threadCount; i++)
        threads[i].join();
    // convert the stack to an array for countif
    int* arr = new int[1000 * threadCount];
    int pos {0};
    while (globalStack.size()){
        arr[pos] = globalStack.pop();
        pos++;
    }
    // ensure no elements were overwritten via race condition
    bool safe {true};
    for (int i = 0; i < threadCount * 1000; i++){
        unsigned count = std::count_if(arr, arr + (1000*threadCount), [i] (int x) {return x == i;});
        if (count != 1){
            std::cout << "Race condition detected! " << i << " appears " << count << " times." << std::endl;
            safe = false;
            break;
        }
    }
    delete[] arr;
    ASSERT_TRUE(safe);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}