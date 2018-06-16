//
// Created by timpo on 15.06.18.
//

#include "huffman_processor.h"
#include "reader.h"
#include "writer.h"
#include <queue>

size_t reader::readed = 0;
size_t writer::writed = 0;

huffman_processor::huffman_processor() {
    for (size_t i = 0; i < 256; ++i) {
        frequency[i] = 0;
    }
};

std::pair<size_t, size_t> huffman_processor::encode(std::istream &input_stream,
                                                    std::ostream &output_stream,
                                                    std::function<void(double)> handler) {

    reader input(input_stream);

    if (input.start == -1) {
        throw std::runtime_error("Input file not found");
    }

    writer output(output_stream);
    size_t message_size = 0;

    while (input.hasNext()) {
        uint8_t readByte = input.readByte();
        ++frequency[readByte];
        ++message_size;
    }

    Huffman_tree tree = build_tree(frequency);
    shifr.resize(WORD_COUNT);
    if (tree.root != NULL_IND) {
        generate_shifr(tree, tree.root, bit_view());
    }

    size_t readed = reader::readed;
    tree.print(output);
    output.write(message_size);
    input.resetToStart();

    size_t mod = message_size / 1000;
    size_t rest = 0;
    if (mod > 0) {
        rest = message_size % (mod);

        size_t mod_i = mod * 100;
        for (size_t i = 0; i < 1000; ++i) {
            for (size_t j = 0; j < mod; ++j) {
                bit_view &bits = shifr[input.readByte()];
                output.write(bits);
            }
            handler(i * mod_i / message_size);
        }
        for (size_t j = 0; j < rest; ++j) {
            bit_view &bits = shifr[input.readByte()];
            output.write(bits);
        }
    } else {
        for (size_t j = 0; j < message_size; ++j) {
            bit_view &bits = shifr[input.readByte()];
            output.write(bits);
        }
    }

    size_t writed = writer::writed;
    output.fix_buff();
    output.flush();
    return {readed, writed};
}

std::pair<size_t, size_t> huffman_processor::encode(std::istream &input_stream, std::ostream &output_stream) {
    return encode(input_stream, output_stream, [](double a) { return; });
}

huffman_processor::Huffman_tree huffman_processor::build_tree(size_t frequency[]) {

    size_t tree_pos = 0;
    std::priority_queue<Node, std::vector<Node>, Node_comporator> priority_queue;
    Huffman_tree tree{};
    for (size_t i = 0; i < 256; ++i) {
        if (frequency[i] > 0) {
            tree.tree[tree_pos] = Node(tree_pos, frequency[i], i);
            priority_queue.emplace(tree.tree[tree_pos]);
            ++tree_pos;
        }
    }
    while (priority_queue.size() > 1) {
        Node first = priority_queue.top();
        priority_queue.pop();
        Node second = priority_queue.top();
        priority_queue.pop();
        tree.tree[tree_pos] = Node(tree_pos, first.my_ind, second.my_ind, first.weight + second.weight);
        priority_queue.emplace(tree.tree[tree_pos]);
        ++tree_pos;
    }
    if (!priority_queue.empty()) {
        tree.root = priority_queue.top().my_ind;
    }

    return tree;
}

void huffman_processor::generate_shifr(const huffman_processor::Huffman_tree &tree, size_t ind, bit_view bits) {
    if (tree.tree[ind].nodes[0] != NULL_IND) {
        bits.push_back(false);
        generate_shifr(tree, tree.tree[ind].nodes[0], bits);
        bits.pop_back();
        bits.push_back(true);
        generate_shifr(tree, tree.tree[ind].nodes[1], bits);
    } else {
        shifr[tree.tree[ind].word] = bits;
    }
}

std::pair<size_t, size_t> huffman_processor::decode(std::istream &input_stream,
                                                    std::ostream &output_stream,
                                                    std::function<void(double)> handler) {
    reader input(input_stream);
    if (input.start == -1) {
        throw std::runtime_error("Input file not found");
    }
    writer output(output_stream);
    Huffman_tree tree;
    tree.build(input);
    size_t message_size = input.read_sizet();
    size_t mod = message_size / 1000;
    size_t rest = 0;
    if (mod > 0) {
        rest = message_size % (mod);

        size_t mod_i = mod * 100;
        for (size_t i = 0; i < 1000; ++i) {
            for (size_t j = 0; j < mod; ++j) {
                output.write_byte(tree.getNextWord(input));
            }
            handler(i * mod_i / message_size);
        }
        for (size_t j = 0; j < rest; ++j) {
            output.write_byte(tree.getNextWord(input));
        }
    } else {
        for (size_t j = 0; j < message_size; ++j) {
            output.write_byte(tree.getNextWord(input));
        }
    }
    handler(100);
    size_t readed = reader::readed;
    size_t writed = output.writed;
    return {readed, writed};
}


std::pair<size_t, size_t> huffman_processor::decode(std::istream &input_stream, std::ostream &output_stream) {
    return decode(input_stream, output_stream, [](double a) { return; });
}


void huffman_processor::Huffman_tree::print(writer &out) {
    std::vector<bool> path;
    std::vector<uint8_t> words;
    if (root != NULL_IND) {
        fill_meta(root, path, words);
    }
    out.write(static_cast<uint16_t >(path.size()));
    out.write(static_cast<uint16_t >(words.size()));
    for (bool bit: path) {
        out.write_bit(bit);
    }
    out.fix_buff();
    for (uint8_t byte : words) {
        out.write_byte(byte);
    }
    out.flush();
}

//00010110
void huffman_processor::Huffman_tree::fill_meta(size_t ind, std::vector<bool> &path, std::vector<uint8_t> &words) {
    if (tree[ind].nodes[0] != NULL_IND) {
        path.push_back(false);
        fill_meta(tree[ind].nodes[0], path, words);
        path.push_back(false);
        fill_meta(tree[ind].nodes[1], path, words);
    } else {
        words.push_back(tree[ind].word);
    }
    path.push_back(true);
}

void huffman_processor::Huffman_tree::build(reader &input) {
    std::vector<bool> path;
    std::vector<uint8_t> words;

    uint16_t tree_size = input.read_size();
    uint16_t words_count = input.read_size();
    for (size_t i = 0; i < tree_size; ++i) {
        path.push_back(input.read_bit());
    }
    input.fix_buff();
    for (size_t i = 0; i < words_count; ++i) {
        words.push_back(input.readByte());
    }
    size_t ind = 0;
    root = 0;
    tree[ind++] = Node(0);
    size_t path_pos = 0, words_pos = 0;
    if (!path.empty()) {
        dfs_build(ind, 0, path_pos, words_pos, path, words);
    }

}

void huffman_processor::Huffman_tree::dfs_build(size_t &free_ind,
                                                size_t ind,
                                                size_t &path_pos,
                                                size_t &words_pos,
                                                std::vector<bool> &path,
                                                std::vector<uint8_t> &words) {
    if (!path[path_pos]) {
        tree[ind].nodes[0] = free_ind;
        tree[free_ind] = Node(free_ind);
        free_ind++;
        ++path_pos;
        if (path_pos >= path.size()) {
            throw std::runtime_error("File Damaged");
        }
        dfs_build(free_ind, tree[ind].nodes[0], path_pos, words_pos, path, words);
        tree[ind].nodes[1] = free_ind;
        tree[free_ind] = Node(free_ind);
        if (path_pos > path.size() || path[path_pos]) {
            throw std::runtime_error("File Damaged");
        }
        ++path_pos;
        ++free_ind;
        dfs_build(free_ind, tree[ind].nodes[1], path_pos, words_pos, path, words);
    } else {
        tree[ind].word = words[words_pos++];
        tree[ind].list = true;

    }
    if (path_pos >= path.size() || !path[path_pos]) {
        throw std::runtime_error("File Damaged");
    }
    ++path_pos;
}

