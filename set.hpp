#ifndef SET_H
#define SET_H

#include "iterator_redblacktree.hpp"
#include "redblacktree.hpp"
#include "utilities.hpp"

namespace ft {

template <class Key, class Compare = std::less<Key>,
          class Allocator = std::allocator<Key> >
class set {
 public:
  //**************************************************
  // Typedefs
  //**************************************************#

  typedef Key key_type;
  typedef Key value_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef Compare key_compare;
  typedef Compare value_compare;
  typedef Allocator allocator_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;

  typedef iterator_rbt<const value_type, rb_node<value_type> >
      iterator;  // Key always const
  typedef iterator_rbt<const value_type, rb_node<value_type> > const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

  typedef redblacktree<value_type, value_compare, allocator_type> tree_type;

  //**************************************************
  // Constructors
  //**************************************************

  set() : tree_(value_compare(), allocator_type()) {}

  explicit set(const Compare& comp, const Allocator& alloc = Allocator())
      : tree_(comp, alloc) {}

  template <class InputIt>
  set(InputIt first, InputIt last, const Compare& comp = Compare(),
      const Allocator& alloc = Allocator())
      : tree_(comp, alloc) {
    insert(first, last);
  }

  set(const set& other) : tree_(other.tree_) {}

  ~set() {}

  //**************************************************
  // Operator overloads
  //**************************************************

  set& operator=(set other) {
    if (*this != other) tree_.swap(other.tree_);
    return *this;
  }

  //**************************************************
  // Member functions
  //**************************************************

  allocator_type get_allocator() const { return tree_.get_allocator(); }

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
    bool erased = tree_.erase(key);
    return erased;
  }

  void swap(set& other) { tree_.swap(other.tree_); }

  //**************************************************
  // Lookup
  //**************************************************

  size_type count(const Key& key) const {
    (void)key;
    iterator it1(tree_.find(key));
    iterator it2(tree_.get_end());
    if (it1 == it2) return 0;
    return 1;
  }

  iterator find(const Key& key) { return iterator(tree_.find(key)); }

  const_iterator find(const Key& key) const {
    return const_iterator(tree_.find(key));
  }

  ft::pair<iterator, iterator> equal_range(const Key& key) {
    return ft::make_pair(lower_bound(key), upper_bound(key));
  }
  ft::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
    return ft::make_pair(lower_bound(key), upper_bound(key));
  }
  iterator lower_bound(const Key& key) {
    return iterator(tree_.lower_bound(key));
  }
  const_iterator lower_bound(const Key& key) const {
    return const_iterator(tree_.lower_bound(key));
  }
  iterator upper_bound(const Key& key) {
    return iterator(tree_.upper_bound(key));
  }
  const_iterator upper_bound(const Key& key) const {
    return const_iterator(tree_.upper_bound(key));
  }

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

template <class Key, class Compare, class Alloc>
bool operator==(const ft::set<Key, Compare, Alloc>& lhs,
                const ft::set<Key, Compare, Alloc>& rhs) {
  if (lhs.size() != rhs.size()) return false;
  return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class Key, class Compare, class Alloc>
bool operator!=(const ft::set<Key, Compare, Alloc>& lhs,
                const ft::set<Key, Compare, Alloc>& rhs) {
  return !(lhs == rhs);
}

template <class Key, class Compare, class Alloc>
bool operator<(const ft::set<Key, Compare, Alloc>& lhs,
               const ft::set<Key, Compare, Alloc>& rhs) {
  return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                     rhs.end());
}

template <class Key, class Compare, class Alloc>
bool operator>(const ft::set<Key, Compare, Alloc>& lhs,
               const ft::set<Key, Compare, Alloc>& rhs) {
  return !(lhs < rhs || lhs == rhs);
}

template <class Key, class Compare, class Alloc>
bool operator<=(const ft::set<Key, Compare, Alloc>& lhs,
                const ft::set<Key, Compare, Alloc>& rhs) {
  return !(lhs > rhs);
}

template <class Key, class Compare, class Alloc>
bool operator>=(const ft::set<Key, Compare, Alloc>& lhs,
                const ft::set<Key, Compare, Alloc>& rhs) {
  return !(lhs < rhs);
}

// Uncomment this to pass the ft-containers-terminator tester even though we are
// not supposed to implement an ft:swap function
/* template <class Key, class Compare, class Alloc>
void swap(ft::set<Key, Compare, Alloc>& lhs,
          ft::set<Key, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
} */

}  // namespace ft

// Specialization of the std::swap function
template <class Key, class Compare, class Alloc>
void swap(ft::set<Key, Compare, Alloc>& lhs,
          ft::set<Key, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}

#endif  // SET_H