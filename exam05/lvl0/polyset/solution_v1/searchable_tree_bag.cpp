#include "searchable_tree_bag.hpp"

searchable_tree_bag::searchable_tree_bag() : tree_bag() {

}

searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag& other) : tree_bag(other) {

}

searchable_tree_bag& searchable_tree_bag::operator=(const searchable_tree_bag& other) {
    if (this != &other) {
        tree_bag::operator=(other);
    }

    return *this;
}

searchable_tree_bag::~searchable_tree_bag() {

}

bool searchable_tree_bag::has(int val) const {
    node *tmp = this->tree;

    while (tmp != NULL) {
        if (val == tmp->value) {
            return true;
        } else if (val < tmp->value) {
            tmp = tmp->l;
        } else {
            tmp = tmp->r;
        }
    }

    return false;
}
