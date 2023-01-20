#ifndef VECTOR_H
#define VECTOR_H
#include <algorithm>
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
  vector(const vector& other)
      : allocator_(allocator_type()),
        start_(NULL),
        finish_(NULL),
        end_of_storage_(NULL) {
    if (other.size()) initialize_vector_with_range(other.begin(), other.end());
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
      for (pointer p1 = start_, p2 = p1 + count; p1 != p2; ++p1)
        construct(p1, value);
      if (count < this->size()) destroy(start_ + count, end_of_storage_);
      end_of_storage_ = start_ + count;
    }
  }

  template <class InputIt>
  void assign(typename ft::enable_if<!std::numeric_limits<InputIt>::is_integer,
                                     InputIt>::type first,
              InputIt last) {
    size_type count = get_distance(first, last);
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

  iterator insert(const const_iterator& pos, const value_type& value) {
    size_type insert_pos = pos - begin();

    if (this->size() == this->capacity())
      return insert_single_realloc(insert_pos, value);
    return insert_single_without_realloc(insert_pos, value);
  }

  iterator insert(const const_iterator& pos, size_type count,
                  const value_type& value) {
    size_type insert_pos = pos - begin();

    if (this->size() < capacity() + count)
      return insert_range_realloc(insert_pos, count, value);
    return insert_range_without_realloc(insert_pos, count, value);
  }

  template <class InputIt>
  iterator insert(
      const const_iterator& pos,
      typename ft::enable_if<!std::numeric_limits<InputIt>::is_integer,
                             InputIt>::type first,
      InputIt last) {
    size_type insert_pos = pos - begin();
    size_type distance = get_distance(first, last);

    if (this->size() < capacity() + distance)
      return insert_range_iterator_realloc(insert_pos, first, last);
    return insert_range_iterator_without_realloc(insert_pos, first, last);
  }

  iterator erase(const iterator& pos) {
    if (empty()) return end();
    size_type pos_to_remove = pos - begin();
    size_type size = this->size();

    if (!ft::is_integral<value_type>::value)
      allocator_.destroy(start_ + pos_to_remove);
    for (unsigned int i = pos_to_remove; i < size; ++i)
      start_[i] = start_[i + 1];

    --end_of_storage_;
    return begin() + pos_to_remove;
  }

  iterator erase(const iterator& first, const iterator& last) {
    size_type pos_to_remove = first - begin();
    size_type distance = last - first;
    size_type size = this->size();

    destroy(first.base(), last.base());
    for (unsigned int i = pos_to_remove; i < size; ++i)
      start_[i] = start_[i + distance];

    end_of_storage_ -= distance;
    return begin() + pos_to_remove;
  }

  void push_back(const value_type& value) {
    size_type size = this->size();
    size_type capacity = this->capacity();
    if (size == capacity) reserve(std::max((size_type)1, capacity * 2));
    construct(start_ + size, value);
    ++end_of_storage_;
  }

  void pop_back() {
    if (this->size()) {
      if (!ft::is_integral<value_type>::value)
        allocator_.destroy(end_of_storage_ - 1);
      --end_of_storage_;
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
        for (unsigned int i = size; i < count; ++i) tmp[i] = value;
        this->~vector();
        start_ = tmp;
        end_of_storage_ = start_ + count;
        finish_ = end_of_storage_;
      } else {
        for (unsigned int i = size; i < count; ++i) (*this)[i] = value;
        end_of_storage_ = start_ + count;
      }
    } else if (size > count) {
      destroy(start_ + count, end_of_storage_);
      end_of_storage_ = start_ + count;
    }
  }

  void swap(vector& other) {
    std::swap(this->allocator_, other.allocator_);
    /* pointer tmp = this->start_;
    this->start_ = other.start_;
    other.start_ = tmp; */
    std::swap(this->start_, other.start_);
    /* tmp = this->finish_;
    this->finish_ = other.finish_;
    other.finish_ = tmp; */
    std::swap(this->finish_, other.finish_);
    /* tmp = this->end_of_storage_;
    this->end_of_storage_ = other.end_of_storage_;
    other.end_of_storage_ = tmp; */
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

  // Helper for insert function with same-value-range insert
  template <class InputIt>
  iterator insert_range_iterator_realloc(
      size_type insert_pos,
      typename ft::enable_if<!std::numeric_limits<InputIt>::is_integer,
                             InputIt>::type& first,
      const InputIt& last) {
    size_type old_size = size();
    size_type distance = get_distance(first, last);
    size_type new_capacity = old_size + distance;
    pointer tmp = allocate(new_capacity);

    // Copy values before new value into tmp
    for (unsigned int i = 0; i < insert_pos; ++i) tmp[i] = start_[i];

    // Copy range
    for (unsigned int i = insert_pos; i < insert_pos + distance; ++i)
      tmp[i] = *(first++);

    // Copy remaining original array
    for (unsigned int i = insert_pos + distance; i < old_size + distance; ++i)
      tmp[i] = start_[i - distance];

    deallocate_all();
    start_ = tmp;
    end_of_storage_ = start_ + old_size + distance;
    finish_ = start_ + new_capacity;

    return begin() + insert_pos;
  }

  // Helper for insert function with same-value-range insert
  template <class InputIt>
  iterator insert_range_iterator_without_realloc(
      size_type insert_pos,
      typename ft::enable_if<!std::numeric_limits<InputIt>::is_integer,
                             InputIt>::type& first,
      const InputIt& last) {
    size_type distance = get_distance(first, last);

    // Case: inserting with iterator end()
    if (insert_pos == size()) {
      for (; first != last; ++first) push_back(*first);
      return end() - distance;
    }
    size_type new_size = size() + distance;

    // Copying from the back
    // Copy values from end_of_storage until end of insert range
    for (unsigned int i = new_size; i > insert_pos + distance; --i)
      start_[i] = start_[i - distance];

    // Construct range of new values
    for (unsigned int i = insert_pos; i < insert_pos + distance; ++i)
      start_[i] = *(first++);

    end_of_storage_ += distance;
    return begin() + insert_pos;
  }

  // Helper for insert function with same-value-range insert
  iterator insert_range_realloc(size_type insert_pos, size_type count,
                                const value_type& value) {
    size_type old_size = size();
    size_type new_capacity = old_size + count;
    pointer tmp = allocate(new_capacity);

    // Copy values before new value into tmp
    for (unsigned int i = 0; i < insert_pos; ++i) tmp[i] = start_[i];

    // Copy range
    for (unsigned int i = insert_pos; i < insert_pos + count; ++i)
      construct(tmp + i, value);

    // Copy remaining original array
    for (unsigned int i = insert_pos + count; i < old_size + count; ++i)
      tmp[i] = start_[i - count];

    deallocate_all();
    start_ = tmp;
    end_of_storage_ = start_ + old_size + count;
    finish_ = start_ + new_capacity;

    return begin() + insert_pos;
  }

  // Helper for insert function with same-value-range insert
  // Enough capacity
  iterator insert_range_without_realloc(size_type insert_pos, size_type count,
                                        const value_type& value) {
    // Case: inserting with iterator end()
    if (insert_pos == size()) {
      for (unsigned int i = 0; i < count; ++i) push_back(value);
      return end() - count;
    }

    size_type new_size = size() + count;

    // Copying from the back
    // Copy values from end_of_storage until end of insert range
    for (unsigned int i = new_size; i > insert_pos + count; --i)
      start_[i] = start_[i - count];

    // Construct range of new values
    for (unsigned int i = insert_pos; i < insert_pos + count; ++i)
      construct(start_ + i, value);

    end_of_storage_ += count;
    return begin() + insert_pos;
  }

  // Helper for insert functions with single position insert
  iterator insert_single_realloc(size_type insert_pos,
                                 const value_type& value) {
    size_type old_size = size();
    size_type new_capacity = std::max((size_type)1, capacity() * 2);
    pointer tmp = allocate(new_capacity);

    // Copy values before new value into tmp
    for (unsigned int i = 0; i < insert_pos; ++i) tmp[i] = start_[i];

    // Construct new value
    construct(tmp + insert_pos, value);

    // Copy values after new value into tmp
    for (unsigned int i = insert_pos; i < old_size; ++i) {
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
    // Case: inserting with iterator end()
    if (insert_pos == size()) {
      push_back(value);
      return end() - 1;
    }
    // Copying from the back
    // Copy values from end_of_storage until insert_pos
    for (unsigned int i = size(); i > insert_pos; i--)
      start_[i] = start_[i - 1];

    // Construct new value
    construct(start_ + insert_pos, value);

    ++end_of_storage_;
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
    size_type distance = get_distance(it1, it2);
    start_ = allocate(distance);
    uninitialized_copy_n(it1, distance, start_);
    end_of_storage_ = start_ + distance;
    finish_ = end_of_storage_;
  }

  // Constructs a range from first until first + n into the uninitialized
  // pointer range start until start + n
  template <class InputIt>
  void uninitialized_copy_n(InputIt first, size_type n, pointer start) {
    for (unsigned int i = 0; i < n; ++i) {
      construct(start, *first);
      ++start;
      ++first;
    }
  }

  // For random_access_iterator
  template <class InputIt>
  difference_type get_distance(
      typename ft::enable_if<
          ft::is_same<typename ft::iterator_traits<InputIt>::iterator_category,
                      std::random_access_iterator_tag>::value,
          InputIt>::type first,
      InputIt last) const {
    return last - first;
  }

  // For every other iterator
  template <class InputIt>
  difference_type get_distance(
      typename ft::enable_if<
          !ft::is_same<typename ft::iterator_traits<InputIt>::iterator_category,
                       std::random_access_iterator_tag>::value,
          InputIt>::type first,
      InputIt last) {
    difference_type distance = 0;
    while (first++ != last) {
      ++distance;
    }
    return distance;
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
    if (ft::is_integral<value_type>::value) return;
    while (first != last) {
      allocator_.destroy(first);
      ++first;
    }
  }
};

//**************************************************
// Non-member operator overloads
//**************************************************

template <class T, class Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  if (lhs.size() != rhs.size()) return false;
  return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
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

//**************************************************
// Non-member functions
//**************************************************

// It is sadly not possible to write a specialization of the std::swap function
// So this is the closest I can make
template <class T, class Alloc>
void swap(ft::vector<T, Alloc>& lhs, ft::vector<T, Alloc>& rhs) {
  lhs.swap(rhs);
}

}  // namespace ft



#endif  // VECTOR_H