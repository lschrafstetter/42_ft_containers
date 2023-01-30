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
    off_the_end_->color = BLACK;
    root_ = off_the_end_;
    first_ = off_the_end_;
    last_ = off_the_end_;
    off_the_end_->parent = last_;
  }

  redblacktree(const redblacktree &other)
      : allocator_(allocator_type()), cmp_(other.cmp_), size_(other.size_) {
    off_the_end_ = new_node_();
    off_the_end_->color = BLACK;
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

  /**
   * @brief Inserts a node into the tree
   *
   * @param value
   * @return ft::pair<node_type *, bool> if there wasn't a node with the value
   * already, returns a pointer to the node and true, otherwise returns a
   * pointer to the already existing duplicate and false
   */
  ft::pair<node_type *, bool> insert(const value_type &value) {
    // Case 1: first insertion. Make a new root.
    if (root_->is_null_node) {
      root_ = new_node_(value, off_the_end_);
      root_->color = BLACK;
      first_ = root_;
      set_last_(root_);
      size_ = 1;
      return ft::pair<node_type *, bool>(root_, true);
    }

    // Case 2: insertion into existing tree
    // Step 1: find spot to insert into
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

    // Step 2: make new node at spot
    tmp = new_node_(value, parent);
    ++size_;
    if (key_is_less_(value, parent->data))
      parent->left_child = tmp;
    else
      parent->right_child = tmp;

    // Step 3:If the key is the smallest and/or greatest, update member
    // variables
    if (key_is_less_(tmp->data, first_->data))
      first_ = tmp;
    else if (key_is_greater_(tmp->data, last_->data))
      set_last_(tmp);

    // Step 4: Rebalance tree
    rebalance_insert_(tmp);

    return ft::pair<node_type *, bool>(tmp, true);
  }

  /**
   * @brief deletes a node
   *
   * @param pos pointer to the node(use the find member function to )
   */
  void erase(node_type *pos) { delete_(pos); }

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

  /**
   * @brief tries to find a node with the given value. Returns a pointer to the
   * off_the_end node if nothing was found
   *
   * @param value
   * @return node_type* either pointer to the found node or the off_the_end node
   */
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

  /**
   * @brief Returns a pointer to the node with the lowest value or a pointer to
   * the off_the_end node if the tree is empty
   *
   * @return node_type* node with the lowest value or a pointer to the
   * off_the_end node
   */
  node_type *get_first() const { return first_; }

  /**
   * @brief Returns a pointer to the node with the greatest value or a pointer
   * to the off_the_end node if the tree is empty
   *
   * @return node_type* node with the greatest value or a pointer to the
   * off_the_end node
   */
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
          color(RED),
          is_null_node(true) {}

    rb_node(const U &value)
        : data(U(value)),
          parent(NULL),
          left_child(NULL),
          right_child(NULL),
          color(RED),
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

  /**
   * @brief recursively rebalances the tree after the insertion. If the new node
   * is the root node, colors it black. Otherwise, it checks for a double-red
   * property violation and rebalances the tree
   *
   * @param node the newly inserted node
   */
  void rebalance_insert_(node_type *node) {
    if (node == root_) {
      node->color = BLACK;
      return;
    }
    node_type *parent = node->parent;
    if (parent->color == RED) {
      node_type *grandparent = parent->parent;

      if (insert_is_zick_zack_(node, parent))
        std::swap(node->data, parent->data);

      node_type *uncle = get_sibling_(parent);
      if (uncle->color == BLACK) {
        node->color = RED;
        parent->color = BLACK;
        grandparent->color = RED;
        restructure_(node, parent, grandparent);
      } else {
        parent->color = BLACK;
        grandparent->color = RED;
        uncle->color = BLACK;
        rebalance_insert_(grandparent);
      }
    }
  }

  /**
   * @brief returns a pointer to the sibling of a node. Using this with root is
   * undefined behaviour
   *
   * @param node
   * @return node_type* pointer to the sibling of node
   */
  node_type *get_sibling_(node_type *node) {
    node_type *parent = node->parent;
    if (node == parent->left_child) return parent->right_child;
    return parent->left_child;
  }

  /**
   * @brief restructure function for the double red, case when inserting node
   * when uncle node (sibling of parent) is black: a and c become b's children.
   * b takes the place of the grandparent. A's sibling becomes c's left child
   *
   * @param a node
   * @param b parent of node
   * @param c grandparent of node
   */
  void restructure_(node_type *a, node_type *b, node_type *c) {
    // relink the sibling of node to uncle
    if (is_left_child_(b))
      c->left_child = get_sibling_(a);
    else
      c->right_child = get_sibling_(a);

    // make b the child of grandgrandparent
    if (c == root_) {
      root_ = b;
      b->parent = off_the_end_;
    } else if (c == c->parent->left_child) {
      c->parent->left_child = b;
      b->parent = c->parent;
    } else {
      c->parent->right_child = b;
      b->parent = c->parent;
    }
    // make a and c the children of b
    make_children_(b, a, c);
  }

  /**
   * @brief makes a node the parent of two nodes. Changes the child pointer of
   * the parent and the parent pointers of the children.
   *
   * @param parent
   * @param child1
   * @param child2
   */
  void make_children_(node_type *parent, node_type *child1, node_type *child2) {
    if (key_is_less_(child1->data, parent->data)) {
      parent->left_child = child1;
      parent->right_child = child2;
    } else {
      parent->left_child = child2;
      parent->right_child = child1;
    }
    child1->parent = parent;
    child2->parent = parent;
  }

  /**
   * @brief
   *
   * @param node
   * @return true returns true if the node is the left child of its parent
   * @return false returns false if the node is the right child of its parent
   */
  bool is_left_child_(node_type *node) {
    if (node->parent->is_null_node) return false;
    return node == node->parent->left_child;
  }

  /**
   * @brief used to determine if a node is the same-side-child as its parent of
   * its grandparent (if not, we have a zick-zack in the family tree)
   *
   * @param node
   * @param parent
   * @return true returns true if node is a left-child and parent is a
   * right-child or vice versa
   * @return false returns false if node and parent are both left-childs or
   * right-childs
   */
  bool insert_is_zick_zack_(node_type *node, node_type *parent) {
    return (is_left_child_(node) != is_left_child_(parent));
  }

  /**
   * @brief Set the last object. Side effect: sets the last node as the parent
   * of the off_the_end node
   *
   * @param node
   */
  void set_last_(node_type *node) {
    last_ = node;
    off_the_end_->parent = last_;
  }

  /**
   * @brief recursively copies a tree
   *
   * @param node the root of the tree to copy
   * @param parent pointer to a parent node to be set for the root node
   * @return node_type* pointer to the root of the new subtree
   */
  node_type *copy_subtree_(node_type *node, node_type *parent) {
    if (node->is_null_node) return off_the_end_;

    node_type *tmp = new_node(node->data);
    tmp->color = node->color;
    tmp->parent = parent;
    tmp->left_child = copy_subtree_(node->left_child);
    tmp->right_child = copy_subtree_(node->right_child);

    return tmp;
  }

  /**
   * @brief recursively destroys a tree
   *
   * @param node
   */
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

  /**
   * @brief makes an empty node with an empty data member and pointers to the
   * off_the_end node
   *
   * @return node_type* pointer to the new node
   */
  node_type *new_node_() {
    node_type *tmp = allocator_.allocate(1);
    allocator_.construct(tmp, node_type());
    tmp->is_null_node = true;
    tmp->left_child = off_the_end_;
    tmp->right_child = off_the_end_;
    tmp->parent = off_the_end_;
    return tmp;
  }

  /**
   * @brief makes a node with a value as data and a parent
   *
   * @param value
   * @param parent
   * @return node_type* pointer to the new node
   */
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
