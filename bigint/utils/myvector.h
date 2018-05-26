#ifndef BIGINT_MYVECTOR_H
#define BIGINT_MYVECTOR_H

#include <cstddef>
#include <cstring>

typedef unsigned int ui;

struct myvector {

public:
    myvector();

    explicit myvector(ui new_size, ui elem = 0);

    myvector(const myvector &other) noexcept;

    myvector &operator=(myvector const &other) noexcept;


    ~myvector();

    void push_back(ui x);

    ui pop_back();

    ui &back();

    void resize(ui new_size, ui elem = 0);

    const ui &operator[](ui index) const {
        return realPtr[index];
    }


    ui &operator[](ui index) {
        return realPtr[index];
    };

    ui size() const {
        return length;
    }

private:
    struct myarray {
        ui refCount;
        ui data[];

        myarray();

        static myarray *constr_myarray(ui size);

        static void destructMyArray(myarray *target);

        myarray &operator=(myarray const &other) = delete;

        static void unLogin(myarray *current) {
            if (current == nullptr) {
                return;
            }
            current->refCount--;
            if (0 == current->refCount)
                myarray::destructMyArray(current);
        }

        static myarray *login(myarray *current) {
            current->refCount++;
            return current;
        }

        ~myarray();
    };

    void move(ui new_size);

    struct forArray {
        ui capacity;
        myarray *location;
    };
    static const ui smallLen = sizeof(forArray) / sizeof(ui);
    ui length;
    ui *realPtr;

    union {
        forArray long_object;
        ui small_object[smallLen];
    };
};
#endif // BIGINT_MYVECTOR_H