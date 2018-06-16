cmake_minimum_required(VERSION 3.10)
project(HuffmanCompress)

set(CMAKE_CXX_STANDARD 11)

add_library(HuffmanLib
        huffman_compressor/huffman_processor.cpp
        huffman_compressor/huffman_processor.h
        huffman_compressor/reader.h
        huffman_compressor/writer.h
        huffman_compressor/bit_view.h
        )

add_executable(huffman_compress main.cpp)

add_executable(huffman_test
        gtest/gtest.h
        gtest/gtest-all.cc
        gtest/gtest_main.cc
        tests.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -g3 ")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O3")


target_link_libraries(huffman_compress HuffmanLib)
target_link_libraries(huffman_test HuffmanLib -lpthread)