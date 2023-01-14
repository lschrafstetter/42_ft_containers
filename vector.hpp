#ifndef VECTOR_H
#define VECTOR_H
#include <limits>
#include <memory>
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
  typedef Iterator<value_type> iterator;
  typedef Iterator<const value_type> const_iterator;

  //**************************************************
  // Constructors
  //**************************************************

  // Basic
  vector()
      : start_(NULL),
        finish_(NULL),
        end_of_storage_(NULL),
        allocator_(allocator_type()) {}

  // Custom allocator
  explicit vector(const allocator_type& alloc)
      : start_(NULL), finish_(NULL), end_of_storage_(NULL), allocator_(alloc) {}

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
    if (start_) {
      clear();
      deallocate_all();
    }
  }

  //**************************************************
  // Operator overloads
  //**************************************************

  // Copy assignment operator
  vector& operator=(const vector& other) {
    if (*this != other) {
      size_type size_other = other.size();
      destroy(start_, finish_);
      if (capacity() < size_other) {
        deallocate_all();
        initialize_vector_with_range(other.begin(), other.end());
      } else {
        std::copy(other.begin(), other.end(), this.begin());
        end_of_storage_ = start_ + size_other;
      }
    }
    return *this;
  }

  //**************************************************
  // Member functions
  //**************************************************

  // General functions
  void assign(size_type count, const T& value) {
    clear();
    deallocate_all();
    initialize_vector_with_value(count, value);
  }

  /* template< class InputIt >
  void assign(InputIt first, InputIt last) {} */

  allocator_type get_allocator() const { return allocator_; }

  // Element access
  reference operator[](size_type pos) { return *(start_ + pos); }
  const_reference operator[](size_type pos) const { return *(start_ + pos); }

  // Iterators
  iterator begin() { return iterator(start_); }
  const_iterator begin() const { return const_iterator(start_); }
  iterator end() { return iterator(end_of_storage_); }
  const_iterator end() const { return const_iterator(end_of_storage_); }

  // Capacity functions
  bool empty() const { return start_ == end_of_storage_; }
  size_type size() const { return end_of_storage_ - start_; }
  size_type max_size() const {
    return std::numeric_limits<difference_type>::max();
  }
  // void reserve(size_type new_cap);
  size_type capacity() const { return finish_ - start_; }

  // Modifiers
  void clear() {
    destroy(start_, end_of_storage_);
    end_of_storage_ = start_;
  }
  // iterator insert( const_iterator pos, const T& value ) {}
  // iterator insert( const_iterator pos, size_type count, const T& value ) {}
  /* template< class InputIt >
  iterator insert( const_iterator pos, InputIt first, InputIt last ) {} */
  // iterator erase( iterator pos ) {}
  // iterator erase( iterator first, iterator last ) {}
  // void push_back( const T& value ) {}
  // void pop_back() {}
  // void resize( size_type count ) {}
  // void swap( vector& other ) {}

  //**************************************************
  // Iterators
  //**************************************************

  template <typename datatype>
  class Iterator {
   public:
    typedef std::random_access_iterator_tag iterator_category;
    typedef datatype value_type;
    typedef datatype* pointer;
    typedef datatype& reference;
    typedef std::ptrdiff_t difference_type;

    // Constructors
    Iterator(pointer p) : data_(p) {}
    Iterator(const Iterator& other) : data_(other.data_) {}

    // Operator overloads
    reference operator*() const { return *data_; }
    Iterator& operator++() {
      ++data_;
      return *this;
    }
    Iterator operator++(int) {
      Iterator tmp(*this);
      ++data_;
      return tmp;
    }
    Iterator& operator--() {
      --data_;
      return *this;
    }
    Iterator operator--(int) {
      Iterator tmp(*this);
      --data_;
      return tmp;
    }
    Iterator& operator+=(difference_type n) {
      data_ += n;
      return *this;
    }
    Iterator operator+(difference_type n) const { return Iterator(data_ + n); }
    Iterator& operator-=(difference_type n) {
      data_ -= n;
      return *this;
    }
    Iterator operator-(difference_type n) const { return Iterator(data_ - n); }
    difference_type operator-(const Iterator& other) const {
      return data_ - other.data_;
    }
    value_type& operator[](difference_type n) const { return data_[n]; }
    bool operator<(const Iterator& other) const { return data_ < other.data_; }
    bool operator>(const Iterator& other) const { return data_ > other.data_; }
    bool operator<=(const Iterator& other) const {
      return data_ <= other.data_;
    }
    bool operator>=(const Iterator& other) const {
      return data_ >= other.data_;
    }
    bool operator==(const Iterator& other) const {
      return data_ == other.data_;
    }
    bool operator!=(const Iterator& other) const {
      return data_ != other.data_;
    }

    // Implicit conversion to const_iterator (e.g. when making const iterator of
    // non-const vector)
    operator Iterator<const value_type>() const {
      return Iterator<const value_type>(this->data_);
    }

   protected:
    pointer data_;
  };

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

  void initialize_vector_with_value(size_type count, const value_type& value) {
    start_ = allocate(count);
    finish_ = start_ + count;
    std::uninitialized_fill_n(start_, count, value);
    end_of_storage_ = finish_;
  }

  template <class InputIt>
  void initialize_vector_with_range(const InputIt& it1, const InputIt& it2) {
    size_type size = it2 - it1;
    start_ = allocate(size);
    uninitialized_copy_n(it1, size, start_);
    end_of_storage_ = start_ + size;
    finish_ = end_of_storage_;
  }

  template <class InputIt>
  void uninitialized_copy_n(InputIt first, size_type n, pointer start) {
    for (unsigned int i = 0; i < n; i++) {
      construct(start, *first);
      //*start = *first;
      start++;
      first++;
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
// Non-member overloads
//**************************************************

template <class T, class Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(equal(lhs.begin(), lhs.end(), rhs.begin()));
}

}  // namespace ft

#endif  // VECTOR_H