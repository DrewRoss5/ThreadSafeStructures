#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "../inc/stack.hpp"
#include "../inc/dynarr.hpp"

int main(){
    // ensure basic functionality of the DynArr class 
    DynArr<int> arr;
    for (int i = 0; i < 32; i++)    
        arr.pushBack(i);
    std::cout << "Dynamic Array: " << std::endl;
    for (int i = 0; i < arr.size(); i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
    // ensure basic functionality of the Stack class
    Stack<int> stack;
    for (int i = 32; i < 64; i++)
        stack.push(i);
    while (stack.size())
        std::cout << stack.pop() << " ";
    std::cout << std::endl;
    // TODO: Test thread safety of these classes
}