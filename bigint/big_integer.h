#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <cstddef>

#include <iosfwd>
#include <vector>
#include "utils/myvector.h"

typedef unsigned long long ull;
typedef unsigned int ui;
const ui BASE = sizeof(ui) * 8;



struct big_integer
{


    big_integer();
    big_integer(big_integer const& other);
    big_integer(int a);
    big_integer(myvector const& data, bool sign);
    explicit big_integer(std::string const& str);

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);


    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

    template <class FunctorT>
    friend big_integer apply_operator(big_integer a, big_integer const& b, FunctorT functor) {
        size_t min = a.size() > b.size() ? b.size() : a.size();
        size_t max = a.size() > b.size() ? a.size() : b.size();
        myvector temp(max);
        for (size_t i = 0; i < min; i++) {
            temp[i] = functor(a.get_real(i), b.get_real(i));
        }
        for (size_t i = min; i < max; i++) {
            temp[i] = functor(a.get_digit(i), b.get_digit(i));
        }
        return big_integer(temp, !(temp.back() & (1 << (BASE - 1))));
    }
    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

    friend big_integer operator+(big_integer a, big_integer const& b);
    friend big_integer operator-(big_integer a, big_integer const& b);
    friend big_integer operator*(big_integer a, big_integer const& b);
    friend big_integer operator/(big_integer a, big_integer const& b);
    friend big_integer operator%(big_integer a, big_integer const& b);

    friend big_integer operator&(big_integer a, big_integer const& b);

    friend  big_integer operator|(big_integer a, big_integer const& b);

    friend big_integer operator^(big_integer a, big_integer const& b);
    friend big_integer operator<<(big_integer a, ui b);
    friend big_integer operator>>(big_integer a, int b);


    friend std::string to_string(big_integer const& a);

private:
    myvector data;
//    std::vector<ui> data;
    void ensure_capacity() ;

    bool sign;
    size_t size() const;
    ui get_digit(size_t ind) const;
    ui get_real(size_t ind) const;
    big_integer abs() const;
    void norma();

    big_integer dec() const;
};






bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);

#endif // BIG_INTEGER_H
