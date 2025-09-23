#pragma once
#include <iostream>
#include <sstream>

class vect2 {
    private:
        int nums[2];
    public:
        vect2();
        vect2(int x, int y);
        vect2(const vect2& other);
        ~vect2();
        
        int& operator[](int i);
        const int& operator[](int i) const;
        vect2& operator=(const vect2& other);
        vect2& operator=(int num);
        vect2& operator+=(const vect2& other);
        vect2& operator*=(int scalar);
        vect2& operator-=(const vect2& other);
        vect2& operator++();
        vect2 operator++(int);
        vect2& operator--();
        vect2 operator--(int);
        vect2 operator-();
        bool operator==(const vect2& other) const;
        bool operator!=(const vect2& other) const;

        friend vect2 operator+(vect2 n1, const vect2& n2);
        friend vect2 operator-(vect2 n1, const vect2& n2);
        friend vect2 operator*(vect2 n,  int scalar);
        friend vect2 operator*(int scalar, const vect2 n);
        friend std::ostream& operator<<(std::ostream& os, const vect2& n);
};

