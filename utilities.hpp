//////////////////////////////////////////////////////////////////////////////
// Contains the utilities for all ft::containers:
// ft::iterator_traits
// ft::reverse_iterator
// ft::enable_if
// ft::is_integral
// ft::equal
// ft::lexicographical_compare
// ft::pair
// ft::make_pair
//////////////////////////////////////////////////////////////////////////////

#ifndef UTILITIES_H
#define UTILITIES_H

namespace ft {

//////////////////////////////////////////////////////////////////////////////
// ft::iterator_traits
//////////////////////////////////////////////////////////////////////////////

template <typename Iterator>
struct iterator_traits {
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
  typedef typename Iterator::iterator_category iterator_category;
};

// Pointer specialization
template <typename T>
struct iterator_traits<T *> {
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef std::random_access_iterator_tag iterator_category;
};

// Const pointer specialization
template <typename T>
struct iterator_traits<const T *> {
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef std::random_access_iterator_tag iterator_category;
};

//////////////////////////////////////////////////////////////////////////////
// ft::reverse_iterator
//////////////////////////////////////////////////////////////////////////////

template <class Iterator>
class reverse_iterator {
  typedef Iterator iterator_type;

  typedef typename std::iterator_traits<iterator_type>::iterator_category iterator_category;
  typedef typename std::iterator_traits<iterator_type>::value_type value_type;
  typedef typename std::iterator_traits<iterator_type>::difference_type difference_type;
  typedef typename std::iterator_traits<iterator_type>::pointer pointer;
  typedef typename std::iterator_traits<iterator_type>::reference reference;

 public:
  //**************************************************
  // Constructors
  //**************************************************

  reverse_iterator() : base(iterator_type()){};
  explicit reverse_iterator(iterator_type x) 
      : base(iterator_type(x)) {};
  template <class U>
  reverse_iterator(const reverse_iterator<U>& other)
      : base(other.base) {};
  
  //**************************************************
  // Operator overloads
  //**************************************************

  // Assignment operator
  template<class U>
  reverse_iterator& operator=(const reverse_iterator<U>& other) {
    this->base = other.base;
    return *this;
  };

  // Access operators
  reference operator*() const {
    return base[-1];
  };

  pointer operator->() const {
    return &(operator*());
  };

  reference operator[](difference_type n) const {
    return base[- n - 1];
  };

  // Arithmetic operators
  

 protected:
  iterator_type base;
};

//////////////////////////////////////////////////////////////////////////////
// ft::enable_if
//////////////////////////////////////////////////////////////////////////////

template <bool, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
  typedef T type;
};

//////////////////////////////////////////////////////////////////////////////
// ft::is_integral
//////////////////////////////////////////////////////////////////////////////

// https://cplusplus.com/reference/type_traits/is_integral/?kw=is_integral
template <class T, T v>
struct integral_constant {
  static const T value = v;
  typedef T value_type;
  typedef integral_constant<T, v> type;
};

template <class T, T v>
const T integral_constant<T, v>::value;

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <class T>
struct is_integral : public integral_constant<bool, false> {};

template <>
struct is_integral<bool> : public true_type {};
template <>
struct is_integral<char> : public true_type {};
template <>
struct is_integral<unsigned char> : public true_type {};
template <>
struct is_integral<short> : public true_type {};
template <>
struct is_integral<unsigned short> : public true_type {};
template <>
struct is_integral<int> : public true_type {};
template <>
struct is_integral<unsigned int> : public true_type {};
template <>
struct is_integral<long> : public true_type {};
template <>
struct is_integral<unsigned long> : public true_type {};
template <>
struct is_integral<long long> : public true_type {};
template <>
struct is_integral<unsigned long long> : public true_type {};

//////////////////////////////////////////////////////////////////////////////
// ft::equal
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// ft::lexicographical_compare
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// ft::pair
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// ft::make_pair
//////////////////////////////////////////////////////////////////////////////

}  // namespace ft

#endif // UTILITIES_H