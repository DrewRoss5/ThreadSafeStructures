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
class ListIterator{
    public:
        ListIterator(ListNode<T>* node);
        T& operator*();
        ListIterator<T>& operator++();
        ListIterator<T>& operator--();
        bool operator==(const ListIterator<T>& rhs);
        bool operator!=(const ListIterator<T>& rhs);
    private:
        ListNode<T>* node;
};

template <typename T>
ListIterator<T>::ListIterator(ListNode<T>* node){
    this->node = node;
}

// returns the value pointed to by a list iterator
template <typename T>
T& ListIterator<T>::operator*(){
    if (!this->node)
        throw std::out_of_range("this iterator is out of range");
    return this->node->data;
}

// increments the list iterator
template <typename T>
ListIterator<T>& ListIterator<T>::operator++(){
    if (!this->node)
        throw std::out_of_range("this iterator is out of range");
    this->node = node->next;
    return *this;
}

// decrements the list iterator
template <typename T>
ListIterator<T>& ListIterator<T>::operator--(){
    if (!this->node)
        throw std::out_of_range("this iterator is out of range");
    this->node = node->prev;
    return *this;
}


// compares two list iterators
template <typename T>
bool ListIterator<T>::operator==(const ListIterator<T>& rhs){
    return this->node == rhs.node;
}
template <typename T>
bool ListIterator<T>::operator!=(const ListIterator<T>& rhs){
    return this->node != rhs.node;
}

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
        ListIterator<T> begin();
        ListIterator<T> end();
    private:
        ListNode<T>* head {nullptr};
        ListNode<T>* tail {nullptr};
        unsigned listSize {0};
        std::mutex mut;
        void emptyList();
};

// constructs a linked list with the given node as the head
template <typename T>
LinkedList<T>::LinkedList(const T& initVal){
    this->head = new ListNode<T>;
    this->head->data = initVal;
    this->tail = this->head;
}

// returns the iterator from the beginning of the lsit
template <typename T>
ListIterator<T> LinkedList<T>::begin(){
    ListIterator<T> itt(this->head);
    return itt;
}

// return the iterator represting the end of the list
template <typename T>
ListIterator<T> LinkedList<T>::end(){
    ListIterator<T> itt(nullptr);
    return itt;
}


// deconstuctor for the LinkedList class
template <typename T>
LinkedList<T>::~LinkedList(){
   this->emptyList();
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

template <typename T>
void LinkedList<T>::lock(){
    this->mut.lock();
}


template <typename T>
void LinkedList<T>::unlock(){
    this->mut.unlock();
}


#endif
