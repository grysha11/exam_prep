#pragma once
#include <sstream>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

class bigint {
	public:
		std::vector<int> _digits;
		void norminette();
		size_t convertForShift(const bigint& shift) const;
		bigint();
		bigint(unsigned long long num);
		bigint(std::string num);
		bigint(const bigint& other);
		~bigint();

		bigint& operator=(const bigint& other);
		bigint operator+(const bigint& other) const;
		bigint& operator+=(const bigint& other);
		bigint& operator++();
		bigint operator++(int);
		bigint operator<<(size_t num) const;
		bigint operator<<(const bigint& other) const;
		bigint operator>>(size_t num) const;
		bigint operator>>(const bigint& other) const;
		bigint operator<<=(size_t num);
		bigint operator<<=(const bigint& other);
		bigint operator>>=(size_t num);
		bigint operator>>=(const bigint& other);
		bool operator<(const bigint& other) const;
		bool operator>(const bigint& other) const;
		bool operator<=(const bigint& other) const;
		bool operator>=(const bigint& other) const;
		bool operator==(const bigint& other) const;
		bool operator!=(const bigint& other) const;

		friend std::ostream& operator<<(std::ostream& os, const bigint& n);
};