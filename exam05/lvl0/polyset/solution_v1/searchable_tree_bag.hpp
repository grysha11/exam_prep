#pragma once
#include "tree_bag.hpp"
#include "searchable_bag.hpp"
#include <iostream>

class searchable_tree_bag : public tree_bag, public virtual searchable_bag {
    public:
        searchable_tree_bag();
        searchable_tree_bag(const searchable_tree_bag& other);
        searchable_tree_bag& operator=(const searchable_tree_bag& other);
        virtual ~searchable_tree_bag();

        virtual bool has(int val) const;
};

