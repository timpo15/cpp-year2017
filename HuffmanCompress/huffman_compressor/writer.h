//
// Created by timpo on 15.06.18.
//

#ifndef HUFFMANCOMPRESS_WRITER_H
#define HUFFMANCOMPRESS_WRITER_H


class writer {
    static const size_t BUFF_SIZE = 128;
    std::ostream &out;
    size_t pos = 0;
    size_t shift = 0;
    uint8_t *buffer = nullptr;

    inline void inc_pos() {
        pos++;
        ++writed;
        if (pos >= BUFF_SIZE) {
            flush();
        }
    }


public:
    static size_t writed;
    explicit writer(std::ostream &out) : out(out), buffer(new uint8_t[BUFF_SIZE]) {
        std::fill(buffer, buffer + BUFF_SIZE, 0);
    }

    void write_bit(bool bit) {
        buffer[pos] |= bit << shift;
        shift++;
        if (shift >= 8) {
            inc_pos();
            shift = 0;
        }
    }

    void write_byte(uint8_t byte) {
        buffer[pos] = byte;
        inc_pos();
    }
    template <typename T>
    void write(T val) {
        for (size_t i = 0; i < sizeof(T); ++i) {
            write_byte(val >> (8 * i));
        }
    }

    void write(bit_view bits) {
        size_t some_place = 8 - shift;
        while (bits.length >= some_place) {
            buffer[pos] |= bits.code << shift;
            bits.length -= some_place;
            bits.code >>= some_place;
            some_place = 8;
            shift = 0;
            inc_pos();
        }
        buffer[pos] |= bits.code << shift;
        shift += bits.length;
    }

    void fix_buff() {
        if (shift != 0) {
            inc_pos();
            shift = 0;
        }
    }
    void flush() {
        auto *char_pointer = reinterpret_cast<char *>(buffer);
        out.write(char_pointer, pos);
        std::fill(buffer, buffer + BUFF_SIZE, 0);
        pos = 0;
    }

    ~writer() {
        auto *char_pointer = reinterpret_cast<char *>(buffer);
        out.write(char_pointer, pos);
        out.flush();
        delete[] buffer;
    }


};


#endif //HUFFMANCOMPRESS_WRITER_H
