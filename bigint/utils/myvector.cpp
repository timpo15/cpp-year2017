#include "myvector.h"
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <algorithm>


#define mv myvector
#define ma myarray

typedef unsigned int ui;

//using mv = mv;
//typedef mv mv;

mv::ma *mv::ma::constr_myarray(ui size) {
    void *p = operator new((size) * (sizeof(ui) + 1));
    auto *result = new(p) ma();
    return result;
}

mv::ma::ma() : refCount(0) {}

mv::ma::~ma() = default;

void mv::ma::destructMyArray(ma *target) {
    if (target->refCount == 0) {
        operator delete(target);
    }
}

mv::mv() : length(0), realPtr(small_object) {}

mv::mv(ui new_size, ui elem) : length(new_size) {
    if (new_size <= smallLen) {
        for (ui i = 0; i < new_size; i++) {
            small_object[i] = elem;
        }
    } else {
        long_object.capacity = new_size;
        ma *temporary = ma::constr_myarray(new_size);
        ma::login(temporary);
        long_object.location = temporary;
        for (ui i = 0; i < new_size; i++) {
            long_object.location->data[i] = elem;
        }
    }
    realPtr = length <= smallLen ? small_object : long_object.location->data;

}

mv::mv(mv const &other) noexcept : length(other.length) {
    realPtr = length <= smallLen ? small_object : other.long_object.location->data;
    if (length > smallLen) {
        long_object.capacity = other.long_object.capacity;
        long_object.location = ma::login(other.long_object.location);
    } else {
        for (ui i = 0; i < smallLen; ++i) {
            small_object[i] = other.small_object[i];
        }
    }
}

mv &mv::operator=(mv const &other) noexcept {
    if (&other == this) {
        return *this;
    }
    if (length > smallLen) {
        ma::unLogin(this->long_object.location);
    }
    mv tmp(other);
    length = tmp.length;
    realPtr = length <= smallLen ? small_object : tmp.long_object.location->data;
    if (other.length <= smallLen) {
        std::swap(small_object, tmp.small_object);
    } else {
        std::swap(long_object, tmp.long_object);
        tmp.long_object.location = nullptr;
    }
    return *this;
}


mv::~mv() {
    if (length > smallLen) {
        ma::unLogin(long_object.location);
    }
}


ui &mv::back() {
    return realPtr[length - 1];
}



void mv::push_back(ui x) {
    if (length > smallLen) {
        if (length == long_object.capacity) {
            move(long_object.capacity * 2);
        }
//        if (long_object.location->refCount != 1) {
//            move(std::max(0u, 10 - long_object.capacity + length) + long_object.capacity);
//        }
        long_object.location->data[length] = x;
    } else if (length < smallLen) {
        small_object[length] = x;
    } else {
        move(smallLen * 2);
        long_object.location->data[length] = x;
    }
    ++length;
}

ui mv::pop_back() {
    ui x{};
    if (length > smallLen) {
        x = long_object.location->data[length - 1];
    } else {
        x = small_object[length - 1];
    }
    if (length == smallLen + 1) {
        move(smallLen);
    }
    --length;
    return x;
}

void mv::resize(ui new_size, ui elem) {
    move(new_size);
    if (new_size > smallLen && new_size > length) {
    } else {
        for (ui i = new_size; i < smallLen; i++) {
            small_object[i] = elem;
        }
    }
    length = new_size;
}

void mv::move(ui new_size) {
    if (new_size <= smallLen) {
        if (length > smallLen) {
            ma *tmp = long_object.location;
            memcpy(small_object, long_object.location->data, smallLen * sizeof(ui));
            ma::unLogin(tmp);
        }
        realPtr = small_object;
    } else {
        ma *new_data = ma::constr_myarray(new_size);
        memcpy(new_data->data, realPtr, std::min(length, new_size) * sizeof(ui));
        if (length> smallLen) ma::unLogin(long_object.location);
        long_object.location = ma::login(new_data);
        long_object.capacity = new_size;
        realPtr = long_object.location->data;
    }
}