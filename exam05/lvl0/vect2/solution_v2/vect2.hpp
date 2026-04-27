#pragma once
#include <iostream>
#include <sstream>

class vect2 {
    private:
        int _nums[2];
    public:
        vect2();
        vect2(int num1, int num2);
        vect2& operator=(const vect2& other);
        int operator[](unsigned int idx);
        const int operator[](unsigned int idx) const;
        friend std::ostream& operator<<(std::ostream& os, const vect2& num);
};

