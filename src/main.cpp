#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <functional>
#include <algorithm>
#include "../inc/stack.hpp"
#include "../inc/dynarr.hpp"

DynArr<int> globalArr;

// fills a dynamic array with all integers in a given range
void populateArr(int start, int end){
    for (int i = start; i < end; i++)
        globalArr.pushBack(i);
}

int main(){
    // ensure basic functionality of the DynArr class 
    DynArr<int> arr;
    for (int i = 0; i < 32; i++)    
        arr.pushBack(i);
    std::cout << "Dynamic Array: " << std::endl;
    for (int i = 0; i < arr.size(); i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
    arr.clear();
    // ensure basic functionality of the Stack class
    Stack<int> stack;
    for (int i = 32; i < 64; i++)
        stack.push(i);
    std::cout << "\nStack:" << std::endl;
    while (stack.size())
        std::cout << stack.pop() << " ";
    std::cout << std::endl;
    // test the the thread safety of the dynamic array class
    std::cout << "Testing Dynamic Array Thread Safety:" << std::endl;
    unsigned threadCount = std::thread::hardware_concurrency();
    globalArr.reserve(1000 * threadCount);
    std::vector<std::thread> threads;
    for (int i = 0; i < threadCount; i++)
        threads.emplace_back(populateArr, i*1000, (i+1)*1000);
    for (int i = 0; i < threadCount; i++)
        threads[i].join();
    std::vector<int> vec(&globalArr[0], &globalArr[threadCount * 1000]);
    for (int i = 0; i < 1000 * threadCount; i++){
        unsigned count = std::count_if(vec.begin(), vec.end(), [i] (int n) {return  n == i;});
        if (count != 1){
            std::cout << i << ": Count = " << count << std::endl;
            return 1;
        }
    }
    std::cout << "DynArr is thread safe" << std::endl;
}
