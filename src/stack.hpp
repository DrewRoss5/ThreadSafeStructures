#ifndef STACK_H
#define STACK_H

#include <stdexcept>
#include <mutex>
#include <cstring>
#include "dynarr.hpp"

template <typename T>
class Stack{
    public:
        Stack();
        Stack(unsigned init_capacity);
        T top();
        unsigned size();
        void push(const T& val);
        T pop();
        void lock();
        void unlock();
    private:
        DynArr<T> arr;
        int topIndex;
        bool freed {false};
};

// default constructor for the Stack, allocates space for 64 items
template <typename T>
Stack<T>::Stack(){
    this->topIndex = -1; 
}

// creates the Stack with a user-provided capacity
template <typename T>
Stack<T>::Stack(unsigned init_capacity){
    this->topIndex = -1;
    this->arr = DynArr<T>(init_capacity);
}

// returns the size of the stack
template <typename T> 
unsigned Stack<T>::size(){
    this->lock();
    unsigned retval = this->topIndex + 1;
    this->unlock();
    return retval;
}

// returns the top value of the stack
template <typename T>
T Stack<T>::top(){
    if (this->topIndex == -1)
        throw std::runtime_error("cannot get the top value of an empty stack");
    return this->arr[this->topIndex];
}

// pushes a value onto the stack
template <typename T>
void Stack<T>::push(const T& val){
    this->lock();
    this->topIndex++;
    this->unlock();
    this->arr.pushBack(val);
}

// pops the top value off of the stack
template <typename T>
T Stack<T>::pop(){
    this->lock();
    topIndex--;
    this->unlock();
    if (this->topIndex < -1)
        throw std::runtime_error("cannot pop from an empty stack");
    return this->arr[topIndex+1];
}

// locks the stack (this is its own function to allow for easy experimentation with other methods)
template <typename T>
void Stack<T>::lock(){
    this->arr.lock();
}

template <typename T>
void Stack<T>::unlock(){
    this->arr.unlock();
}
#endif