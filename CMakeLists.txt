cmake_minimum_required(VERSION 3.16)
project(template_graphs)

set(CMAKE_CXX_STANDARD 20)

add_executable(template_graphs tests.cpp concepts.h iterators.h graphs.h multimethod.h composite.h responsibility_chain.h library.h)