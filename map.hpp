#ifndef MAP_H
#define MAP_H

#include <functional>
#include "utilities.hpp"

namespace ft {

template <class Key, class T, class Compare = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
class map {
 public: 
  //**************************************************
  // Typedefs
  //**************************************************

  typedef Key key_type;
  typedef T mapped_value;
  typedef std::pair<const Key, T> value_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef Compare key_compare;
  typedef Allocator allocator_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef Allocator::pointer pointer;
  typedef Allocator::const_pointer const_pointer;
  // typedef iterator_map<value_type> iterator;
  // typedef iterator_map<const value_type> const_iterator;
  // std::reverse_iterator<iterator> reverse_iterator;
  // std::reverse_iterator<const_iterator> const_reverse_iterator;

 private:
  

};
}  // namespace ft

#endif  // MAP_H