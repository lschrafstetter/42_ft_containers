#ifndef ITERATOR_VECOTR_HPP
#define ITERATOR_VECOTR_HPP

#include <limits>
#include <memory>
#include <stdexcept>
#include "utilities.hpp"

namespace ft {

//**************************************************
// This is a random access iterator
//**************************************************
template <typename datatype>
class Iterator_vector {
 public:
  typedef std::random_access_iterator_tag iterator_category;
  typedef datatype value_type;
  typedef datatype* pointer;
  typedef datatype& reference;
  typedef std::ptrdiff_t difference_type;

  //**************************************************
  // Constructor
  //**************************************************
  Iterator_vector() : data_(NULL) {}
  Iterator_vector(pointer p) : data_(p) {}
  Iterator_vector(const Iterator_vector& other) : data_(other.data_) {}
  ~Iterator_vector() {}

  //**************************************************
  // Operator overloads
  //**************************************************
  reference operator*() const { return *data_; }

  Iterator_vector& operator++() {
    ++data_;
    return *this;
  }
  Iterator_vector operator++(int) {
    Iterator_vector tmp(*this);
    ++data_;
    return tmp;
  }
  Iterator_vector& operator--() {
    --data_;
    return *this;
  }
  Iterator_vector operator--(int) {
    Iterator_vector tmp(*this);
    --data_;
    return tmp;
  }
  Iterator_vector& operator+=(difference_type n) {
    data_ += n;
    return *this;
  }

  Iterator_vector operator+(difference_type n) const {
    Iterator_vector tmp(*this);
    tmp += n;
    return tmp;
  }

  Iterator_vector& operator-=(difference_type n) {
    data_ -= n;
    return *this;
  }

  Iterator_vector operator-(difference_type n) const {
    Iterator_vector tmp(*this);
    tmp -= n;
    return tmp;
  }
  difference_type operator-(const Iterator_vector& other) const {
    return data_ - other.data_;
  }
  reference operator[](difference_type n) const { return this->data_[n]; }
  pointer operator->() const { return this->data_; }
  bool operator<(const Iterator_vector& other) const {
    return this->data_ < other.data_;
  }
  bool operator>(const Iterator_vector& other) const {
    return this->data_ > other.data_;
  }
  bool operator<=(const Iterator_vector& other) const {
    return this->data_ <= other.data_;
  }
  bool operator>=(const Iterator_vector& other) const {
    return this->data_ >= other.data_;
  }
  bool operator==(const Iterator_vector& other) const {
    return this->data_ == other.data_;
  }
  bool operator!=(const Iterator_vector& other) const {
    return this->data_ != other.data_;
  }

  //**************************************************
  // Functions
  //**************************************************
  pointer base() const { return data_; }


  //**************************************************
  // Conversion overloads
  //**************************************************

  // Implicit conversion to const_iterator (e.g. when making const iterator of
  // non-const vector)
  operator Iterator_vector<const value_type>() const {
    return Iterator_vector<const value_type>(this->data_);
  }

 protected:
  pointer data_;
};

//**************************************************
// Non-member operator overloads
//**************************************************

template <typename T>
Iterator_vector<T> operator+(typename Iterator_vector<T>::difference_type n,
                             const Iterator_vector<T>& it) {
  Iterator_vector<T> tmp(it);
  tmp += n;
  return tmp;
}

template <typename T>
typename Iterator_vector<T>::difference_type operator-(
    const Iterator_vector<T>& lhs, const Iterator_vector<T>& rhs) {
  return rhs.base() - lhs.base();
}

template <typename T>
bool operator!=(Iterator_vector<T> lhs, Iterator_vector<const T> rhs) {
  return (lhs.base() != rhs.base());
}

template <typename T>
bool operator==(Iterator_vector<T> lhs, Iterator_vector<const T> rhs) {
  return (lhs.base() == rhs.base());
}

template <typename T>
bool operator<(Iterator_vector<T> lhs, Iterator_vector<const T> rhs) {
  return (lhs.base() < rhs.base());
}

template <typename T>
bool operator<=(Iterator_vector<T> lhs, Iterator_vector<const T> rhs) {
  return (lhs.base() <= rhs.base());
}

template <typename T>
bool operator>(Iterator_vector<T> lhs, Iterator_vector<const T> rhs) {
  return (lhs.base() > rhs.base());
}

template <typename T>
bool operator>=(Iterator_vector<T> lhs, Iterator_vector<const T> rhs) {
  return (lhs.base() >= rhs.base());
}

}  // namespace ft
#endif