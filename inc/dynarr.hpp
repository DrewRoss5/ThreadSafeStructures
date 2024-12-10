#include <mutex>
#include <cstring>
#include <stdexcept>

template <typename T>
class DynArr{
    public:
        DynArr();
        DynArr(unsigned capacity);
        ~DynArr();
        unsigned size();
        T get(unsigned index);
        T& operator[](unsigned index);
        void set(unsigned index, const T& val);
        void pushBack(const T& val);
    private:
        unsigned capacity {16};
        unsigned arrSize {0};
        T* arr {nullptr};
        std::mutex mut;
        bool freed {false};
        void reallocate(unsigned newCap = 0);
        void lock();
        void unlock();
};

// allocates a new dynamic array with 64 slots of capacity
template <typename T>
DynArr<T>::DynArr(){
    this->arr = new T[this->capacity];
}

// allocates a new dynamic array with custom capacity
template <typename T>
DynArr<T>::DynArr(unsigned capacity){
    this->capacity = capacity;
    this->arr = new T[this->capacity];
}

// destructor for the DynArr class
template <typename T>
DynArr<T>::~DynArr(){
    this->lock();
    if (!this->freed){
        delete[] this->arr;
        this->freed = true;
    }
    this->unlock();
}

// returns the current size of the array
template <typename T>
unsigned DynArr<T>::size(){
    this->lock();
    unsigned retval = this->arrSize;
    this->unlock();
    return retval;
}

// returns the value at the given index
template <typename T>
T DynArr<T>::get(unsigned index){
    this->lock();
    if (index > this->capacity)
        this->reallocate(index * 2);
    T retval = this->arr[index];
    this->unlock();
    return retval;
}

// returns the a reference to the value at the given index
template <typename T>
T& DynArr<T>::operator[](unsigned index){
    this->lock();
    if (index > this->capacity)
        this->reallocate(index * 2);
    T& retval = this->arr[index];
    this->unlock();
    return retval;
}

// updates the value at the given index
template <typename T>
void DynArr<T>::set(unsigned index, const T& val){
    this->lock();
    if (index > this->capacity)
        this->reallocate(index * 2);
    this->arr[index] = val;
    this->unlock();
}

// appends a value to the end of the Dynamic Array
template <typename T>
void DynArr<T>::pushBack(const T& val){
    this->lock();
    if (this->arrSize == this->capacity)
        this->reallocate();
    this->arr[this->arrSize++] = val;
    this->unlock();
}

// reallocates the memory the DynArr (keeps data)
template <typename T>
void DynArr<T>::reallocate(unsigned newCap){
    if (!newCap)
        newCap = this->capacity * 2;
    // create the new array
    T* newArr = new T[newCap];
    memcpy(newArr, this->arr, this->arrSize * sizeof(T));
    // clean up and reassign member variables
    delete[] this->arr;
    this->arr = newArr;
    this->capacity = newCap;
}

// locks teh DynArr to the current thread
template <typename T>
void DynArr<T>::lock(){
    this->mut.lock();
}

// unlocks the DynArr
template <typename T>
void DynArr<T>::unlock(){
    this->mut.unlock();
}