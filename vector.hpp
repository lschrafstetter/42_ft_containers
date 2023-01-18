#ifndef VECTOR_H
#define VECTOR_H
#include <limits>
#include <memory>
#include <stdexcept>
#include "iterator_vector.hpp"
#include "utilities.hpp"

namespace ft {

template <typename T, typename Allocator = std::allocator<T> >
class vector {
 public:
  template <typename datatype>
  class Iterator;
  typedef T value_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef Allocator allocator_type;

  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef Iterator_vector<value_type> iterator;
  typedef Iterator_vector<const value_type> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

  //**************************************************
  // Constructors
  //**************************************************

  // Basic
  vector()
      : allocator_(allocator_type()),
        start_(NULL),
        finish_(NULL),
        end_of_storage_(NULL) {}

  // Custom allocator
  explicit vector(const allocator_type& alloc)
      : allocator_(alloc), start_(NULL), finish_(NULL), end_of_storage_(NULL) {}

  // Constructor sets initial size, all with one value
  explicit vector(size_type count, const value_type& value = value_type(),
                  const allocator_type& alloc = allocator_type())
      : allocator_(alloc) {
    initialize_vector_with_value(count, value);
  }

  // Range initialization with Iterators
  template <class InputIt>
  explicit vector(
      typename ft::enable_if<!std::numeric_limits<InputIt>::is_integer,
                             InputIt>::type first,
      InputIt last, const allocator_type& alloc = allocator_type())
      : allocator_(alloc) {
    initialize_vector_with_range(first, last);
  }

  // Copy constructor
  vector(const vector& other) : allocator_(allocator_type()) {
    initialize_vector_with_range(other.begin(), other.end());
  }

  // Destructor
  ~vector() {
    clear();
    deallocate_all();
  }

  //**************************************************
  // Operator overloads
  //**************************************************

  // Copy assignment operator
  // 3 Cases:
  // 1) current capacity is too low
  // 2) capacity is enough, but other.size() > this.size() => just copy other
  // 3) capacity is enough and other.size() <= this.size() => copy, then destroy
  // "leftovers"
  vector& operator=(const vector& other) {
    if (*this != other) {
      size_type size_other = other.size();
      if (this->capacity() < size_other) {
        this->~vector();
        initialize_vector_with_range(other.begin(), other.end());
      } else {
        std::copy(other.begin(), other.end(), this->begin());
        if (other.size() < this->size()) {
          // if you have "left over objects" of *this, destroy them
          destroy(start_ + this->size(), end_of_storage_);
        }
        end_of_storage_ = start_ + size_other;
      }
    }
    return *this;
  }

  //**************************************************
  // Member functions
  //**************************************************

  // General functions

  // See copy assignment
  void assign(size_type count, const T& value) {
    if (this->capacity() < count) {
      this->~vector();
      initialize_vector_with_value(count, value);
    } else {
      for (pointer p1 = start_, p2 = p1 + count; p1 != p2; p1++)
        construct(p1, value);
      if (count < this->size()) destroy(start_ + count, end_of_storage_);
      end_of_storage_ = start_ + count;
    }
  }

  template <class InputIt>
  void assign(typename ft::enable_if<!std::numeric_limits<InputIt>::is_integer,
                                     InputIt>::type first,
              InputIt last) {
    size_type count = last - first;
    if (this->capacity() < count) {
      this->~vector();
      initialize_vector_with_range(first, last);
    } else {
      std::copy(first, last, this->begin());
      if (count < this->size()) destroy(start_ + count, end_of_storage_);
      end_of_storage_ = start_ + count;
    }
  }

  allocator_type get_allocator() const { return allocator_; }

  // Element access
  reference at(size_type pos) {
    if (pos >= this->size())
      throw std::out_of_range("pos out of range in vector::at(size_type pos)");
    return start_[pos];
  }
  const_reference at(size_type pos) const {
    if (pos >= this->size())
      throw std::out_of_range("pos out of range in vector::at(size_type pos)");
    return start_[pos];
  }
  reference operator[](size_type pos) { return *(start_ + pos); }
  const_reference operator[](size_type pos) const { return *(start_ + pos); }
  reference front() { return *start_; }
  const_reference front() const { return *start_; }
  reference back() { return *(end_of_storage_ - 1); }
  const_reference back() const { return *(end_of_storage_ - 1); }
  pointer data() { return start_; }
  const_pointer data() const { return start_; }

  // Iterators
  iterator begin() { return iterator(start_); }
  const_iterator begin() const { return const_iterator(start_); }
  iterator end() { return iterator(end_of_storage_); }
  const_iterator end() const { return const_iterator(end_of_storage_); }

  /* rbegin, rend*/
  reverse_iterator rbegin() { return reverse_iterator(end_of_storage_); }
  const_reverse_iterator rbegin() const {
    return reverse_iterator(end_of_storage_);
  }
  reverse_iterator rend() { return reverse_iterator(start_); }
  const_reverse_iterator rend() const { return reverse_iterator(start_); }

  // Capacity functions

  bool empty() const { return start_ == end_of_storage_; }

  size_type size() const { return end_of_storage_ - start_; }

  size_type max_size() const { return allocator_.max_size(); }

  void reserve(size_type new_cap) {
    if (new_cap > this->max_size())
      throw std::length_error("new_cap exceeded size in vector::reserve()");
    size_type size = this->size();
    if (new_cap > size) {
      pointer tmp = allocate(new_cap);
      std::copy(this->begin(), this->end(), iterator(tmp));
      this->~vector();
      start_ = tmp;
      finish_ = start_ + new_cap;
      end_of_storage_ = start_ + size;
    }
  }

  size_type capacity() const { return finish_ - start_; }

  // Modifiers
  void clear() {
    destroy(start_, end_of_storage_);
    end_of_storage_ = start_;
  }

  iterator insert(const_iterator pos, const value_type& value) {
    size_type insert_pos = pos - begin();

    if (this->size() == this->capacity())
      return insert_single_realloc(insert_pos, value);
    return insert_single_without_realloc(insert_pos, value);
  }

  // iterator insert( const_iterator pos, size_type count, const T& value ) {}
  /* template< class InputIt >
  iterator insert( const_iterator pos, InputIt first, InputIt last ) {} */
  // iterator erase( iterator pos ) {}
  // iterator erase( iterator first, iterator last ) {}

  void push_back(const value_type& value) {
    size_type size = this->size();
    size_type capacity = this->capacity();
    if (!start_) {
      initialize_vector_with_value(1, value);
      return;
    }
    if (size == capacity) reserve(capacity * 2);
    construct(start_ + size, value);
    end_of_storage_++;
  }

  void pop_back() {
    if (this->size()) {
      destroy(start_ + this->size(), end_of_storage_);
      end_of_storage_--;
    }
  }

  // Three cases:
  // 1) size < count and capacity < count: reallocate memory
  // 2) size < count and capacity >= count: just fill with value
  // 3) size > count: erase all "leftover" objects
  void resize(size_type count, value_type value = value_type()) {
    size_type size = this->size();
    if (size < count) {
      if (this->capacity() < count) {
        pointer tmp = allocate(count);
        std::copy(this->begin(), this->end(), iterator(tmp));
        for (unsigned int i = size; i < count; i++) tmp[i] = value;
        this->~vector();
        start_ = tmp;
        end_of_storage_ = start_ + count;
        finish_ = end_of_storage_;
      } else {
        for (unsigned int i = size; i < count; i++) (*this)[i] = value;
        end_of_storage_ = start_ + count;
      }
    } else if (size > count) {
      destroy(start_ + count, end_of_storage_);
      end_of_storage_ = start_ + count;
    }
  }

  void swap(vector& other) {
    std::swap(this->allocator_, other.allocator_);
    std::swap(this->start_, other.start_);
    std::swap(this->finish_, other.finish_);
    std::swap(this->end_of_storage_, other.end_of_storage_);
  }

 private:
  //**************************************************
  // Private member objects
  //**************************************************

  allocator_type allocator_;
  pointer start_;
  pointer finish_;
  pointer end_of_storage_;

  //**************************************************
  // General helper functions
  //**************************************************

  // Helper for insert functions with single position insert
  iterator insert_single_realloc(size_type insert_pos,
                                 const value_type& value) {
    size_type old_size = size();
    size_type new_capacity = capacity() == 0 ? 1 : capacity() * 2;
    pointer tmp = allocate(new_capacity);

    // Copy values before new value into tmp
    for (unsigned int i = 0; i < insert_pos; i++) tmp[i] = start_[i];

    // Construct new value
    construct(tmp + insert_pos, value);

    // Copy values after new value into tmp
    for (unsigned int i = insert_pos; i < old_size; i++) {
      tmp[i + 1] = start_[i];
    }

    // Reassign attributes
    deallocate_all();
    start_ = tmp;
    end_of_storage_ = start_ + old_size + 1;
    finish_ = start_ + new_capacity;

    return begin() + insert_pos;
  }

  // Helper for insert function with single position insert
  // There is enough capacity
  iterator insert_single_without_realloc(size_type insert_pos,
                                         const value_type& value) {
    // Copying from the back
    // Copy values from end_of_storage until insert_pos
    for (unsigned int i = size(); i > insert_pos; i--) start_[i + 1] = start_[i];

    // Construct new value
    construct(start_ + insert_pos, value);

    // Copy values from insert_pos to start_
    for (int i = insert_pos - 1; i >= 0; i--) start_[i + 1] = start_[i];

    end_of_storage_++;
    return begin() + insert_pos;
  }

  // Initializes an uninitialize vector with count elements of a value
  void initialize_vector_with_value(size_type count, const value_type& value) {
    start_ = allocate(count);
    finish_ = start_ + count;
    std::uninitialized_fill_n(start_, count, value);
    end_of_storage_ = finish_;
  }

  // Initializes an uninitialize vector with a range of elements
  template <class InputIt>
  void initialize_vector_with_range(const InputIt& it1, const InputIt& it2) {
    size_type distance = it2 - it1;
    start_ = allocate(distance);
    uninitialized_copy_n(it1, distance, start_);
    end_of_storage_ = start_ + distance;
    finish_ = end_of_storage_;
  }

  // Constructs a range from first until first + n into the uninitialized
  // pointer range start until start + n
  template <class InputIt>
  void uninitialized_copy_n(InputIt first, size_type n, pointer start) {
    for (unsigned int i = 0; i < n; i++) {
      construct(start, *first);
      ++start;
      ++first;
    }
  }

  //**************************************************
  // Allocation helpers
  //**************************************************

  pointer allocate(size_t n) {
    return static_cast<pointer>(allocator_.allocate(n * sizeof(value_type)));
  }

  void deallocate_all() {
    if (start_) {
      allocator_.deallocate(start_, finish_ - start_);
    }
  }

  void construct(pointer position, const value_type& value) {
    allocator_.construct(position, value);
  }

  void destroy(pointer first, pointer last) {
    while (first != last) {
      allocator_.destroy(first++);
    }
  }
};

//**************************************************
// Non-member overloads of vector
//**************************************************

template <class T, class Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  if (lhs.size() != rhs.size()) return false;
  return equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T, class Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(lhs == rhs);
}

template <class T, class Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                     rhs.end());
}

template <class T, class Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(lhs < rhs || lhs == rhs);
}

template <class T, class Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(lhs > rhs);
}

template <class T, class Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(lhs < rhs);
}

}  // namespace ft

#endif  // VECTOR_H