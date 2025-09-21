#include "bigint.hpp"

void bigint::norminette() {
    while (_digits.size() > 1 && _digits.back() == 0) {
        _digits.pop_back();
    }
    if (_digits.empty()) {
        _digits.push_back(0);
    }
}

bigint::bigint() : _digits(1, 0) {
    
}

bigint::bigint(unsigned long long num) {
    if (num == 0) {
        _digits.push_back(0);
    } else {
        while (num > 0) {
            _digits.push_back(static_cast<int>(num % 10));
            num /= 10;
        }
    }
}

bigint::bigint(std::string num) {
    for (std::string::reverse_iterator it = num.rbegin(); it != num.rend(); ++it) {
        if (*it >= '0' && *it <= '9') {
            _digits.push_back(*it - '0');
        }
    }
    if (_digits.empty()) {
        _digits.push_back(0);
    }
    norminette();
}

bigint::bigint(const bigint& other) : _digits(other._digits) {
    
}

bigint::~bigint() {

}

bigint& bigint::operator=(const bigint& other) {
    if (this != &other) {
        _digits = other._digits;
    }
    return *this;
}

bigint bigint::operator+(const bigint& other) const {
    bigint res;
    res._digits.clear();

    int carry = 0;
    size_t n;

    if (_digits.size() > other._digits.size()) {
        n = _digits.size();
    } else {
        n = other._digits.size();
    }

    for (size_t i = 0; i < n || carry; i++) {
        int sum = carry;
        if (i < _digits.size()) {
            sum += _digits[i];
        }
        if (i < other._digits.size()) {
            sum += other._digits[i];
        }
        res._digits.push_back(sum % 10);
        carry = sum / 10;
    }

    return res;
}

bigint& bigint::operator+=(const bigint& other) {
    *this = *this + other;
    return *this;
}

bigint& bigint::operator++() {
    *this += bigint(1);
    return *this;
}

bigint bigint::operator++(int) {
    bigint tmp(*this);
    *this += bigint(1);
    return tmp;
}

size_t bigint::convertForShift(const bigint& shift) const {
    size_t res = 0;

    for (size_t i = 0; i < shift._digits.size(); ++i) {
        res = res * 10 + shift._digits[shift._digits.size() - 1 - i];
    }
    return res;
}

bigint bigint::operator<<(const bigint& other) const {
    size_t shift = convertForShift(other);

    if (*this == bigint(0)) {
        return *this;
    }
    bigint res(*this);
    res._digits.insert(res._digits.begin(), shift, 0);
    return res;
}

bigint bigint::operator<<(size_t num) const {
    if (*this == bigint(0)) {
        return *this;
    }
    bigint res(*this);
    res._digits.insert(res._digits.begin(), num, 0);
    return res;
}

bigint bigint::operator>>(size_t n) const {
    bigint res(*this);
    if (n >= res._digits.size()) return bigint(0);
    res._digits.erase(res._digits.begin(), res._digits.begin() + n);
    res.norminette();
    return res;
}

bigint bigint::operator>>(const bigint &other) const {
    size_t n = convertForShift(other);
    return *this >> n;
}

bigint bigint::operator<<=(size_t num) {
    *this = *this << num;
    return *this;
}

bigint bigint::operator<<=(const bigint& other) {
    *this = *this << other;
    return *this;
}

bigint bigint::operator>>=(size_t num) {
    *this = *this >> num;
    return *this;
}

bigint bigint::operator>>=(const bigint& other) {
    *this = *this >> other;
    return *this;
}

bool bigint::operator<(const bigint& other) const {
    if (_digits.size() != other._digits.size()) {
        return _digits.size() < other._digits.size();
    }
    for (int i = static_cast<int>(_digits.size()) - 1; i >= 0; --i) {
        if (_digits[i] != other._digits[i]) {
            return _digits[i] < other._digits[i];
        }
    }
    return false;
}

bool bigint::operator>(const bigint& other) const {
    return other < *this;
}

bool bigint::operator<=(const bigint& other) const {
    return !(other < *this);
}

bool bigint::operator>=(const bigint& other) const {
    return !(*this < other);
}

bool bigint::operator==(const bigint& other) const {
    return _digits == other._digits;
}

bool bigint::operator!=(const bigint& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const bigint& n) {
    for (std::vector<int>::const_reverse_iterator it = n._digits.rbegin(); it != n._digits.rend(); ++it) {
        os << *it;
    }
    return os;
}
