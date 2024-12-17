#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <mutex>
#include <functional>
#include <stdexcept>


template <typename T>
struct ListNode{
    T data;
    ListNode<T>* next {nullptr};
    ListNode<T>* prev {nullptr};
};

template <typename T>
class LinkedList{
    public:
        LinkedList() {};
        LinkedList(const T& initVal);
        ~LinkedList();
        unsigned size();
        T front();
        T back();
        T at(unsigned targetPos);
        void pushBack(const T& val);
        void pushFront(const T& val);
        void insert(const T& val, unsigned pos);
        T popFront();
        T  popBack();
        void remove(unsigned pos);
        void lock();
        void unlock();
        void clear();
    private:
        ListNode<T>* head {nullptr};
        ListNode<T>* tail {nullptr};
        unsigned listSize {0};
        std::mutex mut;
        bool freed {false};
        void emptyList();
};

// constructs a linked list with the given node as the head
template <typename T>
LinkedList<T>::LinkedList(const T& initVal){
    this->head = new ListNode<T>;
    this->head->data = initVal;
    this->tail = this->head;
}

// deconstuctor for the LinkedList class
template <typename T>
LinkedList<T>::~LinkedList(){
    if (!this->freed){
        this->emptyList();
        this->freed = true;
    }
}

// removes all elements in the linked list
template <typename T>
void LinkedList<T>::emptyList(){
    ListNode<T>* curr = this->head;
    ListNode<T>* tmp;
    while (curr){
        tmp = curr->next;
        delete curr;
        curr = tmp;
    }
}

// clears all elements from the list and resets memeber variables
template <typename T>
void LinkedList<T>::clear(){
    this->lock();
    this->emptyList();
    this->head = nullptr;
    this->tail = nullptr;
    this->listSize = 0;
    this->unlock();
}

// returns the current size of the linked list
template <typename T>
unsigned LinkedList<T>::size(){
    this->lock();
    unsigned retval = this->listSize;
    this->unlock();
    return retval;
}

// returns the value of the list's head. Throws an exception if there is no head
template <typename T>
T LinkedList<T>::front(){
    if (!this->head)
        throw std::runtime_error("this list is empty");
    this->lock();
    T retval = this->head->data;
    this->unlock();
    return retval;
}

// returns the value of the list's tail. Throws an exception if there is no tail
template <typename T>
T LinkedList<T>::back(){
    if (!this->tail)
        throw std::runtime_error("this list is empty");
    this->lock();
    T retval = this->tail->data;
    this->unlock();
    return retval;
}

// returns the element at a specified position, or raises an error if no such position exists
template <typename T>
T LinkedList<T>::at(unsigned targetPos){
    this->lock();
    // ensure the position can be reached
    if (targetPos >= this->listSize){
        this->unlock();
        throw std::out_of_range("position out of range");
    }
    // determine which direction to search from 
    int mod {1}, pos{0};
    ListNode<T>* curr = this->head;
    std::function<ListNode<T>* (ListNode<T>*)> getNext = [] (ListNode<T>* node) {return node->next;}; 
    if (this->listSize < this->listSize - targetPos){
        pos = this->listSize - 1;
        mod = -1;
        curr = this->tail;
        getNext = [] (ListNode<T>* node) {return node->prev;};
    }
    // go to the specified element 
    while (pos != targetPos){
        pos += 1 * mod;
        curr = getNext(curr);
    }   
    // return the selected value
    T retval = curr->data;
    this->unlock();
    return retval;
}

// appends a node to the end of the lists
template <typename T>
void LinkedList<T>::pushBack(const T& val){
    this->lock();
    ListNode<T>* newNode = new ListNode<T>;
    newNode->data = val;
    // this is the first node in the list
    if (!this->head){
        this->head = newNode;
        this->tail = newNode;
        this->listSize++;
        this->unlock();
        return;
    }
    // insert the new node after the current tail
    ListNode<T>* prev = this->tail;
    this->tail = newNode;
    prev->next = this->tail;
    this->tail->prev = prev;
    this->listSize++;
    this->unlock();
}

// appends a node to the start of the list
template <typename T>
void LinkedList<T>::pushFront(const T& val){
    this->lock();
    ListNode<T>* newNode = new ListNode<T>;
    newNode->data = val;
    // this is the first node in the list
    if (!this->head){
        this->head = newNode;
        this->tail = newNode;
        this->unlock();
        this->listSize++;
        return;
    }
    // insert the new node before the current head
    ListNode<T>* next = this->head;
    this->head = newNode;
    next->prev = this->head;
    this->head->next = next;
    this->listSize++;
    this->unlock();
}

// inserts a new value into the specified position in the linked list
template <typename T>
void LinkedList<T>::insert(const T& val, unsigned targetPos){
    this->lock();
    // ensure the position can be reached
    if (targetPos == 0){
        this->unlock();
        this->pushFront(val);
        return;
    }
    else if (targetPos == this->listSize){
        this->unlock();
        this->pushBack(val);
        return;
    }
    if (targetPos > this->listSize)
        throw std::out_of_range("position out of range");
    // determine which direction to search from 
    int mod {1}, pos{0};
    ListNode<T>* curr = this->head;
    std::function<ListNode<T>* (ListNode<T>*)> getNext = [] (ListNode<T>* node) {return node->next;}; 
    if (this->listSize < this->listSize - targetPos){
        pos = this->listSize - 1;
        mod = -1;
        curr = this->tail;
        getNext = [] (ListNode<T>* node) {return node->prev;};
    }
    // go to the specified element
    while (pos != targetPos - 1){
        pos += 1 * mod;
        curr = getNext(curr);
    }   
    // insert the given value
    ListNode<T>* newNode = new ListNode<T>;
    newNode->data = val;
    ListNode<T>* next = curr->next;
    newNode->data = val;
    next->prev = newNode;
    newNode->next = next;
    curr->next = newNode;
    // update the list size
    this->listSize++;
    this->unlock();
}

// pops the front value of the linked list. Raises an error if the list is empty
template <typename T>
T LinkedList<T>::popFront(){
    this->lock();
    if (!this->head){
        this->unlock();
        throw std::out_of_range("this linked list is empty");
    }
    T retval = this->head->data;
    ListNode<T>* tmp = this->head->next;
    delete this->head;
    this->head = tmp;
    this->head->prev = nullptr;
    this->listSize--;
    this->unlock();
    return retval;
}

// pops the back value of the linked lists. Raises an error if the list is empyy
template <typename T>
T LinkedList<T>::popBack(){
    this->lock();
    if (!this->head){
        this->unlock();
        throw std::out_of_range("this linked list is empty");
    }
    T retval = this->tail->data;
    ListNode<T>* tmp = this->tail->prev;
    delete this->tail;
    this->tail = tmp;
    this->tail->next = nullptr;
    this->listSize--;
    this->unlock();
    return retval;
}

// removes the value at the given index
template <typename T>
void LinkedList<T>::remove(unsigned targetPos){
    this->lock();
    if (targetPos > this->listSize)
        throw std::out_of_range("index out of range");
    // traverse the list until the target is found
    int pos {0};
    ListNode<T>* curr = this->head;
    ListNode<T>* prev {nullptr};
    while (pos != targetPos){
        prev = curr;
        curr = curr->next;
        ++pos;
    }
    // remove curr from the list
    if (prev)
        prev->next = curr->next;
    if (curr->next)
        curr->next->prev = curr->prev;
    delete curr;
    // clean up
    this->listSize--;
    this->unlock();
}

template <typename T>
void LinkedList<T>::lock(){
    this->mut.lock();
}


template <typename T>
void LinkedList<T>::unlock(){
    this->mut.unlock();
}


#endif
