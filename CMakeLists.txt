cmake_minimum_required(VERSION 3.31)

project(ThreadSafeStructures VERSION 0.1.0)

# build the library
add_library(Libtss STATIC src/linkedlist.hpp src/stack.hpp src/linkedlist.hpp)
set_target_properties(Libtss PROPERTIES LINKER_LANGUAGE CXX)

# build the testing binary
find_package(GTest)
include(GoogleTest)
add_executable(tests
                test/tests.cpp
                src/dynarr.hpp
                src/stack.hpp
                src/linkedlist.hpp)  
target_link_libraries(tests PRIVATE GTest::gtest)
