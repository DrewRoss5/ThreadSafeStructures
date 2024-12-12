cmake_minimum_required(VERSION 3.31)

project(ThreadSafeStructures VERSION 0.1.0)

#add_library(LibTSS STATIC inc/linkedlist.hpp inc/stack.hpp)

# build the testing binary
find_package(GTest)
include(GoogleTest)
add_executable(tests
                test/tests.cpp
                src/dynarr.hpp
                src/stack.hpp)  
target_link_libraries(tests PRIVATE GTest::gtest)