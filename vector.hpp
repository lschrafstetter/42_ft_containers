#ifndef VECTOR_H
#define VECTOR_H
#include "iterator_vector.hpp"
#include "utilities.hpp"

namespace ft {

template <typename T, typename Allocator = std::allocator<T> >
class vector {
 public:
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

  /**
   * @brief Construct a new vector object
   *
   */
  vector()
      : allocator_(allocator_type()),
        start_(NULL),
        finish_(NULL),
        end_of_storage_(NULL) {}

  /**
   * @brief Construct a new vector object explicitly with a certain allocator
   *
   * @param alloc an allocator
   */
  explicit vector(const allocator_type& alloc)
      : allocator_(alloc), start_(NULL), finish_(NULL), end_of_storage_(NULL) {}

  /**
   * @brief Construct a new vector object
   *
   * @param count size of the vector
   * @param value the value to fill the vector with
   * @param alloc an allocator (optional. Standard: std::allocator<T>)
   */
  explicit vector(size_type count, const value_type& value = value_type(),
                  const allocator_type& alloc = allocator_type())
      : allocator_(alloc), start_(NULL), finish_(NULL), end_of_storage_(NULL) {
    assign(count, value);
  }

  /**
   * @brief Construct a new vector object with a range [first;last)
   *
   * @tparam InputIt
   * @param first Iterator pointing to the start of the range
   * @param last Iterator pointing to the end of the range (excluded)
   * @param alloc an allocator (optional. Standard: std::allocator<T>)
   */
  template <class InputIt>
  explicit vector(
      InputIt first, InputIt last,
      const allocator_type& alloc = allocator_type(),
      typename ft::enable_if<!std::numeric_limits<InputIt>::is_integer,
                             InputIt>::type* = 0)
      : allocator_(alloc), start_(NULL), finish_(NULL), end_of_storage_(NULL) {
    assign(first, last);
  }

  /**
   * @brief Copy constructor
   *
   * @param other vector to copy
   */
  vector(const vector& other)
      : allocator_(allocator_type()),
        start_(NULL),
        finish_(NULL),
        end_of_storage_(NULL) {
    if (other.size()) assign(other.begin(), other.end());
  }

  /**
   * @brief Destroy the vector object
   *
   */
  ~vector() {
    clear();
    deallocate_all();
  }

  //**************************************************
  // Operator overloads
  //**************************************************

  /**
   * @brief assignment operator overload
   *
   * @param other source
   * @return vector& *this after assigning
   */
  vector& operator=(const vector& other) {
    if (*this != other) assign(other.begin(), other.end());
    return *this;
  }

  //**************************************************
  // Member functions
  //**************************************************

  /**
   * @brief rebuilds the vector with the size "count", filled with value "value"
   *
   * @param count
   * @param value
   */
  void assign(size_type count, const T& value) {
    if (this->capacity() < count) {
      this->~vector();
      start_ = allocate(count);
      finish_ = start_ + count;
      uninitialized_fill_n(start_, count, value);
      end_of_storage_ = finish_;
    } else {
      destroy(start_, end_of_storage_);
      for (unsigned int i = 0; i < count; ++i) construct(start_ + i, value);
      end_of_storage_ = start_ + count;
    }
  }

  /**
   * @brief rebuilds the vector with the range [first;last)
   *
   * @tparam InputIt
   * @param first Iterator pointing to the start of the range
   * @param last Iterator pointing to the end of the range
   */
  template <class InputIt>
  void assign(InputIt first, InputIt last,
              typename ft::enable_if<!std::numeric_limits<InputIt>::is_integer,
                                     InputIt>::type* = 0) {
    typedef typename iterator_traits<InputIt>::iterator_category category;
    assign_helper(first, last, category());
  }

  allocator_type get_allocator() const { return allocator_; }

  /**
   * @brief Returns a reference to the position at index "pos"
   *
   * @param pos
   * @return reference
   */
  reference at(size_type pos) {
    if (pos >= this->size())
      throw std::out_of_range("pos out of range in vector::at(size_type pos)");
    return start_[pos];
  }

  /**
   * @brief Returns a const_reference to the position at index "pos"
   *
   * @param pos
   * @return reference
   */
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

  /**
   * @brief increases the internal capacity to new_cap if new_cap is larger than
   * the current capacity
   *
   * @param new_cap
   */
  void reserve(size_type new_cap) {
    if (new_cap > this->max_size())
      throw std::length_error("new_cap exceeded size in vector::reserve()");
    size_type size = this->size();
    if (new_cap > capacity()) {
      pointer tmp = allocate(new_cap);
      if (ft::is_integral<value_type>::value)
        std::memcpy(tmp, start_, size * sizeof(value_type));
      else
        for (unsigned int i = 0; i < size; ++i)
          construct(tmp + i, *(start_ + i));
      this->~vector();
      start_ = tmp;
      finish_ = start_ + new_cap;
      end_of_storage_ = start_ + size;
    }
  }

  size_type capacity() const { return finish_ - start_; }

  /**
   * @brief sets the size to 0 and destroys all stored objects
   *
   */
  void clear() {
    destroy(start_, end_of_storage_);
    end_of_storage_ = start_;
  }

  /**
   * @brief Inserts one element into the vector
   *
   * @param pos An Iterator pointing to the insert position
   * @param value
   * @return iterator An iterator to the position of the new insert
   */
  iterator insert(const const_iterator& pos, const value_type& value) {
    size_type pos_insert = pos.base() - start_;
    _insert(pos, 1, value);
    return iterator(start_ + pos_insert);
  }

  /**
   * @brief Inserts "count" elements into the vector
   *
   * @param pos An Iterator pointing to the insert position
   * @param count
   * @param value
   * @return iterator An iterator to the position of the new insert
   */
  iterator insert(const const_iterator& pos, size_type count,
                  const value_type& value) {
    size_type pos_insert = pos.base() - start_;
    _insert(pos, count, value);
    return iterator(start_ + pos_insert);
  }

  /**
   * @brief Inserts a range [first;last) elements into the vector
   *
   * @tparam InputIt
   * @param pos An Iterator pointing to the insert position
   * @param first An Iterator pointing to the start of the range to be inserted
   * @param last An Iterator pointing to the end of the range to be inserted
   * @return iterator An iterator to the position of the new insert
   */
  template <class InputIt>
  iterator insert(
      const const_iterator& pos, InputIt first, InputIt last,
      typename ft::enable_if<!std::numeric_limits<InputIt>::is_integer,
                             InputIt>::type* = 0) {
    size_type insert_position = pos.base() - start_;
    typedef typename ft::iterator_traits<InputIt>::iterator_category category;
    _insert_helper(pos, first, last, category());
    return (iterator(start_ + insert_position));
  }

  /**
   * @brief Erases one object from the vector
   *
   * @param pos Iterator pointing to the position that is to be erased
   * @return iterator An iterator to the position of the erased object
   */
  iterator erase(const iterator& pos) {
    if (pos == end()) return end();
    size_type pos_first_removal = pos - start_;
    size_type new_size = this->size() - 1;
    size_type n_objects_to_move = new_size - pos_first_removal;

    if (ft::is_integral<value_type>::value) {
      std::memmove(pos.base(), pos.base() + 1,
                   n_objects_to_move * sizeof(value_type));
    } else {
      for (unsigned int i = pos_first_removal; i < new_size; ++i)
        start_[i] = start_[i + 1];
      allocator_.destroy(start_ + new_size);
    }
    --end_of_storage_;
    return pos;
  }

  /**
   * @brief Erases the range [first;last)] from the vector
   *
   * @param first Iterator pointing to the start of the range
   * @param last Iterator pointing to the end of the range
   * @return iterator An iterator to the position of the erased range of objects
   */
  iterator erase(const iterator& first, const iterator& last) {
    size_type distance = get_distance(first, last);
    if (!distance) return first;
    size_type new_size = size() - distance;
    size_type pos_first_removal = first - start_;
    size_type n_objects_to_move = new_size - pos_first_removal;

    if (ft::is_integral<value_type>::value) {
      std::memmove(first.base(), first.base() + distance,
                   n_objects_to_move * sizeof(value_type));
    } else {
      for (unsigned int i = pos_first_removal; i < new_size; ++i)
        start_[i] = start_[i + distance];
      destroy(start_ + new_size, end_of_storage_);
    }

    end_of_storage_ -= distance;
    return first;
  }

  /**
   * @brief Attaches an object "value" to the end of the vector
   *
   * @param value
   */
  void push_back(const value_type& value) {
    size_type size = this->size();
    size_type capacity = this->capacity();
    if (size == capacity) reserve(std::max((size_type)1, capacity * 2));
    construct(start_ + size, value);
    ++end_of_storage_;
  }

  /**
   * @brief Removes the last object of the vector
   *
   */
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
  /**
   * @brief Resizes the vector to size "count". If count is smaller than size,
   * left over objects will be destroyed. If count is bigger than size, objects
   * with value "value" will be created. If count is bigger than the current
   * capacity, reallocation occurs
   *
   * @param count new size
   * @param value value to fill the new space with
   */
  void resize(size_type count, value_type value = value_type()) {
    if (count > max_size())
      throw std::length_error("resize count exceeded max_size()");
    size_type size = this->size();
    if (size < count) {
      if (this->capacity() < count) {
        pointer tmp = allocate(count);

        if (ft::is_integral<value_type>::value)
          std::memcpy(tmp, start_, size * sizeof(value_type));
        else
          for (unsigned int i = 0; i < size; ++i) construct(tmp + i, start_[i]);
        for (unsigned int i = size; i < count; ++i) construct(tmp + i, value);

        this->~vector();
        start_ = tmp;
        end_of_storage_ = start_ + count;
        finish_ = end_of_storage_;
      } else {
        for (unsigned int i = size; i < count; ++i)
          construct(start_ + i, value);
        end_of_storage_ = start_ + count;
      }
    } else if (size > count) {
      destroy(start_ + count, end_of_storage_);
      end_of_storage_ = start_ + count;
    }
  }

  /**
   * @brief Swaps the allocator and all objects/size/capacity with another
   * vector
   *
   * @param other
   */
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

  // Range assign helper function for all iterators except for input iterators
  template <class InputIt>
  void assign_helper(InputIt first, InputIt last, std::forward_iterator_tag) {
    size_type distance = get_distance(first, last);

    if (this->capacity() < distance) {
      this->~vector();
      start_ = allocate(distance);
      // memcpy only for:
      // 1) random_access_iterators: contiguous memory
      // 2) Integral types (otherwise we must construct)
      // 3) If the pointer types are the same (edge case: making vector<long>
      // out of array[int])
      if (ft::is_same<typename ft::iterator_traits<InputIt>::iterator_category,
                      std::random_access_iterator_tag>::value &&
          ft::is_integral<value_type>::value &&
          sizeof(*first) == sizeof(*start_))
        std::memcpy(start_, &(*first), distance * sizeof(value_type));
      else
        uninitialized_copy_n(first, distance, start_);
      end_of_storage_ = start_ + distance;
      finish_ = end_of_storage_;
    } else {
      destroy(start_, end_of_storage_);
      for (unsigned int i = 0; i < distance; ++i) {
        construct(start_ + i, *first);
        ++first;
      }
      end_of_storage_ = start_ + distance;
    }
  }

  // Range assign helper function for input_iterators
  template <class InputIt>
  void assign_helper(InputIt first, InputIt last, std::input_iterator_tag) {
    this->clear();
    vector<value_type> tmp;
    while (first != last) {
      tmp.push_back(*first);
      ++first;
    }
    assign(tmp.begin(), tmp.end());
  }

  void _insert(const const_iterator& pos, size_type count,
               const value_type& value) {
    if (count == 0) return;
    if (capacity() - size() >= count)
      _insert_no_realloc(pos, count, value);
    else {
      _insert_with_realloc(pos, count, value);
    }
  }

  void _insert_no_realloc(const const_iterator& pos, size_type count,
                          const value_type& value) {
    if (pos != end()) {
      // Basic guarantee
      size_type size = this->size();
      size_type new_size = size + count;
      size_type insert_position = pos.base() - start_;
      size_type n_objects_to_move = size - insert_position;
      // First step: Copy from back to new_size
      if (ft::is_integral<value_type>::value)
        std::memmove(start_ + insert_position + count, start_ + insert_position,
                     n_objects_to_move * sizeof(value_type));
      else {
        // Make space for new objects: move objects "count" spaces to "right"
        size_type i = 0;  // number of objects constructed
        size_type index_new_last_element = new_size - 1;
        try {
          // Costruct from behind
          while (i < n_objects_to_move) {
            if (new_size - 1 - i < size)
              allocator_.destroy(start_ + new_size - 1 - i);
            construct(start_ + new_size - 1 - i,
                      start_[new_size - 1 - i - count]);
            ++i;
          }
        } catch (...) {
          // If exception is thrown, destroy all already constructed objects
          destroy(start_ + index_new_last_element, end_of_storage_ + count);
          throw "Exception when inserting new objects into vector";
        }
      }

      // Second step: construct new objects
      if (ft::is_integral<value_type>::value) {
        for (unsigned int i = 0; i < count; ++i)
          start_[insert_position + i] = value;
      } else {
        size_type i = 0;
        try {
          while (i < count) {
            // construct(start_ + insert_position + i, value);
            // allocator_.destroy(start_ + insert_position + + i)
            start_[insert_position + i] = value;
            ++i;
          }
        } catch (...) {
          // if an exception is thrown, just stop and leave old objects in the
          // range
          throw "Exception when inserting new objects into vector";
        }
      }
      end_of_storage_ += count;
    } else {
      // Strong guarantee
      size_type i = 0;
      try {
        while (i < count) {
          construct(end_of_storage_ + i, value);
          ++i;
        }
        end_of_storage_ += count;
      } catch (...) {
        // If an exception occurs, destroy already constructed objects
        destroy(end_of_storage_, end_of_storage_ + i);
        throw "Exception when inserting new objects into vector";
      }
    }
  }

  void _insert_with_realloc(const const_iterator& pos, size_type count,
                            const value_type& value) {
    size_type size = this->size();
    size_type new_size = size + count;
    size_type new_capacity;
    if (count == 1) {
      new_capacity = std::max((size_type)1, capacity() * 2);
      while (new_capacity < new_size) new_capacity *= 2;
    } else {
      new_capacity = new_size;
    }
    if (new_capacity > max_size())
      throw std::length_error("new capacity over max_size");
    pointer tmp = allocate(new_capacity);
    size_type insert_position = pos.base() - start_;
    size_type n_objects_to_move = size - insert_position;
    size_type i = 0;  // Counts the number of objects in tmp
    // Strong guarantee
    try {
      // Copy part before insert to tmp
      if (ft::is_integral<value_type>::value) {
        std::memcpy(tmp, start_, insert_position * sizeof(value_type));
        i += insert_position;
      } else {
        while (i < insert_position) {
          construct(tmp + i, start_[i]);
          ++i;
        }
      }
      // Construct new objects
      size_type pos_end_of_insert = insert_position + count;
      while (i < pos_end_of_insert) {
        construct(tmp + i, value);
        ++i;
      }
      // Copy part after insert to tmp
      if (ft::is_integral<value_type>::value)
        std::memcpy(tmp + i, start_ + insert_position,
                    n_objects_to_move * sizeof(value_type));
      else {
        while (i < new_size) {
          construct(tmp + i, start_[i - count]);
          ++i;
        }
      }
    } catch (...) {
      destroy(tmp, tmp + i + 1);
      allocator_.deallocate(tmp, new_capacity * sizeof(value_type));
      throw "Exception when inserting new objects into vector";
    }
    this->~vector();
    start_ = tmp;
    end_of_storage_ = start_ + new_size;
    finish_ = start_ + new_capacity;
  }

  template <class InputIt>
  iterator _insert_helper(const const_iterator& pos, InputIt first,
                          InputIt last, std::forward_iterator_tag) {
    size_type insert_pos = pos - begin();
    size_type distance = get_distance(first, last);

    if (capacity() - size() >= distance)
      _insert_range_no_realloc(pos, first, last);
    else
      _insert_range_with_realloc(pos, first, last);
    return (iterator(start_ + insert_pos));
  }

  template <class InputIt>
  void _insert_range_no_realloc(const const_iterator& pos, InputIt first,
                                InputIt last) {
    size_type insert_position = pos.base() - start_;
    size_type distance = get_distance(first, last);
    if (pos != end()) {
      // Basic guarantee
      size_type size = this->size();
      size_type new_size = size + distance;
      size_type n_objects_to_move = size - insert_position;
      // First step: Copy from back to new_size
      if (ft::is_integral<value_type>::value)
        std::memmove(start_ + insert_position + distance,
                     start_ + insert_position,
                     n_objects_to_move * sizeof(value_type));
      else {
        // Make space for new objects: move objects "count" spaces to "right"
        size_type i = 0;  // number of objects constructed
        size_type index_new_last_element = new_size - 1;
        try {
          // Costruct from behind
          while (i < n_objects_to_move) {
            if (new_size - 1 - i < size)
              allocator_.destroy(start_ + new_size - 1 - i);
            construct(start_ + new_size - 1 - i,
                      start_[new_size - 1 - i - distance]);
            ++i;
          }
        } catch (...) {
          // If exception is thrown, destroy all already constructed objects
          destroy(start_ + index_new_last_element, end_of_storage_ + distance);
          throw "Exception when inserting new objects into vector";
        }
      }

      // Second step: construct new objects
      if (ft::is_integral<value_type>::value) {
        for (size_type i = 0; i < distance; ++i)
          start_[insert_position + i] = *(first++);
      } else {
        size_type i = 0;
        try {
          while (i < distance) {
            start_[insert_position + i] = *(first++);
            ++i;
          }
        } catch (...) {
          // if an exception is thrown, just stop and leave old objects in the
          // range
          throw "Exception when inserting new objects into vector";
        }
      }
      end_of_storage_ += distance;
    } else {
      // Strong guarantee
      size_type i = 0;
      try {
        while (i < distance) {
          construct(end_of_storage_ + i, *(first++));
          ++i;
        }
        end_of_storage_ += distance;
      } catch (...) {
        // If an exception occurs, destroy already constructed objects
        destroy(end_of_storage_, end_of_storage_ + i);
        throw "Exception when inserting new objects into vector";
      }
    }
  }

  template <class InputIt>
  void _insert_range_with_realloc(const const_iterator& pos, InputIt first,
                                  InputIt last) {
    size_type size = this->size();
    size_type distance = get_distance(first, last);
    size_type new_size = size + distance;
    size_type new_capacity = std::max((size_type)1, capacity());

    while (new_capacity < new_size) new_capacity *= 2;
    if (new_capacity > max_size())
      throw std::length_error("new capacity over max_size");
    pointer tmp = allocate(new_capacity);
    size_type insert_position = pos.base() - start_;
    size_type n_objects_to_move = size - insert_position;
    size_type i = 0;  // Counts the number of objects in tmp
    // Strong guarantee
    try {
      // Copy part before insert to tmp
      if (ft::is_integral<value_type>::value) {
        std::memcpy(tmp, start_, insert_position * sizeof(value_type));
        i += insert_position;
      } else {
        while (i < insert_position) {
          construct(tmp + i, start_[i]);
          ++i;
        }
      }
      // Construct new objects
      size_type pos_end_of_insert = insert_position + distance;
      while (i < pos_end_of_insert) {
        construct(tmp + i, *(first++));
        ++i;
      }

      // Copy part after insert to tmp
      if (ft::is_integral<value_type>::value)
        std::memcpy(tmp + i, start_ + insert_position,
                    n_objects_to_move * sizeof(value_type));
      else {
        while (i < new_size) {
          construct(tmp + i, start_[i - distance]);
          ++i;
        }
      }
    } catch (...) {
      allocator_.deallocate(tmp, new_capacity * sizeof(value_type));
      throw "Exception when inserting new objects into vector";
    }
    this->~vector();
    start_ = tmp;
    end_of_storage_ = start_ + new_size;
    finish_ = start_ + new_capacity;
  }

  template <class InputIt>
  void _insert_helper(const const_iterator& pos, InputIt first, InputIt last,
                      std::input_iterator_tag) {
    size_type size = this->size();
    size_type insert_position = pos.base() - start_;
    vector<value_type> tmp;
    tmp.reserve(size);
    size_type i = 0;  // built elements
    size_type distance_range = 0;
    try {
      while (i < insert_position) tmp.push_back(start_[i++]);
      while (first != last) {
        tmp.push_back(*first);
        ++first;
        distance_range++;
      }
      while (i < size) tmp.push_back(start_[i++]);
    } catch (...) {
      throw "Exception when inserting new objects into vector";
    }
    assign(tmp.begin(), tmp.end());
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

  // Copies the given value value to the first count elements in an
  // uninitialized memory area beginning at first
  void uninitialized_fill_n(pointer first, size_type count,
                            const value_type& value) {
    for (unsigned int i = 0; i < count; ++i) {
      construct(first, value);
      ++first;
    }
  }

  // For random_access_iterator
  template <class InputIt>
  difference_type get_distance(
      InputIt first, InputIt last,
      typename ft::enable_if<
          ft::is_same<typename ft::iterator_traits<InputIt>::iterator_category,
                      std::random_access_iterator_tag>::value,
          InputIt>::type* = 0) const {
    return last - first;
  }

  // For every other iterator
  template <class InputIt>
  difference_type get_distance(
      InputIt first, InputIt last,
      typename ft::enable_if<
          !ft::is_same<typename ft::iterator_traits<InputIt>::iterator_category,
                       std::random_access_iterator_tag>::value,
          InputIt>::type* = 0) {
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
    if (n > max_size()) throw std::bad_alloc();
    return static_cast<pointer>(allocator_.allocate(n * sizeof(value_type)));
  }

  void deallocate_all() {
    if (start_) {
      allocator_.deallocate(start_, (finish_ - start_) * sizeof(value_type));
      start_ = NULL;
      end_of_storage_ = NULL;
      finish_ = NULL;
    }
  }

  template <class U>  // U = value_type
  void construct(
      U* position,
      const typename ft::enable_if<ft::is_integral<U>::value, U>::type& value) {
    *position = value;
  }

  template <class U>  // U = value_type
  void construct(U* position,
                 const typename ft::enable_if<!ft::is_integral<U>::value,
                                              U>::type& value) {
    allocator_.construct(position, value);
  }

  /**
   * @brief Destroys the objects in the range [first;last)
   *
   * @param first
   * @param last
   */
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

// Uncomment this to pass the ft-containers-terminator tester even though we are
// not supposed to implement an ft:swap function
/* template <class T, class Alloc>
void swap(ft::vector<T, Alloc>& lhs, ft::vector<T, Alloc>& rhs) {
  lhs.swap(rhs);
} */

}  // namespace ft

// std::swap specialization for ft::vector
namespace std {
template <class T, class Alloc>
void swap(ft::vector<T, Alloc>& lhs, ft::vector<T, Alloc>& rhs) {
  lhs.swap(rhs);
}

}  // namespace std

#endif  // VECTOR_H