#ifndef VECTOR_H
#define VECTOR_H
#include <memory>
#include "utilities.hpp"

namespace ft {
template <typename T, typename Allocator = std::allocator<T> >
class vector {
  typedef T value_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef Allocator allocator_type;

  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;

 public:
  //**************************************************
  // Constructors
  //**************************************************
  vector() : start_(NULL), finish_(NULL), end_of_storage_(NULL) {}
  explicit vector(const allocator_type& alloc)
      : start_(NULL), finish_(NULL), end_of_storage_(NULL), allocator_(alloc) {}
  explicit vector(size_type count, const value_type& value = value_type(),
                  const allocator_type& alloc = allocator_type()) {
    start_ = allocate(count);
    std::uninitialized_fill_n(start_, count, value);
    finish_ = start_ + count;
    end_of_storage_ = finish_;
  }

 private:
  allocator_type allocator_;
  pointer start_;
  pointer finish_;
  pointer end_of_storage_;

  pointer allocate(size_t n) {
    return static_cast<pointer>(allocator_.allocate(n * sizeof(value_type)));
  }

  void deallocate_all() {
    if (start_) {
      allocator_.deallocate(start_, end_of_storage_ - start_);
    }
  }

  void construct(pointer position, const value_type& x) {
    allocator_.construct(position, x);
  }

  void destroy_all(pointer first, pointer last) {
    while (first != last) {
      allocator_.destroy(first);
      ++first;
    }
  }
};
}  // namespace ft

#endif // VECTOR_H