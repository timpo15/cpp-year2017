#include "myvector.h"
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <algorithm>


typedef unsigned int ui;


myvector::myarray *myvector::myarray::constr_myarray(ui size) {
    void *p = operator new((size) * (sizeof(ui) + 1));
    auto *result = new(p) myarray();
    return result;
}

myvector::myarray::myarray() : refCount(0) {}

myvector::myarray::~myarray() = default;

void myvector::myarray::destructMyArray(myarray *target) {
    if (target->refCount == 0) {
        operator delete(target);
    }
}

myvector::myvector() : length(0), realPtr(small_object) {}

myvector::myvector(ui new_size, ui elem) : length(new_size) {
    if (new_size <= smallLen) {
        for (ui i = 0; i < smallLen; i++) {
            small_object[i] = elem;
        }
        realPtr = small_object;
    } else {
        long_object.capacity = new_size;
        myarray *temporary = myarray::constr_myarray(new_size);
        myarray::login(temporary);
        long_object.location = temporary;
        for (ui i = 0; i < new_size; i++) {
            long_object.location->data[i] = elem;
        }
        realPtr = long_object.location->data;
    }

}

myvector::myvector(myvector const &other) noexcept : length(other.length) {

    if (length > smallLen) {
        long_object.capacity = other.long_object.capacity;
        long_object.location = myarray::login(other.long_object.location);
        realPtr = long_object.location->data;
    } else {
        for (ui i = 0; i < smallLen; ++i) {
            small_object[i] = other.small_object[i];
        }
        realPtr = small_object;
    }
}

myvector &myvector::operator=(myvector const &other) noexcept {
    if (&other == this) {
        return *this;
    }
    if (length > smallLen) {
        myarray::unLogin(this->long_object.location);
    }
    myvector tmp(other);
    length = tmp.length;
    if (other.length <= smallLen) {
        std::swap(small_object, tmp.small_object);
        realPtr = small_object;
    } else {
        std::swap(long_object, tmp.long_object);
        tmp.long_object.location = nullptr;
        realPtr = long_object.location->data;
    }
    return *this;
}


myvector::~myvector() {
    if (length > smallLen) {
        myarray::unLogin(long_object.location);
    }
}


ui &myvector::back() {
    return realPtr[length - 1];
}


void myvector::push_back(ui x) {
    if (length > smallLen) {
        if (length == long_object.capacity || long_object.location->refCount != 1) {
            move(long_object.capacity * 2);
        }
        long_object.location->data[length] = x;
    } else if (length < smallLen) {
        small_object[length] = x;
    } else {
        move(smallLen * 2);
        long_object.location->data[length] = x;
    }
    ++length;
}

ui myvector::pop_back() {
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

void myvector::resize(ui new_size, ui elem) {
    move(new_size);
//    for (ui i = new_size; i < smallLen; i++) {
//        small_object[i] = elem;
//    }
//    length = new_size;
    if (new_size > smallLen && new_size > length) {
        for (ui *ptr = long_object.location->data + length;
             ptr != long_object.location->data + new_size; ++ptr) {
            *ptr = elem;
        }
    } else {
        for (ui i = new_size; i < smallLen; i++) {
            small_object[i] = elem;
        }
    }
    length = new_size;
}

void myvector::move(ui new_size) {
    if (new_size <= smallLen) {
        if (length > smallLen) {
            myarray *tmp = long_object.location;
            memcpy(small_object, long_object.location->data, smallLen * sizeof(ui));
            myarray::unLogin(tmp);
        }
        realPtr = small_object;
    } else {
        myarray *new_data = myarray::constr_myarray(new_size);
//        memcpy(new_data->data, realPtr, std::min(length, new_size) * sizeof(ui));
//        if (length > smallLen) myarray::unLogin(long_object.location);
//        long_object.location = myarray::login(new_data);
//        long_object.capacity = new_size;
//        realPtr = long_object.location->data;
        if (length <= smallLen) {
            memcpy(new_data->data, small_object, smallLen * sizeof(ui));
        } else {
            memcpy(new_data->data, long_object.location->data,
                   std::min(length, new_size) * sizeof(ui));
            myarray::unLogin(long_object.location);
        }
        long_object.location = myarray::login(new_data);
        long_object.capacity = new_size;
        realPtr = long_object.location->data;
    }
}



//link time optimization