#ifndef MAP_H
#define MAP_H

#include "iterator_redblacktree.hpp"
#include "redblacktree.hpp"
#include "utilities.hpp"

namespace ft {

template <class Key, class T, class Compare = std::less<Key>,
          class Allocator = std::allocator<ft::pair<const Key, T> > >
class map {
 public:
  //**************************************************
  // Typedefs
  //**************************************************

  typedef Key key_type;
  typedef T mapped_value;
  typedef ft::pair<const Key, T> value_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef Compare key_compare;
  typedef Allocator allocator_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;

  class value_compare;
  typedef redblacktree<value_type, value_compare, allocator_type> tree_type;

  typedef iterator_rbt<value_type, typename tree_type::node_type> iterator;
  typedef iterator_rbt<const value_type, typename tree_type::node_type>
      const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

  //**************************************************
  // Constructors
  //**************************************************

  map() : tree_(value_compare(), allocator_type()) {}

  explicit map(const Compare& comp, const Allocator& alloc = Allocator())
      : tree_(comp, alloc) {}

  template <class InputIt>
  map(InputIt first, InputIt last, const Compare& comp = Compare(),
      const Allocator& alloc = Allocator())
      : tree_(comp, alloc) {
    insert(first, last);
  }

  map(const map& other) : tree_(other.tree_) {}

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

    value_compare() : comp(key_compare()) {}
    value_compare(key_compare c) : comp(c) {}
    bool operator()(const value_type& lhs, const value_type& rhs) const {
      return comp(lhs.first, rhs.first);
    }

   protected:
    key_compare comp;
  };

  //**************************************************
  // Operator overloads
  //**************************************************

  map& operator=(map other) {
    if (*this != other) tree_.swap(other.tree_);
    return *this;
  }

  //**************************************************
  // Member functions
  //**************************************************

  allocator_type get_allocator() const {}

  //**************************************************
  // Element access
  //**************************************************

  mapped_value& at(const Key& key) {
    iterator ret = find(key);
    if (ret == end()) throw std::out_of_range("No element with key found");
    return (*ret).second;
  }

  const mapped_value& at(const Key& key) const { return at(key); }

  mapped_value& operator[](const Key& key) {
    iterator ret = find(key);
    if (ret == end())
      return (*(insert(value_type(key, mapped_value())).first)).second;
    else
      return (*ret).second;
  }

  //**************************************************
  // Iterators
  //**************************************************

  iterator begin() { return iterator(tree_.get_first()); }
  const_iterator begin() const { return const_iterator(tree_.get_first()); }
  iterator end() { return iterator(tree_.get_end()); }
  const_iterator end() const { return const_iterator(tree_.get_end()); }
  reverse_iterator rbegin() { return reverse_iterator(tree_.get_end()); }
  const_reverse_iterator rbegin() const {
    return reverse_iterator(tree_.get_end());
  }
  reverse_iterator rend() { return reverse_iterator(tree_.get_first()); }
  const_reverse_iterator rend() const {
    return reverse_iterator(tree_.get_first());
  }

  //**************************************************
  // Capacity
  //**************************************************

  bool empty() const { return (size() == 0); }
  size_type size() const { return tree_.size(); }
  size_type max_size() const { return tree_.max_size(); }

  //**************************************************
  // Modifiers
  //**************************************************

  void clear() { tree_.clear(); }

  ft::pair<iterator, bool> insert(const value_type& value) {
    ft::pair<typename tree_type::node_type*, bool> tmp = tree_.insert(value);
    return ft::pair<iterator, bool>(iterator(tmp.first), tmp.second);
  }

  iterator insert(iterator pos, const value_type& value) {
    (void)pos;
    return iterator(tree_.insert(value).first);
  }

  template <class InputIt>
  void insert(InputIt first, InputIt last) {
    while (first != last) tree_.insert(*(first++));
  }

  void erase(iterator pos) { tree_.erase(*pos); }

  void erase(iterator first, iterator last) {
    while (first != last) tree_.erase(*(first++));
  }

  size_type erase(const Key& key) {
    bool erased = tree_.erase(value_type(key, mapped_value()));
    return erased;
  }

  void swap(map& other) { tree_.swap(other.tree_); }

  //**************************************************
  // Lookup
  //**************************************************

  size_type count(const Key& key) const {
    (void)key;
    iterator it1(tree_.find(value_type(key, mapped_value())));
    iterator it2(tree_.get_end());
    if (it1 == it2) return 0;
    return 1;
  }

  iterator find(const Key& key) {
    return iterator(tree_.find(value_type(key, mapped_value())));
  }

  const_iterator find(const Key& key) const {
    return const_iterator(tree_.find(value_type(key, mapped_value())));
  }
  
  // ft::pair<iterator, iterator> equal_range(const Key& key) {}
  // ft::pair<const_iterator, const_iterator> equal_range(const Key& key) const
  // {} iterator lower_bound(const Key& key) {} const_iterator lower_bound(const
  // Key& key) const {} iterator upper_bound(const Key& key) {} const_iterator
  // upper_bound(const Key& key) const {}

  //**************************************************
  // Observers
  //**************************************************

  key_compare key_comp() const { return key_compare(); }

  value_compare value_comp() const { return value_compare(key_compare()); }

 private:
  tree_type tree_;
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
bool operator>(const ft::map<Key, T, Compare, Alloc>& lhs,
                const ft::map<Key, T, Compare, Alloc>& rhs) {
  return !(lhs < rhs || lhs == rhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator<=(const ft::map<Key, T, Compare, Alloc>& lhs,
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
void swap(ft::map<Key, T, Compare, Alloc>& lhs,
          ft::map<Key, T, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}

}  // namespace std

#endif  // MAP_H