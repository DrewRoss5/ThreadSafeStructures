# ThreadSafeStructures
A collection of Thread Safe data structure implementations in C++.<br>
Thread safety is ensured by all structures by using mutexes to read and write protect data.

# API
## Classes:
### DynArr\<T>
An auto-resizing dynamic array class, simillar to `std::vector<T>`
#### Constructors:
##### `DynArr<T>()`:
- The dedault constructor. Creates a dynamic array of type `T`, initalize the array with the starting capacity for 16 elements.
##### `DynArr<T> (int initCapacity)`:
- Creates a dynamic array of type `T` with a starting capacity of `initCapacity`. The main use case of this is to increase performance by minimizing resizing operations, if the max size of the DynArr is known at the time of object creation.
#### Getters:
##### `T get(int index)`:
- Returns the value of the element at `index`
- Notes
  - Throws an exception if the index is out of bounds
##### `T& operator[](int index)`:
- Returns a reference to the element at `index`
- Notes
  - Automatically resizes the array if `index` is out of bounds;
##### `unsigned size()`:
- Returns the current size of the array

#### Modifiers: 
##### `void pushBack(const T& val)`:
- Appends `val` to the end of the array.
##### `void clear()`:
- Clears all values out of the array and sets the size to 0
- Notes:
  - This keeps the array's current capacity
##### `void reserve(unsigned newCap)`:
- Resizes the array to have room for `newCap` elements.
- Notes:
    - if `newCap` is less than or equal to the array's current capacity, this is a non-operation
##### `void lock()`:
- Locks the array's internal mutex
##### `void unlock()`:
- Unlocks the array's internal mutex
### Stack<T>
A simple stack implementation simmilar to std::stack.
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
  - Unlike `std::stack`, this returns the value of the popped item.
##### `void lock()`:
- Locks the Stack's internal mutex.
##### `void unlock()`:
- Unlocks the stack's mutex



