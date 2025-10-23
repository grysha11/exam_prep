#include "set.hpp"

set::set(searchable_bag& bag) : _bag(bag) {

}

set::set(const set& other) : _bag(other._bag) {

}

set::~set() {

}

void set::insert(int val) {
    if (!this->has(val)) {
        this->_bag.insert(val);
    }
}

void set::insert(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        this->insert(arr[i]);
    }
}

bool set::has(int val) const {
    return this->_bag.has(val);
}

void set::print() const {
    this->_bag.print();
}

void set::clear() {
    this->_bag.clear();
}

searchable_bag& set::get_bag() {
    return this->_bag;
}
