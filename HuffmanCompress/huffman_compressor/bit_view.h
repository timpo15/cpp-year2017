//
// Created by timpo on 15.06.18.
//

#ifndef HUFFMANCOMPRESS_BIT_VIEW_H
#define HUFFMANCOMPRESS_BIT_VIEW_H


class bit_view {
    typedef uint64_t code_type;

public:

    code_type code = 0;
    size_t length = 0;
    bit_view() = default;

    void push_back(bool bit) {
        code |= (static_cast<code_type>(bit) << length);
        ++length;
    }

    void pop_back() {
        --length;
        code &= (1 << length) - 1;
    }
};


#endif //HUFFMANCOMPRESS_BIT_VIEW_H
