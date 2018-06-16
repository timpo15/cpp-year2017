cmake_minimum_required(VERSION 2.8)

project(BIGINT)

include_directories(${BIGINT_SOURCE_DIR})

add_executable(big_integer_testing
               big_integer_testing.cpp
               big_integer.h
               big_integer.cpp
               utils/myvector.cpp
               utils/myvector.h
               gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")
#  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG -g" )
#  set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage " )
#  set(CMAKE_CXX_OUTPUT_EXTENSION 1)

target_link_libraries(big_integer_testing  -lpthread)
