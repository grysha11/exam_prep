#pragma once
#include "searchable_bag.hpp"

class set {
    private:
        searchable_bag& _bag;
        set();
        set& operator=(const set& other);
    public:
        set(searchable_bag& bag);
        set(const set& other);
        ~set();

        void insert(int val);
        void insert(int *arr, int size);
        bool has(int val) const;
        void print() const;
        void clear();
        searchable_bag& get_bag();
};
