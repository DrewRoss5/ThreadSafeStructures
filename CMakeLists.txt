cmake_minimum_required(VERSION 3.31)

project(ThreadSafeStructures VERSION 0.1.0)

#add_library(LibTSS STATIC inc/linkedlist.hpp inc/stack.hpp)

#
#find_package(GTest)
#include(GoogleTest)
#add_executable(tests test/test.cpp inc/linkedlists.hpp inc/stack.hpp)
#target_link_libraries(tests PRIVATE GTest::gtest)

add_executable(mainTest inc/linkedlist.hpp inc/stack.hpp inc/dynarr.hpp src/main.cpp)
