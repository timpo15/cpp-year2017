#include "big_integer.h"

#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <limits>

const ui MAX_DIGIT = std::numeric_limits<ui>::max();

void big_integer::ensure_capacity() {
    while (data.size() > 1 && ((sign && data[data.size() - 1] == 0) || (!sign && data[data.size() - 1] == MAX_DIGIT))) {
        data.pop_back();
    }
}

size_t big_integer::size() const {
    return data.size();
}

ui big_integer::get_digit(size_t ind) const {
    if (ind >= data.size()) {
        if (sign) {
            return 0;
        } else {
            return MAX_DIGIT;
        }
    } else {
        return data[ind];
    }
}

ui big_integer::get_real(size_t ind) const {
    return data[ind];
}

big_integer big_integer::abs() const {
    big_integer tmp(*this);
    if (!sign) {
        tmp = -tmp;
        tmp.sign = true;
        return tmp;
    } else return tmp;
}

big_integer::big_integer() : data(1), sign(true) {}

big_integer::big_integer(big_integer const &other) : data(other.data), sign(other.sign) {
}

big_integer::big_integer(int a) : data(1), sign(a >= 0) {
    data[0] = static_cast<ui>(a);

}

big_integer::big_integer(myvector const &data2, bool sign2) : data(data2), sign(sign2) {
}

big_integer::big_integer(std::string const &str) {
    big_integer temp(0);
    bool f = false;
    for (auto c :str) {
        if (c == '-') {
            f = true;
            continue;
        }
        temp *= 10;
        temp += static_cast<ui>(c - '0');
    }
    if (f) {
        *this = -temp;
        return;
    }
    *this = temp;
}

big_integer &big_integer::operator=(big_integer const &other) {
    data = other.data;
    sign = other.sign;
    return *this;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    *this = *this + rhs;
    return *this;
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    *this = *this - rhs;
    return *this;
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    *this = *this * rhs;
    return *this;
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    *this = *this / rhs;
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    *this = *this % rhs;
    return *this;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    *this = *this & rhs;
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    *this = *this | rhs;
    return *this;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    *this = *this ^ rhs;
    return *this;
}

big_integer &big_integer::operator<<=(int rhs) {
    *this = *this << rhs;
    return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
    *this = *this >> rhs;
    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::dec() const {
    myvector ans(size());
    ans.push_back(sign ? 0 : MAX_DIGIT);
    ui min = 1;
    ull carry = 0, now = 0;
    for (size_t i = 0; i < size(); i++) {
        now = ull(get_real(i)) + carry + min;
        ans[i] = static_cast<ui>(now);
        min = 0;
        carry = now >> (BASE);
    }
    return big_integer(ans, sign);
}

big_integer big_integer::operator-() const {
    if (*this == 0) {
        return *this;
    }
    big_integer tmp(*this);

    tmp = ~tmp;
    tmp = tmp.dec();
    tmp.ensure_capacity();
    return tmp;
}

big_integer big_integer::operator~() const {
    myvector temp(data.size());
    for (size_t i = 0; i < data.size(); i++) {
        temp[i] = ~data[i];
    }
    return big_integer(temp, !sign);
}

big_integer &big_integer::operator++() {
    *this = *this + 1;
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer r = *this;
    ++*this;
    return r;
}

big_integer &big_integer::operator--() {
    *this = *this - 1;

    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer r = *this;
    --*this;
    return r;
}

big_integer operator+(big_integer a, big_integer const &b) {
    a.ensure_capacity();
    big_integer tmp(b);
    tmp.ensure_capacity();
    size_t min = a.size() > tmp.size() ? tmp.size() : a.size();
    size_t max = a.size() > tmp.size() ? a.size() : tmp.size();
    bool f = a.sign;
    if (b.abs() >= a.abs() && a.sign != b.sign) {
        f = !f;
        if (-b == a) {
            return big_integer(0);
        }
    }
    ull sum = 0, carry = 0;
    myvector temp(max);
    temp.push_back(f ? 0 : MAX_DIGIT);
    for (size_t i = 0; i < min; i++) {
        sum = (carry + a.get_real(i)) + tmp.get_real(i);
        temp[i] = static_cast<ui>(sum);
        carry = (sum >> BASE);
    }
    for (size_t i = min; i < max; i++) {
        sum = static_cast<ull>(a.get_digit(i)) + static_cast<ull>(tmp.get_digit(i)) + carry;
        temp[i] = static_cast<ui>(sum);
        carry = (sum >> BASE);
    }
    if (a.sign == b.sign && a.sign) {
        temp[max] += carry;
    }
    big_integer t(temp, f);
    t.ensure_capacity();
    return t;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a + (-b);
}

void mul_vector(myvector &temp, myvector a, myvector b) {
    for (size_t i = 0; i < a.size(); i++) {
        ull carry = 0, mul = 0, per = 0;
        for (size_t j = 0; j < b.size(); j++) {
            mul = static_cast<ull>(a[i]) * b[j];
            per = (mul & MAX_DIGIT) + carry + temp[i + j];
            temp[i + j] = static_cast<ui>(per);
            carry = (mul >> BASE) + (per >> BASE);
        }
        temp[i + b.size()] += static_cast<ui>(carry);
    }
    while (temp.size() > 1 && temp.back() == 0) {
        temp.pop_back();
    }
}

void big_integer::norma() {
    if (sign) {
        return;
    }
    if (size() == 0 || *this == 0) {
        return;
    }
    *this = -*this;
    this->sign = !this->sign;
}

big_integer operator*(big_integer a, big_integer const &b) {
    bool sign = (b.sign ^ a.sign);
    big_integer temp1(a.abs()), temp2(b.abs());
    myvector temp(a.size() + b.size());
    temp.push_back(!sign ? 0 : MAX_DIGIT);
    mul_vector(temp, temp1.data, temp2.data);
    big_integer ans(temp, !sign);
    ans.ensure_capacity();
    ans.norma();
    ans.ensure_capacity();
    return ans;
}

ui get_estimate(ui const &a, ui const &b, ui const &c) {
    ull temp;
    temp = ((ull(a) << BASE) + b) / c;
    if (temp > MAX_DIGIT) {
        temp = MAX_DIGIT;
    }
    return static_cast<ui>(temp);
}

bool test_vectors(myvector const &a, myvector const &b) {
    for (size_t i = 1; i <= a.size(); i++) {
        if (a[a.size() - i] != b[a.size() - i]) {
            return a[a.size() - i] < b[a.size() - i];
        }
    }
    return false;
}

void mul_vectors(myvector &a, myvector const &b, ui const &estim) {
    ull carry = 0, mul = 0;
    ull tmp = 0;
    a.resize(b.size() + 1);
    for (size_t i = 0; i < b.size(); i++) {
        mul = ull(b[i]) * estim;
        tmp = (mul & MAX_DIGIT) + carry;
        a[i] = static_cast<ui>(tmp);
        carry = (tmp >> BASE) + (mul >> BASE);
    }
    a[b.size()] = static_cast<ui>(carry);
}

void sub_vectors(myvector  &a, myvector const &b) {
    ull sub = 0, carry = 1;
    for (size_t i = 0; i < b.size(); i++) {
        sub = static_cast<ull>(a[i]) + static_cast<ull>(~b[i]) + carry;
        a[i] = static_cast<ui>(sub);
        carry = sub >> BASE;
    }
}

big_integer operator/(big_integer a, big_integer const &b) {
    bool sign = a.sign ^ b.sign;
    big_integer l(a.abs()), r(b.abs());
    if (l < r) {
        return 0;
    }
    ui normal = 1;
    if (r.data.back() < (MAX_DIGIT / 2)) {
        normal = 1 + static_cast<ui>((static_cast<ull>(MAX_DIGIT) + 1) / 2 /     (static_cast<ull>(r.data.back()) + 1));
    }
    l *= normal;
    r *= normal;
    l.ensure_capacity();
    r.ensure_capacity();

    l.ensure_capacity();
    r.ensure_capacity();
    myvector ans(l.size() - r.size() + 1), ltemp(r.size() + 1), rtemp(r.size() + 1, 0);
    for (size_t i = 0; i < r.size(); i++) {
        ltemp[i] = l.get_real(l.size() + i - r.size());
    }
    ltemp[r.size()] = l.get_digit(l.size());
    for (size_t i = 0; i < l.size() - r.size() + 1; i++) {
        ltemp[0] = l.get_real(l.size() - r.size() - i);
        ui estimate = get_estimate(ltemp[r.size()], ltemp[r.size() - 1], r.data.back());
        mul_vectors(rtemp, r.data, estimate);
        while (estimate >= 0 && test_vectors(ltemp, rtemp)) {
            mul_vectors(rtemp, r.data, --estimate);
        }
        ans[l.size() - r.size() - i] = estimate;
        sub_vectors(ltemp, rtemp);
        for (size_t j = ltemp.size() - 1; j > 0; j--) {
            ltemp[j] = ltemp[j - 1];
        }

    }
    big_integer res(ans, !sign);
    res.norma();
    return res;

}

big_integer operator%(big_integer a, big_integer const &b) {
    big_integer tmp = a.abs(), tmp2 = b.abs();
    big_integer tmp3 = tmp - (tmp / tmp2) * tmp2;
    if (tmp3 == 0) {

        tmp3.norma();
        return tmp3;
    } else {
        tmp3.sign = a.sign;
        tmp3.norma();
        return tmp3;
    }
}

big_integer operator&(big_integer a, big_integer const &b) {
    return apply_operator(a, b, std::bit_and<ui>());
}


big_integer operator|(big_integer a, big_integer const &b) {
    return apply_operator(a, b, std::bit_or<ui>());
}


big_integer operator^(big_integer a, big_integer const &b) {
    return apply_operator(a, b, std::bit_xor<ui>());
}

big_integer operator<<(big_integer a, ui b) {
    if (b == 0) {
        return a;
    }
    size_t added = b >> 5;
    size_t inner = b & (BASE - 1);
    myvector temp(a.size() + added + 1);
    temp[added] = static_cast<ui>(a.get_real(added) << inner);
    for (size_t i = added + 1; i < a.size() + added + 1; i++) {
        ull r = ull(a.get_real(i - added - 1)) >> (BASE - inner);
        ull l = ull(a.get_digit(i - added)) << inner;
        temp[i] = static_cast<ui>(r | l);
    }
    return big_integer(temp, a.sign);
}

big_integer operator>>(big_integer a, int b) {
    if (b == 0) {
        return a;
    }
    size_t added = b >> 5;
    size_t inner = b & (BASE - 1);
    size_t newsize = a.size() - added > 0 ? a.size() - added : 0;
    myvector temp(newsize);
    for (size_t i = 0; i < newsize; i++) {
        ull r = ull(a.get_digit(i - added + 1)) << (BASE - inner);
        ull l = ull(a.get_real(i - added)) >> inner;
        temp[i] = static_cast<ui>((r | l));
    }
    return big_integer(temp, a.sign);
}


bool operator==(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return false;
    }
    big_integer t1(a), t2(b);
    t1.ensure_capacity();
    t2.ensure_capacity();
    if (t1.size() != t2.size()) {
        return false;
    }
    for (size_t i = t1.size(); i > 0; i--) {
        if (t1.get_real(i - 1) != t2.get_real(i - 1)) {

            return false;
        }
    }
    return true;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return b.sign;
    }
    if (a.size() != b.size()) {
        return a.size() < b.size();
    }
//    big_integer t1(a), t2(b);
//    t1.ensure_capacity();
//    t2.ensure_capacity();
    for (size_t i = a.size(); i > 0; i--) {
        if (a.get_real(i - 1) != b.get_real(i - 1)) {

            return a.get_real(i - 1) < b.get_real(i - 1);
        }
    }
    return false;
}

bool operator>(big_integer const &a, big_integer const &b) {
    return b < a && !(a == b);
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return a < b || a == b;
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return b < a || b == a;
}

std::string to_string(big_integer const &a) {
    if (a == 0) {
        return "0";
    }
    ui const BASE10 = 1000000000;
    std::string ans = "";
    big_integer aa = a.abs();
    while (aa > 0) {
        ui temp = (aa % BASE10).get_real(0);
        for (size_t i = 0; i < 9; i++) {
            ans.push_back('0' + temp % 10);
            temp /= 10;
        }
        aa /= BASE10;
    }
    while (ans.size() > 0 && ans.back() == '0') {
        ans.pop_back();
    }
    if (!a.sign) {
        ans.push_back('-');
    }
    std::reverse(ans.begin(), ans.end());
    return ans;

}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}

