#ifndef VECTOR_H
#define VECTOR_H
#include <memory>
#include "utilities.hpp"

namespace ft {
template <typename T, typename Allocator = std::allocator<T> >
class vector {
  typedef T                               value_type;
  typedef std::size_t                     size_type;
  typedef std::ptrdiff_t                  difference_type;
  typedef Allocator                       allocator_type;

  typedef Allocator::pointer              pointer;
  typedef const Allocator::const_pointer  const_pointer;
  typedef value_type&                     reference;
  typedef const value_type&               const_reference;
  
  
};
}  // namespace ft

#endif VECTOR_H