cmake_minimum_required(VERSION 3.8)
project(trampoline)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp args_count.h stack.cpp stack.hpp trampoline.hpp allocator.cpp allocator.hpp)
add_executable(trampoline ${SOURCE_FILES})