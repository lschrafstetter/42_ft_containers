#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "utilities.hpp"

namespace ft {

template <class T, class Compare = std::less<T>,
          class Allocator = std::allocator<T> >
class redblacktree {
 public:
  //**************************************************
  // Typedefs
  //**************************************************

  enum node_color { RED, BLACK };
  template <class U>
  class rb_node;
  typedef Compare key_compare;

  typedef T value_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  typedef rb_node<value_type> node_type;
  typedef typename Allocator::template rebind<node_type>::other allocator_type;

  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;

  //**************************************************
  // Constructors
  //**************************************************

  redblacktree(key_compare comparator)
      : allocator_(allocator_type()), cmp_(comparator), size_(0) {
    off_the_end_ = new_node_();
    root_ = off_the_end_;
    first_ = off_the_end_;
    last_ = off_the_end_;
    off_the_end_->parent = last_;
  }

  redblacktree(const redblacktree &other)
      : allocator_(allocator_type()), cmp_(other.cmp_), size_(other.size_) {
    off_the_end_ = new_node_();
    root_ = off_the_end_;
    first_ = off_the_end_;
    last_ = off_the_end_;

    if (other.root_->is_null_node) return;

    root_ = new_node_(other.root_->data);
    root_->color = BLACK;
    root_->left_child = copy_subtree_(other.root_->left_child, root_);
    root_->right_child = copy_subtree_(other.root_->right_child, root_);

    node_type *tmp = root_;
    while (!tmp->is_null_node) tmp = tmp->left_child;
    first_ = tmp;

    tmp = root_;
    while (!tmp->is_null_node) tmp = tmp->right_child;
    set_last_(tmp);
  }

  ~redblacktree() {
    clear();
    destroy_node_(off_the_end_);
  }

  //**************************************************
  // Operator overloads
  //**************************************************

  redblacktree &operator=(redblacktree other) {
    swap(*this, other);
    return *this;
  }

  //**************************************************
  // Member functions
  //**************************************************

  ft::pair<node_type *, bool> insert(const value_type &value) {
    if (root_->is_null_node) {
      root_ = new_node_(value, off_the_end_);
      first_ = root_;
      set_last_(root_);
      size_ = 1;
      return ft::pair<node_type *, bool>(root_, true);
    }

    node_type *tmp = root_;
    node_type *parent = off_the_end_;
    while (!tmp->is_null_node) {
      parent = tmp;
      if (key_is_less_(value, tmp->data)) {
        tmp = tmp->left_child;
      } else if (key_is_greater_(value, tmp->data)) {
        tmp = tmp->right_child;
      } else
        return ft::pair<node_type *, bool>(tmp, false);
    }

    tmp = new_node_(value, parent);
    tmp->color = RED;
    ++size_;

    if (key_is_less_(value, parent->data))
      parent->left_child = tmp;
    else
      parent->right_child = tmp;

    if (key_is_less_(tmp->data, first_->data))
      first_ = tmp;
    else if (key_is_greater_(tmp->data, last_->data))
      set_last_(tmp);

    // rebalance_insert()
    return ft::pair<node_type *, bool>(tmp, true);
  }

  void erase(node_type *pos) {}

  // size_type erase(const Key &key) {}

  size_type size() const { return size_; }

  void clear() {
    destroy_subtree_(root_);
    size_ = 0;
  }

  void swap(redblacktree &other) {
    std::swap(this->root_, other.root_);
    std::swap(this->first_, other.first_);
    std::swap(this->last, other.last);
    std::swap(this->off_the_end_, other.off_the_end_);
    std::swap(this->allocator_, other.allocator_);
    std::swap(this->cmp_, other.cmp_);
    std::swap(this->size_, other.size_);
  }

  node_type *find(const value_type &value) const {
    node_type *current = root_;
    while (!current->is_null_node) {
      if (key_is_equal_(value, current->data))
        return current;
      else if (key_is_less_(value, current->data))
        current = current->left_child;
      else
        current = current->right_child;
    }
    return current;
  }

  node_type *get_first() const { return first_; }

  node_type *get_end() const { return off_the_end_; }

  //**************************************************
  // Member classes
  //**************************************************

  template <class U>
  class rb_node {
   public:
    rb_node()
        : data(U()),
          parent(NULL),
          left_child(NULL),
          right_child(NULL),
          color(BLACK),
          is_null_node(true) {}

    rb_node(const U &value)
        : data(U(value)),
          parent(NULL),
          left_child(NULL),
          right_child(NULL),
          color(BLACK),
          is_null_node(false) {}

    U data;
    rb_node *parent;
    rb_node *left_child;
    rb_node *right_child;
    node_color color;
    bool is_null_node;
  };

  //**************************************************
  // Private member objects
  //**************************************************

 private:
  node_type *root_;
  node_type *first_;
  node_type *last_;
  node_type *off_the_end_;
  allocator_type allocator_;
  key_compare cmp_;
  size_type size_;

  //**************************************************
  // General helper functions
  //**************************************************

  void set_last_(node_type *node) {
    last_ = node;
    off_the_end_->parent = last_;
  }

  node_type *copy_subtree_(node_type *node, node_type *parent) {
    if (node->is_null_node) return off_the_end_;

    node_type *tmp = new_node(node->data);
    tmp->color = BLACK;
    tmp->parent = parent;
    tmp->left_child = copy_subtree_(node->left_child);
    tmp->right_child = copy_subtree_(node->right_child);

    return tmp;
  }

  void destroy_subtree_(node_type *node) {
    if (node->is_null_node) return;
    destroy_subtree_(node->left_child);
    destroy_subtree_(node->right_child);
    destroy_node_(node);
  }

  bool key_is_less_(const value_type &pair1, const value_type &pair2) const {
    return cmp_(pair1, pair2);
  }

  bool key_is_greater_(const value_type &pair1, const value_type &pair2) const {
    return cmp_(pair2, pair1);
  }

  bool key_is_equal_(const value_type &pair1, const value_type &pair2) const {
    return !cmp_(pair1, pair2) && !cmp_(pair2, pair1);
  }

  node_type *new_node_() {
    node_type *tmp = allocator_.allocate(1);
    allocator_.construct(tmp, node_type());
    tmp->is_null_node = true;
    tmp->left_child = off_the_end_;
    tmp->right_child = off_the_end_;
    tmp->parent = off_the_end_;
    return tmp;
  }

  node_type *new_node_(const value_type &value, node_type *parent) {
    node_type *tmp = allocator_.allocate(1);
    allocator_.construct(tmp, node_type(value));
    tmp->is_null_node = false;
    tmp->left_child = off_the_end_;
    tmp->right_child = off_the_end_;
    tmp->parent = parent;
    return tmp;
  }

  void destroy_node_(node_type *node) {
    allocator_.destroy(node);
    allocator_.deallocate(node, sizeof(node_type));
  }
};

}  // namespace ft

#endif  // REDBLACKTREE_H
