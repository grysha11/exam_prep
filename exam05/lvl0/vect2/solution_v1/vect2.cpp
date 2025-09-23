#include "vect2.hpp"

vect2::vect2() {
    nums[0] = 0;
    nums[1] = 0;
}

vect2::vect2(int x, int y) {
    nums[0] = x;
    nums[1] = y;
}

vect2::vect2(const vect2& other) {
    nums[0] = other.nums[0];
    nums[1] = other.nums[1];
}

vect2::~vect2() {

}
        
int& vect2::operator[](int i) {
    return nums[i];
}

const int& vect2::operator[](int i) const {
    return nums[i];
}

vect2& vect2::operator=(const vect2& other) {
    if (this != &other) {
        nums[0] = other.nums[0];
        nums[1] = other.nums[1];
    }
    return *this;
}

vect2& vect2::operator=(int num) {
    nums[0] = num;
    nums[1] = num;
    return *this;
}

vect2& vect2::operator+=(const vect2& other) {
    nums[0] += other.nums[0];
    nums[1] += other.nums[1];
    return *this;
}

vect2& vect2::operator*=(int scalar) {
    nums[0] *= scalar;
    nums[1] *= scalar;
    return *this;
}

vect2& vect2::operator-=(const vect2& other) {
    nums[0] -= other.nums[0];
    nums[1] -= other.nums[1];
    return *this;
}

vect2& vect2::operator++() {
    ++nums[0];
    ++nums[1];
    return *this;
}

vect2 vect2::operator++(int) {
    vect2 temp(*this);
    ++(*this);
    return temp;
}

vect2& vect2::operator--() {
    --nums[0];
    --nums[1];
    return *this;
}

vect2 vect2::operator--(int) {
    vect2 temp(*this);
    --(*this);
    return temp;
}

vect2 vect2::operator-() {
    vect2 res(-nums[0], -nums[1]);
    return res;
}

bool vect2::operator==(const vect2& other) const {
    return nums[0] == other.nums[0] && nums[1] == other.nums[1];
}

bool vect2::operator!=(const vect2& other) const {
    return !(*this == other);
}

vect2 operator+(vect2 n1, const vect2& n2) {
    n1 += n2;
    return n1;
}

vect2 operator-(vect2 n1, const vect2& n2) {
    n1 -= n2;
    return n1;
}

vect2 operator*(vect2 n,  int scalar) {
    n *= scalar;
    return n;
}

vect2 operator*(int scalar, vect2 n) {
    n *= scalar;
    return n;
}

std::ostream& operator<<(std::ostream& os, const vect2& n) {
    os << "{" << n[0] << ", " << n[1] << "}" << std::endl;
    return os;
}
