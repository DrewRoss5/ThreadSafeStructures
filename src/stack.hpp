#include <stdexcept>
#include <mutex>
#include <cstring>

template <typename T>
class Stack{
    public:
        Stack();
        Stack(unsigned init_capacity);
        T top();
        unsigned size();
        void push(const T& val);
        T pop();
        ~Stack();
    private:
        T* arr;
        unsigned capacity {};
        int topIndex {-1};
        std::mutex mut;
        bool freed;
        void reallocate();
        void lock();
        void unlock();

};

// default constructor for the Stack, allocates space for 64 items
template <typename T>
Stack<T>::Stack(){
    this->arr = new T[this->capacity]; 
}

// creates the Stack with a user-provided capacity
template <typename T>
Stack<T>::Stack(unsigned init_capacity){
    this->capacity = init_capacity;
    this->arr = new T[this->capacity];
}

// destructor for the Stack, frees the dynamically allocated memory
template <typename T>
Stack<T>::~Stack(){
    this->lock();
    if (!this->freed){
        this->freed = true;
        delete[] arr;
    }
    this->unlock();
}

// returns the size of the stack
template <typename T> 
unsigned Stack<T>::size(){
    this->lock();
    unsigned retval = this->topIndex + 1;
    this->unlock();
    return retval;
}

// reallocates the Stack's memory. This doubles the capacity
template <typename T>
void Stack<T>::reallocate(){
    unsigned newCapacity = this->capacity * 2;
    T* newArr = new T[newCapacity];
    memcpy(newArr, this->arr, this->capacity * sizeof(T));
    // clean up and update member variables
    delete[] this->arr;
    this->arr = newArr;
    this->capacity = newCapacity;
}

// returns the top value of the stack
template <typename T>
T Stack<T>::top(){
    if (this->topIndex == -1)
        throw std::runtime_error("cannot get the top value of an empty stack");
    this->lock();
    T retval = this->arr[this->topIndex];
    this->unlock();
    return retval;
}

// pushes a value onto the stack
template <typename T>
void Stack<T>::push(const T& val){
    this->lock();
    // reallocate the stack if needed
    if (this->topIndex == this->capacity)
        this->reallocate();
    this->arr[++this->topIndex] = val;
    this->unlock();
}

// pops the top value off of the stack
template <typename T>
T Stack<T>::pop(){
    this->lock();
    if (this->topIndex == -1){
        this->unlock();
        throw std::runtime_error("cannot pop from an empty stack");
    }
    T retval = this->arr[topIndex--];
    this->unlock();
    return retval;
}

// locks the stack (this is its own function to allow for easy experimentation with other methods)
template <typename T>
void Stack<T>::lock(){
    this->mut.lock();
}

template <typename T>
void Stack<T>::unlock(){
    this->mut.unlock();
}