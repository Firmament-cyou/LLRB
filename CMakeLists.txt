cmake_minimum_required(VERSION 3.19)
project(LLRB)

set(CMAKE_CXX_STANDARD 20)

add_executable(LLRB main.cpp LLRBMap.hpp RBNode.hpp)