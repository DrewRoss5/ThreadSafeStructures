# ThreadSafeStructures
A collection of Thread Safe data structure implementations in C++.<br>
Thread safety is ensured by all structures by using mutexes to read and write protect data.

# API
## Classes:
### DynArr\<T>
----
An auto-resizing dynamic contiguous array class, simillar to `std::vector<T>`
#### Constructors:
##### `DynArr<T>()`:
- The dedault constructor. Creates a dynamic array of type `T`, initalize the array with the starting capacity for 16 elements
##### `DynArr<T> (int initCapacity)`:
- Creates a dynamic array of type `T` with a starting capacity of `initCapacity`. The main use case of this is to increase performance by minimizing resizing operations, if the max size of the DynArr is known at the time of object creation
#### Getters:
##### `T get(int index)`:
- Returns the value of the element at `index`
- Notes
  - Throws an exception if the index is out of bounds
##### `T& operator[](int index)`:
- Returns a reference to the element at `index`
- Notes
  - Automatically resizes the array if `index` is out of bounds
##### `unsigned size()`:
- Returns the current size of the array
#### Modifiers: 
##### `void pushBack(const T& val)`:
- Appends `val` to the end of the array
##### `void clear()`:
- Clears all values out of the array and sets the size to 0
- Notes:
  - This keeps the array's current capacity
##### `void reserve(unsigned newCap)`:
- Resizes the array to have room for `newCap` elements.
- Notes:
    - if `newCap` is less than or equal to the array's current capacity, this is a non-operation
##### `void lock()`:
- Locks the array's mutex
##### `void unlock()`:
- Unlocks the array's mutex



### Stack\<T>
----
A simple stack implementation simmilar to std::stack, with values of type T.
#### Constructors:
##### `Stack<T>()`:
- Creates a new stack with the starting capacity for 16 elements.
##### `Stack<T>(int initCapacity)`:
- Creates a new stack with a starting capacity of `initCapacity` elements. The main use case of this is to increase performance by minimizing resizing operations, if the max size of the Stack is known at the time of object creation.
#### Getters:
##### `unsigned size()`:
- Returns the current size of the stack
##### `T top()`:
- Returns the value of the top element of the stack
#### Modifiers:
##### `void push(const T& val)`:
- Pushes `val` to the top of the stack
##### `T pop()`:
- Removes the top element of the stack
- Notes:
  - Unlike `std::stack`, this returns the value of the popped item
##### `void lock()`:
- Locks the Stack's mutex
##### `void unlock()`:
- Unlocks the stack's mutex



### LinkedList\<T>
----
An implementation of a doubly linked list with nodes of type T.
#### Constructors:
##### `LinkedList<T>()`:
- Creates a new linked list with no elements and a size of 0
- Notes:
   - Positions are zero-indexed
##### `LinkedList<T>(const T& initVal)`:
- Creates a new liked list with a size of 1 and `initVal` serving has both the front and back
#### Getters:
##### `unsigned size()`:
- Returns the number of nodes currently in the LinkedList
##### `T front():`
- Returns the value of the list's head
##### `T back()`:
- Returns the value of the lists' tail
##### `T at(unsigned position)`:
- Returns the value of the element at the given position (starting from the head)
- Notes:
  - Raises an exception if the position is out of range
#### Modifiers 
##### `void pushFront(const T& val)`:
- Appends `val` to the front of the list
##### `void pushBack(const T& val)`:
- Appends `val` to the back of the list
##### `void insert(const T& val, unsigned pos)`:
- Inserts `val` in the position `pos`, starting from the front
- Notes:
  - Maintains order other than the new node that was inserted
  - Throws an exception if `pos` is out of range
##### `T popFront()`:
- Returns the value of the head of the list, and removes it from the list
- Notes:
  - Throws an exception if the list is empty
##### `T popBack()`:
- Returns the value of the tail of the list, and removes it from the list
- Notes:
  - Throws an exception if the list is empty
##### `void remove(unsigned pos)`:
- Removes the node at position `pos`
- Notes:
  - Throws an exception if `pos` is out of range
  - Throws an exception if `pos` is the first or last element of the list
##### `void clear()`
- Sets the list's size to zero, and deletes all nodes
##### `void lock()`:
- Locks the list's mutex
##### `void unlock()`
- Unlocks the list's mutex
