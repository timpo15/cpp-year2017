//
// Created by timpo on 15.06.18.
//
#include <iostream>
#include <vector>
#include <functional>
#include "bit_view.h"
#include "writer.h"
#include "reader.h"

#ifndef HUFFMANCOMPRESS_HUFFMAN_PROCESSOR_H
#define HUFFMANCOMPRESS_HUFFMAN_PROCESSOR_H


class huffman_processor {
    typedef uint8_t one_word;
    static const size_t NULL_IND = 666;
    static const size_t WORD_COUNT = (1u << sizeof(one_word) * 8);
    size_t frequency[512];
    std::vector<bit_view> shifr;

    struct Node {
        size_t my_ind = NULL_IND,
//                left = NULL_IND,
//                right = NULL_IND,
                weight = 0;
        one_word word = 0;
        size_t nodes[2] = {NULL_IND, NULL_IND};
        bool list = false;

        Node() = default;

        Node(size_t my_ind, size_t weight, one_word word) : my_ind(my_ind), weight(weight), word(word) {
        };

        Node(size_t my_ind, size_t l, size_t r, size_t weight) : my_ind(my_ind),
                                                                 nodes{l, r},
                                                                 weight(weight) {
//            nodes[0] = left;
//            nodes[1] = right;
        }

        explicit Node(size_t my_ind) : my_ind(my_ind) {}

    };

    struct Huffman_tree {
        size_t root = NULL_IND;
        Node tree[512];

        void print(writer &out);

        void fill_meta(size_t ind, std::vector<bool> &path, std::vector<uint8_t> &words);

        void build(reader &input);

        void dfs_build(size_t &free_ind,
                       size_t ind,
                       size_t &path_pos,
                       size_t &words_pos,
                       std::vector<bool> &path,
                       std::vector<uint8_t> &words);


        inline one_word getNextWord(reader &input) {
            Node *cur = &tree[0];
            while (!cur->list) {
                auto f = input.read_bit();
                cur = &tree[cur->nodes[f]];

            }
            return cur->word;
        }

        // work but a lot of branch - misses(((
//        inline one_word getNextWord(reader &input) {
//            Node *cur = &tree[0];
//            while (cur->left != NULL_IND ) {
//                auto f = input.read_bit();
//                if (f) {

//                    cur = &tree[cur->right];
//                } else {
//                    cur = &tree[cur->left];
//                }
//            }
//            return cur->word;
//        }


        // work but a lot of instructions
//        inline one_word getNextWord(reader& input) {
//            size_t cur = 0;
//            while (tree[cur].left != NULL_IND) {
//                bool f = input.read_bit();
//                if (!f) {
//                    cur = tree[cur].left;
//                } else {
//                    cur = tree[cur].right;
//                }
//            }
//            return tree[cur].word;
//        }
    };

    struct Node_comporator {
        bool operator()(Node a, Node b) {
            return a.weight > b.weight;
        }
    };

    Huffman_tree build_tree(size_t[]);

    void generate_shifr(Huffman_tree const &tree, size_t ind, bit_view);


public:
    huffman_processor();

    std::pair<size_t, size_t> encode(std::istream &input_stream, std::ostream &output_stream, std::function<void(double)> handler);


    std::pair<size_t, size_t> encode(std::istream &input_stream, std::ostream &output_stream);

    std::pair<size_t, size_t> decode(std::istream &input_stream, std::ostream &output_stream, std::function<void(double)> handler);

    std::pair<size_t, size_t> decode(std::istream &input_stream, std::ostream &output_stream);


};


#endif //HUFFMANCOMPRESS_HUFFMAN_PROCESSOR_H
