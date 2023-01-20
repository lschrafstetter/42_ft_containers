#ifndef ITERATOR_MAP_HPP
#define ITERATOR_MAP_HPP

#include <limits>
#include <memory>
#include <stdexcept>
#include "utilities.hpp"

namespace ft {

//**************************************************
// This is a bidirectional iterator
//**************************************************

template <typename datatype>
class Iterator_map {
 public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef datatype value_type;
  typedef datatype* pointer;
  typedef datatype& reference;
  typedef std::ptrdiff_t difference_type;

  //**************************************************
  // Constructors
  //**************************************************

  Iterator_map() : data_(NULL) {}
  Iterator_map(pointer p) : data_(p) {}
  Iterator_map(const Iterator_map& other) {}
  ~Iterator_map() {}

  //**************************************************
  // Operator overloads
  //**************************************************

  reference operator*() const {}

  Iterator_map& operator++() {}
  Iterator_map operator++(int) {}
  Iterator_map& operator--() {}
  Iterator_map operator--(int) {}

  pointer operator->() const {}

  bool operator==(const Iterator_map& other) const {}
  bool operator!=(const Iterator_map& other) const {}

  //**************************************************
  // Functions
  //**************************************************
  pointer base() const { return data_; }

  //**************************************************
  // Conversion overloads
  //**************************************************

  // Implicit conversion to const_iterator (e.g. when making const iterator of
  // non-const vector)
  operator Iterator_map<const value_type>() const {
    return Iterator_map<const value_type>(this->data_);
  }

 protected:
  pointer data_;
};

//**************************************************
// Non-member operator overloads
//**************************************************

template <typename T>
bool operator!=(Iterator_map<T> lhs, Iterator_map<const T> rhs) {}

template <typename T>
bool operator==(Iterator_map<T> lhs, Iterator_map<const T> rhs) {}

}  // namespace ft
#endif