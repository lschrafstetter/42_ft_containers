#ifndef iterator_rbt_HPP
#define iterator_rbt_HPP

#include <limits>
#include <memory>
#include <stdexcept>
#include "utilities.hpp"

namespace ft {

//**************************************************
// This is a bidirectional iterator
//**************************************************

template <class datatype, class node_type>
class iterator_rbt {
 public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef datatype value_type;
  typedef datatype *pointer;
  typedef datatype &reference;
  typedef std::ptrdiff_t difference_type;

  //**************************************************
  // Constructors
  //**************************************************

  iterator_rbt() : node_(NULL) {}
  iterator_rbt(node_type *p) : node_(p) {}
  iterator_rbt(const iterator_rbt &other) : node_(other.node_) {}
  ~iterator_rbt() {}

  //**************************************************
  // Operator overloads
  //**************************************************

  iterator_rbt &operator=(const iterator_rbt &other) {
    this->node_ = other->node_;
    return *this;
  }

  reference operator*() const { return node_->data; }

  iterator_rbt &operator++() {
    inorder_successor_();
    return *this;
  }
  iterator_rbt operator++(int) {
    iterator_rbt tmp(*this);
    inorder_successor_();
    return tmp;
  }
  iterator_rbt &operator--() {
    inorder_predecessor_();
    return *this;
  }
  iterator_rbt operator--(int) {
    iterator_rbt tmp(*this);
    inorder_predecessor_();
    return tmp;
  }

  pointer operator->() const { return &(node_->data); }

  bool operator==(const iterator_rbt &other) const {
    return this->node_ == other.node_;
  }

  bool operator!=(const iterator_rbt &other) const {
    return this->node_ != other.node_;
  }

  //**************************************************
  // Functions
  //**************************************************

  pointer base() const { return &(node_->data); }

  //**************************************************
  // Conversion overloads
  //**************************************************

  // Implicit conversion to const_iterator (e.g. when making const iterator of
  // non-const vector)
  operator iterator_rbt<const value_type, node_type>() const {
    return iterator_rbt<const value_type, node_type>(this->node_);
  }

 protected:
  node_type *node_;

 private:
  void inorder_successor_() {
    if (!node_->right_child->is_null_node)
      node_ = min_value_(node_->right_child);
    else {
      node_type *parent = node_->parent;
      while (!parent->is_null_node && node_ == parent->right_child) {
        node_ = parent;
        parent = node_->parent;
      }
      node_ = parent;
    }
  }

  void inorder_predecessor_() {
    // The only null node is the off_the_end_ node which parent is last_
    if (node_->is_null_node) {
      node_ = node_->parent;
      return;
    }
    if (!node_->left_child->is_null_node)
      node_ = max_value_(node_->left_child);
    else {
      node_type *parent = node_->parent;
      while (!parent->is_null_node && node_ == parent->left_child) {
        node_ = parent;
        parent = node_->parent;
      }
      if (parent->is_null_node) {
        node_ = NULL;
        return;
      }
      node_ = parent;
    }
  }

  /**
   * @brief gets the smallest value node of a binary search tree
   *
   * @param node the root of the tree. Must not point to NULL.
   * @return node_type* the smallest value node.
   */
  node_type *min_value_(node_type *node) const {
    node_type *current = node;
    while (!current->left_child->is_null_node) current = current->left_child;
    return current;
  }

  /**
   * @brief gets the greatest value node of a binary search tree
   *
   * @param node the root of the tree. Must not point to NULL.
   * @return node_type* the greatest value node.
   */
  node_type *max_value_(node_type *node) const {
    node_type *current = node;
    while (!current->right_child->is_null_node) current = current->right_child;
    return current;
  }
};

//**************************************************
// Non-member operator overloads
//**************************************************

template <class value_type, class node_type>
bool operator==(iterator_rbt<value_type, node_type> lhs,
                iterator_rbt<const value_type, node_type> rhs) {
  return lhs.base() == rhs.base();
}

template <class value_type, class node_type>
bool operator!=(iterator_rbt<value_type, node_type> lhs,
                iterator_rbt<const value_type, node_type> rhs) {
  return lhs.base() != rhs.base();
}

}  // namespace ft
#endif