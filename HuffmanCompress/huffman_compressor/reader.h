//
// Created by timpo on 15.06.18.
//
#include <istream>

#ifndef HUFFMANCOMPRESS_READER_H
#define HUFFMANCOMPRESS_READER_H


class reader {
    static const size_t BYTE = 8;
    static const size_t BUFFER_SIZE = 128;
    std::istream &input;
    uint8_t *buffer;
    size_t pos = 0;
    size_t end_pos = 0;
    size_t shift = 0;

    void update_buffer() {
        auto *char_pointer = reinterpret_cast<char *>(buffer);
        input.read(char_pointer, BUFFER_SIZE);
        pos = 0;
        end_pos = static_cast<size_t>(input.gcount());
    }

    void nextSymbol() {
        ++pos;
        ++reader::readed;
        if (pos >= end_pos) {
            update_buffer();
        }
    }

public:
    static size_t readed;
    size_t start = 0;

    explicit reader(std::istream &input) : input(input), buffer(new uint8_t[BUFFER_SIZE]),
                                           start(static_cast<size_t>(input.tellg())) {

        update_buffer();
    }

    ~reader() {
        delete[] buffer;
    }

    uint8_t readByte() {
        uint8_t ans = buffer[pos];
        nextSymbol();
        return ans;
    }

    uint16_t read_size() {
        uint16_t ans = readByte();
        ans += (readByte() << 8);
        return ans;
    }

    bool hasNext() {
        return pos != end_pos;
    }

    void resetToStart() {
        input.clear();
        input.seekg(start);
        update_buffer();
    }

    bool read_bit() {

        bool res = (buffer[pos] & (1u << shift)) > 0;
        shift++;
        if (shift == BYTE) {
            nextSymbol();
            shift = 0;
        }
        return res;
    }

    void fix_buff() {
        if (shift != 0) {
            shift = 0;
            nextSymbol();
        }
    }

    uint64_t read_sizet() {
        uint64_t ans = 0;
        for (size_t i = 0; i < 8; i++) {
            ans += (readByte() << (8 * i));
        }
        return ans;
    }
};


#endif //HUFFMANCOMPRESS_READER_H
