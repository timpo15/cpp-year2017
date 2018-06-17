//
// Created by timpo on 16.06.18.
//

#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "huffman_compressor/huffman_processor.h"


TEST(correctness, simple_test) {
    std::string test_string = "This is test string";
    std::stringstream in(test_string);
    std::stringstream out;
    std::stringstream out2;
    huffman_processor huffmanProcessor;
    huffmanProcessor.encode(in, out);
    std::stringstream in2(out.str());
    huffmanProcessor.decode(in2, out2);
    EXPECT_EQ(test_string, out2.str());
}

TEST(correctness, file_not_found) {
    std::ifstream in("123qwerty.txt");
    std::ofstream out("1234qwe.txt");
    huffman_processor huffmanProcessor;
    EXPECT_ANY_THROW(huffmanProcessor.encode(in, out));
}


TEST(correctness, empty_file) {
    std::string test_string = "";

    std::stringstream in(test_string);
    std::stringstream out;
    std::stringstream out2;
    huffman_processor huffmanProcessor;
    huffmanProcessor.encode(in, out);
    std::stringstream in2(out.str());
    huffmanProcessor.decode(in2, out2);
    EXPECT_EQ(test_string, out2.str());
}
TEST(correctness, all_chars) {
    std::string test_string = "";
    for (int i = 0; i > 10000; ++i) {
        for (int i = 0; i < 256; i++) {
            test_string.push_back((i));
        }
    }
    std::stringstream in(test_string);
    std::stringstream out;
    std::stringstream out2;
    huffman_processor huffmanProcessor;
    huffmanProcessor.encode(in, out);
    std::stringstream in2(out.str());
    huffmanProcessor.decode(in2, out2);
    EXPECT_EQ(test_string, out2.str());
}

TEST(correctness, incorect_decompress) {
    std::string test_string = "This is test string";
    std::stringstream in(test_string);
    std::stringstream out;
    huffman_processor huffmanProcessor;
    EXPECT_ANY_THROW(huffmanProcessor.decode(in, out));
}

TEST(correctness, big_random){
    std::string null;
    for (int i = 0; i < 10000000; i++){
        null.push_back(static_cast<char>(rand()));
    }
    std::stringstream in(null);
    std::stringstream out;
    std::stringstream out2;
    huffman_processor huffmanProcessor;
    huffmanProcessor.encode(in, out);
    std::stringstream in2(out.str());
    huffmanProcessor.decode(in2, out2);
    EXPECT_EQ(null, out2.str());
}