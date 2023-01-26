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

  //**************************************************
  // Constructors
  //**************************************************

  map() {}
  explicit map(const Compare& comp, const Allocator& alloc = Allocator()) {}
  template <class InputIt>
  map(InputIt first, InputIt last, const Compare& comp = Compare(),
      const Allocator& alloc = Allocator()) {}
  map(const map& other) {}
  ~map() {}

  //**************************************************
  // Member classes
  //**************************************************

  class value_compare
      : public std::binary_function<value_type, value_type, bool> {
   public:
    typedef bool result_type;
    typedef value_type first_argument_type;
    typedef value_type second_argument_type;

   protected:
    key_compare comp;
    value_compare() {}
    value_compare(Compare c) {}
    bool operator()(const value_type& lhs, const value_type& rhs) const {
      return comp(lhs, rhs);
    }
  };

  //**************************************************
  // Operator overloads
  //**************************************************

  map& operator=(const map& other) {}

  //**************************************************
  // Member functions
  //**************************************************

  allocator_type get_allocator() const {}

  //**************************************************
  // Element access
  //**************************************************

  reference at(const Key& key) {}
  const_reference at(const Key& key) const {}
  reference operator[](const Key& key) {}

  //**************************************************
  // Iterators
  //**************************************************

  // iterator begin() {}
  // const_iterator begin() const {}
  // iterator end() {}
  // const_iterator end() const {}
  // reverse_iterator rbegin() {}
  // const_reverse_iterator rbegin() const {}
  // reverse_iterator rend() {}
  // const_reverse_iterator rend() const {}

  //**************************************************
  // Capacity
  //**************************************************

  bool empty() const {}
  size_type size() const {}
  size_type max_size() const {
    return std::numeric_limits<difference_type>::max();
  }

  //**************************************************
  // Modifiers
  //**************************************************

  void clear() {}
  /* std::pair<iterator, bool> insert(const value_type& value) {}
  iterator insert(iterator pos, const value_type& value) {}
  template <class InputIt>
  void insert(InputIt first, InputIt last) {}
  void erase( iterator pos ) {}
  void erase( iterator first, iterator last ) {} */
  size_type erase(const Key& key) {}
  void swap(map& other) {}

  //**************************************************
  // Lookup
  //**************************************************

  size_type count(const Key& key) const {}
  /* iterator find(const Key& key) {}
  const_iterator find(const Key& key) const {}
  std::pair<iterator, iterator> equal_range(const Key& key) {}
  std::pair<const_iterator, const_iterator> equal_range(const Key& key) const {}
  iterator lower_bound(const Key& key) {}
  const_iterator lower_bound(constKey& key) const {}
  iterator upper_bound(const Key& key) {}
  const_iterato upper_bound(const Key& key) const {} */

  //**************************************************
  // Observers
  //**************************************************

  key_compare key_comp() const {}
  ft::map<Key, T, Compare, Allocator>::value_comp value_comp() const {}

 private:
};

//**************************************************
// Non-member functions
//**************************************************

template <class Key, class T, class Compare, class Alloc>
bool operator==(const ft::map<Key, T, Compare, Alloc>& lhs,
                const ft::map<Key, T, Compare, Alloc>& rhs) {
  if (lhs.size() != rhs.size()) return false;
  return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class Key, class T, class Compare, class Alloc>
bool operator!=(const ft::map<Key, T, Compare, Alloc>& lhs,
                const ft::map<Key, T, Compare, Alloc>& rhs) {
  return !(lhs == rhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator<(const ft::map<Key, T, Compare, Alloc>& lhs,
               const ft::map<Key, T, Compare, Alloc>& rhs) {
  return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                     rhs.end());
}

template <class Key, class T, class Compare, class Alloc>
bool operator<=(const ft::map<Key, T, Compare, Alloc>& lhs,
                const ft::map<Key, T, Compare, Alloc>& rhs) {
  return !(lhs < rhs || lhs == rhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator>(const ft::map<Key, T, Compare, Alloc>& lhs,
               const ft::map<Key, T, Compare, Alloc>& rhs) {
  return !(lhs > rhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator>=(const ft::map<Key, T, Compare, Alloc>& lhs,
                const ft::map<Key, T, Compare, Alloc>& rhs) {
  return !(lhs < rhs);
}

}  // namespace ft

namespace std {

// Specialization of the std::swap function
template <class Key, class T, class Compare, class Alloc>
void swap(std::map<Key, T, Compare, Alloc>& lhs,
          std::map<Key, T, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}

}  // namespace std

#endif  // MAP_H